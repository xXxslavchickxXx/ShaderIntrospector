#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <iostream>
#include <ostream>

#include <toString/toString.h>
#include <glm/glm.hpp>
#include <uniformReflector/uniform/uniform_entry.h>

#include <tools.h>

namespace shader {
	class uniform_reflector {
		std::unordered_map<std::string, uniform_entry> uniforms;

	public:
		uniform_reflector(GLuint id);

		const uniform_entry& operator[](const std::string& name) const;
		const uniform_entry& getUniform(const std::string& name) const;
		uniform_entry& operator[](const std::string& name);
		uniform_entry& getUniform(const std::string& name);

	public:
		uniform_reflector& operator=(const uniform_reflector&) = default;
		uniform_reflector(const uniform_reflector&) = default;
		uniform_reflector& operator=(uniform_reflector&&) = default;
		uniform_reflector(uniform_reflector&&) = default;

	private:
		void reflectUniforms(GLint newId);
		friend std::ostream& operator<<(std::ostream& os, const uniform_reflector& reflector);
	};
}