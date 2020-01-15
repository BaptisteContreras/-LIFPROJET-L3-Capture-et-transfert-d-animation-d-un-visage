/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton_open_webcam;
    QPushButton *pushButton_close_webcam;
    QPushButton *pushButton_open_file;
    QPushButton *pushButton_poly;
    QPushButton *pushButton_test;
    QPushButton *pushButton_swap;
    QPushButton *pushButton_memorize;
    QLabel *label;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1780, 1300);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton_open_webcam = new QPushButton(centralWidget);
        pushButton_open_webcam->setObjectName(QStringLiteral("pushButton_open_webcam"));
        pushButton_open_webcam->setGeometry(QRect(90, 60, 171, 81));
        pushButton_close_webcam = new QPushButton(centralWidget);
        pushButton_close_webcam->setObjectName(QStringLiteral("pushButton_close_webcam"));
        pushButton_close_webcam->setGeometry(QRect(90, 160, 171, 81));
        pushButton_open_file = new QPushButton(centralWidget);
        pushButton_open_file->setObjectName(QStringLiteral("pushButton_open_file"));
        pushButton_open_file->setGeometry(QRect(90, 290, 171, 81));
        pushButton_poly = new QPushButton(centralWidget);
        pushButton_poly->setObjectName(QStringLiteral("pushButton_poly"));
        pushButton_poly->setGeometry(QRect(90, 380, 171, 81));
        pushButton_test = new QPushButton(centralWidget);
        pushButton_test->setObjectName(QStringLiteral("pushButton_test"));
        pushButton_test->setGeometry(QRect(90, 465, 171, 81));
        pushButton_swap = new QPushButton(centralWidget);
        pushButton_swap->setObjectName(QStringLiteral("pushButton_swap"));
        pushButton_swap->setGeometry(QRect(90, 550, 171, 81));
        pushButton_memorize = new QPushButton(centralWidget);
        pushButton_memorize->setObjectName(QStringLiteral("pushButton_memorize"));
        pushButton_memorize->setGeometry(QRect(90, 635, 170, 81));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(310, 60, 640, 480));
        label2 = new QLabel(centralWidget);
        label2->setObjectName(QStringLiteral("label2"));
        label2->setGeometry(QRect(1059, 60, 731, 431));
        label3 = new QLabel(centralWidget);
        label3->setObjectName(QStringLiteral("label3"));
        label3->setGeometry(QRect(1059, 540, 731, 431));
        label4 = new QLabel(centralWidget);
        label4->setObjectName(QStringLiteral("label4"));
        label4->setGeometry(QRect(310, 550, 731, 431));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 862, 1110, 18));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        pushButton_open_webcam->setText(QApplication::translate("MainWindow", "Open Webcam", Q_NULLPTR));
        pushButton_close_webcam->setText(QApplication::translate("MainWindow", "Close Webcam", Q_NULLPTR));
        pushButton_open_file->setText(QApplication::translate("MainWindow", "Select second media", Q_NULLPTR));
        pushButton_poly->setText(QApplication::translate("MainWindow", "Show poly", Q_NULLPTR));
        pushButton_test->setText(QApplication::translate("MainWindow", "Test mode", Q_NULLPTR));
        pushButton_swap->setText(QApplication::translate("MainWindow", "Swap mouth", Q_NULLPTR));
        pushButton_memorize->setText(QApplication::translate("MainWindow", "Screen neutral face", Q_NULLPTR));
        label->setText(QString());
        label2->setText(QString());
        label3->setText(QString());
        label4->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
