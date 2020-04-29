/**
 * This class is made with the cpprestsdk from microsoft
 * github: https://github.com/microsoft/cpprestsdk.git
 * 
 * LICENSE of the cpprestsdk:
C++ REST SDK 

The MIT License (MIT)

Copyright (c) Microsoft Corporation

All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#ifndef SWAP_FACE_SERVER
#define SWAP_FACE_SERVER

#include <cpprest/http_listener.h>              // HTTP server
#include <iostream>

#define BRAD "/cv/libs/API_swapFace/build/bradPitt.jpg"
#define TRUMP "/cv/libs/API_swapFace/build/donald_trump.jpg"
#define THE_ROCK "/cv/libs/API_swapFace/build/the_rock.jpg"
#define WONDER_WOMAN "/cv/libs/API_swapFace/build/wonder_woman.jpg"
#define IRON_MAN "/cv/libs/API_swapFace/build/iron_man.jpg"
#define CAPTAIN_MARVEL "/cv/libs/API_swapFace/build/captain_marvel.jpg"
#define COUNT U("images_count")
#define IMG_COUNT 6

#define IMAGES U("images")
#define SWAP U("swap")

#define OK status_codes::OK
#define NOT_FOUND 404
#define EXT_ERROR 400
#define INTERN_ERROR 500

/**
 * Server HTTP for Face Swapping. 
 * made with cpprestsdk from microsoft.
 * Handle GET, POST and OPTIONS (for preflight request CORS).
 */
class SwapFaceServer {
private:
    void handle_get(web::http::http_request message);
    void handle_post(web::http::http_request message);
    void handle_options(web::http::http_request message);
    void addHeaders(web::http::http_response &response);
    void sendResponse(const std::vector<unsigned char> &data, web::http::http_request &message);
    void sendError(size_t error_code, utility::string_t msg, web::http::http_request &message);
    std::string getImageNameByIndex(utility::string_t indexStr);

    web::http::experimental::listener::http_listener m_listener;

public:
    SwapFaceServer(utility::string_t url);
    ~SwapFaceServer();

    pplx::task<void> open() { return m_listener.open(); }
    pplx::task<void> close() { return m_listener.close(); }

};

#endif
