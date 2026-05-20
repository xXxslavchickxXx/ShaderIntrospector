#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <iostream>
#include <ostream>

#include <toString/toString.h>
#include <glm/glm.hpp>

#include <uniformReflector/uniform/uniform_entry.h>

namespace shader {
	class UniformReflector {
		mutable std::unordered_map<std::string, uniform_entry> uniforms;

	public:
		UniformReflector() = default;
		UniformReflector(GLuint id);

		const uniform_entry& operator[](const std::string& name) { return getUniform(name); }
		const uniform_entry& getUniform(const std::string& name) const;
		
		void reflectUniforms(GLint newId);

	public:
		UniformReflector& operator=(const UniformReflector&) = delete;
		UniformReflector(const UniformReflector&) = delete;
		UniformReflector& operator=(UniformReflector&&) = default;
		UniformReflector(UniformReflector&&) = default;

	private:
		void setDefaults();
		void setDefault(GLint loc, GLenum type);
		void checkUniformValue(const std::string& name, GLenum type);

	private:
		friend std::ostream& operator<<(std::ostream& os, const UniformReflector& reflector) {
			os << "this shader have " << reflector.uniforms.size() << " uniforms:\n";
			for (const auto& [name, unit] : reflector.uniforms) {
				auto iter = 0;
				while (iter < unit.elements) {
					const auto unit_el = reflector.getUniform(name).at(iter);
					os << "  " << unit_el.name << ": {\n\tlocation: " << unit_el.location;
					os << "\n\telements: " << unit_el.elements;
					os << "\n\ttype: " << toString(unit_el.type);
					os << "\n\tvalue: " << unit_el;
					os << "\n  }\n";
					iter++;
				}
			}
			return os;
		}

		template<typename T, typename F>
		void setUniform(const std::string& name, const T& value, F&& glFunc) {
			auto it = uniforms.find(name);
			if (it == uniforms.end()) {
				std::cerr << "Uniform not found: " << name << std::endl;
				return;
			}

			GLint loc = it->second.location;
			glFunc(loc, value);
		}
	};
}