#ifndef CDRAWFILEX_H
#define CDRAWFILEX_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QListWidget>
#include <QPixmap>

#include "cinifile.h"

class cDrawFilex : public QObject
{

    Q_OBJECT

    //Атрибуты
    QListWidget * ListWidget;
    int iAngle = 90;

public:
    static qreal dx;
    static qreal dy;

    explicit cDrawFilex(QListWidget * other, QObject *parent = 0);
    explicit cDrawFilex(QObject *parent = 0);

    //Методы
    void install(QListWidget * other);

    QString execRotate(int angle);
    void execRotateCW90();
    void execRotateCCW90();

    void scaleImage(QString, int width, int height);

signals:
    void foundMissingFile(QString s);

    void draw(QString s);
    void showExecStatus(QString s);

public slots:
    void execActionRotateCW();
    void execActionRotateCCW();
    void execSpinBoxAngle(int angle);

};

#endif // CDRAWFILEX_H
