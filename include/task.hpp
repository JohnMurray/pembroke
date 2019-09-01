#pragma once

#include <functional>

#include "util/func.hpp"

namespace pembroke {

    /**
     * @brief A simple task to run in the context of a Reactor
     */
    class Task {
    private:
        std::function<void()> m_f = util::nop_f;
        
    public:
        Task(std::function<void()> f);
        void run() const noexcept;
    };
} // namespace pembroke