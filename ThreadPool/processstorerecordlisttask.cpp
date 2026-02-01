#include "processstorerecordlisttask.h"

ProcessStoreRecordListTask::ProcessStoreRecordListTask(int taskId, QObject *receiver)
    : m_taskId(taskId), m_receiver(receiver)
{
    setAutoDelete(true); // Автоматическое удаление после выполнения
}

void ProcessStoreRecordListTask::run()
{
    //---Стартовое сообщение---
    QString qsStartMessage = "StoreRecordListTask:";
    qsStartMessage += QString::number(m_taskId);
    qsStartMessage += " run in thread";

    qDebug() << "Task #" << m_taskId << " run in thread:" << QThread::currentThreadId();

    QMetaObject::invokeMethod(m_receiver, "execBeginMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsStartMessage));
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Имитация длительной обработки
//    for (int i = 0; i < 100; ++i)
//    {
//        QThread::msleep(30); // Задержка 30 мс

//        // Отправка прогресса (каждые 5%)
//        if (i % 5 == 0)
//        {
//            QMetaObject::invokeMethod(m_receiver, "updateProgressStoreRecordListTask",
//                                      Qt::QueuedConnection,
//                                      Q_ARG(int, i));
//        }
//    }
    //----
    std::unique_ptr<QList<cRecord> > ptrRecordList(new QList<cRecord>());
    cRecord::RecordList = ptrRecordList.get();

    if(cRecord::readDirectory(cIniFile::IniFile.getDirectoryPah()) > 0)
    {
        //Аварийное авершение задачи
        QMetaObject::invokeMethod(m_receiver, "updateProgressStoreRecordListTask",
                                  Qt::QueuedConnection,
                                  Q_ARG(int, 100));

        // Уведомление о завершении задачи
        QString qsErrorMessage = "Directory not found: " + cIniFile::IniFile.getDirectoryPah() + " task#";
        qsErrorMessage += QString::number(m_taskId);
        qsErrorMessage += " completed";
        QMetaObject::invokeMethod(m_receiver, "execErrorMessage",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, qsErrorMessage));

        return;
    }
    else
    {

        // Уведомление о завершении чтения каталога в RecordList
        QString qsInfoMessage0 = "ReadDirectory complete, RecordList count=";
        qsInfoMessage0 += QString::number(cRecord::RecordList->count());

        QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, qsInfoMessage0));

        // Уведомление о начале формирования списка
        QString qsInfoMessage1 = "Begin to store RecordsList";
        QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, qsInfoMessage1));


        //---------------------------------------------------------------------

        QStringList List;
        List.clear();

        int iCounter = 0;
        float fProgressWeight = 100.0 / cRecord::RecordList->count();
        int Progress = 0;

        for(QList<cRecord>::iterator it = cRecord::RecordList->begin(); it != cRecord::RecordList->end(); ++it)
        //for(QList<cRecord>::iterator it = cRecord::RecordList->begin(); it != cRecord::RecordList->begin() + 100; ++it)
        {
            const cRecord rec = *it;

            QString s = rec.qsPath/* + rec.qsName*/ + ":" + QString::number(rec.iSize);
            List.append(s);

            //---индикация процесса импорта
            iCounter++;

            //qDebug() << "ProcessImportTask:iCounter=" << iCounter;

            Progress = (int)(fProgressWeight * iCounter);

            if(Progress < 100)
            {
                QMetaObject::invokeMethod(
                            m_receiver, "updateProgressStoreRecordListTaskProcess",
                            Qt::QueuedConnection,
                            Q_ARG(int, Progress));
            }

        }//End of for(QList<cRecord>::iterator it = cRecord::RecordList->begin(); it != cRecord::RecordList->end(); ++it)

        qDebug() << "RecordsList count=" << List.count();

        //---------------------------------------------------------------------

        long FileLength = 0;
        for(int i = 0; i < List.count() - 1; i++)
        {
            FileLength += List.at(i).count();
        }

        qDebug() << "storeRecords():FileLength=" << FileLength;

        QString fileName = "../RecordsList.txt";
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
            QTextStream out(&file);
            if(cEnvironment::iSystemType == WINDOWS_SYSTEM_TYPE)
                out.setCodec("Windows-1251");
            else
                out.setCodec("UTF-8");

            for (const QString& str : List)
            {
                out << str << "\n";
            }

            file.close();
            // Уведомление об успешной записи файла
            QString qsInfoMessage = "Write to file: ";
            qsInfoMessage += fileName;
            qsInfoMessage += " success";

            QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                                      Qt::QueuedConnection,
                                      Q_ARG(QString, qsInfoMessage));
        }

    }//End of else (cRecord::readDirectory(cIniFile::IniFile.getDirectoryPah()) > 0
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    //Завершение задачи
    QMetaObject::invokeMethod(m_receiver, "updateProgressStoreRecordListTask",
                              Qt::QueuedConnection,
                              Q_ARG(int, 100));

    // Уведомление о завершении задачи
    QString qsFinishMessage = "Task:";
    qsFinishMessage += QString::number(m_taskId);
    qsFinishMessage += " completed";
    QMetaObject::invokeMethod(m_receiver, "execEndMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsFinishMessage));
}

