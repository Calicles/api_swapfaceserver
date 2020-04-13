#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>                       // JSON library
#include <cpprest/uri.h>

#include <vector>    
#include <fstream>
#include <iostream>
#include <sstream>


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

void SwapFaceServer::addHeaders(http_response &response) {
    response.headers().add(U("Allow"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    response.headers().add(U("Access-Control-Allow-Methods"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type"));
}

void SwapFaceServer::sendResponse(const std::vector<unsigned char> &data, http_request &message) {
    http_response response(OK);
    this->addHeaders(response);
    response.headers().add(U("Content-Type"), U("aplication/octet-stream"));
    response.headers().add(U("Content-Disposition"), U("inline; filename=res.png")); // or attachment otherwise inline
    response.set_body(data);
    message.reply(response);
}

void SwapFaceServer::sendError(size_t error_code, string_t msg, http_request &message) {
    http_response response(error_code);
    this->addHeaders(response);
    response.set_body(msg);
    message.reply(response);
}

std::string SwapFaceServer::getImageNameByIndex(string_t indexStr) {
    size_t index(0);
    std::istringstream(indexStr) >> index;
    switch (index) {
        case 1:
            return BRAD;

        case 2:
            return TRUMP;

        case 3:
            return THE_ROCK;

        case 4:
            return WONDER_WOMAN;
    }
    return "";
}

  void SwapFaceServer::handle_options(http_request message)
  {
    http_response response(status_codes::OK);
    this->addHeaders(response);
    message.reply(response);
    return;
  }

void SwapFaceServer::handle_get(http_request message) {
    std::vector<unsigned char> bytes;
    std::string filePath;
    std::vector<string_t> paths = web::uri::split_path(
        message.relative_uri().path()
    );

    // Request to have a picture
    if (paths[0] == IMAGES && !(filePath = this->getImageNameByIndex(paths[1])).empty()) {
            std::ifstream ifs(filePath.c_str(), std::ios_base::in);
            unsigned char c = ifs.get();

            while (ifs.good()) {
                bytes.push_back(c);
                c = ifs.get();
            }
        this->sendResponse(bytes, message);

    // request to swap from two images by indexes
    } else if (paths[0] == SWAP) {
        std::string filePath2;
        if (!(filePath = this->getImageNameByIndex(paths[1])).empty() &&
                !(filePath2 = this->getImageNameByIndex(paths[2])).empty()) {
            FaceSwapper swapper(filePath, filePath2);
            swapper.process_swap();
            swapper.copyImgSwappedTo(bytes);
            this->sendResponse(bytes, message);
        }else {
            this->sendError(EXT_ERROR, U("image not found"), message);
        }

    // Request for images count
    }else if (paths[0] == COUNT) {
        json::value json;
        json[U("count")] = IMG_COUNT;
        http_response response(OK);

        this->addHeaders(response);
        response.set_body(json);
        message.reply(response);

    // ununderstood request
    } else {
        this->sendError(EXT_ERROR, U("request not understood"), message);
    }
    return;
}

void SwapFaceServer::handle_post(http_request message) {
    message.extract_json()       
        .then([this, &message](json::value json)
        {
            std::vector<unsigned char> bytes;

            try{
                //json::value v = json::value::parse(json);
                json::array jsonArray = json[U("image")].as_array();
                string_t image2Name = json[U("image2Name")].as_string();

                size_t size = jsonArray.size();
                bytes.reserve(size);
                size_t width(json[U("imageWidth")].as_integer());
                size_t height(json[U("imageHeight")].as_integer());

                for (size_t i = 0; i < size; i++) {
                    bytes.push_back(jsonArray[i].as_integer());
                }
                FaceSwapper swapper(bytes, image2Name);
                swapper.process_swap();
                swapper.copyImgSwappedTo(bytes);
                this->sendResponse(bytes, message);
            }catch (std::exception e) {
                string_t msg(U("Intern error "));
                msg.append(e.what());
                this->sendError(INTERN_ERROR, msg, message);
            }
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

