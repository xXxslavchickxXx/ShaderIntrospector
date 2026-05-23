#pragma once

#include <templates/template_iterator.h>

#include <GL/glew.h>

template<typename Derived, typename EntryType>
class opengl_template_iterator : public template_iterator<Derived, EntryType> {
	using Base = template_iterator<Derived, EntryType>;

protected:
	GLint program;

public:
	opengl_template_iterator() : Base() {}
	opengl_template_iterator(const std::string& name, GLint program) : Base(name), program(program) {}

	opengl_template_iterator(opengl_template_iterator&&) = default;
	opengl_template_iterator& operator=(opengl_template_iterator&&) = default;
	opengl_template_iterator(const opengl_template_iterator&) = default;
	opengl_template_iterator& operator = (const opengl_template_iterator&) = default;

	void print(std::ostream& os, int indent = 0) const override {
		os << std::string(indent, ' ') << this->name << " @ program " << program << "\n";
		for (const auto& entry : this->entries) {
			os << std::string(indent + 2, ' ') << "-> " << entry << "\n";
		}
	}
};