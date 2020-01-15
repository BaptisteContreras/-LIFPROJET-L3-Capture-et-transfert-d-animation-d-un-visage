#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>

#include <QApplication>
#include <QDebug>
#include "mainwindow.h"
#include "train/trainer.h"

using namespace std;
using namespace cv;
using namespace cv::face;


int main(int argc,char** argv)
{
     bool training = false;


    if (argc > 1){
        if(!strcmp(argv[1],"help")){
            cout << "usage :  ./AM1 [training]" << endl;
            return 0;
        } else if(!strcmp(argv[1],"true")){
            training = true;
        }
    }

    if (training){
        cout << "training mode" << endl;
        trainer* t = new trainer("../../data/train/happy_faces/");
        t->loadAll();
        t->detect("../../data/train/out/");
        return 0;
    }else{
        cout << "normal mode" << endl;
        qDebug() << QT_VERSION_STR;
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();
    }


}