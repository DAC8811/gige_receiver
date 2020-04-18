#ifndef GVSPFILESAVE_H
#define GVSPFILESAVE_H
#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "gvsp.h"
#include <QThread>
#include <QTime>
namespace Ui { class MainWindow; }

class GvspFileSave:public QThread
{
public:
    explicit GvspFileSave(Ui::MainWindow* ui);
    ~GvspFileSave();

    uint num;
    uint width;
    uint height;
    quint32 type;
    uchar* p=nullptr;
    QString save_path;
    QString file_type;
    size_t size;

    QTime start_time;
    QTime end_time;


    void Save();
    void Set(uint num,uint width,uint height,quint32 type,size_t size,uchar* p, const QString save_path, const QString file_type);


private:
    void run();

    Ui::MainWindow* ui;
};


#endif // GVSPFILESAVE_H
