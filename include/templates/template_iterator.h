#pragma once

#include <GL/glew.h>
#include <string>
#include <consoleTools.h>
#include <iostream>
#include <vector>

template<typename Derived, typename ContainerType, typename OperArg, typename EntryType>
class template_container_iterator {
protected:
    ContainerType entries;
public:
    std::string name;

public:
    using entry_type = EntryType;

    template_container_iterator() = default;
    template_container_iterator(const std::string& name) : name(name) {}

    template_container_iterator(template_container_iterator&&) = default;
    template_container_iterator& operator=(template_container_iterator&&) = default;
    template_container_iterator(const template_container_iterator&) = default;
    template_container_iterator& operator=(const template_container_iterator&) = default;

    size_t size() const { return entries.size(); }

    const EntryType& operator[](OperArg i) const {
        if constexpr (std::is_integral_v<OperArg>) {
            // Для индексов (size_t)
            if (i >= entries.size()) {
                std::cerr << "Index out of range (size=" << entries.size() << ")" << std::endl;
            }
            return entries[i];
        }
        else {
            // Для строковых ключей (const std::string&)
            auto it = entries.find(i);
            if (it == entries.end()) {
                std::cerr << "Key not found: " << i << std::endl;
                static EntryType empty;
                return empty;
            }
            return it->second;
        }
    }

    EntryType& operator[](OperArg i) {
        if constexpr (std::is_integral_v<OperArg>) {
            if (i >= entries.size()) {
                std::cerr << "Index out of range (size=" << entries.size() << ")" << std::endl;
            }
            return entries[i];
        }
        else {
            auto it = entries.find(i);
            if (it == entries.end()) {
                std::cerr << "Key not found: " << i << std::endl;
                static EntryType empty;
                return empty;
            }
            return it->second;
        }
    }

    auto begin() { return entries.begin(); }
    auto end() { return entries.end(); }
    auto begin() const { return entries.begin(); }
    auto end() const { return entries.end(); }

    virtual void print(std::ostream& os, int indent = 0) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const template_container_iterator& it) {
        it.print(os);
        return os;
    }
};

template<typename Derived, typename EntryType>
class template_iterator : public template_container_iterator<Derived, std::vector<EntryType>, size_t, EntryType>
{
    using Base = template_container_iterator<Derived, std::vector<EntryType>, size_t, EntryType>;

public:
    using Base::Base;

    EntryType* operator->() { return &this->entries[0]; }
    operator EntryType& () { return this->entries[0]; }

    void add_entry(EntryType&& entry) { this->entries.push_back(std::forward<EntryType>(entry)); }
};