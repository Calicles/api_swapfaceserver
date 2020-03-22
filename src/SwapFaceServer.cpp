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
    void handle_get(http_request message);
    void handle_put(http_request message);
    void handle_post(http_request message);
    void handle_delete(http_request message);
    void handle_options(http_request message);

    http_listener m_listener;

public:
    SwapFaceServer();
    SwapFaceServer(string_t url);
    ~SwapFaceServer();

    pplx::task<void> open() { return m_listener.open(); }
    pplx::task<void> close() { return m_listener.close(); }

};

SwapFaceServer::SwapFaceServer() {

}

SwapFaceServer::SwapFaceServer(utility::string_t url) : m_listener(url) {
    m_listener.support(methods::GET, std::bind(&SwapFaceServer::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&SwapFaceServer::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::OPTIONS, std::bind(&SwapFaceServer::handle_options, this, std::placeholders::_1));
}

SwapFaceServer::~SwapFaceServer(){}

  void SwapFaceServer::handle_options(http_request message)
  {
    http_response response(status_codes::OK);
    response.headers().add(U("Allow"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    response.headers().add(U("Access-Control-Allow-Methods"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type"));
    ucout << string_t(U("handle request OPTIONS")) << std::endl;
    message.reply(response);
  }

void SwapFaceServer::handle_get(http_request message) {
    ucout << utility::string_t(U("Have a request get !!!")) << std::endl;
    ucout << message.to_string() << std::endl;
    auto paths = uri::split_path(uri::decode(message.relative_uri().path()));

    utility::string_t rep;

    std::map<string_t, string_t> query = 
        uri::split_query(uri::decode(message.request_uri().query()));

    auto index = query.find(U("index"));

    // handle one image, the second by index in uri
    if (index != query.end() && !index->second.empty()) {
        ucout << index->second << std::endl;
    
    // handle two images to swap
    } else {
        rep = U("Error");
        ucout << U("handle nothing") << std::endl;
    }
    rep = U("Hello");
    
    http_response response(status_codes::OK);
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    response.set_body(string_t(U("hello world")));
    message.reply(response);
    return;
}

void SwapFaceServer::handle_put(http_request message) {

}

void SwapFaceServer::handle_post(http_request message) {
    ucout << message.to_string() << std::endl;
    /* in view: 
    fetch(URL)
  .then(res=>{return res.blob()})
  .then(blob=>{
    var img = URL.createObjectURL(blob);
    // Do whatever with the img
    document.getElementById('img').setAttribute('src', img);
  })*/
  ucout << "1" << std::endl;
         json::value temp;
    message.extract_string()       //extracts the request content into a json
        .then([=](string_t json)
        {
            json::value v = json::value::parse(json);
            ucout << v[U("index")] << std::endl;
        });
    ucout << "2" << std::endl;
    http_response response(status_codes::OK);
    response.headers().add(U("Allow"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    response.headers().add(U("Access-Control-Allow-Methods"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type"));
    response.set_body("hello");
    message.reply(response);
    return;
}

void SwapFaceServer::handle_delete(http_request message) {

}

std::unique_ptr<SwapFaceServer>g_http;

void on_initialize(const string_t& address)
{
    // Build our listener's URI from the configured address and the hard-coded path "swapFace"

    uri_builder uri(address);
    uri.append_path(U("swapFace"));

    auto addr = uri.to_uri().to_string();

    g_http = std::unique_ptr<SwapFaceServer>(new SwapFaceServer(addr));
    g_http->open().wait();

    ucout << utility::string_t(U("Listening for request at : ")) << addr << std::endl;

    return;
}

void on_shutdown() {
    g_http->close().wait();

    ucout << utility::string_t(U("Server closed")) << std::endl;
    return;
}


int main (int argc, char *argv[]) {
    utility::string_t port= U("34568");
    utility::string_t address = U("http://localhost:");
    address.append(port);

    on_initialize(address);

    std::cout << "Press ENTER to exit." << std::endl;
    std::string line;
    std::getline(std::cin, line);

    on_shutdown();

    return 0;
}
