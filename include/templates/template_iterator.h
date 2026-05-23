#pragma once

#include <GL/glew.h>
#include <string>
#include <templates/consoleTools.h>
#include <iostream>
#include <vector>

class iterable_base {
public:
    virtual ~iterable_base() = default;
    virtual size_t size() const = 0;
    virtual std::string get_name(size_t index) const = 0;
    virtual void print(std::ostream& os, size_t index, int indent) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const iterable_base& obj) {
        auto cursor = get_cursor_position();
        obj.print_title(os);

        for (size_t i = 0; i < obj.size(); i++) {
            os << std::string(cursor.x, ' ') << "-> " << obj.get_name(i) << "\n";
            obj.print(os, i, cursor.x + 4);
        }
        return os;
    }

protected:
    virtual void print_title(std::ostream& os) const {
        os << "--- iterable object ---\n";
    }
};

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

    const EntryType& operator[](OperArg i) const { return entries[i]; }
    EntryType& operator[](OperArg i) { return entries[i]; }

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

    void add_entry(EntryType&& entry) { this->entries.push_back(std::forward<EntryType>(entry)); }
};