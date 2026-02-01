#ifndef CLOADFILES_H
#define CLOADFILES_H

#include <QSettings>
#include <memory>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QListWidget>

#include "cimportfiles.h"
#include "cinifile.h"

class cLoadFiles
{
    //Атрибуты
    QListWidget * ListWidget;

    //Методы
    void appEndItem(QListWidgetItem * item);

public:
    //Конструкторы
    cLoadFiles(QListWidget * other);

    //Методы
    void execLoadFilesSignedIsRotated();

    static bool saveStringListToFile(const QString& fileName, const QStringList& list);
    static QStringList loadStringListFromFile(const QString& fileName);

};

#endif // CLOADFILES_H
