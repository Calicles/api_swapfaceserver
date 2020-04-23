#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>                       // JSON library
#include <cpprest/uri.h>

#include <vector>    
#include <fstream>
#include <iostream>
#include <sstream>


#include "FaceSwapper.h"
#include "SwapFaceServer.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;
using namespace web::http;                  // Common HTTP functionality

/**
 * Constructor with the URL to listen.
 * 
 * @param url to listen
 */
SwapFaceServer::SwapFaceServer(utility::string_t url) : m_listener(url) {
    m_listener.support(methods::GET, std::bind(&SwapFaceServer::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&SwapFaceServer::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::OPTIONS, std::bind(&SwapFaceServer::handle_options, this, std::placeholders::_1));
}

SwapFaceServer::~SwapFaceServer(){}

/**
 * Add the CORS headers to a response.
 * Prevent a CORS error.
 * 
 * @param response the reference to the response to decorate
 */
void SwapFaceServer::addHeaders(http_response &response) {
    response.headers().add(U("Allow"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    response.headers().add(U("Access-Control-Allow-Methods"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type"));
}

/**
 * Send a response to the request.
 * Add the Headers for CORS.
 * 
 * @param data the bytes array of the image to send
 * @param message the request
 */
void SwapFaceServer::sendResponse(const std::vector<unsigned char> &data, http_request &message) {
    http_response response(OK);
    this->addHeaders(response);
    response.headers().add(U("Content-Type"), U("aplication/octet-stream"));
    response.headers().add(U("Content-Disposition"), U("inline; filename=res.png")); // or attachment otherwise inline
    response.set_body(data);
    message.reply(response);
}

/**
 * Send a response with error code.
 * 
 * @see sendResponse
 * @param error_code the code of the error occured
 * @param message
 */
void SwapFaceServer::sendError(size_t error_code, string_t msg, http_request &message) {
    http_response response(error_code);
    this->addHeaders(response);
    response.set_body(msg);
    message.reply(response);
}

/**
 * Return the absolute path of an image by the index.
 * The images are sort by index.
 * Only this number must be send to the server.
 * 
 * @param indexStr the index in string type
 * @return the absolute path that map with or
 *  an empty string if : index < 0 or index > images_count
 */
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
        
        case 5:
            return IRON_MAN;
        
        case 6:
            return CAPTAIN_MARVEL;
    }
    return "";
}

/**
 * Handle an OPTIONS request.
 * For Allow the preflight CORS request.
 * 
 * @param message the request to handle
 */
void SwapFaceServer::handle_options(http_request message)
{
    http_response response(status_codes::OK);
    this->addHeaders(response);
    message.reply(response);
    return;
}

/**
 * Handle GET request.
 * Three types of request:
 * 
 * _Have the count of images stores in the server.
 *  |_usage: swapFace/images_count
 * 
 * _ Have an image by its index.
 *  |_usage: swapFace/images/<<index in [1-images_count]>>
 * 
 * _ Swapp two images by theire indexes.
 *  |_usage: swapFace/swap/indexImg1/indexImg2
 * 
 * @param message the request to handle
 */
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
            try {
                FaceSwapper swapper(filePath, filePath2);
                swapper.process_swap();
                swapper.copyImgSwappedTo(bytes);
                this->sendResponse(bytes, message);
            } catch (std::exception &e) {
                this->sendError(INTERN_ERROR, U("Error occured"), message);
            } catch (std::string &error) {
                this->sendError(INTERN_ERROR, error, message);
            }
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

/**
 * Handle POST request.
 * Read a JSON file with values :
 * -image : the bytes array of the image to swapp the face.
 * -image2Index : the index of the image stores in the server
 * to swapp with.
 * -width and -height are not use yet.
 * 
 * @param message the request to handle
 */
void SwapFaceServer::handle_post(http_request message) {
    try {
        message.extract_json(true)       
        .then([this, &message](json::value json)
         {
            std::vector<unsigned char> bytes;

            try{
                //json::value v = json::value::parse(json);
                json::array jsonArray = json[U("image")].as_array();
                string_t image2Index = json[U("image2Index")].as_string();
                std::string image2Name = this->getImageNameByIndex(image2Index);
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
            }catch (std::exception &e) {
                string_t msg(U("Intern error "));
                msg.append(e.what());
                this->sendError(INTERN_ERROR, msg, message);
            }catch (std::string &error) {
                this->sendError(INTERN_ERROR, U("error reading user photo"), message);
            }
        }).wait();
    }catch (std::exception &e) {
        this->sendError(INTERN_ERROR, U("erreur de format du fichier json envoyé"), message);
    }catch (std::string &error) {
        this->sendError(INTERN_ERROR, U(error), message);
    }
    return;
}

// pointer of the server type
std::unique_ptr<SwapFaceServer>g_http;

/**
 * init the server and open it.
 * 
 * @param address the socket to listen in IPV4
 */
void on_initialize(const string_t& address)
{
    // Build our listener's URI from the configured address and the hard-coded path "swapFace"

    uri_builder uri(address);
    uri.append_path(U("swapFace"));

    auto addr = uri.to_uri().to_string();

    g_http = std::unique_ptr<SwapFaceServer>(new SwapFaceServer(addr));
    g_http->open().wait();

    //ucout << utility::string_t(U("Listening for request at : ")) << addr << std::endl;

    return;
}

/**
 * Close the server.
 */
void on_shutdown() {
    g_http->close().wait();

    ucout << utility::string_t(U("Server closed")) << std::endl;
    return;
}

int main (int argc, char *argv[]) {
    utility::string_t port= U("34568");
    utility::string_t address = U("http://0.0.0.0:");
    address.append(port);

    on_initialize(address);

    //std::cout << "Press ENTER to exit." << std::endl;
    std::string line;
    std::getline(std::cin, line);

    on_shutdown();

    return 0;
}

