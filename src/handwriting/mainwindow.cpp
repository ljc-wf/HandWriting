#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pix = QPixmap(this->width(),this->height());
    pix.fill(Qt::black);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::knnPredicted(Mat img)
{
    qDebug()<<"knn预测开始";
    qDebug()<<"开始加载knn模型";
    Ptr<ml::KNearest> knn=Algorithm::load<ml::KNearest>("knn.xml");
    qDebug()<<"knn模型加载完毕";

    cv::Mat result;//没有用，用于占位置
    float predicted = knn->predict(img,result);
    int digit = static_cast<int>(predicted);
    qDebug()<<"knn预测结束";

    return digit;
}

int MainWindow::svmPredicted(Mat img)
{
    qDebug()<<"svm预测开始";
    qDebug()<<"开始导入svm模型";
    Ptr<ml::SVM> svm =Algorithm::load<ml::SVM>("svm.xml");
    qDebug()<<"svm模型导入成功";
    qDebug()<<"varcount"<<svm->getVarCount();
    float res = svm->predict(img);
    int digit = static_cast<int>(res);
    qDebug()<<"svm预测结束";
    return digit;
    //return 1;
}

void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("Open"),QString(),QString("*.png *.jpg *.bmp\n*"));

    if(filePath.isEmpty()){
        QMessageBox::information(this,tr("Warning"),tr("Select nothing"),QMessageBox::Ok);
        return;
    }

    pix.load(filePath);
    img = cv::imread(filePath.toStdString());
}

void MainWindow::on_actionSave_triggered()
{
     QString filePath = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.png *.bmp *.jpg)"));
     if(filePath.isEmpty()){
         QMessageBox::information(this,tr("Tips"),tr("Nothing"),QMessageBox::Ok);
         return;
     }
     bool isOk = pix.save(filePath);
     if(isOk){
         QMessageBox::information(this,tr("Tips"),tr("Picture has been saved"),QMessageBox::Ok);
     }
     else{
         QMessageBox::information(this,tr("Tips"),tr("Something was wrong"),QMessageBox::Ok);
     }
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit(0);
}

void MainWindow::on_actionAbout_triggered()
{
    QString msg=QString("<h1>HandWriting</h1>"
                        "<h2>A tool to recoginize handwriting character</h2>"
                        "<h3>JackeyLea@Github</h3>"
                        "<h3>Mail:1768478912@qq.com</h3>"
                        "<h3>Phone:13812991101</h3>"
                        "<h4>If you got any problem please contact me</h4>");
    QMessageBox::information(this,tr("About"),msg,QMessageBox::Ok);
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_actionGithub_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/JackeyLea/HandWriting"));
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter pp(&pix);
    QPen pen;
    pen.setColor(Qt::white);
    pen.setWidth(10);
    pp.setPen(pen);
    pp.drawLine(lastPoint,endPoint);
    pp.setRenderHint(QPainter::HighQualityAntialiasing,true);

    lastPoint = endPoint;
    QPainter painter(this);
    painter.drawPixmap(0,0,pix);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        lastPoint = event->pos();
    }
    endPoint = lastPoint;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        endPoint = event->pos();
        update();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        endPoint = event->pos();
        update();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    pix = QPixmap(event->size());
    pix.fill(Qt::black);
    update();
}

void MainWindow::on_actionClear_triggered()
{
    pix.fill(Qt::white);
    update();
}

void MainWindow::on_actionGo_triggered()
{
//    QImage img = pix.toImage();
//    Mat matImg = toMat(img);//CV_8UC4 24
//    cv::resize(matImg,matImg,Size(28,28));
//    Mat gray = getGrayImg(matImg);
//    qDebug()<<"gray "<<gray.type();

    qDebug()<<img.type();
    Mat b;
    img.convertTo(b,CV_32S);
    qDebug()<<b.type();
}
