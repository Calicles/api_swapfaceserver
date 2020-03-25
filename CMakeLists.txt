cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 14)
project( faceswap )

find_package(dlib REQUIRED)
#include_directories(${dlib_INCLUDE_DIRS})
#target_link_libraries(your_app dlib::dlib)

find_package( OpenCV REQUIRED )

find_package( Boost COMPONENTS system filesystem REQUIRED )

find_package( OpenSSL REQUIRED )

find_library( cpprestsdk-lib cpprest )
find_library( SSL_LIB libssl.a REQUIRED )
find_library( CRYPTO_LIB libcrypto.a REQUIRED )
find_library( DL_LIB libdl.a REQUIRED )

file(GLOB SRC src/*.cpp src/*.h)

add_executable( faceswap ${SRC})
target_link_libraries( faceswap 
dlib::dlib 
${OpenCV_LIBS} 
${cpprest_LIBS} 
cpprest 
OpenSSL::Crypto
${Boost_FILESYSTEM_LIBRARY}
${Boost_SYSTEM_LIBRARY}
${SSL_LIB}
${CRYPTO_LIB}
${DL_LIB})
