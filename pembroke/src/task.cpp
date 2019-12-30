#include "pembroke/task.hpp"

#include <exception>
#include <type_traits>
#include <fmt/format.h>

#include "pembroke/internal/logging.hpp"

namespace pembroke {

    void Task::run(TaskContext &ctx) const noexcept {
        try {
            std::visit([&ctx](auto &&arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, TaskFunc>) {
                    arg();
                }
                else if constexpr (std::is_same_v<T, TaskFuncWithCtx>) {
                    arg(ctx);
                }
            }, m_task_function);
        }
        catch (const std::exception &ex) {
            auto error = fmt::format("Unexpected exception occurred when executing a task: {}", ex.what());
            pembroke::logger::error(error);
        }
        catch (...) {
            pembroke::logger::error(
                "Unexpected exception of unknown type when executing a task. "
                "Detailed error information is not available"
            );
        }
    }

} // namespace pembroke