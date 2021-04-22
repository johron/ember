#pragma once

namespace ember::graph {
struct input_pin {
	QString m_name;
};

struct output_pin {
	QString m_name;
};

struct node {
	node(QString name)
		: m_name(name) {}

	std::vector<input_pin> m_input;
	std::vector<output_pin> m_output;
	QString m_name;
};

struct accumulate_node : public node {
	accumulate_node()
		: node("accumulate") {
		m_input.emplace_back(input_pin{"first"});
		m_input.emplace_back(input_pin{"second"});
		m_output.emplace_back(output_pin{"sum"});
	}
};

template <class value_t>
struct constant_node : public node {
	constant_node(const value_t& value)
		: node("constant") 
		, m_value(value) {
		m_output.emplace_back(output_pin{"value"});
	}

	value_t m_value;
};

}  // namespace ember::graph