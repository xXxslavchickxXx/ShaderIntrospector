#pragma once

#include <GL/glew.h>
#include <ostream>
#include <string>
#include <unordered_map>

#include <toString/toString.h>
#include <attributeReflector/attribute/attribute_entry.h>

namespace shader {
	class attribute_reflector {
		std::unordered_map<std::string, attribute_entry> attributes;

	public:
		attribute_reflector(GLint programId) { reflect(programId); }

		attribute_reflector(const attribute_reflector&) = delete;
		attribute_reflector& operator=(const attribute_reflector&) = delete;
		attribute_reflector(attribute_reflector&&) = default;
		attribute_reflector& operator=(attribute_reflector&&) = default;

		const attribute_entry& operator[](const std::string& name) const;
		const attribute_entry& getAttribute(const std::string& name) const;

		void reflect(GLint programId);

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
		friend std::ostream& operator<<(std::ostream& os, const attribute_reflector& reflector);
	};
}