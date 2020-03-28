#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>                       // JSON library
#include <cpprest/uri.h>

#include <vector>    
#include <stdio.h>

#include "FaceSwapper.h"
#include "base64.h"
#include "SwapFaceServer.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;
using namespace web::http;                  // Common HTTP functionality

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
    http_response response(status_codes::OK);
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    response.set_body(string_t(U("hello world")));
    message.reply(response);
    return;
}

void SwapFaceServer::handle_post(http_request message) {
    //ucout << message.to_string() << std::endl;
    

    message.extract_string()       
        .then([=](string_t json)
        {
            json::value v = json::value::parse(json);
            json::array jsonArray = v[U("image")].as_array();

            int size = jsonArray.size();
            this->bytes.reserve(size);

            size_t width(v[U("imageWidth")].as_integer());
            size_t height(v[U("imageHeight")].as_integer());

            for (int i = 0; i < size; i++) {
                this->bytes.push_back(jsonArray[i].as_integer());
                if (i == size -1) {
                    int nbr= this->bytes.back();
                }
            }
            /*FaceSwapper swapper(width, height, this->bytes, "./donald_trump.jpg");
    ucout << "in before" << std::endl;
            swapper.process_swap();
           swapper.copyImgSwappedTo(this->bytes);*/
        }).wait();
    /*
    size_t n = this->bytes.size();
    json::value answer;
    json::value img = answer.array(n);
    ucout << n << std::endl;
    for (size_t i = 0; i < n; i++) {
        //img[i] = json::value::number(this->bytes[i]);
        string_t byte = utility::conversions::to_base64(this->bytes[i]);
        img[i] = json::value::string(byte);
    }
    //json::value JSON;
    //JSON[U("image")] = img;
    //std::stringstream ssbuff;
    //std::copy(this->bytes.begin(), this->bytes.end(), std::ostream_iterator<unsigned char>(ssbuff, ""));

    /*std::ifstream InFile( "./test.png", std::ifstream::binary );
    std::vector<char> data( ( std::istreambuf_iterator<char>( InFile ) ), std::istreambuf_iterator<char>() );

    string_t res("");
    for (size_t i = 0; i < data.size(); i++) {
        res += utility::conversions::to_base64(data[i]);
    }
    std::string code = base64_encode((unsigned char*)&data[0], (unsigned int)data.size());

    json::value obj = json::value::string(code);
    
    string_t url = U("/home/antoine/dev/c++Appli/rest/API_swapFace/test.png");
    */
    http_response response(status_codes::OK);
    response.headers().add(U("Allow"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    response.headers().add(U("Access-Control-Allow-Methods"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type"));
    response.set_body(this->bytes);
    message.reply(response);
    ucout << "in after" << std::endl;
    return;
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

