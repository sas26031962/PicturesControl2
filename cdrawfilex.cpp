#include "cdrawfilex.h"

qreal cDrawFilex::dx = 0;//-200;
qreal cDrawFilex::dy = 0;//-200;

cDrawFilex::cDrawFilex(QListWidget * other, QObject *parent) : QObject(parent)
{
    ListWidget = other;
}

cDrawFilex::cDrawFilex(QObject *parent) : QObject(parent)
{
    //...
}

//=============================================================================

void cDrawFilex::install(QListWidget * other)
{
    ListWidget = other;
}

//
// Поворот изображения
//
QString cDrawFilex::execRotate(int angle)
{
    QListWidgetItem * item0 = new QListWidgetItem("==execRotate==");
    item0->setForeground(Qt::blue);
    QListWidgetItem * item1 = new QListWidgetItem("Angle=" + QString::number(angle));
    ListWidget->addItem(item0);
    ListWidget->addItem(item1);

    //--- Читаем значения из INI-файла
    cIniFile::getCurrentImagePath();

    int x = cIniFile::currentImagePath.indexOf('.');
    QString qsRootOfName, qsExtOfName;
    if(x > 0)
    {
        qsRootOfName = cIniFile::currentImagePath.mid(0,x);
        qsExtOfName = cIniFile::currentImagePath.mid(x + 1);
    }
    else
    {
        //qDebug() << "Wrong file format:" << cIniFile::currentImagePath;
        QListWidgetItem * item3 = new QListWidgetItem("Wrong file format:" + cIniFile::currentImagePath);
        item3->setForeground(Qt::red);
        ListWidget->addItem(item3);

        return cIniFile::currentRotatedImagePath;
    }
    //qDebug() << "RootOfName=" << qsRootOfName << " ExtOfName=" << qsExtOfName;

    cIniFile::currentRotatedImagePath = qsRootOfName + "_1" + "." + qsExtOfName;

    QListWidgetItem * item4 = new QListWidgetItem("TargetFileName:" + cIniFile::currentRotatedImagePath);
    ListWidget->addItem(item4);

    QImage originalImage(cIniFile::currentImagePath);

    // Создаем новое изображение для хранения повернутого изображения
    QImage::Format format = originalImage.format();
    QSize size = originalImage.size();
    int iW = size.width();
    int iH = size.height();
    int iSize;
    if(iW > iH)iSize = iW; else iSize = iH;
    //QSize newSize = QSize(iH, iW);
    QSize newSize = QSize(iSize, iSize);

    //QImage rotatedImage(originalImage.size(), originalImage.format());
    QImage rotatedImage(newSize, format);
    //QImage rotatedImage(size, format);

    //rotatedImage.fill(Qt::transparent); // Заполняем прозрачным (фактически - чёрный)
    rotatedImage.fill(Qt::blue); // Заполняем голубым
    //---
    QPainter painter(&rotatedImage);
    painter.setRenderHint(QPainter::SmoothPixmapTransform); // Сглаживание для более качественного поворота

    // Центр поворота (по умолчанию центр изображения)
    QPoint center = originalImage.rect().center();

    // Создаем матрицу трансформации
    QTransform transform;
    transform.translate(center.x(), center.y());    // Перемещаем систему координат в центр изображения
    transform.rotate(angle);                        // Выполняем поворот
    transform.translate(-center.x(), -center.y());  // Возвращаем систему координат

    painter.setTransform(transform);
    painter.drawImage(cDrawFilex::dy, cDrawFilex::dx, originalImage); // Рисуем исходное изображение на повернутом

    painter.end();
    //---
    rotatedImage.save(cIniFile::currentRotatedImagePath); // Сохраняем повернутое изображение

    //qDebug() << "Image rotated successfully";
    QListWidgetItem * item5 = new QListWidgetItem("Image rotated successfully");
    ListWidget->addItem(item5);

    return cIniFile::currentRotatedImagePath;

}//End of void cDrawFilex::execRotate(int Angle)

//=============================================================================

void cDrawFilex::execRotateCW90()
{
    QListWidgetItem * item0 = new QListWidgetItem("==execRotateCW90==");
    item0->setForeground(Qt::blue);
    ListWidget->addItem(item0);

    //--- Читаем значения из INI-файла
    cIniFile::getCurrentImagePath();

    int x = cIniFile::currentImagePath.indexOf('.');
    QString qsRootOfName, qsExtOfName;
    if(x > 0)
    {
        qsRootOfName = cIniFile::currentImagePath.mid(0,x);
        qsExtOfName = cIniFile::currentImagePath.mid(x + 1);
    }
    else
    {
        //qDebug() << "Wrong file format:" << cIniFile::currentImagePath;
        QListWidgetItem * item1 = new QListWidgetItem("Wrong file format:" + cIniFile::currentImagePath);
        item1->setForeground(Qt::red);
        ListWidget->addItem(item1);

        return;
    }
    //qDebug() << "RootOfName=" << qsRootOfName << " ExtOfName=" << qsExtOfName;
    // Формируем имя результирующего файла
    cIniFile::currentRotatedImagePath = qsRootOfName + "_1" + "." + qsExtOfName;
    QListWidgetItem * item2 = new QListWidgetItem("TargetFileName:" + cIniFile::currentRotatedImagePath);
    ListWidget->addItem(item2);


    //---

    QImage originalImage(cIniFile::currentImagePath);

    // Создаем новое изображение для хранения повернутого изображения
    //QImage::Format format = originalImage.format();
    QSize size = originalImage.size();
    int iW = size.width();
    int iH = size.height();
    int iSize;
    int iVerticalShift;
    if(iW > iH)
    {
        iSize = iW;
        iVerticalShift = (iW - iH)/2;
    }
    else
    {
        iSize = iH;
        iVerticalShift = (iH - iW)/2;
    }
    //QSize newSize = QSize(iH, iW);
    QSize newSize = QSize(iSize, iSize);

    //--- ДЕЙСТВИЕ

    QImage source = originalImage.convertToFormat(QImage::Format_ARGB32);
    //QImage::Format format = originalImage.format();
    QImage::Format format = source.format();
    QImage rotatedImage(newSize, format);
    //rotatedImage.fill(Qt::transparent); // Заполняем прозрачным (фактически - чёрный)
    rotatedImage.fill(Qt::lightGray); // Заполняем голубым

    //медленный поворот на 90 градусов по часовой стрелке

    int x1, y1;
    int errors = 0;
    qDebug() << "$$$CW90 rotation";
    for (int y = 0; y < source.height(); ++y)
    {
        for (int x = 0; x < source.width(); ++x)
        {
            QRgb pixel = source.pixel(x, y);
            x1 = source.width() - y;
            x1 = x1 - iVerticalShift;
            y1 = x;
            if(x1 < 0) x1 = -x1;//20250625
            if(y1 < 0) y1 = -y1;//20250625
            if(rotatedImage.valid(x1,y1))
            {
                rotatedImage.setPixel(x1, y1, qRgba(
                                          qRed(pixel),
                                          qGreen(pixel),
                                          qBlue(pixel),
                                          qAlpha(pixel)
                                          ));
            }
            else
            {
                //qDebug() << "Invalid: x1=" << x1 << " y1=" << y1;
                QListWidgetItem * itemX = new QListWidgetItem("Invalid: x1=" + QString::number(x1) + " y1=" + QString::number(y1));
                itemX->setForeground(Qt::red);
                ListWidget->addItem(itemX);
                if(errors++ > 10)
                {
                    break;
                }
            }
        }
    }

    //--- КОНЕЦ ДЕЙСТВИЯ

    if(errors < 10)
    {
        // Сохраняем повернутое изображение
        if(rotatedImage.save(cIniFile::currentRotatedImagePath))
        {
            QListWidgetItem * itemX = new QListWidgetItem("Stored in file:" + cIniFile::currentRotatedImagePath);
            itemX->setForeground(Qt::green);
            ListWidget->addItem(itemX);
        }
        else
        {
            QListWidgetItem * itemX = new QListWidgetItem("!Error stored in file:" + cIniFile::currentRotatedImagePath);
            itemX->setForeground(Qt::red);
            ListWidget->addItem(itemX);
        }
    }
    else
    {
        //qDebug() << "Invalid rotate operation:" << cIniFile::currentImagePath ;
        QListWidgetItem * item3 = new QListWidgetItem("Invalid rotate operation:" + cIniFile::currentRotatedImagePath);
        item3->setForeground(Qt::red);
        ListWidget->addItem(item3);
    }
    //---

}//End of void cDrawFilex::execRotateCW90()

//=============================================================================

void cDrawFilex::execRotateCCW90()
{
    QListWidgetItem * item0 = new QListWidgetItem("==execRotateCCW90==");
    item0->setForeground(Qt::blue);
    ListWidget->addItem(item0);

    //--- Читаем значения из INI-файла
    cIniFile::getCurrentImagePath();

    int x = cIniFile::currentImagePath.indexOf('.');
    QString qsRootOfName, qsExtOfName;
    if(x > 0)
    {
        qsRootOfName = cIniFile::currentImagePath.mid(0,x);
        qsExtOfName = cIniFile::currentImagePath.mid(x + 1);
    }
    else
    {
        //qDebug() << "Wrong file format:" << cIniFile::currentImagePath;
        QListWidgetItem * item1 = new QListWidgetItem("Wrong file format:" + cIniFile::currentImagePath);
        item1->setForeground(Qt::red);
        ListWidget->addItem(item1);

        return;
    }
    //qDebug() << "RootOfName=" << qsRootOfName << " ExtOfName=" << qsExtOfName;
    // Формируем имя результирующего файла
    cIniFile::currentRotatedImagePath = qsRootOfName + "_1" + "." + qsExtOfName;
    QListWidgetItem * item2 = new QListWidgetItem("TargetFileName:" + cIniFile::currentRotatedImagePath);
    ListWidget->addItem(item2);

    //---

    QImage originalImage(cIniFile::currentImagePath);

    // Создаем новое изображение для хранения повернутого изображения
    //QImage::Format format = originalImage.format();
    QSize size = originalImage.size();
    int iW = size.width();
    int iH = size.height();
    int iSize;
    int iVerticalShift;
    if(iW > iH)
    {
        iSize = iW;
        iVerticalShift = (iW - iH)/2;
    }
    else
    {
        iSize = iH;
        iVerticalShift = (iH - iW)/2;
    }
    //QSize newSize = QSize(iH, iW);
    QSize newSize = QSize(iSize, iSize);

    //--- ДЕЙСТВИЕ

    QImage source = originalImage.convertToFormat(QImage::Format_ARGB32);
    //QImage::Format format = originalImage.format();
    QImage::Format format = source.format();
    QImage rotatedImage(newSize, format);
    rotatedImage.fill(Qt::lightGray); // Заполняем серым

    //медленный поворот на 90 градусов против часовой стрелки

    int x1, y1;
    int errors = 0;
    qDebug() << "$$$CCW90 rotation";
    for (int y = 0; y < source.height(); ++y)
    {
        for (int x = 0; x < source.width(); ++x)
        {
            QRgb pixel = source.pixel(x, y);
            x1 = y;
            x1 = x1 + iVerticalShift;
            y1 = source.width() - 1 - x;
            if(x1 < 0) x1 = -x1;//20250625
            if(y1 < 0) y1 = -y1;//20250625
            if(rotatedImage.valid(x1,y1))
            {
                rotatedImage.setPixel(x1, y1, qRgba(
                                          qRed(pixel),
                                          qGreen(pixel),
                                          qBlue(pixel),
                                          qAlpha(pixel)
                                          ));
            }
            else
            {
                QListWidgetItem * itemX = new QListWidgetItem("Invalid: x1=" + QString::number(x1) + " y1=" + QString::number(y1));
                itemX->setForeground(Qt::red);
                ListWidget->addItem(itemX);
                //qDebug() << "Invalid: x1=" << x1 << " y1=" << y1;
                if(errors++ > 10)
                {
                    break;
                }
            }
        }
    }

    //--- КОНЕЦ ДЕЙСТВИЯ

    if(errors < 10)
    {
        // Сохраняем повернутое изображение
        if(rotatedImage.save(cIniFile::currentRotatedImagePath))
        {
            QListWidgetItem * itemX = new QListWidgetItem("Stored in file:" + cIniFile::currentRotatedImagePath);
            itemX->setForeground(Qt::green);
            ListWidget->addItem(itemX);
        }
        else
        {
            QListWidgetItem * itemX = new QListWidgetItem("!Error stored in file:" + cIniFile::currentRotatedImagePath);
            itemX->setForeground(Qt::red);
            ListWidget->addItem(itemX);
        }
    }
    else
    {
       //qDebug() << "Invalid rotate operation:" << cIniFile::currentImagePath ;
        QListWidgetItem * item3 = new QListWidgetItem("Invalid rotate operation:" + cIniFile::currentRotatedImagePath);
        item3->setForeground(Qt::red);
        ListWidget->addItem(item3);
    }
    //---

}//End of void cDrawFilex::execRotateCCW90()

//=============================================================================

//
// Масштабирование изображения
//
void cDrawFilex::scaleImage(QString path, int width, int height)
{
    QListWidgetItem * item0 = new QListWidgetItem("==execScaleImage==");
    item0->setForeground(Qt::blue);
    QListWidgetItem * item1 = new QListWidgetItem(path);
    ListWidget->addItem(item0);
    ListWidget->addItem(item1);

    int newWidth = width;
    int newHeight = height;

    QImage originalImage(path);

    QString status;

    if (originalImage.isNull())
    {
        QListWidgetItem * item3 = new QListWidgetItem("ScaleImage Error: Original image is Null. Path= " + path);
        item3->setForeground(Qt::red);
        ListWidget->addItem(item3);

        emit foundMissingFile(path);

        //Замена оригинального изображения мемом
        originalImage = QImage(":/img/programm/img/tmp/SmileMissing.png");

    }

    int oldWidth = originalImage.width();
    int oldHeight = originalImage.height();

    QString info = "Original:";
    info += " width=";
    info += QString::number(oldWidth);
    info += " height=";
    info += QString::number(oldHeight);

    QListWidgetItem * item4 = new QListWidgetItem(info);
    ListWidget->addItem(item4);

    info = " Scaled:";
    info += " width=";
    info += QString::number(newWidth);
    info += " height=";
    info += QString::number(newHeight);

    QListWidgetItem * item5 = new QListWidgetItem(info);
    ListWidget->addItem(item5);

    // Масштабирование изображения
    QImage scaledImage = originalImage.scaled(
        newWidth,
        newHeight,
        Qt::KeepAspectRatio, // Сохранять пропорции
        Qt::SmoothTransformation // Использовать сглаживание
    );

    QString scaledImagePath = cIniFile::scaledImagePath;
    status = "Image scaling to file";
    status += scaledImagePath;
    if(scaledImagePath.count() > 0)
    {
        scaledImage.save(scaledImagePath);    // Сохраняем масштабированное изображение
        status += " sucsess!";
    }
    else
    {
        status += " fault";
    }

    QListWidgetItem * item6 = new QListWidgetItem(status);
    ListWidget->addItem(item6);

}

//=============================================================================

void cDrawFilex::execActionRotateCW()
{
    QString s = "DrawFilesInstance::ActionRotateCW()";

    execRotateCW90();

    emit draw(cIniFile::currentRotatedImagePath);

    //===
    emit showExecStatus(s);
    //===
}

//=============================================================================

void cDrawFilex::execActionRotateCCW()
{
    QString s = "DrawFilesInstance::ActionRotateCCW()";

    execRotateCCW90();

    emit draw(cIniFile::currentRotatedImagePath);

    //===
    emit showExecStatus(s);
    //===

}

//=============================================================================

void cDrawFilex::execSpinBoxAngle(int angle)
{
    qDebug() << "Angle:" << angle;
    iAngle = angle;

    execRotate(iAngle);

    emit draw(cIniFile::currentRotatedImagePath);

}


//=============================================================================

