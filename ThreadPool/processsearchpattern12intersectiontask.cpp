#include "processsearchpattern12intersectiontask.h"

ProcessSearchPattern12IntersectionTask::ProcessSearchPattern12IntersectionTask(int taskId, QObject *receiver)
    : m_taskId(taskId), m_receiver(receiver)
{
    setAutoDelete(true); // Автоматическое удаление после выполнения
}

void ProcessSearchPattern12IntersectionTask::run()
{
    QString qsStartMessage = "Task:";
    qsStartMessage += QString::number(m_taskId);
    qsStartMessage += " run in thread";
    //qsStartMessage += QThread::currentThreadId();

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
//            QMetaObject::invokeMethod(m_receiver, "updateProgressSearchPattern12IntersectionTask",
//                                      Qt::QueuedConnection,
//                                      Q_ARG(int, i));
//        }
//    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    QString qsInfoMessage0 = "Pattern1Path=";
    qsInfoMessage0 += cIniFile::pattern1StringListFilePath;

    QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsInfoMessage0));

    QString qsInfoMessage1 = "Pattern2Path=";
    qsInfoMessage1 += cIniFile::pattern2StringListFilePath;

    QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsInfoMessage1));

    QStringList qslPattern1 = cLoadFiles::loadStringListFromFile(cIniFile::pattern1StringListFilePath);
    QStringList qslPattern2 = cLoadFiles::loadStringListFromFile(cIniFile::pattern2StringListFilePath);
    QStringList qslIntersection;

    QString qsInfoMessage2 = "Pattern1_ItemsCount=";
    qsInfoMessage2 += QString::number(qslPattern1.count());

    QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsInfoMessage2));

    QString qsInfoMessage3 = "Pattern2_ItemsCount=";
    qsInfoMessage3 += QString::number(qslPattern2.count());

    QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsInfoMessage3));

    // Очистка результата
    int iCountFounded = 0;
    int iCount = 0;
    float fProgressWeight = 100.0 / qslPattern1.count();
    int Progress = 0;

    int Year = 1900;
    int Month = 1;
    int Day = 1;
    int Hour = 0;
    int Min = 0;
    int Sec = 0;
    QString qsMirror = "1900-00-00 00-00-00";
    QString sX;

    QListIterator<QString> readIt(qslPattern1);
    while (readIt.hasNext())
    {
        QString qsSection = readIt.next();
        //qDebug() << qsSection;

        //Проверка строки на соответствие шаблону 1
        QString pattern1 = "^20[0-9]{6}_[0-9]{6}";
        QRegularExpression re(pattern1);
        bool match = re.match(qsSection.toLower()).hasMatch();
        if (match)
        {
            //Извлечение даты
            Year = qsSection.mid(0, 4).toInt();
            Month = qsSection.mid(4, 2).toInt();
            Day = qsSection.mid(6, 2).toInt();
            Hour = qsSection.mid(9, 2).toInt();
            Min = qsSection.mid(11, 2).toInt();
            Sec = qsSection.mid(13, 2).toInt();
            //qDebug() << "Строка " << qsSection << " is Ok for:" << pattern1 << ": Year=" << Year << " Month=" << Month << " Day=" << Day << " Hour=" << Hour << " Min=" << Min << " Sec=" << Sec;

            sX = "Pattern1_Item=";
            sX += qsSection;
//            sX += " Data=";
//            sX += QString::number(Year);
//            sX += ".";
//            sX += QString::number(Month);
//            sX += ".";
//            sX += QString::number(Day);
//            sX += "_";
//            sX += QString::number(Hour);
//            sX += ":";
//            sX += QString::number(Min);
//            sX += ":";
//            sX += QString::number(Sec);

            //Формирование строки данных по стандарту шаблона 2
            qsMirror = QString::number(Year);
            qsMirror += "-";
            if(Month < 10)qsMirror += "0";
            qsMirror += QString::number(Month);
            qsMirror += "-";
            if(Day < 10)qsMirror += "0";
            qsMirror += QString::number(Day);
            qsMirror += " ";
            if(Hour < 10)qsMirror += "0";
            qsMirror += QString::number(Hour);
            qsMirror += "-";
            if(Min < 10)qsMirror += "0";
            qsMirror += QString::number(Min);
            qsMirror += "-";
            if(Sec < 10)qsMirror += "0";
            qsMirror += QString::number(Sec);

            //qDebug() << "Строка " << qsSection << " is Ok for:" << pattern1 << ": Mirror=" << qsMirror;

            //Поиск сформированной строки в списке 2
            if(qslPattern2.contains(qsMirror))
            {
                qslIntersection.append(qsSection);
                //Подсчёт совпадений
                iCountFounded++;

                sX += " has mirror: ";
                sX += qsMirror;

                //Удаление найденной строки из конфигурационного файла
                //NavigationInstance->deleteSection(qsMirror);//!!!
            }
        }
        else
        {
            sX = "Pattern1 not match for " + qsSection + " Mirror=" + qsMirror;
        }

        QString qsInfoMessageY = sX;

        QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, qsInfoMessageY));

        iCount++;
        Progress = (int)(fProgressWeight * iCount);

        if(Progress < 100)
        {
            QMetaObject::invokeMethod(
                        m_receiver, "updateProgressSearchPattern12IntersectionTask",
                        Qt::QueuedConnection,
                        Q_ARG(int, Progress));
        }
    }//End of while (readIt.hasNext())
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Уведомление о завершении задачи
    QMetaObject::invokeMethod(
                m_receiver, "updateProgressSearchPattern12IntersectionTask",
                Qt::QueuedConnection,
                Q_ARG(int, 100)
                );
    // Сохранение списка пересечений по типу Pattern1 в файл
    QString qsFileNameInstersection = "../programm/data/pattern12IntersectionStringListFile.txt";
    cLoadFiles::saveStringListToFile(qsFileNameInstersection, qslIntersection);

    QString qsFinishMessage = "Task:";
    qsFinishMessage += QString::number(m_taskId);
    qsFinishMessage += " completed";
    QMetaObject::invokeMethod(m_receiver, "execEndMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsFinishMessage));
}

