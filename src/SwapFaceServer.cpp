#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>                       // JSON library
#include <cpprest/uri.h>                        // URI library
#include <cpprest/ws_client.h>                  // WebSocket client
#include <cpprest/containerstream.h>            // Async streams backed by STL containers
#include <cpprest/interopstream.h>              // Bridges for integrating Async streams with STL and WinRT streams
#include <cpprest/rawptrstream.h>               // Async streams backed by raw pointer to memory
#include <cpprest/producerconsumerstream.h> 

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams; 

class SwapFaceServer {
private:
    static void handle_get(http_request message);
    static void handle_put(http_request message);
    static void handle_post(http_request message);
    static void handle_delete(http_request message);

    http_listener m_listener;

public:
    SwapFaceServer();
    SwapFaceServer(utility::string_t url);
    ~SwapFaceServer();

    pplx::task<void> open() { return m_listener.open(); }
    pplx::task<void> close() { return m_listener.close(); }

};

SwapFaceServer::SwapFaceServer() {

}

SwapFaceServer::SwapFaceServer(utility::string_t url) {

}

SwapFaceServer::~SwapFaceServer(){}

void handle_get(http_request message) {

}

void handle_put(http_request message) {

}

void handle_post(http_request message) {

}

void handle_delete(http_request message) {

}

std::unique_ptr<SwapFaceServer>g_http;

int main (int argc, char *argv[]) {
    //utility::string_t port= U("34568");
    //utility::string_t adress = U("http://localhost:");

    auto fileStream = std::make_shared<ostream>();

    pplx::task<void> requestTask = fstream::open_ostream(U("results.html"))
        .then([=] (ostream outFile) {
            *fileStream = outFile;

            http_client client(U("http://www.bing.com/"));

            uri_builder builder(U("/search"));
            builder.append_query(U("q"), U("cpprestsdk github"));
            return client.request(methods::GET, builder.to_string());
        })
        .then([=] (http_response response) {
            printf("Received response status code:%u\n", response.status_code());

            return response.body().read_to_end(fileStream->streambuf());
        })
        .then([=](size_t) {
            return fileStream->close();
        });

        try {
            requestTask.wait();
        }
        catch (const std::exception &e) {
            printf("Error exception:%s\n", e.what());
        }
        return 0;
}