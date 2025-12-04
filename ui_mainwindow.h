/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *videoWidget;
    QLabel *currenttime;
    QSlider *progress;
    QLabel *totaltime;
    QSlider *volume;
    QLabel *volumeLabel;
    QComboBox *speedComboBox;
    QPushButton *Pausebutton;
    QPushButton *Playbutton;
    QPushButton *Filebutton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1492, 838);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        videoWidget = new QWidget(centralwidget);
        videoWidget->setObjectName(QString::fromUtf8("videoWidget"));
        videoWidget->setGeometry(QRect(100, 20, 721, 391));
        videoWidget->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(0, 0, 0);"));
        currenttime = new QLabel(centralwidget);
        currenttime->setObjectName(QString::fromUtf8("currenttime"));
        currenttime->setGeometry(QRect(100, 440, 100, 30));
        currenttime->setMinimumSize(QSize(100, 30));
        currenttime->setMaximumSize(QSize(100, 30));
        currenttime->setStyleSheet(QString::fromUtf8("background-color: rgb(136, 138, 133);"));
        progress = new QSlider(centralwidget);
        progress->setObjectName(QString::fromUtf8("progress"));
        progress->setGeometry(QRect(230, 450, 590, 22));
        progress->setOrientation(Qt::Horizontal);
        totaltime = new QLabel(centralwidget);
        totaltime->setObjectName(QString::fromUtf8("totaltime"));
        totaltime->setGeometry(QRect(860, 440, 120, 30));
        totaltime->setMinimumSize(QSize(120, 30));
        totaltime->setMaximumSize(QSize(120, 30));
        totaltime->setStyleSheet(QString::fromUtf8("background-color: rgb(136, 138, 133);"));
        volume = new QSlider(centralwidget);
        volume->setObjectName(QString::fromUtf8("volume"));
        volume->setGeometry(QRect(1100, 40, 22, 350));
        volume->setOrientation(Qt::Vertical);
        volumeLabel = new QLabel(centralwidget);
        volumeLabel->setObjectName(QString::fromUtf8("volumeLabel"));
        volumeLabel->setGeometry(QRect(1030, 450, 120, 30));
        volumeLabel->setMinimumSize(QSize(120, 30));
        volumeLabel->setMaximumSize(QSize(120, 30));
        volumeLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(136, 138, 133);"));
        speedComboBox = new QComboBox(centralwidget);
        speedComboBox->setObjectName(QString::fromUtf8("speedComboBox"));
        speedComboBox->setGeometry(QRect(370, 520, 173, 34));
        Pausebutton = new QPushButton(centralwidget);
        Pausebutton->setObjectName(QString::fromUtf8("Pausebutton"));
        Pausebutton->setGeometry(QRect(240, 520, 91, 37));
        Playbutton = new QPushButton(centralwidget);
        Playbutton->setObjectName(QString::fromUtf8("Playbutton"));
        Playbutton->setGeometry(QRect(100, 520, 101, 31));
        Filebutton = new QPushButton(centralwidget);
        Filebutton->setObjectName(QString::fromUtf8("Filebutton"));
        Filebutton->setGeometry(QRect(580, 520, 101, 37));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1492, 31));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        currenttime->setText(QString());
        totaltime->setText(QString());
        volumeLabel->setText(QString());
        Pausebutton->setText(QApplication::translate("MainWindow", "Pause", nullptr));
        Playbutton->setText(QApplication::translate("MainWindow", "Play", nullptr));
        Filebutton->setText(QApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
