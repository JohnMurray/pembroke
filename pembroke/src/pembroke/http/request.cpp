#include "pembroke/http/request.hpp"

extern "C" {
#include <event2/buffer.h>
#include <event2/http.h>
}

namespace pembroke::http {
    // ---
    // ConstRequest Implementation
    // ---
    ConstRequest::ConstRequest(evhttp_request *req) noexcept
        : m_req(req) {}

    ConstRequest::~ConstRequest() noexcept {
        if (m_req != nullptr) {
            evhttp_request_free(m_req);
        }
    }

    ConstRequest::ConstRequest(ConstRequest &&other) noexcept {
        this->m_req = other.m_req;
        other.m_req = nullptr;
    }

    ConstRequest &ConstRequest::operator=(ConstRequest &&other) noexcept {
        this->m_req = other.m_req;
        other.m_req = nullptr;
        return *this;
    }
} // namespace pembroke::http