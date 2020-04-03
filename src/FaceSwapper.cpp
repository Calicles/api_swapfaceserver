#include <opencv2/opencv.hpp>

#include <vector>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "FaceLandMark.hpp"
#include "FaceSwapper.h"

using namespace std;
using namespace cv;

FaceSwapper::FaceSwapper(const std::vector<unsigned char> &img1, const std::string &fileNameImg2) 
    : m_img1(img1, true), m_img2(imread(fileNameImg2)), m_img1FileName("user.jpg"), m_img2FileName(fileNameImg2)
{
    this->m_img1 = imdecode(this->m_img1, IMREAD_COLOR);
    try {
        if (this->m_img1.empty()) {
            cout << "empty" << endl;
        }
    writeImg(this->m_img1FileName, this->m_img1);
    }catch (cv::Exception e) {
        cout << e.what() << endl;
    }
    cout << "in constructor Faceswap" << endl;
}

FaceSwapper::FaceSwapper(const std::string &img1FileName, const std::string &img2FileName) 
    : m_img1FileName(img1FileName), m_img2FileName(img2FileName)
{
    loadImg1(img1FileName);
    loadImg2(img2FileName);
}

FaceSwapper::~FaceSwapper(){}

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

void FaceSwapper::writeImg(const std::string &imgFileName,const  Mat &img) const {
    std::vector<int> params(2);
    params.push_back(IMWRITE_JPEG_QUALITY);
    params.push_back(100);
    imwrite(imgFileName, img, params);
}

void FaceSwapper::copyImgSwappedTo(std::vector<unsigned char> &dst) {
    //reserve the size of Mat in dst
    /*if (this->m_imgSwapped.isContinuous()) {
        dst.assign(this->m_imgSwapped.data, this->m_imgSwapped.data + this->m_imgSwapped.total());
    } else {
        for (int i = 0; i < this->m_imgSwapped.rows; i++) {
            dst.insert(dst.end(), this->m_imgSwapped.ptr<unsigned char>(i), this->m_imgSwapped.ptr<unsigned char>(i) + this->m_imgSwapped.cols);
        }
    }*/
    imencode(".png", this->m_imgSwapped, dst);
}

std::vector<unsigned char> FaceSwapper::toVector() const  {
    std::vector<unsigned char> res;
    return res;
}

void FaceSwapper::applyAffineTransform(Mat &warpImage, Mat &src, vector<Point2f> &srcTri, vector<Point2f> &dstTri)
{
    // Given a pair of triangles, find the affine transform.
    Mat warpMat = getAffineTransform( srcTri, dstTri );
    
    // Apply the Affine Transform just found to the src image
    warpAffine( src, warpImage, warpMat, warpImage.size(), INTER_LINEAR, BORDER_REFLECT_101);
}


// Calculate Delaunay triangles for set of points
// Returns the vector of indices of 3 points for each triangle
void FaceSwapper::calculateDelaunayTriangles(Rect rect, vector<Point2f> &points, vector< vector<int> > &delaunayTri){

	// Create an instance of Subdiv2D
    Subdiv2D subdiv(rect);

	// Insert points into subdiv
    for( vector<Point2f>::iterator it = points.begin(); it != points.end(); it++)
        subdiv.insert(*it);	        

	vector<Vec6f> triangleList;
	subdiv.getTriangleList(triangleList);
	vector<Point2f> pt(3);
	vector<int> ind(3);

	for( size_t i = 0; i < triangleList.size(); i++ )
	{
		Vec6f t = triangleList[i];
		pt[0] = Point2f(t[0], t[1]);
		pt[1] = Point2f(t[2], t[3]);
		pt[2] = Point2f(t[4], t[5 ]);

		if ( rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2])){
			for(int j = 0; j < 3; j++)
				for(size_t k = 0; k < points.size(); k++)
					if(abs(pt[j].x - points[k].x) < 1.0 && abs(pt[j].y - points[k].y) < 1)						
						ind[j] = k;					

			delaunayTri.push_back(ind);
		}
	}
		
}


// Warps and alpha blends triangular regions from img1 and img2 to img
void FaceSwapper::warpTriangle(Mat &img1, Mat &img2, vector<Point2f> &t1, vector<Point2f> &t2)
{
    
    Rect r1 = boundingRect(t1);
    Rect r2 = boundingRect(t2);
    
    // Offset points by left top corner of the respective rectangles
    vector<Point2f> t1Rect, t2Rect;
    vector<Point> t2RectInt;
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
    fs::FaceLandMark detector(DAT);

    std::vector<Point2f> points1, points2;
    points1 = detector.getLandMark(this->m_img1FileName);
    points2 = detector.getLandMark(this->m_img2FileName);
    std::cout << "p1: " << points1.size() << " p2: " << points2.size() << std::endl;

    this->m_img1.convertTo(this->m_img1, CV_32F);
    img1Warped.convertTo(img1Warped, CV_32F);

    std::vector<cv::Point2f> hull1;
    std::vector<cv::Point2f> hull2;
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

    img1Warped.convertTo(img1Warped, CV_8UC3);
    seamlessClone(img1Warped, this->m_img2, mask, center, this->m_imgSwapped, NORMAL_CLONE);
    imwrite("test.png", this->m_imgSwapped);

    return true;
}
/*
int main (int argc, char *arv[]) {
    FaceSwapper swapper("./patLaf.jpg", "./donald_trump.jpg");

    swapper.process_swap();

    std::cout << "done" << std::endl;


    return 0;
}
*/
