#ifndef FACE_SWAPPER
#define FACE_SWAPPER

#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/objdetect.hpp>


#include <vector>
#include <string>


class FaceSwapper {
private:
    void divideIntToTriangles(cv::Rect rect, std::vector<cv::Point2f> &points, std::vector< std::vector<int> > &delaunayTri);
    void warpTriangle(cv::Mat &img1, cv::Mat &img2, std::vector<cv::Point2f> &triangle1, std::vector<cv::Point2f> &triangle2);

    cv::Mat m_img1;
    cv::Mat m_img2;

public:
    FaceSwapper();
    FaceSwapper(int row, int col, const std::vector<unsigned char> &data);
    ~FaceSwapper();

    bool loadImg1(const std::string &filename);
    bool loadImg2(const std::string &filename);
    bool process_swap();
    void writeImg() const;
    std::vector<unsigned char> toVector() const;
    static bool myDetector( cv::InputArray image, cv::OutputArray Rois, cv::CascadeClassifier *cascade_face );

    std::string configFilePath;
    std::string cascadeFacePath;
};

#endif