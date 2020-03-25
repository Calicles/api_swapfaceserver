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

#define DAT "./shape_predictor_68_face_landmarks.dat"


class FaceSwapper {
private:
    void applyAffineTransform(cv::Mat &warpImage, cv::Mat &src, std::vector<cv::Point2f> &srcTri, std::vector<cv::Point2f> &dstTri);
    void warpTriangle(cv::Mat &img1, cv::Mat &img2, std::vector<cv::Point2f> &t1, std::vector<cv::Point2f> &t2);
    auto getLandMark(cv::Mat mat);
    void initPredictor();
    static void calculateDelaunayTriangles(cv::Rect rect, std::vector<cv::Point2f> &points, std::vector< std::vector<int> > &delaunayTri);


    cv::Mat m_img1;
    cv::Mat m_img2;
    dlib::frontal_face_detector m_detector;
    dlib::shape_predictor m_sp;

public:
    FaceSwapper(const std::vector<unsigned char> &img1, const std::string &fileNameImg2);
    FaceSwapper(int row, int col, const std::vector<unsigned char> &data);
    ~FaceSwapper();

    bool loadImg1(const std::string &filename);
    bool loadImg2(const std::string &filename);
    bool process_swap();
    void writeImg() const;
    std::vector<unsigned char> toVector() const;

    std::string configFilePath;
    std::string cascadeFacePath;
};

#endif