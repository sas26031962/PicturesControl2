#ifndef CLISTWIDGETPROPERTY_H
#define CLISTWIDGETPROPERTY_H

/*****************************************************************************
 * Класс для работы со списком Property
 *****************************************************************************/

#include <QObject>
#include <QString>
#include <QStringList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QDebug>
#include <QMenu>
#include <QFile>
#include <QTextStream>

#include "clistwidgettype.h"
#include "cinifile.h"
#include "cloadfiles.h"

class cListWidgetProperty : public QObject
{
    Q_OBJECT
public:

    //Атрибуты
    QString qsFileNameHashTag;                              //Имя файла для записи/чтения списка
    ListWidgetType lwtListType;                             //Тип списка для работы с контекстными меню
    QStringList * qslHashTagList;                           //Рабочий список строк

    //Визуальные компоненты
    QRect qrListWidget;// = QRect(10, 10, 261, 261);
    QListWidget * listWidget;                               //Список строк как визуальная компонента

    QRect qrLineEdit;// = QRect(10, 280, 261, 25);
    QLineEdit * qleAddItem;                                 //Поле для ввода нового значения элемента

    //Конструктор
    explicit cListWidgetProperty(QWidget * qwidget, QObject *parent = 0);
    ~cListWidgetProperty();

    //Методы
    void addOrRemovePlaceItemToRecord();
    bool loadHashTagList();
    bool addItemToList();

signals:
    void showExecStatus(QString s);
    void showCurrentIndexPicture();

public slots:
    void execRequest(const QPoint &pos);
    void execListWidgetItemClicked();
    void execLineEditSearchAllKeysTextChanched(QString s);
};

#endif // CLISTWIDGETPROPERTY_H
