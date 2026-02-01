#include "processimporttask.h"

std::atomic<int> iCurrentIndexImportLocal(0); //Индекс для импорта файлов

processImportTask::processImportTask(int taskId, QObject *receiver)
    : m_taskId(taskId), m_receiver(receiver)
{
    setAutoDelete(true); // Автоматическое удаление после выполнения
}

void processImportTask::run()
{
    //---Стартовое сообщение---
    QString qsStartMessage = "ImportTask:";
    qsStartMessage += QString::number(m_taskId);
    qsStartMessage += " run in thread";

    qDebug() << "Task #" << m_taskId << " run in thread:" << QThread::currentThreadId();

    QMetaObject::invokeMethod(m_receiver, "execBeginMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsStartMessage));

/* Фрагмент из исходного примера
    // Имитация длительной обработки
    for (int i = 0; i < 100; ++i)
    {
        QThread::msleep(30); // Задержка 30 мс

        QMetaObject::invokeMethod(m_receiver, "updateProgressImportTask",
                                      Qt::QueuedConnection,
                                      Q_ARG(int, i));
    }
*/
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //
    //Чтение исходного каталога
    //

    std::unique_ptr<QList<cRecord> > ptrRecordList(new QList<cRecord>());
    cRecord::RecordList = ptrRecordList.get();

    //int ReadRecordsResult, RecordsCount;
    int ReadRecordsResult = cRecord::readDirectory(cIniFile::IniFile.getDirectoryPah());

    if(ReadRecordsResult > 0) cRecord::RecordsCount = -1; else cRecord::RecordsCount = cRecord::RecordList->count();

    qDebug() << "ReadRecordsResult=" << ReadRecordsResult << " RecordsCount=" << cRecord::RecordsCount;

    //Информационное сообщение
    QString qsInfoMessage0 = "ImportTask:Read records count=";
    qsInfoMessage0 += QString::number(cRecord::RecordsCount);

    QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsInfoMessage0));

    //---

    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);
    QStringList Groups = settings.childGroups();//Загрузка полного списка групп
    qDebug() << "ProcessImportTask:GroupsCount=" << Groups.count();

    //Информационное сообщение
    QString qsInfoMessage1 = "ImportTask:Initial groups count=";
    qsInfoMessage1 += QString::number(cRecord::RecordsCount);

    QMetaObject::invokeMethod(m_receiver, "execInfoMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsInfoMessage1));


    //---Добавление идентификационной секции
    cImportFiles::MaxIndexValue = cRecord::RecordsCount;
    cIniFile::IniFile.addInitalSection(cImportFiles::MaxIndexValue);

    //---

    //---Подготовка к обработке файла данных
    iCurrentIndexImportLocal.store(0);

    int iErrorFilesCounter = 0;
    int iSkippedFilesCounter= 0;
    int iAddedFilesCounter = 0;

    int iCounter = 0;
    float fProgressWeight = cImportFiles::MaxIndexValue / 1000.0;
    int Progress = 0;

    qDebug() << "ProcessImportTask:fProgressWeight=" << fProgressWeight;


    //---Начало обработки файла данных

    for(QList<cRecord>::iterator it = cRecord::RecordList->begin(); it != cRecord::RecordList->end(); ++it)
    {

        iCurrentIndexImportLocal.fetch_add(1, std::memory_order_relaxed);
        int id = iCurrentIndexImportLocal.load(std::memory_order_relaxed);

        const cRecord rec = *it;

        QString name = rec.qsName;
        int iDotPosition = name.indexOf('.');
        QString groupName = name.mid(0, iDotPosition);

        QString path = rec.qsPath;
        int iNamePosition = path.indexOf(name);
        QString PathWithoutName = path.mid(0, iNamePosition - 1);

        int size = rec.iSize;

        int iExtensionPosition = path.indexOf('.');
        QString qsExtension = path.mid(iExtensionPosition + 1);

        bool IsError = false;
        int width = 0;
        int height = 0;

        if(qsExtension.toLower() == "mp4")
        {
            qDebug() << "Id=" << iCurrentIndexImportLocal.load(std::memory_order_relaxed) << "Extension: mp4" " Weight=" << Progress;
            IsError = true;
            iErrorFilesCounter++;
        }
        else if(qsExtension.toLower() == "tif")
        {
            qDebug() << "Id=" << iCurrentIndexImportLocal.load(std::memory_order_relaxed) << "Extension: tif" " Weight=" << Progress;
            IsError = true;
            iErrorFilesCounter++;
        }
        else if(qsExtension.toLower() == "txt")
        {
            qDebug() << "Id=" << iCurrentIndexImportLocal.load(std::memory_order_relaxed) << "Extension: txt" " Weight=" << Progress;
            IsError = true;
            iErrorFilesCounter++;
        }
        else
        {
            //qDebug() << "Id=" << iCurrentIndexGlobal;

            //Фрагмент для обработки файлов изображений
            QImage image(path);//name
            if(image.isNull())
            {
                IsError = true;
                iErrorFilesCounter++;
            }
            else
            {
                width = image.width();
                height = image.height();
            }
        }

        //Добавление записи в конфигурационный файл, если её там нет
        if(!Groups.contains(groupName))
        {

            qDebug() << "###Add section:" << groupName << " Weight=" << Progress;
            iAddedFilesCounter++;

            settings.beginGroup(groupName);
            settings.setValue("Id", id);
            settings.setValue("name", name);
            settings.setValue("path", PathWithoutName);
            settings.setValue("size", size);
            settings.setValue("new", true);//Признак новой записи
            if(IsError)
            {
                settings.setValue("error", true);
            }
            else
            {
                settings.setValue("width", width);
                settings.setValue("height", height);
            }
            settings.endGroup();

        }
        else
        {
            iSkippedFilesCounter++;

            qDebug() << "Skip existing section:" << groupName << " Weight=" << Progress;
        }

        //---индикация процесса импорта
        iCounter++;

        //qDebug() << "ProcessImportTask:iCounter=" << iCounter;

        Progress = (int)(fProgressWeight * iCounter);

        if(Progress < 100)
        {
            QMetaObject::invokeMethod(
                        m_receiver, "updateProgressImportTask",
                        Qt::QueuedConnection,
                        Q_ARG(int, Progress));
        }

    }//End of for(QList<cRecord>::iterator it = cRecord::RecordList->begin(); it != cRecord::RecordList->end(); ++it)

    qDebug() << "ProcessImportTask:final of iCounter=" << iCounter;

    //---Конец обработки файла данных

    settings.sync();

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    //--- Уведомление о завершении задачи ---
    QMetaObject::invokeMethod(
                m_receiver, "updateProgressImportTask",
                Qt::QueuedConnection,
                Q_ARG(int, 100)
                );

    //---Итоговое сообщение---
    QString qsFinishMessage = "ImportTask:";
    qsFinishMessage += QString::number(m_taskId);
    qsFinishMessage += " completed";
    QMetaObject::invokeMethod(m_receiver, "execEndMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsFinishMessage));

}//End of void processImportTask::run()

