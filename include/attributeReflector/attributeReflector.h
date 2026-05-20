#pragma once

#include <GL/glew.h>
#include <ostream>
#include <string>
#include <unordered_map>

#include <toString/toString.h>

namespace shader {
	struct AttributeInfo {
		std::string name;
		GLint location;
		GLint size;
		GLenum type;
	};

	class AttributeReflector {
		std::unordered_map<std::string, AttributeInfo> attributes;

	public:
		AttributeReflector() = default;
		AttributeReflector(GLuint programId) { reflect(programId); }

		AttributeReflector(const AttributeReflector&) = delete;
		AttributeReflector& operator=(const AttributeReflector&) = delete;
		AttributeReflector(AttributeReflector&&) = default;
		AttributeReflector& operator=(AttributeReflector&&) = default;

		const AttributeInfo& operator[](const std::string& name) { return getAttribute(name); }

		void reflect(GLuint programId);

		const AttributeInfo& getAttribute(const std::string& name);
		size_t getAttributesSize() const { return attributes.size(); }

	private:
		friend std::ostream& operator<<(std::ostream& os, const AttributeReflector& reflector) {
			os << "this shader have " << reflector.getAttributesSize() << " attributes:\n";
			for (const auto& [name, attribute] : reflector.attributes) {
				os << "  " << attribute.name << ": {\n\tlocation: " << attribute.location;
				os << "\n\tsize: " << attribute.size;
				os << "\n\ttype: " << toString(attribute.type);
				os << "\n  }\n";
			}
			return os;
		}
	};
}