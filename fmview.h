#ifndef FMVIEW_H
#define FMVIEW_H

#include <QObject>
#include <QDialog>
#include <QDebug>
#include <QStatusBar>
#include <QString>
#include <QValidator>
#include <QListWidget>
#include <QPixmap>

#include "cinifile.h"
#include "cdrawfilex.h"

#define INT_VALIDATOR_VALUE 3000

namespace Ui {
class fmView;
}

class fmView : public QDialog
{
    Q_OBJECT

public:

    //Атрибуты
    int iSize = 821;
    QString currentImagePath;
    QListWidget * ListWidget;
    cDrawFilex * DrawFilesInstance;

    explicit fmView(QWidget *parent = 0);
    ~fmView();

private:
    Ui::fmView *ui;

    //Методы

private slots:

    void execShowExecStatus(QString s);
    void execHorizontalSliderValueChanged(int x);

public slots:
    void execDraw(QString s);

signals:
    void showExecStatus(QString s);

};

#endif // FMVIEW_H
