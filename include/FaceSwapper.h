#ifndef FACE_SWAPPER
#define FACE_SWAPPER

#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>

#include <vector>


class FaceSwapper {
private:
    void vectorToMat(const std::vector<char> &dataIn, cv::Mat &matOut);

    cv::Mat m_mat;

public:
    FaceSwapper(int row, int col, const std::vector<char> &data);
    ~FaceSwapper();

    void createJPGFrom(const std::vector<char> &bytes);
    void writeImg();

};

#endif