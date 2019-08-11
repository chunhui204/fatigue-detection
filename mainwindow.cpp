#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    capture_status(0),
    first_frame(0)
{
    ui->setupUi(this);
    initWidgets();
    m_capture.reset(new Capture);
    m_detection.reset(new FaceDetection);
    m_landmark.reset(new FaceLandmark);
    m_detectThread.reset(new Thread);
    m_landmarkThread.reset(new Thread);

    m_detection->moveToThread(m_detectThread.get());
    m_landmark->moveToThread(m_landmarkThread.get());

    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<FrameTask>("FrameTask");
    connect(this, &MainWindow::reset_config, m_landmark.get(), &FaceLandmark::on_reset_config);
    connect(m_capture.get(), &Capture::started, this, &MainWindow::on_capture_started);
    connect(m_capture.get(), &Capture::matReady, m_detection.get(), &FaceDetection::queue);
    connect(m_detection.get(), &FaceDetection::taskReady, m_landmark.get(), &FaceLandmark::queue);
    connect(m_landmark.get(), &FaceLandmark::taskReady, this, [&](FrameTask task)
    {
        tasks.push(task);
    });

    ui->textEdit->setReadOnly(true);
    m_detectThread->start();
    m_landmarkThread->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_capture_started(){
    capture_status = 1;
    this->ui->btn_open->setText(QString("close"));
    m_timer.start(100, this);
}

void MainWindow::timerEvent(QTimerEvent * ev) {

    if (ev->timerId() != m_timer.timerId()) return;
    if(tasks.empty()) return;
    first_frame++;
    if(first_frame>100) first_frame=100;

    FrameTask task = tasks.front(); tasks.pop();


    int w = ui->imageview->width(), h = ui->imageview->height();
    double r = double(task.frame.cols) / task.frame.rows;
//    if(w > h)
//        cv::resize(task.frame, task.frame, cv::Size(int(r*h), h));
//    else
//        cv::resize(task.frame, task.frame, cv::Size(w, int(w/r)));

    cv::cvtColor(task.frame, task.frame, CV_BGR2RGB);
//    cv::imshow("0", task.frame);
//    cv::waitKey(10);
    if(ui->box_check->checkState() == Qt::Checked)
    {
        cv::rectangle(task.frame, task.face_box, cv::Scalar(255,0,0), 1);
    }
    if(ui->landmark_check->checkState() == Qt::Checked)
    {
        for(auto p : task.landmarks)
            cv::circle(task.frame, p, 2, cv::Scalar(0,0,255));
    }

    ui->imageview->setImage(task.frame);

//    ui->textEdit->clear();
    if(task.is_reseting)
    {
        cv::putText(task.frame, "Calibration...", cv::Point(20,40), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(125,128,128));
    }
    else if(first_frame>=20)
    {
        //metrics
//        ui->textEdit->append("EAR left: "+ QString::number(task.metrics["EAR1"]));
//        ui->textEdit->append("EAR right: "+ QString::number(task.metrics["EAR2"]));
//        ui->textEdit->append("EAR: "+ QString::number(task.metrics["EAR"]));
//        ui->textEdit->append("mouth open: "+ QString::number(task.metrics["mouth"]));
//        ui->textEdit->append("eye: "+ QString::number(task.metrics["eye"]));
//        ui->textEdit->append("close_time: "+ QString::number(task.metrics["close_time"]));
//        ui->textEdit->append("flag: "+ QString::number(task.metrics["f"]));
        if(task.isFatigue)
        {
//            cv::putText(task.frame, "Fatigue!", cv::Point(20,40), cv::FONT_HERSHEY_COMPLEX,1, cv::Scalar(125,128,128));
            QDateTime date = QDateTime::currentDateTime();
            ui->textEdit->append(date.toString("yyyy-MM-dd hh:mm:ss") + " fatigue!!!");
        }
        if(task.metrics["attention"]==0)
        {
            QDateTime date = QDateTime::currentDateTime();
            ui->textEdit->append(date.toString("yyyy-MM-dd hh:mm:ss") + " inattention!!!");
        }
        if(task.metrics["1"]>=0) qDebug()<< "eye " << task.metrics["1"]<< " mouth " <<  task.metrics["2"]<< " PERCLOS "<<task.metrics["3"]<<" thres "<<task.metrics["4"]<< task.metrics["5"]<< task.isFatigue;
        ui->btn_calibration->setDisabled(false);
    }

    task.release();
}

void MainWindow::initWidgets()
{
//    ui->btn_open->setIcon(QIcon(":/images/open.png"));
    ui->btn_open->setIcon(QIcon(QPixmap("/home/liuch/C/test1/res/open.png")));
//    ui->btn_open->setFlat(true);
}

void MainWindow::on_btn_open_clicked()
{

    if(capture_status == 0)
    {
        m_capture->start(0);
    }
    else
    {
        m_capture->stop();
        capture_status = 0;
        m_timer.stop();
        std::queue<FrameTask> empty;
        std::swap(empty, tasks);

        this->ui->btn_open->setText(QString("open"));
    }
}

void MainWindow::on_btn_calibration_clicked()
{
    ui->btn_calibration->setDisabled(true);
    emit reset_config();
}
