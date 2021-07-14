#ifndef ALC_REFLECTION_TYPE_HPP
#define ALC_REFLECTION_TYPE_HPP
#include "typehash.hpp"
#include <memory>

namespace alc {

	class type;

	class member_variable final {
	public:

		// sets the value on the instance of this member variable
		template<typename Class, typename Value>
		bool set_value(Class* instance, const Value& value);

		// gets the value on the instance of this member variable
		template<typename Class, typename Value>
		bool get_value(Class* instance, Value& outValue);

		// checks if this is valid
		operator bool() const;

		// checks if this is valid
		bool is_valid() const;

		// returns the class type that this is a member of 
		type get_class_type() const;

		// returns the type for this member
		type get_type() const;

		// returns the name of the variable
		const std::string& get_name() const;

	private:
		using set_val = void (*)(void* instance, const void* inValue);
		using get_val = void (*)(void* instance, void* outValue);
		using type_get = type(*)();
		type_get m_classType;
		type_get m_variableType;
		set_val m_setValue;
		get_val m_getValue;
		std::string m_name;
	public:
		template<auto MemberVar, typename ClassTy, typename ValueTy>
		static member_variable __make(const std::string& name, ValueTy(ClassTy::* var));
		member_variable();
	};

	class type final {
		struct type_assign;
	public:

		// static /////////////////////////

		// returns the type instance for Ty
		template<typename Ty>
		static type get();

		// assigns data to the type for Ty
		// overwrites any previous assigned data
		template<typename Ty>
		static type_assign assign(const std::string& type_name);

		// instance ///////////////////////

		// constructs an invalid type
		type();

		// checks if type is valid
		operator bool() const;

		// checks if type is valid
		bool is_valid() const;

		// the name of this type
		const std::string& get_name() const;

		// the number of member variables that are exposed 
		size_t vars_size() const;

		// returns the member_variable at the given index
		member_variable var_get(size_t index) const;

		// the number of parents to this type
		size_t parents_size() const;

		// returns the parent type at the given index
		type parent_get(size_t index) const;

		// checks if these are the same types
		bool operator==(const type& other) const;

		// checks if these are different types
		bool operator!=(const type& other) const;

		// type assignment ////////////////

	private:
		struct data_t;
		// is used to assign data to a type
		struct type_assign {

			// adds a variable to the type
			template<auto MemberVar>
			type_assign variable(const std::string& var_name);

			// adds a type as a parent type
			template<typename Ty>
			type_assign parent();

		private:
			data_t* m_typeData;
		public:
			type_assign(data_t* m_typeData);
			~type_assign();
		};
		using cstructor = void* (*)();
		struct data_t {
			typehash m_hash;
			std::string m_typeName;
			std::vector<member_variable> m_memberVariables;
			std::vector<type> m_parents;
			cstructor m_constructor;
		};
		std::shared_ptr<data_t> m_data;
		template<typename Ty> static cstructor get_constructor();
		type(typehash typehash_, void* (*constructor)());
		friend type_assign;
	};

	// template implementations /////////

	template<typename Class, typename Value>
	inline bool member_variable::set_value(Class* instance, const Value& value) {
		// is invalid
		if (!is_valid()) return false;
		// one incorrect type
		if (type::get<Class>() != get_class_type() || type::get<Value>() != get_type())
			return false;

		// set value
		const void* ptr = &value;
		m_setValue(instance, ptr);

		return true;
	}

	template<typename Class, typename Value>
	inline bool member_variable::get_value(Class* instance, Value& outValue) {
		// is invalid
		if (!is_valid()) return false;
		// one incorrect type
		if (type::get<Class>() != get_class_type() || type::get<Value>() != get_type())
			return false;

		// set value
		void* ptr = &outValue;
		m_getValue(instance, ptr);

		return true;
	}

	template<auto MemberVar, typename ClassTy, typename ValueTy>
	inline member_variable member_variable::__make(const std::string& name, ValueTy(ClassTy::* var)) {
		member_variable mem;

		mem.m_name = name;
		mem.m_classType = []()->type { return type::get<ClassTy>(); };
		mem.m_variableType = []()->type { return type::get<ValueTy>(); };

		mem.m_setValue = [](void* instance, const void* value) {
			ClassTy* object = reinterpret_cast<ClassTy*>(instance);
			const ValueTy* val = reinterpret_cast<const ValueTy*>(value);
			(object->*MemberVar) = *val;
		};
		mem.m_getValue = [](void* instance, void* value) {
			ClassTy* object = reinterpret_cast<ClassTy*>(instance);
			ValueTy* val = reinterpret_cast<ValueTy*>(value);
			*val = (object->*MemberVar);
		};

		return mem;
	}

	template<typename Ty>
	inline type type::get() {
		static type _type(get_typehash<Ty>(), get_constructor<Ty>());
		return _type;
	}

	template<typename Ty>
	inline type::type_assign type::assign(const std::string& type_name) {
		type ty = get<Ty>();
		ty.m_data->m_typeName = type_name;
		return type_assign(ty.m_data.get());
	}

	template<typename Ty>
	inline type::cstructor type::get_constructor() {
		if constexpr (std::is_default_constructible_v<Ty>)
			return []() -> void* { return new Ty(); };
		return nullptr;
	}

	template<auto MemberVar>
	inline type::type_assign type::type_assign::variable(const std::string& var_name) {
		member_variable var = member_variable::__make<MemberVar>(var_name, MemberVar);
		m_typeData->m_memberVariables.push_back(var);
		return *this;
	}

	template<typename Ty>
	inline type::type_assign type::type_assign::parent() {
		m_typeData->m_parents.push_back(type::get<Ty>());
		return *this;
	}

}

#endif // !ALC_REFLECTION_TYPE_HPP