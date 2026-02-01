#ifndef CIMPORTFILES_H
#define CIMPORTFILES_H

//20250710
#include <QObject>

#include <memory>
#include <QLabel>

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <atomic>
#include <QListWidget>
#include <QListWidgetItem>
#include <QProgressBar>
#include <QMessageBox>

#include "crecord.h"
#include "cinifile.h"
#include "cloadfiles.h"

#define MAX_INDEX_DEFAULT_VALUE 100


//-----------------------------------------------------------------------------
//Глобальные переменные
//-----------------------------------------------------------------------------

extern std::atomic<int> iCurrentIndexGlobal;

class cImportFiles : public QObject
{
    Q_OBJECT

    //Методы
    void appEndItem(QListWidgetItem * item);

public:
    //Атрибуты
    static int MaxIndexValue;
    static QString labelExecStatusText;
    static bool IslabelExecStatusTextChanged;


    static QString labelFileNameText;
    static bool IslabelFileNameTextChanged;

    QListWidget * ListWidgetOther;
    QListWidget * ListWidgetFounded;
    QListWidget * ListWidgetKeys;

    //Конструкторы и деструкторы
    explicit cImportFiles(QListWidget * other, QListWidget * founded, QListWidget * keys, QObject *parent = 0);
    ~cImportFiles();

    //Методы
    static void execSearchNewFiles();

    void execImport();
    bool getKeysList();
    void getGroupsList();

signals:
    void showExecStatus(QString s);
    void gotoInstallNavigation();

public slots:
    void execActionImportInitial();
    void execActionGetGroupsList();
    void execActionGetKeysList();
    void execActionLoad();

};

#endif // CIMPORTFILES_H
