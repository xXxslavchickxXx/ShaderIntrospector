#pragma once

#include <templates/template_iterator.h>

#include <GL/glew.h>

// Концептик на проверку, есть ли в методе оператор[string]
template<typename EntryType>
concept HasStringOperator = requires(EntryType & e) {
	e[std::string{}];
};
// Концепт на проверку конвертации текущего типа в стринг
template<typename T>
concept StringLike = std::is_convertible_v<T, std::string>;

template<typename Derived, typename EntryType>
class opengl_template_iterator : public template_iterator<Derived, EntryType> {
	using Base = template_iterator<Derived, EntryType>;

protected:
	using Base::entries;
	GLint program;

	template<StringLike T>
	auto& operator[](T&& name) {
		return this->entries[0][std::forward<T>(name)];
	}
	template<StringLike T>
	const auto& operator[](T&& name) const {
		return this->entries[0][std::forward<T>(name)];
	}

	auto& operator[](size_t index) {
		if (index >= entries.size()) {
			throw std::out_of_range("OTI out_of_range: size_t");
		}
		return this->entries[index];
	}

	const auto& operator[](size_t index) const {
		if (index >= entries.size()) {
			throw std::out_of_range("OTI out_of_range: size_t");
		}
		return this->entries[index];
	}

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