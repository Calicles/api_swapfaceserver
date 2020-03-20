

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

class SwapFaceServer {
private:
    static void handle_get(http_request message);
    static void handle_put(http_request message);
    static void handle_post(http_request message);
    static void handle_delete(http_request message);

    http_listener m_listener;

public:
    SwapFaceServer();
    SwapFaceServer(utility::string url);
    ~SwapFaceServer();

    pplx::task<void> open() { return m_listener.open(); }
    pplx::task<void> close() { return m_listener.close(); }

};

std::unique_ptr<SwapFaceServer>g_http;

int main (int argc, char *argv[]) {
    utility::string_t port= U("34568");
    utility::string_t adress = U("http://localhost:");

    
}