#include "fmview.h"
#include "ui_fmview.h"

fmView::fmView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fmView)
{
    ui->setupUi(this);

    DrawFilesInstance = new cDrawFilex();

    connect(this, SIGNAL(showExecStatus(QString)), this, SLOT( execShowExecStatus(QString)));
    connect(this->ui->horizontalSliderScale, SIGNAL(valueChanged(int)), this, SLOT( execHorizontalSliderValueChanged(int)));

}

fmView::~fmView()
{
    delete ui;
    delete DrawFilesInstance;

}

void fmView::execDraw(QString s)
{
    currentImagePath = s;
    qDebug() << "fmView:currentImagePath=" << s;
    emit showExecStatus(s + " Size=" + QString::number(iSize));
    DrawFilesInstance->scaleImage(s, iSize, iSize);
    QPixmap pmMain(cIniFile::scaledImagePath);//
    ui->labelMain->setPixmap(pmMain);
}

void fmView::execShowExecStatus(QString s)
{
    ui->labelInfo->setText(s);
}

void fmView::execHorizontalSliderValueChanged(int x)
{
    ui->labelInfo->setText("Picture size:" + QString::number(x));
    iSize = x;
    execDraw(currentImagePath);
}

