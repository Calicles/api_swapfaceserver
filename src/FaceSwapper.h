/**
 * This class is widely made from 
 * github: https://github.com/guangyu-wang/image_face_swap.git
 * 
 */

#ifndef FACE_SWAPPER_H
#define FACE_SWAPPER_H

#include <opencv2/core/mat.hpp>

#include <vector>
#include <string>

#define DAT "/cv/libs/API_swapFace/build/shape_predictor_68_face_landmarks.dat"


/**
 * Class of swapFace processing.
 * Take two images from path or bytes vector,
 * transform them to opencv Matrix
 * and process the face swapping.
 */
class FaceSwapper {
private:
    void applyAffineTransform(cv::Mat &warpImage, cv::Mat &src, std::vector<cv::Point2f> &srcTri, std::vector<cv::Point2f> &dstTri);
    void warpTriangle(cv::Mat &img1, cv::Mat &img2, std::vector<cv::Point2f> &t1, std::vector<cv::Point2f> &t2);
    auto getLandMark(const std::string &imgFileName);
    static void calculateDelaunayTriangles(cv::Rect rect, std::vector<cv::Point2f> &points, std::vector< std::vector<int> > &delaunayTri);


    cv::Mat m_img1;
    cv::Mat m_img2;
    cv::Mat m_imgSwapped;

public:
    FaceSwapper();
    FaceSwapper(const std::vector<unsigned char> &img1, const std::string &fileNameImg2);
    FaceSwapper(const std::string &img1Filename, const std::string &img2FileName);
    ~FaceSwapper();

    bool loadImg(bool isImage1, const std::string &filename);
    bool process_swap();
    void writeImg(const std::string &imgFileName, const cv::Mat &img) const;
    void copyImgSwappedTo(std::vector<unsigned char> &dst);

    std::string m_img1FileName;
    std::string m_img2FileName;
};

#endif 