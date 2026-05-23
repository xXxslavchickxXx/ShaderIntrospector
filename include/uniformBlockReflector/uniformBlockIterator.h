#pragma once

namespace shader {
	class uniform_block_iterator {


	public:
        const uniform_block_iterator operator[](size_t index) const;
        const uniform_block_iterator at(size_t index = 0) const;

        uniform_block_iterator operator[](size_t index);
        uniform_block_iterator at(size_t index = 0);

        size_t size() const { return elements; }

        uniform_block_iterator(const uniform_block_iterator&) = default;
        uniform_block_iterator& operator=(const uniform_block_iterator&) = default;
        uniform_block_iterator(uniform_block_iterator&&) = default;
        uniform_block_iterator& operator=(uniform_block_iterator&&) = default;

        template<typename T>
        operator T() const;

        template<typename T>
        uniform_block_iterator& operator=(T&& value) {
            set(std::forward<T>(value));
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const uniform_entry& entry);
	};
}