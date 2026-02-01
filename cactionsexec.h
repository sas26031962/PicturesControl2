#ifndef CACTIONSEXEC_H
#define CACTIONSEXEC_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QListWidget>

#include "cinifile.h"
#include "cimportfiles.h"

class cActionsExec : public QObject
{
    Q_OBJECT

    //Методы
    void appEndItem(QListWidgetItem * item);

public:
    //Атрибуты
    QListWidget * ListWidget;

    //Конструкторы
    explicit cActionsExec(QListWidget * other, QObject *parent = 0);

    //Методы

signals:
    void execShowExecStatus(QString s);

public slots:
    void execActionRemove_Js(bool x);
    void execActionRemove_Html(bool x);
    void execActionRemoveMovie(bool x);
    void execActionRemoveText(bool x);
    void execActionRemoveTif(bool x);
    void execActionRemoveBin(bool x);
    void execActionRemove3gp(bool x);
    void execActionShowNewFiles(bool x);

};//End of class cActionsExec : public QObject

#endif // CACTIONSEXEC_H
