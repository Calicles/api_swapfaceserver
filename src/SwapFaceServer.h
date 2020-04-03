#ifndef SWAP_FACE_SERVER
#define SWAP_FACE_SERVER

#include <cpprest/http_listener.h>              // HTTP server

#define BRAD "./bradPitt.jpg"
#define TRUMP "./donaled_trump"
#define CLINTON "./hillary_clinton.jpg"

#define IMAGES U("images")
#define SWAP U("swap")

class SwapFaceServer {
private:
    void handle_get(web::http::http_request message);
    void handle_put(web::http::http_request message);
    void handle_post(web::http::http_request message);
    void handle_delete(web::http::http_request message);
    void handle_options(web::http::http_request message);
    void SendResponse(const bool responseOk = true, const std::vector<unsigned char> &data, http_request &message);

    web::http::experimental::listener::http_listener m_listener;

public:
    SwapFaceServer();
    SwapFaceServer(utility::string_t url);
    ~SwapFaceServer();

    pplx::task<void> open() { return m_listener.open(); }
    pplx::task<void> close() { return m_listener.close(); }

};


#endif