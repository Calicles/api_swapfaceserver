#ifndef FACE_SWAPPER
#define FACE_SWAPPER

#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>


class FaceSwapper {
private:

    cv::Mat m_mat;


public:
    FaceSwapper();
    ~FaceSwapper();

    void createJPGFrom(const std::vector<uint8_t> &bytes);
    void writeImg(std::vector<uint8_t> &bytes);

};

#endif