#include <attributeReflector/attributeReflector.h>

#include <iostream>
#include <vector>
#include <format>

namespace shader {
	void attribute_reflector::reflect(GLint programId) {
		GLint numAttributes = 0;
		glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &numAttributes);
		if (numAttributes == 0) return;

		GLint maxNameLength = 0;
		glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLength);
		std::vector<GLchar> nameBuffer(maxNameLength);

		for (GLint i = 0; i < numAttributes; i++) {
			GLsizei length = 0;
			GLint elements = 0;
			GLenum type = 0;

			glGetActiveAttrib(programId, i, nameBuffer.size(), &length, &elements, &type, nameBuffer.data());

			std::string name(nameBuffer.data(), length);

			if (elements > 1) {
				name = name.substr(0, name.find("[0]"));
				attributes[name] = attribute_iterator(std::format("{} ({}[{}])", name, toString(type), elements), programId);

				for (size_t i = 0; i < elements; i++) {
					std::string iter_name = std::format("{}[{}]", name, i);
					GLint location = glGetAttribLocation(programId, iter_name.c_str());
					attributes[name].add_entry(attribute_handle(std::format("[{}]", i), location, type));
				}
			}
			else {
				attributes[name] = attribute_iterator(std::format("{} ({})", name, toString(type)), programId);
				GLint location = glGetAttribLocation(programId, name.c_str());
				attributes[name].add_entry(attribute_handle(name, location, type));
			}
		}
	}

	const attribute_iterator& attribute_reflector::operator[](const std::string& name) const {
		auto iter = attributes.find(name);
		if (iter == attributes.end()) {
			std::cerr << ("this attribute doesn't exist in this shader: " + name);
			return attribute_iterator();
		}
		return iter->second;
	}

	const attribute_iterator& attribute_reflector::get_attribute(const std::string& name) const {
		auto iter = attributes.find(name);
		if (iter == attributes.end()) throw std::out_of_range(("this attribute doesn't exist in this shader: " + name));
		return iter->second;
	}

	std::ostream& operator<<(std::ostream& os, const attribute_reflector& reflector) {
		os << "=== Shader Attributes ===\n";
		if (reflector.attributes.empty()) {
			os << "  (no uniforms)\n";
			return os;
		}

		os << "\n";
		size_t count = 0;
		for (const auto& [name, unit] : reflector.attributes) {
			os << unit << '\n';
			if (++count < reflector.attributes.size()) {
				os << "";
			}
		}
		os << "\n";
		return os;
	}
}
