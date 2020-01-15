#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/face/facemark.hpp>
#include <QtWidgets/QLabel>
#include "worker.h"

using namespace std;
using namespace cv;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private slots:
    // EVENT METHODS
    void on_pushButton_open_webcam_clicked();

    void on_pushButton_close_webcam_clicked();

    void on_pushButton_open_file_clicked();
    void on_pushButton_poly_clicked();
    void on_pushButton_test_clicked();
    void on_pushButton_swap_clicked();
    void on_pushButton_memorize_clicked();

    // UPDATE METHOD
    void update_window();

    /**
     * Draw the delaunay triangulation for all the face
     * @param img
     * @param subdiv
     * @param delaunay_color
     */
    void draw_delaunay( Mat& img, Subdiv2D& subdiv, Scalar delaunay_color );

    /**
     * Draw the delaunay triangulation only for the mouth
     * @param img
     * @param subdiv
     * @param delaunay_color
     */
    void draw_delaunay2( Mat& img, vector<Vec6f> triangleList, Scalar delaunay_color );

    /**
     * Draw voronoi polygons
     * @param img2
     * @param subdiv
     * @return
     */
    Mat draw_voronoi( Mat img2, Subdiv2D& subdiv );

    /**
     * Draw points for voronoi polygons
     * @param img
     * @param fp
     * @param color
     */
    void draw_point( Mat& img, Point2f fp, Scalar color );

    /**
     * Compute and draw voronoi polygons
     * @param f
     * @param landmarks
     * @param computed
     * @return
     */
    Mat voronoi(Mat f,vector<vector<Point2f> > landmarks,bool computed);

    /**
     * Compute and draw the delaunay triangulation
     * if computed = false, the method will detect and compute new face/landmarks
     * else the method use given landmarks
     * This method is not used anymore
     * @param f
     * @param landmarks
     * @param computed
     * @return
     */
    Mat triangulation(Mat f,vector<vector<Point2f> > landmarks,bool computed);
    /**
     * Compute and draw the delaunay triangulation ONLY for the mouth
     * if computed = false, the method will detect and compute new face/landmarks
     * else the method use given landmarks
     * triangleSave is a variable where the triangulation will be stored to avoid to recompute it later (very usefull for media)
     * @param f
     * @param landmarks
     * @param computed
     * @return
     */
    Mat triangulation2(Mat f,vector<vector<Point2f> > landmarks,bool computed,vector<Vec6f> &triangleSave);

    /**
     * Detect a face on a given image and compute/save the appriopriate landmarks
     * in $save (same as triangleSave cf triangulation2() )
     * @param f
     * @param save
     * @return
     */
    Mat detectFace(Mat f,vector<vector<Point2f>> &save);

    /**
     * Draw on the image the given landmarks
     * @param f
     * @param landmarks
     * @return
     */
    Mat drawFaceLandmarks(Mat f,vector<vector<Point2f>> landmarks);

    /**
     * Hight level method to draw the triangulation on the image
     * @param f
     * @param landmarks
     * @return
     */
    Mat drawTriangulation(Mat f,vector<vector<Point2f>> landmarks);

    /**
     * display a given Mat on a label
     * if resize = true, it resize the label with the width of the image
     * @param f
     * @param labelSelected
     * @param resize
     */
    void drawImageOnScreen(Mat f,QLabel* labelSelected,bool resize);


private:
    Ui::MainWindow *ui;
    /**
     * Casacade Haar classifier for face detection
     */
    CascadeClassifier faceDetector;

    /**
     * Landmarks detector
     */
    Ptr<cv::face::Facemark> facemark;

    QTimer *timer;
    VideoCapture cap;
    bool isopened,poly,successWebcam,successMedia,testMode,firstLaunch,swapMode;
    /**
     * current second media opened
     */
    QString currentFile;
    Rect rectMedia,rectWebcam;
    /**
     * worker
     * contains code for image processing
     */
    worker *landmarksWorker;
    /**
     * saved delaunay triangulation
     */
    vector<Vec6f> triangleWebcam, triangleMedia,triangleMemorized;
    /**
     * saved landmarks for webcam
     */
    vector<vector<Point2f>> landmarksWebcam;
    /**
    * saved landmarks for the second media
    */
    vector<vector<Point2f>> landmarksMedia;
    /**
    * saved landmarks for the neutral face
    */
    vector<vector<Point2f>> landmarksMemorized;

    Mat frame,img,imtest,framecopy,imgcopy,memorizedExpression;
    QImage qt_image;
};

#endif // MAINWINDOW_H