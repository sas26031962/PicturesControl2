#include "processrotateccw90task.h"

ProcessRotateCCW90Task::ProcessRotateCCW90Task(int taskId, QObject *receiver)
    : m_taskId(taskId), m_receiver(receiver)
{
    setAutoDelete(true); // Автоматическое удаление после выполнения
}

void ProcessRotateCCW90Task::run()
{
    QString qsStartMessage = "Task:";
    qsStartMessage += QString::number(m_taskId);
    qsStartMessage += " run in thread";

    qDebug() << "Задача" << m_taskId << "запущена в потоке" << QThread::currentThreadId();

    QMetaObject::invokeMethod(m_receiver, "execBeginMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsStartMessage));

    // Имитация длительной обработки
//    for (int i = 0; i < 100; ++i)
//    {
//        QThread::msleep(30); // Задержка 30 мс

//        // Отправка прогресса (каждые 5%)
//        if (i % 5 == 0)
//        {
//            QMetaObject::invokeMethod(m_receiver, "updateProgressSearchPattern1Task",
//                                      Qt::QueuedConnection,
//                                      Q_ARG(int, 0));
//        }
//    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    QString qsInfoMessage0 = "==execRotateCCW90==";

    QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsInfoMessage0));

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
        // Уведомление об ошибке записи файла
        QString qsErrorMessage = "Wrong file format:";
        qsErrorMessage += cIniFile::currentImagePath;
        qsErrorMessage += " error!!!";

        QMetaObject::invokeMethod(m_receiver, "execErrorMessage",
            Qt::QueuedConnection,
            Q_ARG(QString, qsErrorMessage));
    }
    qDebug() << "RootOfName=" << qsRootOfName << " ExtOfName=" << qsExtOfName;

    // Формируем имя результирующего файла
    cIniFile::currentRotatedImagePath = qsRootOfName + "_1" + "." + qsExtOfName;

    QString qsInfoMessage1 = "TargetFileName:";
    qsInfoMessage1 += cIniFile::currentRotatedImagePath;

    QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsInfoMessage1));

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
    qDebug() << "$$$CCW90 rotation";

    int iCounter = 0;
    float fProgressWeight = 100.0 / (source.width() * source.height());
    int Progress = 0;

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
            //---
                iCounter++;
                Progress = (int)(fProgressWeight * iCounter);

                if((Progress < 100)&&(!(Progress % 5)))
                {
                    QMetaObject::invokeMethod(
                                m_receiver, "updateProgressRotateCCW90Task",
                                Qt::QueuedConnection,
                                Q_ARG(int, Progress));
                }
            //---
            }
            else
            {
                //qDebug() << "Invalid: x1=" << x1 << " y1=" << y1;
                QString qsErrorMessageX = "Invalid: x1=";
                qsErrorMessageX += QString::number(x1);
                qsErrorMessageX += " y1=";
                qsErrorMessageX += QString::number(y1);

                QMetaObject::invokeMethod(m_receiver, "execErrorMessage",
                    Qt::QueuedConnection,
                    Q_ARG(QString, qsErrorMessageX));

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
            QString qsInfoMessage2 = "Stored in file:";
            qsInfoMessage2 += cIniFile::currentRotatedImagePath;

            QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                                      Qt::QueuedConnection,
                                      Q_ARG(QString, qsInfoMessage2));
        }
        else
        {
            QString qsErrorMessageY = "!Error stored in file:";
            qsErrorMessageY += cIniFile::currentRotatedImagePath;

            QMetaObject::invokeMethod(m_receiver, "execErrorMessage",
                Qt::QueuedConnection,
                Q_ARG(QString, qsErrorMessageY));
        }
    }
    else
    {
        //qDebug() << "Invalid rotate operation:" << cIniFile::currentImagePath ;
        QString qsErrorMessageZ = "Invalid rotate operation:";
        qsErrorMessageZ += cIniFile::currentRotatedImagePath;

        QMetaObject::invokeMethod(m_receiver, "execErrorMessage",
            Qt::QueuedConnection,
            Q_ARG(QString, qsErrorMessageZ));
    }
    //---

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Уведомление о завершении задачи
    QMetaObject::invokeMethod(
                m_receiver, "updateProgressRotateCCW90Task",
                Qt::QueuedConnection,
                Q_ARG(int, 100)
                );

    QString qsFinishMessage = "Task:";
    qsFinishMessage += QString::number(m_taskId);
    qsFinishMessage += " completed";
    QMetaObject::invokeMethod(m_receiver, "execEndMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsFinishMessage));
}

