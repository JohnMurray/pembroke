#include <memory>

#include "pembroke/http/common.hpp"

extern "C" {
struct evhttp_request;
}

namespace pembroke::http {

    /**
     * @brief A lazily-constructed, immutable request class.
     * 
     * ConstRequest tries to accurately represent a request that is received by a server
     * (thus is const). Values of the request (body, URI, headers, etc) are lazily
     * queried and constructed from the underlying `evhttp_request` object and are cached
     * so that subsequent lookups do not require recalculating the values.
     * 
     * @note The lifetime of the `evhttp_request` struct will be tied to the lifetime of
     *       the `ConstRequest` object
     */
    class ConstRequest {
    public:
        ConstRequest(evhttp_request *req) noexcept;
        ~ConstRequest() noexcept;

        ConstRequest(const ConstRequest &r) = delete;
        ConstRequest(ConstRequest &&r) noexcept;

        auto operator=(const ConstRequest &other) -> ConstRequest & = delete;
        auto operator=(ConstRequest &&other) noexcept -> ConstRequest &;

        // TODO: Body functions
        // TODO: Header functions
        // TODO: Uri functions

        // TODO: Request attribute functions (method, other things??)

    private:
        evhttp_request *m_req;
    };

    // TODO: Implement spec
    class Request {
        Request() noexcept;
        Request(evhttp_request *req) noexcept;
    };

} // namespace pembroke::http