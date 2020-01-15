#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/face.hpp>
#include <QtWidgets/QFileDialog>
#include "drawLandmarks.hpp"
#include "mainwindow.moc"
#include "worker.h"
#include "vector"

using namespace cv;
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);

    timer = new QTimer(this);
    isopened = false;
    poly = false;
    testMode = false;
    swapMode = false;
    firstLaunch = true;
    currentFile = "../../data/e.jpg";
    faceDetector = CascadeClassifier(
            "../../data/haarcascade_frontalface_alt2.xml");
    facemark = cv::face::FacemarkLBF::create();
    // Load landmark detector
    facemark->loadModel("../../data/lbfmodel.yaml");
    landmarksWorker = new worker();
    Point2f a;
   // landmarksWorker->stretchDebug(Mat::zeros(16,16,16),a,vector<vector<double>>());

}

MainWindow::~MainWindow() {
    delete ui;
    delete landmarksWorker;
}

//***************************************UI methods**************************************************************************


//******************************************** OPEN WEBCAM********************************************************


void MainWindow::on_pushButton_open_webcam_clicked() {
    if (isopened){
        cout << "the webcam is already openned ! " << endl;
        return;
    }
    isopened = true;
    cap.open(0);


    if (!cap.isOpened())  // Check if we succeeded
    {
        cout << "camera is not open" << endl;
    } else {
        cout << "camera is open" << endl;

        connect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
        timer->start(20);
    }
    // load 2nd image
    img = imread(currentFile.toStdString(), IMREAD_COLOR); // Read the file
    Size size(640, 480);
    cv::resize(img, img, size);
    imgcopy = img.clone(); // clone it to have a clean copy
    img = MainWindow::detectFace(img,landmarksMedia); // detect landmarks and save it

    MainWindow::drawFaceLandmarks(img,landmarksMedia); // draw landmarks on it
    if (swapMode)
       img =  MainWindow::triangulation2(img,landmarksMedia,true,triangleMedia); // if swap mode, detect and save triangulation for mouth
    if (testMode)
        img =  MainWindow::triangulation2(img,landmarksMedia,true,triangleMedia); // if swap mode, detect and save triangulation for mouth
    else{
        //MainWindow::drawTriangulation(img,landmarksMedia);
        img =  MainWindow::triangulation2(img,landmarksMedia,true,triangleMedia);
    }
   // img = MainWindow::triangulation(img);



    cvtColor(img, img, COLOR_BGR2RGB);
    cvtColor(imgcopy, imgcopy, COLOR_BGR2RGB);



    QImage qt_img = QImage((const unsigned char *) (img.data), img.cols, img.rows, static_cast<int>(img.step),
                           QImage::Format_RGB888);
    ui->label2->setPixmap(QPixmap::fromImage(qt_img));
    ui->label2->resize(ui->label2->pixmap()->size());


}
//******************************************** CLOSE WEBCAM********************************************************

void MainWindow::on_pushButton_close_webcam_clicked() {
    disconnect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
    cap.release();

    Mat image = Mat::zeros(frame.size(), CV_8UC3);

    qt_image = QImage((const unsigned char *) (image.data), image.cols, image.rows,
                      QImage::Format_A2RGB30_Premultiplied);

    ui->label->setPixmap(QPixmap::fromImage(qt_image));
    ui->label->resize(ui->label->pixmap()->size());

    cout << "camera is closed" << endl;
    isopened = false;
}
//******************************************** UPDATE WINDOW********************************************************

void MainWindow::update_window() {
    cap >> frame; // cap current frame
    cvtColor(frame, frame, COLOR_BGR2RGB); // OpenCv use BGR but QT use RGB, this line fix the color output errors

    framecopy = frame.clone(); // clean copy of the current frame
    img = imgcopy.clone(); // refresh media
    frame = MainWindow::detectFace(frame,landmarksWebcam); // detect landmarks on webcam

    if (poly){ // Poly mode -- unused
        vector<vector<Point2f>>l;
        frame = MainWindow::voronoi(frame, landmarksWebcam,true);
    }else{
        MainWindow::drawFaceLandmarks(frame,landmarksWebcam); // draw landmarks
        if (swapMode){ // swap mode 1 -- unused
           frame =  MainWindow::triangulation2(frame,landmarksWebcam,true,triangleWebcam);
            if (!landmarksWebcam.empty() && !landmarksMedia.empty()){
                img = landmarksWorker->applyFacialExpression(framecopy,img,landmarksWebcam[0],landmarksMedia[0],triangleWebcam,triangleMedia);
                drawImageOnScreen(img,ui->label2,false);
            }

        }else if(testMode){ // WIP PART -----------------------------------------------------------------------------------------
            frame =  MainWindow::triangulation2(frame,landmarksWebcam,true,triangleWebcam); // triangulation on mouth of webcam
            if (!landmarksWebcam.empty() && !landmarksMedia.empty()) // face on webcam detected
                img = landmarksWorker->applyFacialExpression2(framecopy,img,landmarksWebcam[0],landmarksMedia[0],landmarksMemorized[0],triangleWebcam,triangleMedia,triangleMemorized);
                drawImageOnScreen(img,ui->label2,false);

        }else{ // END WIP part -------------------------------------------------------------------------------------------------------

            MainWindow::drawTriangulation(frame,landmarksWebcam); // default : draw face triangulation
        }

    }


    if (firstLaunch){
        drawImageOnScreen(frame,ui->label,true);
        firstLaunch = false;
    }else{
        drawImageOnScreen(frame,ui->label,false);
    }



}

//******************************************** SELECT MEDIA ********************************************************

void MainWindow::on_pushButton_open_file_clicked() {
    if (!isopened) {
        cout << "select a file !" << endl;
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open Image"), "./",
                                                        tr("Image Files (*.png *.jpg *.bmp *.mp4 *.avi)"));

        cout << fileName.toStdString() << " has been selected ! " << endl;
        if (fileName.size() == 0)
            return;

        // load 2nd image
        img = imread(fileName.toStdString(), IMREAD_COLOR); // Read the file
        Size size(640, 480);
        cv::resize(img, img, size);
        cvtColor(img, img, COLOR_BGR2RGB);

        QImage qt_img = QImage((const unsigned char *) (img.data), img.cols, img.rows, static_cast<int>(img.step),
                               QImage::Format_RGB888);
        ui->label2->setPixmap(QPixmap::fromImage(qt_img));
        ui->label2->resize(ui->label2->pixmap()->size());
        currentFile = fileName;
    } else {
        cout << "You must close the webcam before open a new file" << endl;
    }


}

//******************************************** POLY MODE********************************************************

void MainWindow::on_pushButton_poly_clicked() {
    if (poly){
        poly = false;
        cout << "poly desactivate" << endl;
    }
    else{
        poly = true;
        cout << "poly activate" << endl;
        vector<vector<Point2f>>l;
        Mat img2 = MainWindow::voronoi(img, landmarksMedia,false);
        //  cvtColor(img2, img2, COLOR_BGR2RGB);


        QImage qt_img = QImage((const unsigned char *) (img2.data), img2.cols, img2.rows, static_cast<int>(img2.step),
                               QImage::Format_RGB888);
        ui->label3->setPixmap(QPixmap::fromImage(qt_img));
        ui->label3->resize(ui->label3->pixmap()->size());
    }


}
//******************************************** TEST MODE********************************************************
void MainWindow::on_pushButton_test_clicked() {
    if (testMode){
        testMode = false;
        cout << "test mode off " << endl;

    }else{
        testMode = true;
        swapMode = false;
        cout << "test mode on " << endl;
    }

}
//******************************************** SWAP MODE********************************************************


void MainWindow::on_pushButton_swap_clicked() {
    if (swapMode){
        swapMode = false;
        cout << "swap mode off " << endl;

    }else{
        swapMode = true;
        testMode = false;
        cout << "swap mode on " << endl;
    }
}

void MainWindow::on_pushButton_memorize_clicked() {
    if (isopened){
        memorizedExpression = framecopy; // clean copy of the current framre
        memorizedExpression = MainWindow::detectFace(memorizedExpression,landmarksMemorized); // landmarks for memorized face
        MainWindow::drawFaceLandmarks(memorizedExpression,landmarksMemorized); // draw landmarks on it
        memorizedExpression =  MainWindow::triangulation2(memorizedExpression,landmarksMemorized,true,triangleMemorized); // triangulation for mouth of face memorized
        drawImageOnScreen(memorizedExpression,ui->label4,false);
        cout << "Your neutral face has been saved ! " << endl;
    }else{
        cout << "webcam need to be opened ! " << endl;
    }
}

//*************************************************Work methods****************************************************\\

//****************************DetectFace************************************************

Mat MainWindow::detectFace(Mat f,vector<vector<Point2f>> &save) {
    Mat gray;
    vector<Rect> faces;
    vector<Rect> faceIsolated;
    // Convert frame to grayscale because
    // faceDetector requires grayscale image.
    cvtColor(f, gray, COLOR_BGR2GRAY);


    // Detect faces
    faceDetector.detectMultiScale(gray, faces);
    if (!faces.empty())
        faceIsolated.push_back(faces[0]); // we isolate the first face detected
    else
        faceIsolated = faces;

    // Variable for landmarks.
    // Landmarks for one face is a vector of points
    // There can be more than one face in the image. Hence, we
    // use a vector of vector of points.
    vector<vector<Point2f> > landmarks;

    // Run landmark detector
    bool success = facemark->fit(f, faceIsolated, landmarks);

    save = landmarks; // save landmarks

    return f;
}

//*******************************************************************************************************

//******************** Triangulation*********************************************************************

//******************************Version 1*****************************************
void MainWindow::draw_delaunay(Mat &img, Subdiv2D &subdiv, Scalar delaunay_color) {
    vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    vector<Point> pt(3);
    Size size = img.size();
    Rect rect(0,0, size.width, size.height);    // Find faces

    for( size_t i = 0; i < triangleList.size(); i++ )
    {
        Vec6f t = triangleList[i];
        pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = Point(cvRound(t[4]), cvRound(t[5]));

        // Draw rectangles completely inside the image.
        if ( rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2]))
        {
            line(img, pt[0], pt[1], delaunay_color, 1, CV_AA, 0);
            line(img, pt[1], pt[2], delaunay_color, 1, CV_AA, 0);
            line(img, pt[2], pt[0], delaunay_color, 1, CV_AA, 0);
        }
    }

}

Mat MainWindow::triangulation(Mat f,vector<vector<Point2f> > landmarks,bool computed) {

    // Define colors for drawing.
    Scalar delaunay_color(255,255,255);


    // Rectangle to be used with Subdiv2D
    Size size = f.size();
    Rect rect(0, 0, size.width, size.height);

    // Create an instance of Subdiv2D
    Subdiv2D subdiv(rect);

    // Create a vector of points.
    vector<Point2f> points;
    // <redondant>
    if (!computed){
        Mat gray;
        vector<Rect> faces;
        cvtColor(f, gray, COLOR_BGR2GRAY);
        faceDetector.detectMultiScale(gray, faces);
        vector<vector<Point2f> > landmarks;
        // Run landmark detector
        facemark->fit(f, faces, landmarks);
    }

    // </redondant>
    for (int i = 0; i < landmarks[0].size();i++){
        if (landmarks[0][i].x > 640 || landmarks[0][i].y > 480){
           // cout << "coordonate out of image bounds, unable to draw tiangulation, skip instead of crash" << endl;

        }else{

            points.push_back(Point2f(landmarks[0][i].x,landmarks[0][i].y));
        }
       // cout << landmarks[0][i].x;
    }


    // Insert points into subdiv
    for( vector<Point2f>::iterator it = points.begin(); it != points.end(); it++)
    {
        subdiv.insert(*it);

    }

    // Draw delaunay triangles
    draw_delaunay( f, subdiv, delaunay_color );
    return f;
}
//******************************Version 2*****************************************

Mat MainWindow::triangulation2(Mat f, vector<vector<Point2f> > landmarks, bool computed, vector<Vec6f> &triangleSave) {

    // Define colors for drawing.
    Scalar delaunay_color(255,255,255);


    // Rectangle to be used with Subdiv2D
    Size size = f.size();
    Rect rect(0, 0, size.width, size.height);

    // Create an instance of Subdiv2D
    Subdiv2D subdiv(rect);

    // Create a vector of points.
    vector<Point2f> points;
    // <redondant>
    if (!computed){
        Mat gray;
        vector<Rect> faces;
        cvtColor(f, gray, COLOR_BGR2GRAY);
        faceDetector.detectMultiScale(gray, faces);
        vector<vector<Point2f> > landmarks;
        // Run landmark detector
        facemark->fit(f, faces, landmarks);
    }

    // </redondant>
    // just for the mouth
    // 48 -> 59 Outer lip
    // 60 -> 67 Inner lip
    int start = 48;
    int end = 68; // < end
    if (landmarks.empty() || landmarks[0].size() < 68)
        return f;
    for (int i = start ; i < end;i++){
        if (landmarks[0][i].x > 640 || landmarks[0][i].y > 480){
            //cout << "coordonate out of image bounds, unable to draw tiangulation, skip instead of crash" << endl;

        }else{

            points.push_back(Point2f(landmarks[0][i].x,landmarks[0][i].y));
        }
        // cout << landmarks[0][i].x;
    }


    // Insert points into subdiv
    for( vector<Point2f>::iterator it = points.begin(); it != points.end(); it++)
    {
        subdiv.insert(*it);

    }
    vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    triangleSave = triangleList;
    // Draw delaunay triangles
    Mat f2 = f.clone();
    draw_delaunay2( f2, triangleList, delaunay_color );
    return f2;
}

void MainWindow::draw_delaunay2(Mat &img, vector<Vec6f> triangleList, Scalar delaunay_color) {
    vector<Point> pt(3);
    Size size = img.size();
    Rect rect(0,0, size.width, size.height);

    for( size_t i = 0; i < triangleList.size(); i++ )
    {
        Vec6f t = triangleList[i];
        pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = Point(cvRound(t[4]), cvRound(t[5]));

        // Draw rectangles completely inside the image.
        if ( rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2]))
        {
            line(img, pt[0], pt[1], delaunay_color, 1, CV_AA, 0);
            line(img, pt[1], pt[2], delaunay_color, 1, CV_AA, 0);
            line(img, pt[2], pt[0], delaunay_color, 1, CV_AA, 0);
        }
    }

}
//*************************************************************************************************

//**************************** Voronoi Diagram*****************************************************

Mat MainWindow::draw_voronoi(Mat img2, Subdiv2D &subdiv) {

    vector<vector<Point2f> > facets;
    vector<Point2f> centers;
    subdiv.getVoronoiFacetList(vector<int>(), facets, centers);


    vector<Point> ifacet;
    vector<vector<Point> > ifacets(1);
    for( size_t i = 0; i < facets.size(); i++ )
    {
        ifacet.resize(facets[i].size());
        for( size_t j = 0; j < facets[i].size(); j++ )
            ifacet[j] = facets[i][j];

        Scalar color;
        color[0] = rand() & 255;
        color[1] = rand() & 255;
        color[2] = rand() & 255;
        fillConvexPoly(img2, ifacet, color, 8, 0);

        ifacets[0] = ifacet;
        polylines(img2, ifacets, true, Scalar(), 1, CV_AA, 0);
        circle(img2, centers[i], 3, Scalar(), CV_FILLED, CV_AA, 0);
    }

    return img2;

}

void MainWindow::draw_point(Mat &img, Point2f fp, Scalar color) {
    circle( img, fp, 2, color, CV_FILLED, CV_AA, 0 );
}

Mat MainWindow::voronoi(Mat f, vector<vector<Point2f> > landmarks, bool computed) {
    // Define colors for drawing.
    Scalar points_color(0, 0, 255);


    // Rectangle to be used with Subdiv2D
    Size size = f.size();
    Rect rect(0, 0, size.width, size.height);

    // Create an instance of Subdiv2D
    Subdiv2D subdiv(rect);

    // Create a vector of points.
    vector<Point2f> points;
    bool success = false;
    // <redondant>
    if (!computed){
        Mat gray;
        vector<Rect> faces;
        cvtColor(f, gray, COLOR_BGR2GRAY);
        faceDetector.detectMultiScale(gray, faces);

        // Run landmark detector
         success = facemark->fit(f, faces, landmarks);
    }
    if ((!success && landmarks.empty()) || landmarks.empty())
        return f;
    for (int i = 0; i < landmarks[0].size();i++){
        if (landmarks[0][i].x > 640 || landmarks[0][i].y > 480){
            //cout << "coordonate out of image bounds, unable to draw tiangulation, skip instead of crash" << endl;

        }else{

            points.push_back(Point2f(landmarks[0][i].x,landmarks[0][i].y));
        }
    }
    Mat img_voronoi = Mat::zeros(f.rows, f.cols, CV_8UC3);
    // Draw points
    for( vector<Point2f>::iterator it = points.begin(); it != points.end(); it++)
    {
        subdiv.insert(*it);
        draw_point(img_voronoi, *it, points_color);
    }

    // Allocate space for voronoi Diagram

    // Draw voronoi diagram
   img_voronoi= draw_voronoi( img_voronoi, subdiv );
   // cout << img_voronoi << endl;


    return img_voronoi;

}
//**********************************************************************************************************


//************************************ High level debug method**********************************************


Mat MainWindow::drawFaceLandmarks(Mat f, vector<vector<Point2f>> landmarks) {
    if (landmarks.empty())
        return f;
    drawLandmarks(f, landmarks[0]);
    return f;
}

Mat MainWindow::drawTriangulation(Mat f, vector<vector<Point2f>> landmarks) {
    if (landmarks.empty())
        return f;
    MainWindow::triangulation(f,landmarks,true);
    return f;

}

void MainWindow::drawImageOnScreen(Mat f, QLabel* labelSelected,bool resize) {
    qt_image = QImage((const unsigned char *) (f.data), f.cols, f.rows, static_cast<int>(f.step),
                      QImage::Format_RGB888);
    labelSelected->setPixmap(QPixmap::fromImage(qt_image));
    if (resize){
        labelSelected->resize(labelSelected->pixmap()->size());
    }
}





//**********************************************************************************






