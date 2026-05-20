#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace shader {
	template<typename T> inline constexpr GLenum mapGlType = 0;

	template<>
	inline constexpr GLenum mapGlType<float> = GL_FLOAT;

	template<>
	inline constexpr GLenum mapGlType<int> = GL_INT;

	template<>
	inline constexpr GLenum mapGlType<unsigned int> = GL_UNSIGNED_INT;

	template<>
	inline constexpr GLenum mapGlType<bool> = GL_BOOL;

	template<>
	inline constexpr GLenum mapGlType<glm::vec2> = GL_FLOAT_VEC2;

	template<>
	inline constexpr GLenum mapGlType<glm::vec3> = GL_FLOAT_VEC3;

	template<>
	inline constexpr GLenum mapGlType<glm::vec4> = GL_FLOAT_VEC4;

	template<>
	inline constexpr GLenum mapGlType<glm::ivec2> = GL_INT_VEC2;

	template<>
	inline constexpr GLenum mapGlType<glm::ivec3> = GL_INT_VEC3;

	template<>
	inline constexpr GLenum mapGlType<glm::ivec4> = GL_INT_VEC4;

	template<>
	inline constexpr GLenum mapGlType<glm::uvec2> = GL_UNSIGNED_INT_VEC2;

	template<>
	inline constexpr GLenum mapGlType<glm::uvec3> = GL_UNSIGNED_INT_VEC3;

	template<>
	inline constexpr GLenum mapGlType<glm::uvec4> = GL_UNSIGNED_INT_VEC4;

	template<>
	inline constexpr GLenum mapGlType<glm::bvec2> = GL_BOOL_VEC2;

	template<>
	inline constexpr GLenum mapGlType<glm::bvec3> = GL_BOOL_VEC3;

	template<>
	inline constexpr GLenum mapGlType<glm::bvec4> = GL_BOOL_VEC4;

	template<>
	inline constexpr GLenum mapGlType<glm::mat2> = GL_FLOAT_MAT2;

	template<>
	inline constexpr GLenum mapGlType<glm::mat3> = GL_FLOAT_MAT3;

	template<>
	inline constexpr GLenum mapGlType<glm::mat4> = GL_FLOAT_MAT4;

	template<>
	inline constexpr GLenum mapGlType<glm::mat2x3> = GL_FLOAT_MAT2x3;

	template<>
	inline constexpr GLenum mapGlType<glm::mat2x4> = GL_FLOAT_MAT2x4;

	template<>
	inline constexpr GLenum mapGlType<glm::mat3x2> = GL_FLOAT_MAT3x2;

	template<>
	inline constexpr GLenum mapGlType<glm::mat3x4> = GL_FLOAT_MAT3x4;

	template<>
	inline constexpr GLenum mapGlType<glm::mat4x2> = GL_FLOAT_MAT4x2;

	template<>
	inline constexpr GLenum mapGlType<glm::mat4x3> = GL_FLOAT_MAT4x3;
}