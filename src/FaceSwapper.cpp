#include <opencv2/opencv.hpp>

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv/cv_image.h>

#include <vector>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "FaceSwapper.h"

using namespace cv;

FaceSwapper::FaceSwapper(const std::vector<unsigned char> &img1, const std::string &fileNameImg2) 
    : m_detector(dlib::get_frontal_face_detector()), m_img1(1, img1.size(), CV_8UC3, (void*) img1.data()), m_img2(imread(fileNameImg2))
{
    this->m_img1 = imdecode(this->m_img1, IMREAD_COLOR);
    initPredictor();
}

FaceSwapper::FaceSwapper(int rows, int cols, const std::vector<unsigned char> &data) : m_img1(1, data.size(), CV_8UC3, (void*) data.data())
 {
     this->m_img1 = imdecode(this->m_img1, IMREAD_COLOR);
 }

FaceSwapper::~FaceSwapper(){}

void FaceSwapper::initPredictor() {
    std::fstream stream(DAT, std::ios::in | std::ios::binary);
    deserialize(m_sp, stream);
}

auto FaceSwapper::getLandMark(Mat mat) {
    dlib::array2d<dlib::rgb_pixel> img;
    //load_image
    assign_image(img, dlib::cv_image<dlib::bgr_pixel>(mat));

    std::vector<dlib::rectangle> dets = this->m_detector(img);
    std::cout << "Number of faces detected: " << dets.size() << std::endl;

    std::vector<Point2f> ans;
    for (size_t i = 0; i < dets.size(); i++) {
        dlib::full_object_detection shape = this->m_sp(img, dets[i]);
        std::cout << "Number of parts: " << shape.num_parts() << std::endl;
        for (size_t j = 0; j < shape.num_parts(); j++) {
            ans.push_back(Point2f(shape.part(j).x(), shape.part(j).y()));
        }
    }
    return ans;
}

bool FaceSwapper::loadImg1(const std::string &filename) {
    this->m_img1 = imread(filename, IMREAD_UNCHANGED);
    if (this->m_img1.empty()) {
        std::cout << "error loading img1" << std::endl;
        exit(1);
    } else
    {
        std::cout << "img1 loaded" << std::endl;
    }
    return true;   
}

bool FaceSwapper::loadImg2(const std::string &filename) {
    this->m_img2 = imread(filename, IMREAD_UNCHANGED);
    if (this->m_img2.empty()) {
        std::cout << "error loading img2" << std::endl;
        exit(1);
    } else
    {
        std::cout << "img2 loaded" << std::endl;
    }
    
    return true;
}

void FaceSwapper::writeImg() const {
    std::vector<int> params(2);
    params.push_back(IMWRITE_JPEG_QUALITY);
    params.push_back(100);
    imwrite("abis.jpg", this->m_img1, params);
}

std::vector<unsigned char> FaceSwapper::toVector() const  {
    std::vector<unsigned char> res;
    return res;
}

static void calculateDelaunayTriangle(Rect rect, std::vector<Point2f> &points, std::vector< std::vector<int> > &delaunayTri) {
    Subdiv2D subdiv(rect);

    for (std::vector<Point2f>::iterator it = points.begin(); it != points.end(); it++) {
        subdiv.insert(*it);
    }
    std::vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    std::vector<Point2f> pt(3);
    std::vector<int> ind(3);

    for (size_t i = 0; i < triangleList.size(); i++) {
        Vec6f t = triangleList[i];
        pt[0] = Point2f(t[0], t[1]);
        pt[1] = Point2f(t[2], t[3]);
        pt[2] = Point2f(t[4], t[5]);
    }
    if (rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2])) {
        for (size_t j = 0; j < 3; j++) {
            for (size_t k = 0; k < points.size(); k++) {
                if (abs(pt[j].x - points[k].x) < 1.0 && abs(pt[j].y - points[k].y) < 1.0) {
                    ind[j] = k;
                }
            }
        }
        delaunayTri.push_back(ind);
    }
}

void FaceSwapper::warpTriangle(Mat &img1, Mat &img2, std::vector<Point2f> &t1, std::vector<Point2f> &t2)
{
    
    Rect r1 = boundingRect(t1);
    Rect r2 = boundingRect(t2);
    
    // Offset points by left top corner of the respective rectangles
    std::vector<Point2f> t1Rect, t2Rect;
    std::vector<Point> t2RectInt;
    for(int i = 0; i < 3; i++)
    {

        t1Rect.push_back( Point2f( t1[i].x - r1.x, t1[i].y -  r1.y) );
        t2Rect.push_back( Point2f( t2[i].x - r2.x, t2[i].y - r2.y) );
        t2RectInt.push_back( Point(t2[i].x - r2.x, t2[i].y - r2.y) ); // for fillConvexPoly

    }
    
    // Get mask by filling triangle
    Mat mask = Mat::zeros(r2.height, r2.width, CV_32FC3);
    fillConvexPoly(mask, t2RectInt, Scalar(1.0, 1.0, 1.0), 16, 0);
    
    // Apply warpImage to small rectangular patches
    Mat img1Rect;
    img1(r1).copyTo(img1Rect);
    
    Mat img2Rect = Mat::zeros(r2.height, r2.width, img1Rect.type());
    
    applyAffineTransform(img2Rect, img1Rect, t1Rect, t2Rect);
    
    multiply(img2Rect,mask, img2Rect);
    multiply(img2(r2), Scalar(1.0,1.0,1.0) - mask, img2(r2));
    img2(r2) = img2(r2) + img2Rect;
}

bool FaceSwapper::process_swap() {
    Mat img1Warped = this->m_img2.clone();

    std::vector<Point2f> points1, points2;
    points1 = getLandMark(this->m_img1);
    points2 = getLandMark(this->m_img2);
    std::cout << "p1: " << points1.size() << " p2: " << points2.size() << std::endl;

    this->m_img1.convertTo(this->m_img1, CV_32F);
    img1Warped.convertTo(img1Warped, CV_32F);

    std::vector<Point2f> hull1;
    std::vector<Point2f> hull2;
    std::vector<int> hullIndex;

    convexHull(points2, hullIndex, false, false);

    for (size_t i = 0; i < hullIndex.size(); i++) {
        hull1.push_back(points1[hullIndex[i]]);
        hull2.push_back(points2[hullIndex[i]]);
    }

    std::vector< std::vector<int> > dt;
    Rect rect(0, 0, img1Warped.cols, img1Warped.rows);
    calculateDelaunayTriangles(rect, hull2, dt);

    for (size_t i = 0; i < dt.size(); i++) {
        std::vector<Point2f> t1, t2;
        for (size_t j = 0; j < 3; j++) {
            t1.push_back(hull1[dt[i][j]]);
            t2.push_back(hull2[dt[i][j]]);
        }

        warpTriangle(this->m_img1, img1Warped, t1, t2);
    }

    std::vector<Point> hull8U;
    for (size_t i = 0; i < hull2.size(); i++) {
        Point pt(hull2[i].x, hull2[i].y);
        hull8U.push_back(pt);
    }

    Mat mask = Mat::zeros(this->m_img2.rows, this->m_img2.cols, this->m_img2.depth());
    fillConvexPoly(mask, &hull8U[0], hull8U.size(), Scalar(255, 255, 255));

    Rect r = boundingRect(hull2);
    Point center = (r.tl() + r.br()) / 2;

    Mat output;
    img1Warped.convertTo(img1Warped, CV_8UC3);
    seamlessClone(img1Warped, this->m_img2, mask, center, output, NORMAL_CLONE);
    imwrite("test.png", output);

    return true;
}

/*
int main (int argc, char *arv[]) {
    FaceSwapper swapper;
    std::string img1Path("donald_trump.jpg");
    std::string img2Path("hillary_clinton.jpg");
    std::string haarcascadeFace("haarcascade_frontalface_default.xml");
    std::string configFile("sample_config_file.xml");

    swapper.loadImg1(img1Path);
    swapper.loadImg2(img2Path);
    swapper.configFilePath = "./sample_config_file.xml";
    swapper.cascadeFacePath = haarcascadeFace;
    swapper.process_swap();

    std::cout << "done" << std::endl;


    return 0;
}
*/
