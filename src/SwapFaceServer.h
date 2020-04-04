#ifndef SWAP_FACE_SERVER
#define SWAP_FACE_SERVER

#include <cpprest/http_listener.h>              // HTTP server
#include <iostream>

#define BRAD "./bradPitt.jpg"
#define TRUMP "./donaled_trump"
#define CLINTON "./hillary_clinton.jpg"
#define WONDER_WOMAN "./wonder_woman.jpg"
#define COUNT U("images_count")
#define IMG_COUNT 4

#define IMAGES U("images")
#define SWAP U("swap")

#define OK status_codes::OK
#define NOT_FOUND 404
#define EXT_ERROR 400
#define INTERN_ERROR 500

class SwapFaceServer {
private:
    void handle_get(web::http::http_request message);
    void handle_put(web::http::http_request message);
    void handle_post(web::http::http_request message);
    void handle_delete(web::http::http_request message);
    void handle_options(web::http::http_request message);
    void addHeaders(web::http::http_response &response);
    void sendResponse(const std::vector<unsigned char> &data, web::http::http_request &message);
    void sendError(size_t error_code, utility::string_t msg, web::http::http_request &message);
    std::string getImageNameByIndex(utility::string_t indexStr);

    web::http::experimental::listener::http_listener m_listener;

public:
    SwapFaceServer();
    SwapFaceServer(utility::string_t url);
    ~SwapFaceServer();

    pplx::task<void> open() { return m_listener.open(); }
    pplx::task<void> close() { return m_listener.close(); }

};

#endif