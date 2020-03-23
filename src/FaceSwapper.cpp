#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "FaceSwapper.h"

FaceSwapper::FaceSwapper(int rows, int cols, const std::vector<char> &data) : m_mat(rows, cols, CV_8UC3, (char*) data.data())
 {
    std::cout << "after" << std::endl;
    std::vector<int> params(2);
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(100);
    char img[data.size()];
    for (int i = 0; i < data.size(); i++) {
        img[i] = data[i];
    }
    cv::Mat rawData(1,sizeof(img),CV_8SC1,(void*)data.data());
    cv::Mat decodedMat= imdecode(rawData, -CV_LOAD_IMAGE_COLOR);
    imwrite("a.jpg",decodedMat);
    /*FILE *output;
    output = fopen("test.jpg", "wb");
    fwrite(img, sizeof(char), sizeof(img), output);
    fclose(output);*/
    
        //cv::imwrite("brad.jpg", this->m_mat, params);
 }

FaceSwapper::~FaceSwapper(){}

void FaceSwapper::createJPGFrom(const std::vector<char> &bytes) {

}

void FaceSwapper::writeImg() {
    std::vector<int> params(2);
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(100);
    
    //try {
        cv::imwrite("brad.jpg", this->m_mat, params);
    //} catch (runtime_error &err) {
     //   std::cout << "error saving img " << err.what() << std::endl;
    //}
}

void FaceSwapper::vectorToMat(const std::vector<char> &dataIn, cv::Mat &matOut) {
    /*std::vector<float>::const_iterator it = dataIn.begin();
    cv::MatIterator_<uchar> jt, end;
    jt = matOut.begin<uchar>();
    for (; it != dataIn.end(); ++it) {
        *jt++ = (uchar) (*it * 255);
    }*/
    //memcpy(matOut.data, dataIn.data(), dataIn.size() * sizeof(unsigned char));
}
