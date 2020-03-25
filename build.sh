cd build
rm *
g++ -std=c++11 -o main ../src/SwapFaceServer.cpp ../src/FaceSwapper.cpp -I../include -lcpprest -lssl -lcrypto -lboost_system -lpthread `pkg-config --cflags --libs opencv4`