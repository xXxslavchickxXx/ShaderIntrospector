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

	class attribute_reflector {
		std::unordered_map<std::string, AttributeInfo> attributes;

	public:
		attribute_reflector(GLuint programId) { reflect(programId); }

		attribute_reflector(const attribute_reflector&) = delete;
		attribute_reflector& operator=(const attribute_reflector&) = delete;
		attribute_reflector(attribute_reflector&&) = default;
		attribute_reflector& operator=(attribute_reflector&&) = default;

		const AttributeInfo& operator[](const std::string& name) const;
		const AttributeInfo& getAttribute(const std::string& name) const;

		void reflect(GLuint programId);

		size_t getAttributesSize() const { return attributes.size(); }

		const auto& get_attributes() const { return attributes; }
		// Для const итерации
		auto begin() const { return attributes.begin(); }
		auto end() const { return attributes.end(); }
		auto cbegin() const { return attributes.cbegin(); }
		auto cend() const { return attributes.cend(); }

		// Для non-const итерации
		auto begin() { return attributes.begin(); }
		auto end() { return attributes.end(); }

	private:
		friend std::ostream& operator<<(std::ostream& os, const attribute_reflector& reflector) {
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