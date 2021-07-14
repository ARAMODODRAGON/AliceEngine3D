#include "type.hpp"

namespace alc {

	member_variable::operator bool() const { return is_valid(); }

	bool member_variable::is_valid() const {
		return m_setValue && m_getValue && m_name != "" && m_classType && m_variableType;
	}

	type member_variable::get_class_type() const {
		if (m_classType) return m_classType();
		return type();
	}

	type member_variable::get_type() const {
		if (m_variableType) return m_variableType();
		return type();
	}

	const std::string& member_variable::get_name() const {
		return m_name;
	}

	member_variable::member_variable()
		: m_classType(nullptr), m_variableType(nullptr), m_name("")
		, m_setValue(nullptr), m_getValue(nullptr) { }

	type::type() : m_data(nullptr) { }

	type::operator bool() const { return is_valid(); }

	bool type::is_valid() const {
		return m_data.get();
	}

	const std::string& type::get_name() const {
		return m_data->m_typeName;
	}

	size_t type::vars_size() const {
		return m_data->m_memberVariables.size();
	}

	member_variable type::var_get(size_t index) const {
		return m_data->m_memberVariables[index];
	}

	size_t type::parents_size() const {
		return m_data->m_parents.size();
	}

	type type::parent_get(size_t index) const {
		return m_data->m_parents[index];
	}

	bool type::operator==(const type& other) const {
		if (!this->is_valid() || !other.is_valid()) return false;
		return m_data->m_hash == other.m_data->m_hash;
	}

	bool type::operator!=(const type& other) const {
		if (!this->is_valid() || !other.is_valid()) return false;
		return m_data->m_hash != other.m_data->m_hash;
	}

	type::type(typehash typehash_, void* (*constructor)())
		: m_data(new data_t()) {
		m_data->m_hash = typehash_;
		m_data->m_constructor = constructor;
		m_data->m_typeName = "Unnamed";
	}

	type::type_assign::type_assign(data_t* m_typeData) : m_typeData(m_typeData) {
		m_typeData->m_memberVariables.clear();
		m_typeData->m_parents.clear();
	}

	type::type_assign::~type_assign() { }

}