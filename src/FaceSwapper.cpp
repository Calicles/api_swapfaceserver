#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include "opencv2/face.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/photo.hpp" // seamlessClone()


#include <vector>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <opencv2/face/face_alignment.hpp>
#include "FaceSwapper.h"

using namespace cv;
using namespace cv::face;

FaceSwapper::FaceSwapper() {}

FaceSwapper::FaceSwapper(int rows, int cols, const std::vector<unsigned char> &data) : m_img1(1, data.size(), CV_8UC3, (void*) data.data())
 {
     //this->m_img1 = imdecode(this->m_img1, -CV_LOAD_IMAGE_COLOR);
 }

FaceSwapper::~FaceSwapper(){}

bool FaceSwapper::loadImg1(const std::string &filename) {
    this->m_img1 = imread(filename, -1);
    return true;   
}

bool FaceSwapper::loadImg2(const std::string &filename) {
    this->m_img2 = imread(filename, -1);
    return true;
}

bool FaceSwapper::process_swap() {
    CascadeClassifier face_cascade;
    face_cascade.load(this->cascadeFacePath);

    FacemarkKazemi::Params params;

    params.configfile = this->configFilePath;;

    Ptr<FacemarkKazemi> facemark = FacemarkKazemi::create(params);
    facemark->setFaceDetector((FN_FaceDetector)FaceSwapper::myDetector, &face_cascade);

    std::vector<Rect> faces1, faces2;
    std::vector< std::vector<Point2f>> shape1, shape2;
    float ratio1 = (float) this->m_img1.cols / (float) this->m_img1.rows; float ratio2 = (float) this->m_img2.cols / (float) this->m_img2.rows; 
    std::cout << "in process " << ratio1 << " " << ratio2 << std::endl;
    resize(this->m_img1, this->m_img1, Size(640*ratio1, 640*ratio1), 0, 0, INTER_LINEAR_EXACT);
    resize(this->m_img2, this->m_img2, Size(640*ratio2, 640*ratio2), 0, 0, INTER_LINEAR_EXACT);

    cv::Mat img1Warped = this->m_img2.clone();

    facemark->getFaces(this->m_img1, faces1);
    facemark->getFaces(this->m_img2, faces2);
    facemark->fit(this->m_img1, faces1, shape1);
    facemark->fit(this->m_img2, faces2, shape2);

    unsigned long numswaps = (unsigned long) min( (unsigned long)shape1.size(), (unsigned long)shape2.size());
    for (unsigned long z = 0; z < numswaps; z++) {
        std::vector<Point2f> points1 = shape1[z];
        std::vector<Point2f> points2 = shape2[z];
        this->m_img1.convertTo(this->m_img1, CV_32F);
        img1Warped.convertTo(img1Warped, CV_32F);

        std::vector<Point2f> boundary_image1;
        std::vector<Point2f> boundary_image2;
        std::vector<int> index;

        convexHull(cv::Mat(points2), index, false, false);
        for(size_t i = 0; i < index.size(); i++) {
            boundary_image1.push_back(points1[index[i]]);
            boundary_image2.push_back(points2[index[i]]);
        }
        std::vector< std::vector<int>> triangles;
        Rect rect(0, 0, img1Warped.cols, img1Warped.rows);
        divideIntToTriangles(rect, boundary_image2, triangles);

        for (size_t i = 0; i < triangles.size(); i++) {
            std::vector<Point2f> triangle1, triangle2;
            for (size_t j = 0; j < 3; j++) {
                triangle1.push_back(boundary_image1[triangles[i][j]]);
                triangle2.push_back(boundary_image2[triangles[i][j]]);
            }
            warpTriangle(this->m_img1, img1Warped, triangle1, triangle2);
        }
        std::vector<Point> hull;
        for (size_t i = 0; i < boundary_image2.size(); i++) {
            Point pt((int) boundary_image2[i].x, (int) boundary_image2[i].y);
            hull.push_back(pt);
        }

        cv::Mat mask = Mat::zeros(this->m_img2.rows, this->m_img2.cols, this->m_img2.depth());
        fillConvexPoly(mask, &hull[0], (int) hull.size(), Scalar(255, 255, 255));
        Rect r = boundingRect(boundary_image2);
        Point center = (r.tl() + r.br()) / 2;
        Mat output;
        img1Warped.convertTo(img1Warped, CV_8UC3);
        seamlessClone(img1Warped, this->m_img2, mask, center, output, NORMAL_CLONE);

        this->m_img1 = output;
    }
    return true;
}

bool FaceSwapper::myDetector( InputArray image, OutputArray ROIs, CascadeClassifier *face_cascade ) {
    cv::Mat gray;
    std::vector<Rect> faces;
    if(image.channels()>1){
        cvtColor(image.getMat(),gray,COLOR_BGR2GRAY);
    }
    else{
        gray = image.getMat().clone();
    }
    equalizeHist( gray, gray );
    face_cascade->detectMultiScale( gray, faces, 1.4, 2, CASCADE_SCALE_IMAGE, Size(30, 30) );
    Mat(faces).copyTo(ROIs);
    return true;
}

void FaceSwapper::writeImg() const {
    std::vector<int> params(2);
    //params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(100);
    imwrite("abis.jpg", this->m_img1, params);
}

std::vector<unsigned char> FaceSwapper::toVector() const  {
    std::vector<unsigned char> res;
    return res;
}

void FaceSwapper::divideIntToTriangles(Rect rect, std::vector<Point2f> &points, std::vector< std::vector<int> > &Tri){

    // Create an instance of Subdiv2D
    Subdiv2D subdiv(rect);
    // Insert points into subdiv
    for( std::vector<Point2f>::iterator it = points.begin(); it != points.end(); it++)
        subdiv.insert(*it);
    std::vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    std::vector<Point2f> pt(3);
    std::vector<int> ind(3);
    for( size_t i = 0; i < triangleList.size(); i++ )
    {
        Vec6f triangle = triangleList[i];
        pt[0] = Point2f(triangle[0], triangle[1]);
        pt[1] = Point2f(triangle[2], triangle[3]);
        pt[2] = Point2f(triangle[4], triangle[5]);
        if ( rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2])){
            for(int j = 0; j < 3; j++)
                for(size_t k = 0; k < points.size(); k++)
                    if(abs(pt[j].x - points[k].x) < 1.0 && abs(pt[j].y - points[k].y) < 1)
                        ind[j] =(int) k;
            Tri.push_back(ind);
        }
    }
}

void FaceSwapper::warpTriangle(Mat &img1, Mat &img2, std::vector<Point2f> &triangle1, std::vector<Point2f> &triangle2)
{
    Rect rectangle1 = boundingRect(triangle1);
    Rect rectangle2 = boundingRect(triangle2);
    // Offset points by left top corner of the respective rectangles
    std::vector<Point2f> triangle1Rect, triangle2Rect;
    std::vector<Point> triangle2RectInt;
    for(int i = 0; i < 3; i++)
    {
        triangle1Rect.push_back( Point2f( triangle1[i].x - rectangle1.x, triangle1[i].y - rectangle1.y) );
        triangle2Rect.push_back( Point2f( triangle2[i].x - rectangle2.x, triangle2[i].y - rectangle2.y) );
        triangle2RectInt.push_back( Point((int)(triangle2[i].x - rectangle2.x),(int) (triangle2[i].y - rectangle2.y))); // for fillConvexPoly
    }
    // Get mask by filling triangle
    Mat mask = Mat::zeros(rectangle2.height, rectangle2.width, CV_32FC3);
    fillConvexPoly(mask, triangle2RectInt, Scalar(1.0, 1.0, 1.0), 16, 0);
    // Apply warpImage to small rectangular this->m_img2ches
    Mat img1Rect;
    img1(rectangle1).copyTo(img1Rect);
    Mat img2Rect = Mat::zeros(rectangle2.height, rectangle2.width, img1Rect.type());
    Mat warp_mat = getAffineTransform(triangle1Rect, triangle2Rect);
    warpAffine( img1Rect, img2Rect, warp_mat, img2Rect.size(), INTER_LINEAR, BORDER_REFLECT_101);
    multiply(img2Rect,mask, img2Rect);
    multiply(img2(rectangle2), Scalar(1.0,1.0,1.0) - mask, img2(rectangle2));
    img2(rectangle2) = img2(rectangle2) + img2Rect;
}

int main (int argc, char *arv[]) {
    FaceSwapper swapper;
    std::string img1Path("./a.jpg");
    std::string img2Path("./patLaf.jpg");
    std::string haarcascadeFace("./lbpcascade_frontalFace.xml");
    std::string configFile("./sample_config_file.xml");

    swapper.loadImg1(img1Path);
    swapper.loadImg2(img2Path);
    swapper.configFilePath = configFile;
    swapper.cascadeFacePath = haarcascadeFace;

    swapper.process_swap();

    std::cout << "done" << std::endl;


    return 0;
}

