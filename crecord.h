#ifndef CRECORD_H
#define CRECORD_H

//#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QDirIterator>
#include <QImage>

#include "cenvironment.h"
#include "crecorditem.h"

class cRecord : public cRecordItem
{

public:
    //Атрибуты

    /**************************************************************************
     * Приведённый ниже список является динамическим. Для его создания используется
     * умный указатель std::unique_ptr<QList<cRecord> >. После завершения подпрограммы,
     * в которой используется указанный список, он уничтожается.
     **************************************************************************/
    static QList<cRecord> * RecordList;

    static int RecordsCount;

    //Конструктор
    cRecord();

    //Методы
    static void showList();
    static int readDirectory(QString directoryPath);
    static int storeRecords();

};

#endif // CRECORD_H
