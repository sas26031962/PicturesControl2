#ifndef CRECORDITEM_H
#define CRECORDITEM_H

#include <iostream>
#include <QString>
#include <QDebug>

class cRecordItem
{
    //Атрибуты
public:
    QString qsName = "NoName";
    QString qsPath = "";
    size_t iSize = 0;
    bool IsDir = false;

    //Конструктор
    cRecordItem();

    //Методы
    void show() const;

};

#endif // CRECORDITEM_H
