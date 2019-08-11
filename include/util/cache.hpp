namespace event::util {

    template<typename T>
    class Memoize {
        Memoize(): m_value(T{}), m_is_calculated(false) {}
        Memoize(T t): m_value(t), m_is_calculated(true) {}
        Memoize(const T &t): m_value(t), m_is_calculated(true) {}

        ~Memoize() = default;

        /* Not move'able or copy'able because we can't make assumptions
           about the underlying type */
        Memoize(const Memoize &) = delete;
        Memoize(const Memoize &&) = delete;
        Memoize &operator=(const Memoize &) = delete;
        Memoize &operator=(const Memoize &&) = delete;

        void cache(T t) noexcept {
            m_value = t;
        }
        void cache(const T &t) noexcept {
            m_value = t;
        }

        [[nodiscard]]
        bool has_value() const noexcept {
            return m_is_calculated;
        }

        const T &value() const noexcept {
            return m_value;
        }
    private:
        bool m_is_calculated = false;
        T m_value;
    };

} // event::util