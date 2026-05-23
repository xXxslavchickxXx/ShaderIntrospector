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

template<typename Derived, typename EntryType>
class template_iterator {
protected:
    std::vector<EntryType> entries;
    std::string name;

public:
    template_iterator() = default;
    template_iterator(const std::string& name) : name(name) {}
    
    template_iterator(template_iterator&&) = default;
    template_iterator& operator=(template_iterator&&) = default;
    template_iterator(const template_iterator&) = default;
    template_iterator& operator=(const template_iterator&) = default;

    void add_entry(EntryType&& entry) { entries.push_back(std::move(entry)); }

    size_t size() const { return entries.size(); }
    const EntryType& operator[](size_t i) const { return entries[i]; }
    EntryType& operator[](size_t i) { return entries[i]; }

    auto begin() { return entries.begin(); }
    auto end() { return entries.end(); }
    auto begin() const { return entries.begin(); }
    auto end() const { return entries.end(); }

    // Вывод
    virtual void print(std::ostream& os, int indent = 0) const {
        os << std::string(indent, ' ') << name << ":\n";
        for (const auto& entry : entries) {
            os << std::string(indent + 2, ' ') << "-> " << entry << "\n";
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const template_iterator& it) {
        it.print(os);
        return os;
    }
};