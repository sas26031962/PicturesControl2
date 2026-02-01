#include "crecorditem.h"

cRecordItem::cRecordItem()
{

}

void cRecordItem::show() const
{
    qDebug() << "Name: " << qsName;
    qDebug() << "Path: " << qsPath;
    qDebug() << "Size: " << iSize;
    //qDebug() << "Is Directory: " << IsDir;
    //qDebug() << ">>Record size=" << sizeof (this);
    qDebug() << "--------------------";

}

