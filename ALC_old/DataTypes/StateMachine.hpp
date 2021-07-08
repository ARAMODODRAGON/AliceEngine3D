/*
* MIT License
*
* Copyright (c) 2021 Domara Shlimon
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef ALC_DATATYPES_STATEMACHINE_HPP
#define ALC_DATATYPES_STATEMACHINE_HPP
#include "../General.hpp"

namespace ALC {

	/// <summary>
	/// handler to hold the current state and the next state to switch to
	/// </summary>
	/// <typeparam name="Class">the class that this statemachine will invoke functions on</typeparam>
	/// <typeparam name="...Params"></typeparam>
	/// <typeparam name="StateTy"></typeparam>
	template<typename Class, typename StateTy, typename... Params>
	class StateMachine final {
		ALC_NO_COPY(StateMachine);
	public:

		// the integer convertable type used for the state
		using Statetype = StateTy;

		// called during that state
		using StateFunc = void(Class::*)(Params...);

		// called when switching to a state
		// Statetype is the last state
		using BeginStateFunc = void(Class::*)(const Statetype, Params...);

		// called when switching away from a state
		// Statetype is the next state
		using EndStateFunc = void(Class::*)(const Statetype, Params...);

		// defaults the state to 0
		StateMachine(Class* instance)
			: m_instance(instance)
			, m_currentState(static_cast<Statetype>(0))
			, m_nextState(m_currentState)
			, m_begin(true) {
			ALC_ASSERT(instance, "instance must not be null");
		}

		// sets the state 
		StateMachine(Class* instance, Statetype state)
			: m_instance(instance)
			, m_currentState(state)
			, m_nextState(m_currentState)
			, m_begin(true) {
			ALC_ASSERT(instance, "instance must not be null");
		}

		~StateMachine() { }

		// adds or modifies a state in the statemachine
		// set null to unbind
		void Bind(const Statetype state, StateFunc step, BeginStateFunc begin = nullptr, EndStateFunc end = nullptr) {
			if (step != nullptr) {
				// create the binding
				StateBinding binding{ begin, step, end };
				// set the state
				m_boundStates.emplace(state, binding).first->second = binding;
			} else {
				// remove the state
				m_boundStates.erase(m_boundStates.find(state));
			}
		}

		// returns the current state
		Statetype GetState() const { return m_currentState; }

		// sets the next state to be called
		void ChangeState(const Statetype state) {
			m_nextState = state;
			if (m_begin) m_currentState = m_nextState;
		}

		// invokes the current state
		void operator()(Params... params) {

			// change state
			if (m_currentState != m_nextState) {
				auto laststate = m_boundStates.find(m_currentState);
				auto nextstate = m_boundStates.find(m_nextState);
				ALC_ASSERT(nextstate != m_boundStates.end(), "next state must be a valid state");

				auto laststateV = m_currentState;
				m_currentState = m_nextState;

				// end last state and begin new state
				if (laststate->second.end) (m_instance->*(laststate->second.end))(m_currentState, params...);
				if (nextstate->second.begin) (m_instance->*(nextstate->second.begin))(laststateV, params...);

			}

			// called first
			if (m_begin) {
				m_begin = false;
				auto state = m_boundStates.find(m_currentState);
				ALC_ASSERT(state != m_boundStates.end(), "current state must be a valid state");
				if (state->second.begin) (m_instance->*(state->second.begin))(m_currentState, params...);
			}

			// step
			(m_instance->*(m_boundStates[m_currentState].step))(params...);

		}

	private:

		Class* m_instance;
		Statetype m_currentState;
		Statetype m_nextState;
		bool m_begin;

		struct StateBinding {
			BeginStateFunc begin;
			StateFunc step;
			EndStateFunc end;
		};
		unordered_map<Statetype, StateBinding> m_boundStates;

	};

	template<typename Class, typename StateTy>
	using EntityStateMachine = StateMachine<Class, StateTy, Entity, Timestep>;

}

#endif // !ALC_DATATYPES_STATEMACHINE_HPP