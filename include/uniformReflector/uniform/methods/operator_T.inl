namespace shader {
    template<typename T>
    uniform_entry::operator T() const {
        return get<T>();
    }
}