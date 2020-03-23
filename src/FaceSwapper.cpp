#include <opencv2/core.hpp>
#include <iostream>

#include "FaceSwapper.h"

FaceSwapper::FaceSwapper() {}

FaceSwapper::~FaceSwapper(){}

void FaceSwapper::createJPGFrom(const std::vector<uint8_t> &bytes) {

}

void FaceSwapper::writeImg(std::vector<uint8_t> &bytes) {
    std::cout << bytes.size() << std::endl;
}
