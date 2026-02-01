#include "cinifile.h"

//=============================================================================
//Глобальные переменные
//=============================================================================

    // Путь к INI-файлу
    QString cIniFile::iniFilePath;
    QString cIniFile::pattern1StringListFilePath;
    QString cIniFile::pattern2StringListFilePath;
    QString cIniFile::patternXStringListFilePath;
    QString cIniFile::scaledImagePath;
    QString cIniFile::filePathRemovedSectionList;
    QString cIniFile::fileSubjectHashTag;
    QString cIniFile::filePlaceHashTag;
    QString cIniFile::filePropertyHashTag;
    QString cIniFile::fileTheameHashTag;

    cIniFile cIniFile::IniFile = cIniFile();

    QString cIniFile::currentImagePath = "";
    QString cIniFile::currentRotatedImagePath = "";

    QString cIniFile::GarbageCollectorPath = "";

    QString cIniFile::fileNewItems;

    QStringList * cIniFile::Groups;
    QStringList * cIniFile::Keys;
    QStringList * cIniFile::SearchKeys;

    QStringList * cIniFile::currentGroups;

    QStringList cIniFile::qslDeletedSections;

    QList<cRecord> * cRecord::RecordList;



cIniFile::cIniFile()
{
    qDebug() << "cIniFile ctor";
}

void cIniFile::setDirectoryPaht(QString sourcePath)
{
    directoryPath = sourcePath;
}

QString cIniFile::getDirectoryPah()
{
    return directoryPath;
}

void cIniFile::addInitalSection(int n)
{
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);
    cIniFile::IniFile.iRecordListLength = n;

    settings.beginGroup("RecordList");
    settings.setValue("root_path", cIniFile::IniFile.getDirectoryPah());
    settings.setValue("length", cIniFile::IniFile.iRecordListLength);
    settings.setValue("index", 0);
    settings.endGroup();
    settings.sync();
}
/******************************************************************************
 * Функция добавления данных из списка QList<cRecord> в конфигурационный файл
 *
 *****************************************************************************/
void cIniFile::addRecordListData()
{
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);
    iCurrentIndexGlobal.store(0);
    for(QList<cRecord>::iterator it = cRecord::RecordList->begin(); it != cRecord::RecordList->end(); ++it)
     {
        iCurrentIndexGlobal.fetch_add(1, std::memory_order_relaxed);

        const cRecord rec = *it;

        QString name = rec.qsName;
        int iDotPosition = name.indexOf('.');
        QString groupName = name.mid(0, iDotPosition);

        QString path = rec.qsPath;
        int iNamePosition = path.indexOf(name);
        QString PathWithoutName = path.mid(0, iNamePosition - 1);

        int size = rec.iSize ;

        int iExtensionPosition = path.indexOf('.');
        QString qsExtension = path.mid(iExtensionPosition + 1);

        bool IsError = false;
        int width = 0;
        int height = 0;

        if(qsExtension.toLower() == "mp4")
        {
            qDebug() << "Id=" << iCurrentIndexGlobal.load(std::memory_order_relaxed) << "Extension: mp4";
            IsError = true;
        }
        else
        {
            //Фрагмент для обработки файлов изображений
            QImage image(path);//name

            width = image.width();
            height = image.height();
            qDebug() << "Id=" << iCurrentIndexGlobal.load(std::memory_order_relaxed);
        }

            int id = iCurrentIndexGlobal.load(std::memory_order_relaxed);

            settings.beginGroup(groupName);
            settings.setValue("Id", id);
            settings.setValue("name", name);
            settings.setValue("path", PathWithoutName);
            settings.setValue("size", size);
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

    }//End of for(QList<cRecord>::iterator it = cRecord::RecordList->begin(); it != cRecord::RecordList->end(); ++it)
    settings.sync();
    qDebug() << "==================Task is done!!!=========================";
}

void cIniFile::getCurrentImagePath()
{
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);
    QString imagePath = "";

    //--- Читаем значения из INI-файла
    QString GroupName = cIniFile::Groups->at(iCurrentIndexGlobal.load(std::memory_order_relaxed));

    settings.beginGroup(GroupName);

    QString qsPath = settings.value("path","").toString();
    QString qsName = settings.value("name","").toString();

    settings.endGroup();

    imagePath = qsPath + '/' + qsName;
    qDebug() << "OriginalPath:" << imagePath;

    cIniFile::currentImagePath = imagePath;

    settings.sync();
}

int cIniFile::getCurrentIndex()
{
    std::unique_ptr<bool> ptrOk = std::make_unique<bool>(true);
    bool* Ok = ptrOk.get();

    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);
    // Читаем значение текущего индекса из INI-файла
    settings.beginGroup("RecordList");

    QString qsCurrentIndex = settings.value("index", "0").toString();
    int LoadedCurrentIndex = qsCurrentIndex.toInt(Ok);
    if(!*Ok)LoadedCurrentIndex = 0;

    qDebug() << "Loaded CurrentIndex:" << LoadedCurrentIndex;

    return LoadedCurrentIndex;
}

