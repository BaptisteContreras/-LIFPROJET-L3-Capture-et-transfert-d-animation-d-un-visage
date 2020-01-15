//
// Created by baptiste on 31/03/19.
//

#ifndef TEST_TRAINER_H
#define TEST_TRAINER_H
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <string>
#include "stdio.h"


using namespace std;
using namespace cv;

class trainer {

public:

    /**
     * Path to load the image
     */
    string folder;
    /**
     * default constructor
     * @param f path to the folder
     */
    trainer(string f);
    /**
     * Load all image from the folder
     * @return number of image loaded
     */
    size_t loadAll();
    /**
     * Map with image name / Mat of the image
     */
    map<string, Mat> images;

    /**
     * detect landamrks on all image and write the result in out
     * @param out where to write the result
     */
    void detect(string out);

private:
    /**
     * write landmarks in the given location
     * @param out where to write
     * @param name of the image
     * @param landmarks
     */
    void writeinFile(string out, string name, vector<Point2f> landmarks);

    /**
     * detect landmarks in an image
     * @param img where to detect
     * @return vector of landmarks
     */
    vector<Point2f> detectlandmarks(Mat img);

    /**
     * Facedetector
     * cf opencv
     */
    CascadeClassifier faceDetector;
    /**
     * Landmarks detector
     * cf opencv
     */
    Ptr<cv::face::Facemark> facemark;
    /**
     * split a given string with the delemiter
     * @param sentence to split
     * @param delemiter
     * @return sentence splited
     */
    vector<string> split(string sentence, char delemiter);





};


#endif //TEST_TRAINER_H
