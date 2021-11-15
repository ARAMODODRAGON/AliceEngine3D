#ifndef TYPE_SYSTEM_DEMO_HPP
#define TYPE_SYSTEM_DEMO_HPP
#include <iostream>
#include <alc\alc.hpp>
#include <alc\core\alice_events.hpp>
//#include <alc\objects\object_factory.hpp>
#include <alc\reflection\type.hpp>

class Parent { };

class Tester : public Parent {
public:

	Tester() : m_value(0.0f) { }

	float GetValue() const { return m_value; }

private:

	float m_value;

public:
	static void AssignTypeData() {
		alc::type::assign<Tester>("Tester")
			.parent<Parent>()
			.variable<&Tester::m_value>("m_value");
	}
};

inline void TypeSystemDemo() {

	Tester::AssignTypeData();

	Tester test;

	std::cout << "Value: " << test.GetValue() << std::endl;

	alc::type t = alc::type::get<Tester>();

	alc::member_variable var;
	for (size_t i = 0; i < t.vars_size(); i++) {
		if (t.var_get(i).get_name() == "m_value") {
			var = t.var_get(i);
			break;
		}
	}

	if (var) {
		var.set_value(&test, 10.0f);
		float outValue = 0.0f;
		if (var.get_value(&test, outValue))
			std::cout << "Value: " << outValue << std::endl;
		else
			std::cout << "Could not read value from member m_value" << std::endl;
	}
	// failure
	else {
		std::cout << "Could not find member with name 'm_value'" << std::endl;
	}
}

#endif // !TYPE_SYSTEM_DEMO_HPP