#ifndef CINIFILE_H
#define CINIFILE_H

#include <QString>
#include <QSettings>
#include <QDebug>
#include <QImage>
#include <atomic>
#include <memory>

#include "cenvironment.h"
#include "crecord.h"

//-----------------------------------------------------------------------------
//Глобальные переменные
//-----------------------------------------------------------------------------

extern std::atomic<int> iCurrentIndexGlobal;

class cIniFile
{
private:
    QString directoryPath;
public:
    //Атрибуты
    static QStringList * Groups;
    static QStringList * Keys;
    static QStringList * SearchKeys;
    static QStringList * currentGroups;

    static QStringList  qslDeletedSections;

    static QString iniFilePath;
    static QString scaledImagePath;
    static QString currentImagePath;
    static QString currentRotatedImagePath;

    static QString GarbageCollectorPath;

    static QString filePathRemovedSectionList;
    static QString fileSubjectHashTag;
    static QString filePlaceHashTag;
    static QString filePropertyHashTag;
    static QString fileTheameHashTag;

    static QString fileNewItems;

    static QString pattern1StringListFilePath;
    static QString pattern2StringListFilePath;
    static QString patternXStringListFilePath;

    static cIniFile IniFile;


    int iRecordListLength = 0;  //Полное число записей

    //Конструктор
    cIniFile();

    //Методы
    QString getDirectoryPah();
    void setDirectoryPaht(QString sourcePath);

    void addInitalSection(int n);
    void addRecordListData();

    static void getCurrentImagePath();
    static int getCurrentIndex();

};

#endif // CINIFILE_H
