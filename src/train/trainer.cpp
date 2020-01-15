//
// Created by baptiste on 31/03/19.
//
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include "trainer.h"
#include "vector"


using namespace std;
using namespace cv;

trainer::trainer(string f) {
    folder = f;
    faceDetector = CascadeClassifier(
            "../../data/haarcascade_frontalface_alt2.xml");
    facemark = cv::face::FacemarkLBF::create();
    // Load landmark detector
    facemark->loadModel("../../data/lbfmodel.yaml");
}

size_t trainer::loadAll() {
    vector<cv::String> files;
    cv::glob(folder+"*.png", files, false);
    size_t count = files.size(); //number of png files in images folder
    for (size_t i=0; i<count; i++){
        images[files[i]] = imread(files[i]);




    }

    cout << count << " image(s) chargée(s)" << endl;
    return count;
}

void trainer::detect(string out) {
    map<string, Mat>::iterator it;

    for ( it = images.begin(); it != images.end(); it++ )
    {
       vector<Point2f> landmarks = detectlandmarks(it->second);
       writeinFile(out,it->first,landmarks);
    }

}

void trainer::writeinFile(string out, string name, vector<Point2f> landmarks) {
    vector<string> tokens = split(name, '.');
    vector<string> tokens2 = split(tokens[1], '/');
    cout << out+tokens2[tokens2.size()-1]+".txt" << " nb : " << landmarks.size() << endl;
    ofstream file;
   // cout << out + name << " ok" << endl;
    file.open (out+tokens2[tokens2.size()-1]+".txt");
  //  file << "Please writr this text to a file.\n this text is written using C++\n";
    for (int i = 0; i < landmarks.size(); ++i) {
        file << landmarks[i].x << " " << landmarks[i].y << "\n";
    }
    file.close();
}

vector<Point2f> trainer::detectlandmarks(Mat img) {
    Mat gray;
    vector<Rect> faces;
    vector<Rect> faceIsolated;

    cvtColor(img, gray, COLOR_BGR2GRAY);

    // Detect faces
    faceDetector.detectMultiScale(gray, faces);
    if (!faces.empty())
        faceIsolated.push_back(faces[0]);
    else
        faceIsolated = faces;

    vector<vector<Point2f> > landmarks;

    // Run landmark detector
    bool success = facemark->fit(img, faceIsolated, landmarks);
    return landmarks[0];
}

vector<string> trainer::split(string sentence, char delemiter) {
    istringstream iss(sentence);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(iss, token, delemiter)) {
        if (!token.empty())
            tokens.push_back(token);
    }

    return tokens;
}


