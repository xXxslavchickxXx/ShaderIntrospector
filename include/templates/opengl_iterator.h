#pragma once

#include <templates/template_iterator.h>

#include <GL/glew.h>

template<typename Derived, typename EntryType>
class opengl_template_iterator : public template_iterator<Derived, EntryType> {
	using Base = template_iterator<Derived, EntryType>;

protected:
	using Base::entries;
	GLint program;

public:
	opengl_template_iterator() : Base(), program(-1) {}
	opengl_template_iterator(const std::string& name, GLint program = -1) : Base(name), program(program) {}

	opengl_template_iterator(opengl_template_iterator&&) = default;
	opengl_template_iterator& operator=(opengl_template_iterator&&) = default;
	opengl_template_iterator(const opengl_template_iterator&) = default;
	opengl_template_iterator& operator = (const opengl_template_iterator&) = default;
	
	GLint byte_size() const {
		GLint common_size = 0;
		for (auto entry : this->entries) {
			common_size += entry.byte_size;
		}
		return common_size;
	}
	size_t size() const { return this->entries.size(); }

	void print(std::ostream& os, int indent = 0) const {
		os << std::string(indent, ' ') << this->name << " @ program " << program << "\n";
		for (const auto& entry : this->entries) {
			os << std::string(indent + 2, ' ') << "-> " << entry << "\n";
		}
	}

	friend std::ostream& operator<<(std::ostream& os, opengl_template_iterator<Derived, EntryType>& it) {
		it.print(os);
		return os;
	}
};