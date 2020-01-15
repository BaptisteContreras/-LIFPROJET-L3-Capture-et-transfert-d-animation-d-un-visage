//
// Created by baptiste on 12/03/19.
//

#ifndef TEST_WORKER_H
#define TEST_WORKER_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/face/facemark.hpp>
#include "vector"
#include "structs/pixel.h"

using namespace std;
using namespace cv;

class worker {

public:

    /**
     * Compute the new landmark for the morphed image
     * Take landmarks from picture src and dest
     * @param src
     * @param dest
     * @return
     */
    vector<Point2f> determineNewLandmarks(vector<Point2f> src, vector<Point2f> dest);
    /**
     * First version of mouthswap
     * Transfer pixel form mouth src to mouth dest
     * @param src
     * @param dest
     * @param land1
     * @param land2
     * @param triangle1
     * @param triangle2
     * @return
     */
    Mat applyFacialExpression(Mat src, Mat dest, vector<Point2f> land1,vector<Point2f> land2,vector<Vec6f> triangle1,vector<Vec6f> triangle2);

    /**
     * Current version of mouth swap
     * @param src
     * @param dest
     * @param land1
     * @param land2
     * @param landMemorized
     * @param triangle1
     * @param triangle2
     * @param triangleMemorized
     * @return
     */
    Mat applyFacialExpression2(Mat src, Mat dest, vector<Point2f> land1,vector<Point2f> land2,vector<Point2f> landMemorized,vector<Vec6f> triangle1,vector<Vec6f> triangle2,vector<Vec6f> triangleMemorized);

    /**
     * Compute the distance between  two given points
     * @param a
     * @param b
     * @return
     */
    double distance(Point2f a,Point2f b);

    /**
     * Transform a the 6 coordonates of a triangle in a vector of point2f
     * @param triangle
     * @return
     */
    vector<Point2f> triangle2points(vector<Vec6f> triangle);

    /**
     * Transfer the pixel form mouth src to mouth dest
     * @param src
     * @param dest
     * @param rectSrc
     * @param rectDest
     * @param wrap
     * @return
     */
    Mat transferMouthTo(Mat src, Mat dest, Rect rectSrc, Rect rectDest,vector<vector<double>> wrap);
    /**
     * Wrap the pixel form the given area
     * @param img
     * @param rect
     * @return
     */
    vector<vector<double>> wrapZone(Mat img, Rect rect);
    /**
     * Compute the vector between base face and current
     * @param base
     * @param dest
     * @return
     */
    vector<vector<double>> computeTransitionVector(vector<Point2f> base,vector<Point2f> dest);
    /**
     * Custom version of morph face
     * @param img1
     * @param imgM
     * @param t1
     * @param t
     * @param alpha
     */
    void morphTriangle(Mat &img1, Mat &imgM, vector<Point2f> &t1, vector<Point2f> &t, double alpha);

    /**
     * Custom version 2
     * @param img1
     * @param imgM
     * @param t1
     * @param t
     * @param alpha
     */
    void morphTriangle2(Mat &img1, Mat &imgM, vector<Point2f> &t1, vector<Point2f> &t, double alpha);

    /**
     * Compute the delanay triangulation for the morphed image
     * @param f
     * @param landmarks
     * @return
     */
    vector<Vec6f> triangulationMorphed(Mat f,vector<Point2f>  landmarks);
    /**
     * Morh face
     * don't work
     * @param img1
     * @param img2
     * @param img
     * @param t1
     * @param t2
     * @param t
     * @param alpha
     */
    Mat morphTriangle3(Mat &img1, Mat &img2, Mat &img, vector<Point2f> &t1, vector<Point2f> &t2, vector<Point2f> &t, double alpha);

    /**
     * Apply affine transform to given image
     * @param warpImage
     * @param src
     * @param srcTri
     * @param dstTri
     */
    void applyAffinTransform(Mat &warpImage, Mat &src, vector<Point2f> &srcTri, vector<Point2f> &dstTri);

    Mat stretch(Mat img,Point2f src, vector<vector<double>> deplacements);

    int clamp(int num, int min,int max);

    /**
     * Return the value pixel for float coord
     * @param image
     * @param x
     * @param y
     * @return
     */
    struct pixel getPixelRounded(Mat image, float x, float y);

    /**
     * getter for red component
     * @param image
     * @param x
     * @param y
     * @return
     */
    int getRedComponent(Mat image,int x,int y);

    /**
     * getter for blue component
     * @param image
     * @param x
     * @param y
     * @return
     */
    int getBlueComponent(Mat image,int x,int y);

    /**
     * getter for green component
     * @param image
     * @param x
     * @param y
     * @return
     */
    int getGreenComponent(Mat image,int x,int y);
    /**
     * Return the pixel for given coord
     * @param image
     * @param x
     * @param y
     * @return struct pixel
     */
    struct pixel getPixel(Mat image,int x,int y);

    /**
     * Return the pixel for given coord
     * pixel's value are between 0 and 1
     * @param image
     * @param x
     * @param y
     * @return struct pixelFloat
     */
    struct pixelFloat getFloatPixel(Mat image,int x, int y);

    /**
     * Set a new pixel for given coord
     * @param image
     * @param x
     * @param y
     * @param pix
     */
    void setPixel(Mat image,int x, int y, struct pixel pix);

    /**
     * Set a new pixel for given coord
     * float pixel is translate to pixel
     * @param image
     * @param x
     * @param y
     * @param pix
     */
    void setPixelFloat(Mat image,int x, int y, struct pixelFloat pix);

    /**
     * Set new value for red channel for given coord
     * @param image
     * @param x
     * @param y
     * @param val
     */
    void setRedChannel(Mat image,int x, int y, int val);

    /**
     * Set new value for green channel for given coord
     * @param image
     * @param x
     * @param y
     * @param val
     */
    void setGreenChannel(Mat image,int x, int y, int val);

    /**
     * Set new value for blue channel for given coord
     * @param image
     * @param x
     * @param y
     * @param val
     */
    void setBlueChannel(Mat image,int x, int y, int val);

    /**
     * debug setter and getter for pixel
     * @param img
     * @param src
     * @param deplacements
     * @return
     */
    Mat stretchDebug(Mat img,Point2f src, vector<vector<double>> deplacements);

    /**
     * compute the transition vector for all points of the mouth
     * @param base
     * @param dest
     * @return
     */
    vector<vector<double>> computeDestForMouht(vector<Point2f> base,vector<Point2f> dest);

    /**
     * move the given point and its neighbours
     * @param img
     * @param vect
     * @param base
     */
    void transformImage(Mat img,vector<double> vect,Point2f base);

    /**
     * compute the distance between 2 pixels
     * @param x
     * @param y
     * @param base
     * @return
     */
    int pixelDistance(int x,int y, Point2f base);







};


#endif //TEST_WORKER_H
