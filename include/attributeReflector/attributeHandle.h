#pragma once

#include <templates/opengl_iterator.h>
#include <tools.h>

namespace shader {
	struct attribute_handle {
		std::string name;
		GLint location;
		GLenum type;

	public:
		attribute_handle(const std::string& name, GLint loc, GLenum type)
			: name(name), location(loc), type(type) {}

		friend std::ostream& operator<<(std::ostream& os, const attribute_handle& entry) {
			return os << entry.name << " (loc=" << entry.location << ")";
		}
	};

	class attribute_iterator : public opengl_template_iterator<attribute_iterator, attribute_handle> {
		using Base = opengl_template_iterator<attribute_iterator, attribute_handle>;

	public:
		using Base::Base;
	};
}