#include <attributeReflector/attributeReflector.h>

#include <iostream>
#include <vector>

namespace shader {
	void AttributeReflector::reflect(GLuint programId) {
		GLint numAttributes = 0;
		glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &numAttributes);
		if (numAttributes == 0) return;

		GLint maxNameLength = 0;
		glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLength);
		std::vector<GLchar> nameBuffer(maxNameLength);

		for (GLint i = 0; i < numAttributes; i++) {
			GLsizei length = 0;
			GLint size = 0;
			GLenum type = 0;

			glGetActiveAttrib(programId, i, nameBuffer.size(), &length, &size, &type, nameBuffer.data());

			std::string name(nameBuffer.data(), length);
			GLint location = glGetAttribLocation(programId, name.c_str());

			attributes[name] = AttributeInfo{ name, location, size, type };
		}
	}

	const AttributeInfo& AttributeReflector::operator[](const std::string& name) const {
		auto iter = attributes.find(name);
		if (iter == attributes.end()) {
			std::cerr << ("this attribute doesn't exist in this shader: " + name);
			return AttributeInfo();
		}
		return iter->second;
	}

	const AttributeInfo& AttributeReflector::getAttribute(const std::string& name) const {
		auto iter = attributes.find(name);
		if (iter == attributes.end()) throw std::out_of_range(("this attribute doesn't exist in this shader: " + name));
		return iter->second;
	}
}
