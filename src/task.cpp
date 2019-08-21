#include "task.hpp"

#include <sstream>

#include "internal/logging.hpp"

namespace pembroke {

    Task::Task(std::function<void()> f): m_f(f) {}

    void Task::run() const noexcept {
        try {
            m_f();
        } catch (const std::exception &ex) {
            std::stringstream ss;
            ss << "Uncaught exception encountered while running task"
               << ex.what();
            logger::error(ss.str());
        } catch (...) {
            logger::error(
                "Uncaught exception of an uknown type encountered "
                "(not defined within class heirarcy of std::exception) "
                "while running task");
        }
    }

} // namespace pembroke