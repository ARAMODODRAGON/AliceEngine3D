#ifndef ALC_DATATYPES_FUNCTION_HPP
#define ALC_DATATYPES_FUNCTION_HPP
#include <tuple>
#include <memory>
#include <list>

namespace ALC {

	template<typename _RetTy, typename... ArgsTy>
	struct Function;

	namespace detail {

		template<typename Sig>
		struct func_sig;

		template<typename RetTy, typename... ArgsTy>
		struct func_sig<RetTy(*)(ArgsTy...)> {
			using sig_t = RetTy(*)(ArgsTy...);
			using ret_t = RetTy;
			using class_ty = void;
		};

		template<typename RetTy, typename ClassTy, typename... ArgsTy>
		struct func_sig<RetTy(ClassTy::*)(ArgsTy...)> {
			using sig_t = RetTy(ClassTy::*)(ArgsTy...);
			using ret_ty = RetTy;
			using class_ty = ClassTy;
		};

		template<typename Sig>
		struct get_function;

		template<typename Ret, typename... Args>
		struct get_function<Ret(*)(Args...)> {
			using type = Function<Ret, Args...>;
		};

		template<typename Ret, typename... Args>
		struct get_function<Ret(Args...)> {
			using type = Function<Ret, Args...>;
		};

		template<typename Ret, typename Class, typename... Args>
		struct get_function<Ret(Class::*)(Args...)> {
			using type = Function<Ret, Args...>;
		};

		template<auto Fun>
		using get_function_t = typename get_function<decltype(Fun)>::type;

	}

	template<typename _RetTy, typename... ArgsTy>
	struct Function {

		using returnty = _RetTy;
		using funcsigty = returnty(*)(ArgsTy...);
		using arguments = std::tuple<ArgsTy...>;

		constexpr Function(std::nullptr_t = nullptr)
			: m_instance(nullptr), m_function(nullptr), m_invokeable(nullptr) { }

		template<typename T>
		Function(T lambda)
			: m_instance(nullptr), m_function(), m_invokeable(nullptr) {
			bind(lambda);
		}

		operator bool() const {
			return m_instance != nullptr || m_function != nullptr || m_invokeable.get() != nullptr;
		}

		bool operator==(const Function& other) const {
			if (m_instance)		 return this->m_instance == other.m_instance && this->m_memberbinding == other.m_memberbinding;
			else if (m_function) return this->m_function == other.m_function;
			else				 return this->m_invokeable.get() == other.m_invokeable.get();
		}

		bool operator!=(const Function& other) const {
			return !operator==(other);
		}

		void unbind() {
			m_invokeable.reset();
			m_instance = nullptr;
			m_function = nullptr;
			m_memberbinding = nullptr;
		}

		template<funcsigty Function>
		void bind() {
			m_invokeable.reset();
			m_instance = nullptr;
			m_memberbinding = nullptr;
			m_function = Function;
		}

		template<typename T>
		void bind(T lambda) {
			m_instance = nullptr;
			m_memberbinding = nullptr;
			m_function = nullptr;
			if constexpr (std::is_convertible_v<T, funcsigty>) // easily converted lambda
				m_function = lambda;
			else  // heap allocated invokable object
				m_invokeable.reset(new invokeable<T>(lambda));
		}

		template<auto Member, typename ClassTy = typename detail::func_sig<decltype(Member)>::class_ty>
		void bind(ClassTy* instance) {
			m_invokeable.reset();
			m_instance = instance;
			m_function = nullptr;
			m_memberbinding = [](void* ptr, ArgsTy... args) { // binding lambda
				return (reinterpret_cast<ClassTy*>(ptr)->*Member)(args...);
			};
		}

		returnty operator()(ArgsTy... args) const {
			// invoke member function
			if (m_instance)			return m_memberbinding(m_instance, args...);
			// invoke non-member function
			else if (m_function)	return m_function(args...);
			// invoke pointer to invokeable
			else					return m_invokeable->invoke(args...);
		}

	private:

		struct _invokable {
			virtual returnty invoke(ArgsTy...) = 0;
		};
		template<typename ObjectTy>
		struct invokeable : _invokable {
			ObjectTy object;
			returnty invoke(ArgsTy... args) override {
				object(args...);
			}
			invokeable(ObjectTy& object_) : object(object_) { }
		};
		template<typename ObjectTy>
		struct invokeable<ObjectTy&> : _invokable {
			ObjectTy& object;
			returnty invoke(ArgsTy... args) override {
				object(args...);
			}
			invokeable(ObjectTy& object_) : object(object_) { }
		};
		template<typename ObjectTy>
		struct invokeable<ObjectTy*> : _invokable {
			ObjectTy* object;
			returnty invoke(ArgsTy... args) override {
				(*object)(args...);
			}
			invokeable(ObjectTy* object_) : object(object_) { }
		};

		void* m_instance;
		union {
			returnty(*m_memberbinding)(void*, ArgsTy...);
			funcsigty m_function;
		};
		std::shared_ptr<_invokable> m_invokeable;
	};

	template<auto Func, typename Function = detail::get_function_t<Func>>
	Function make_function() {
		Function f;
		f.bind<Function>();
		return f;
	}

	template<auto Member, typename ClassTy, typename Function = detail::get_function_t<Member>>
	Function make_function(ClassTy* instance) {
		Function f;
		f.bind<Member>(instance);
		return f;
	}

	template<auto Lambda, typename... ArgsHint, typename Function = Function<ArgsHint...>>
	Function make_function() {
		Function f;
		f.bind<Lambda>();
		return f;
	}

	template<typename... ArgsHint, typename Function = Function<ArgsHint...>, typename Lambda>
	constexpr Function make_function(Lambda lambda) {
		return Function(lambda);
	}

	template<typename _RetTy, typename... ArgsTy>
	struct Event {
		using returnty = _RetTy;
		using funcsigty = returnty(*)(ArgsTy...);
		using Function = Function<_RetTy, ArgsTy...>;
		using arguments = std::tuple<ArgsTy...>;

		Event() { }

		Event& operator+=(const Function& f) {
			for (auto& func : m_listeners) {
				if (func == f) {
					return *this;
				}
			}
			m_listeners.push_back(f);
			return *this;
		}

		Event& operator-=(const Function& f) {
			m_listeners.remove_if([f](auto& func) {return func == f; });
			return *this;
		}

		void operator()(ArgsTy... args) const {
			for (auto& func : m_listeners) func(args...);
		}

		template<typename T>
		void operator()(T returncallback, ArgsTy... args) {
			for (auto& func : m_listeners) returncallback(func(args...));
		}

		void clear() {
			m_listeners.clear();
		}

	private:
		std::list<Function> m_listeners;
	};

}

#endif // !ALC_DATATYPES_FUNCTION_HPP