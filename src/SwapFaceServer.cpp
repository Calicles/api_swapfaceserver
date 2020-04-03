#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>                       // JSON library
#include <cpprest/uri.h>

#include <vector>    
#include <fstream>

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

void SwapFaceServer::sendResponse(const bool responseOk, const std::vector<unsigned char> &data, http_request &message) {
    int statusCode(status_codes::OK);
    if (!responseOk) {
        statusCode = status_codes::FAILED;
    }
    http_response response(statusCode);
    response.headers().add(U("Allow"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    response.headers().add(U("Access-Control-Allow-Methods"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type"));
    if (data != null && date.size() > 0) {
        response.headers().add(U("Content-Type"), U("aplication/octet-stream"));
        response.headers().add(U("Content-Disposition"), U("inline; filename=res.png")); // or attachment otherwise inline
        response.set_body(bytes);
        message.reply(response);
    }
    massage.reply(response);
}

bool getPictureNameByIndex(const size_t index, std::string &dest) {
    switch (index) {
        case "1":
            dest = BRAD;
        return true;

        case "2":
            dest = TRUMP;
        return true;

        case "3":
            dest = CLINTON;
        return true;

        case "4":
        return true;
    }
    return false;
}

  void SwapFaceServer::handle_options(http_request message)
  {
    this->sendResponse(true, null, message);
    return;
  }

void SwapFaceServer::handle_get(http_request message) {
    std::vector<unsigned char> bytes;
    bool succes(false);
    std::string filePath;
    std::vector<string_t> paths = http_request::split_path(
        message.relative_uri().path()
    );
    // Request to have a picture
    if (paths[0] == IMAGES && this->getPictureNameByIdex(paths[1], filePath)) {
            std::ifstream ifs(filePath.c_str(), ios_base::in);
            unsigned char c = 0;

            while (ifs.get(c)) {
                bytes.push_back(c);
            }
            success = true;
    // request to swap from two pictures by indexes
    } else if (paths[0] == SWAP) {
        std::string filePath2;
        if (this->getPictureNameByIndex(paths[1], filePath) && this->getPictureNameByIndex(paths[2], filePath2)) {
            FaceSwapper swapper(filePath, filePath2);
            swapper.process_swap();
            swapper.copyImageSwappedTo(bytes);
            success = true;
        }
    // ununderstood request
    } else {
        success = false;
    }
    this->sendResponse(success, bytes, message);
    return;
}

void SwapFaceServer::handle_post(http_request message) {
    message.extract_json()       
        .then([=](json::value json)
        {
            std::vector<unsigned char> bytes;
            bool succes(false);

            try{
                //json::value v = json::value::parse(json);
                json::array jsonArray = value[U("image")].as_array();

                size_t size = jsonArray.size();
                bytes.reserve(size);
                size_t width(v[U("imageWidth")].as_integer());
                size_t height(v[U("imageHeight")].as_integer());

                for (size_t i = 0; i < size; i++) {
                    bytes.push_back(jsonArray[i].as_integer());
                }
                FaceSwapper swapper(bytes, "./brad-pitt-acteur.jpg");
                swapper.process_swap();
                swapper.copyImgSwappedTo(bytes);
                success = true;
            }catch (Exception e) {
                ucout << U("exception thrown") << e.what() << std::endl;
            }
            this->SendResponse(success, bytes, message);
        }).wait();
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

