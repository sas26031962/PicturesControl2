#include "crecord.h"

int cRecord::RecordsCount = 0;

cRecord::cRecord() : cRecordItem()
{
    //qDebug() << "#cRecrod created";
}

void cRecord::showList()
{
    int RecordListCount = cRecord::RecordList->count();
    qDebug() << "RecordList count=" << RecordListCount;
    qDebug() << "--------------------";

    for(int i = 0; i < RecordListCount; i++)
     {
        qDebug() << "Index=" << i;
        cRecord::RecordList->at(i).show();
    }
}

int cRecord::readDirectory(QString directoryPath)
{
    int result = 0;

    // Объект QDir для работы с каталогом.
    QDir directory(directoryPath);

    // Проверка существования  каталога
    if (!directory.exists())
    {
        result = 1;
        qDebug() << "!!!Directory '" << directoryPath << "' not found";
        return result;
    }

    qDebug() << ">>>readDirectory from " << directoryPath << " begin";
    // Фильтры для выбора нужных файлов и каталогов.
    QDir::Filters filters = QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot;
    directory.setFilter(filters);

    // Порядок сортировки.
    directory.setSorting(QDir::Name);

    // Список файлов и каталогов.
    QFileInfoList fileList = directory.entryInfoList();

    // Перебор списка и выведение информации о каждом файле и каталоге.
    qDebug() << "Directory: " << directoryPath;

    cRecord Record;

    //---Запись данных---
    for (const QFileInfo &fileInfo : fileList)
    {
        QString name = fileInfo.fileName();
        QString path = fileInfo.filePath();
        size_t size = fileInfo.size();
        bool isDir = fileInfo.isDir();

        Record.qsName  = name;
        Record.qsPath = path;
        Record.iSize  = size;
        Record.IsDir = isDir;

        //Record.show();

        //Рекурсивный вызов
        if(isDir)
        {
            QString s = fileInfo.filePath();
            cRecord::readDirectory(s);
        }
        else
        {
            cRecord::RecordList->append(Record);
        }
    }
    qDebug() << ">>>readDirectory from " << directoryPath << " complete";

    return result;
}

int cRecord::storeRecords()
{
    int result = 0;
    QString fileName = "../RecordsList.txt";
    QStringList List;
    List.clear();
    //-------------------------------------------------------------------------

    //for(QList<cRecord>::iterator it = cRecord::RecordList->begin(); it != cRecord::RecordList->end(); ++it)
    for(QList<cRecord>::iterator it = cRecord::RecordList->begin(); it != cRecord::RecordList->begin() + 100; ++it)
    {
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
        {
            QImage image(path);
            if(image.isNull())
            {
                IsError = true;
            }
            else
            {
                width = image.width();
                height = image.height();
            }
        }

        //Добавление записи в список
        if(!IsError)
        {
            if((width > 0) && (height > 0))
            {
                QString s = path + ":" + QString::number(size);
                List.append(s);
                //qDebug() << s;
            }
        }
        else
        {
            qDebug() << "File " << path << " is not image!!!";
        }

    }//End of for(QList<cRecord>::iterator it = cRecord::RecordList->begin(); it != cRecord::RecordList->end(); ++it)

    qDebug() << "RecordsList count=" << List.count();

    //-------------------------------------------------------------------------

    long FileLength = 0;
    for(int i = 0; i < List.count() - 1; i++)
    {
        FileLength += List.at(i).count();
    }

    qDebug() << "storeRecords():FileLength=" << FileLength;

    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }
    else
    {
        // Установка кодировки
        QTextStream out(&file);
        if(cEnvironment::iSystemType == WINDOWS_SYSTEM_TYPE)
            out.setCodec("Windows-1251");
        else
            out.setCodec("UTF-8");

        for (const QString& str : List) {
            out << str << "\n";
        }

        file.close();
        return true;
    }

    return result;
}
