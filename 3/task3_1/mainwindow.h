#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QImage>

#include "vplayer.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

   // char* rgb_buffer;
    QImage mImage;
    vplayer *myplayer;

private slots:
    void slotGetOneFrame(QImage img);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void slotEndOfFrame();

private:
    bool isPicOnLabel;
    QImage myImage;
    Ui::MainWindow *ui;

    void ShowPic();
    void StopPic();
    void PausePic();
    void ResumePic();
};

#endif // MAINWINDOW_H
