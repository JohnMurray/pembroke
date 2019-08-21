#pragma once

#include <functional>

namespace pembroke {

    constexpr auto nop_f = []() -> void {};

    /**
     * @brief A simple task to run in the context of a Reactor
     */
    class Task {
    private:
        std::function<void()> m_f = nop_f;
        
    public:
        Task(std::function<void()> f);
        void run() const noexcept;
    };
} // namespace pembroke