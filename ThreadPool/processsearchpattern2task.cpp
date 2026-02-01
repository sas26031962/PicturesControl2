#include "processsearchpattern2task.h"

ProcessSearchPattern2Task::ProcessSearchPattern2Task(int taskId, QObject *receiver)
    : m_taskId(taskId), m_receiver(receiver)
{
    setAutoDelete(true); // Автоматическое удаление после выполнения
}

void ProcessSearchPattern2Task::run()
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
//            QMetaObject::invokeMethod(m_receiver, "updateProgressSearchPattern2Task",
//                                      Qt::QueuedConnection,
//                                      Q_ARG(int, 0));
//        }
//    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    QString pattern = "^20[0-9]{2}-[0-9]{2}-[0-9]{2} [0-9]{2}-[0-9]{2}-[0-9]{2}";
    QRegularExpression re(pattern);

    QString qsInfoMessage0 = "==execSearchByNamePattern==";

    QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsInfoMessage0));

    QString qsInfoMessage1 = "<Pattern>=";
    qsInfoMessage1 += pattern;

    QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsInfoMessage1));

    // Создаем объект QSettings с указанием формата INI и пути к файлу
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    // Читаем значения из INI-файла

    QStringList TotalGroups = settings.childGroups();//Загрузка полного списка групп

    QString qsInfoMessage2 = "TotalGroups length: ";
    qsInfoMessage2 += QString::number(TotalGroups.count());

    QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsInfoMessage2));

    cIniFile::Groups->clear();//Очистка результата

    int iCounterYes = 0;// Очистка счётчика найденных объектов
    int iCounter = 0;
    float fProgressWeight = 100.0 / TotalGroups.count();
    int Progress = 0;

    for(QStringList::iterator readIt = TotalGroups.begin(); readIt != TotalGroups.end(); ++readIt)
    {
        QString qsSection = *readIt;
        //qDebug() << qsSection;
        bool match = re.match(qsSection.toLower()).hasMatch();
        if(match)
        {
            cIniFile::Groups->append(qsSection);
            //qDebug() << "iterator: section=" << qsSection << " contain pattern:" << pattern << " count=" << iCount;
            iCounterYes++;
        }

        iCounter++;
        Progress = (int)(fProgressWeight * iCounter);

        if(Progress < 100)
        {
            QMetaObject::invokeMethod(
                        m_receiver, "updateProgressSearchPattern2Task",
                        Qt::QueuedConnection,
                        Q_ARG(int, Progress));
        }
    }//End of for(QStringList::iterator readIt = TotalGroups.begin(); readIt != TotalGroups.end(); ++readIt)

    QString qsInfoMessage3;
    if(iCounterYes > 0)
    {
        qsInfoMessage3 = "<Pattern> in file names detected in ";
        qsInfoMessage3 += QString::number(iCounterYes);
        qsInfoMessage3 += " files";
    }
    else
    {
        qsInfoMessage3 = "No <Pattern> in file names detected";
    }

    QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsInfoMessage3));

    //---
    //bool x = cLoadFiles::saveStringListToFile(cIniFile::Pattern2StringListFilePath, *cIniFile::Groups);
    long FileLength = 0;
    for(int i = 0; i < cIniFile::Groups->count(); i++)
    {
        FileLength += cIniFile::Groups->at(i).count();
    }

    qDebug() << "searchPattern2():FileLength=" << FileLength;

    QString fileName = cIniFile::pattern2StringListFilePath;
    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // Уведомление об ошибке записи файла
        QString qsErrorMessage = "Write to file: ";
        qsErrorMessage += fileName;
        qsErrorMessage += " error!!!";

        QMetaObject::invokeMethod(m_receiver, "execErrorMessage",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, qsErrorMessage));
    }
    else
    {
        // Установка кодировки
        qDebug() << "Store to file:" << fileName << " begin";

        QTextStream out(&file);
        if(cEnvironment::iSystemType == WINDOWS_SYSTEM_TYPE)
            out.setCodec("Windows-1251");
        else
            out.setCodec("UTF-8");

        for(QStringList::iterator readIt = cIniFile::Groups->begin(); readIt != cIniFile::Groups->end(); ++readIt)
        {
            QString record = *readIt;
            //qDebug() << record;
            out << record << "\n";
        }

        qDebug() << "Store to file:" << fileName << " complete";

        file.close();

        qDebug() << "File:" << fileName << " closed";

        // Уведомление об успешной записи файла
        QString qsInfoMessage = "Write to file: ";
        qsInfoMessage += fileName;
        qsInfoMessage += " success";

        QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, qsInfoMessage));
    }

    //---

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Уведомление о завершении задачи
    QMetaObject::invokeMethod(
                m_receiver, "updateProgressSearchPattern2Task",
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

