#include "clistwidgetsubject.h"

cListWidgetSubject::cListWidgetSubject(QWidget * qwidget, QObject *parent) : QObject(parent)
{
    qslHashTagList = new QStringList;

    //Задание типа меню
    lwtListType = ListWidgetType::SUBJECT_TYPE;

    qsFileNameHashTag = cIniFile::fileSubjectHashTag;

    QRect qrListWidget = QRect(10, 10, 261, 261);
    listWidget = new QListWidget(qwidget);
    listWidget->setGeometry(qrListWidget);

    QRect qrLineEdit = QRect(10, 280, 261, 25);
    qleAddItem = new QLineEdit(qwidget);
    qleAddItem->setGeometry(qrLineEdit);

    //Очистка визуального списка
    listWidget->clear();
    //Загрузка списка хеш-тегов Places
    if(loadHashTagList())
    {
        listWidget->addItems(*qslHashTagList);

        // Настройка контекстного меню
        listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(listWidget, &QListWidget::customContextMenuRequested, this, &cListWidgetSubject::execRequest);
        connect(listWidget, &QListWidget::itemClicked, this, &cListWidgetSubject::execListWidgetItemClicked);
        connect(qleAddItem, &QLineEdit::textChanged, this, &cListWidgetSubject::execLineEditSearchAllKeysTextChanched);
    }
}

cListWidgetSubject::~cListWidgetSubject()
{
    delete qslHashTagList;
    delete listWidget;
    delete qleAddItem;
}

void cListWidgetSubject::execRequest(const QPoint &pos)
{

    QString s = "execWidgetListSubjectCustomContextMenuRequested()";

    QListWidgetItem * item = listWidget->itemAt(pos);
    if(!item)
    {
        s += ": no item selected!";
        //---
        emit showExecStatus(s);
        //---
        return;
    }

    int index = listWidget->row(item);
    QString qsItem = item->text();

    QMenu contextMenu;
    QAction *actionAddOrRemoveItemToRecord = contextMenu.addAction("Добавить(удалить) элемент в запись");
    QAction *actionRemoveItemFromList = contextMenu.addAction("Удалить элемент из списка");
    QAction *actionInsertItemToList = contextMenu.addAction("Добавить элемент в список");

    QAction *selectedAction = contextMenu.exec(listWidget->viewport()->mapToGlobal(pos));

    if (selectedAction == actionAddOrRemoveItemToRecord)
    {
        // Обработка первого действия
        qDebug() << "exec actionAddOrRemoveItemToRecord: item=" << item->text()<< " index of this item=" << index;

        addOrRemovePlaceItemToRecord();
    }

    else if (selectedAction == actionRemoveItemFromList)
    {
        // Обработка второго действия
        qDebug() << "exec actionRemoveItemFromList: item=" << item->text()<< " index of this item=" << index;

        //---Загрузка списка Subject

        if(!loadHashTagList())
        {
            qDebug() << "Error: Could not load HashTagListSubject from file: " << qsFileNameHashTag;
            return;
        }

        qDebug() << ": loadHashTagListSubject is sucsess#";
        //Здесь должна быть проверка на наличие удаляемого значения в списке
        if(qslHashTagList->indexOf(qsItem) > 0)
        {
            qslHashTagList->removeAll(qsItem);
            listWidget->clear();
            listWidget->addItems(*qslHashTagList);

            //Сохранение нового списка Subject

            cLoadFiles::saveStringListToFile(qsFileNameHashTag, *qslHashTagList);

            //Информационное сообщение
            s += "Removed item: ";
            s += qsItem;
        }
        else
        {
            qDebug() << "Item " << qsItem << "not found in HashTagListSubject";
        }
        //---

    }

    else if (selectedAction == actionInsertItemToList)
    {
        // Обработка третьего действия
        qDebug() << "exec actionInsertItemToList: item=" << qleAddItem->text();
        addItemToList();
    }

    //---
    emit showExecStatus(s);
    //---

}

void cListWidgetSubject::addOrRemovePlaceItemToRecord()
{
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);
    QString s = "execSubjectItemClicked()";
    QString item = listWidget->currentItem()->text();
    qDebug() << "listWidgetPlace: item=" << item;

    // Сохранение параметра в INI-файле
    if(cIniFile::Groups->count() > 0)
    {
        QString qsGroupName = cIniFile::Groups->at(iCurrentIndexGlobal.load(std::memory_order_relaxed));
        settings.beginGroup(qsGroupName);
        QStringList list = settings.childKeys();
        if(list.contains(item))
        {
            qDebug() << qsGroupName << " contains " << item;
            settings.remove(item);
        }
        else
        {
            qDebug() << qsGroupName << " not contains " << item;
            settings.setValue(item, "true");
        }
        settings.endGroup();
        settings.sync();
    }
    else
    {
        s = "List is empty, exec Load function!!!";
    }
    // Отобразить картинку
    emit showCurrentIndexPicture();

    //---
    emit showExecStatus(s);
    //---
}

bool cListWidgetSubject::loadHashTagList()
{

    QFile file(qsFileNameHashTag);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Error: Could not open file: " << qsFileNameHashTag;
        return false;
    }

    QTextStream in(&file);
    if(cEnvironment::iSystemType == WINDOWS_SYSTEM_TYPE)
    {
        in.setCodec("Windows-1251");
        qDebug() << "Select Windows-1251 codec in loading case";
    }

    qslHashTagList->clear();

    while (!in.atEnd())
    {
        QString line = in.readLine();
        qslHashTagList->append(line);
    }

    file.close();
    qDebug() << "Load " << qslHashTagList->count() << " strings";
    return true;
}

bool cListWidgetSubject::addItemToList()
{
    QString s = "execActionInsertSubject()";

    QString qsGoal = qleAddItem->text();

    if(qsGoal.length() <= 0)
    {
        //Информационное сообщение
        s += ": Empty string, nothing to do!";
        s += qsGoal;

        //---
        emit showExecStatus(s);
        //---

        return false;
    }

    //---Загрузка списка Subject

    if(!loadHashTagList())
    {
        qDebug() << "Error: Could not load HashTagListSubject from file: " << qsFileNameHashTag;
        return false;
    }

    qDebug() << ": loadHashTagListSubject is sucsess";

    //Здесь должна быть проверка на наличие нового значения в списке
    if(qslHashTagList->indexOf(qsGoal) < 0)
    {
        listWidget->clear();
        int iLast = qslHashTagList->count() - 1;
        if(qslHashTagList->at(iLast) == "")
        {
            qslHashTagList->replace(iLast, qsGoal);
        }
        else
        {
            qslHashTagList->append(qsGoal);
        }
        listWidget->addItems(*qslHashTagList);

        //Сохранение нового списка Subject

        cLoadFiles::saveStringListToFile(qsFileNameHashTag, *qslHashTagList);

        //Информационное сообщение
        s += ": ";
        s += qsGoal;
    }
    else
    {
        //Информационное сообщение
        s += ": HashTagListSubject already contain ";
        s += qsGoal;
    }

    //---
    emit showExecStatus(s);
    //---
    return true;
}

void cListWidgetSubject::execListWidgetItemClicked()
{
    QString s = "Use RightMouseButton to Add / Remove item to record";
    //---
    emit showExecStatus(s);
    //---
}


void cListWidgetSubject::execLineEditSearchAllKeysTextChanched(QString s)
{
    QString result = "ListWidgetSubjectInstance: LineEditSearchAllKeysTextChanged=";
    result += s;
    int length = listWidget->count();
    if(length > 0)
    {
        int iKeys = 0;
        QListWidgetItem * qlswLine;
        QString qsLine;

        for(int i = 0; i < length; i++)
        {
            qlswLine = listWidget->item(i);
            qsLine = qlswLine->text();

            if(qsLine.contains(s))
            {
                iKeys++;
                listWidget->setCurrentRow(i);
            }
        }
        result += ">>>";
        result += QString::number(iKeys);
        result += " records";
    }
    else
    {
        result += ">>>nothing to do";
    }
    qDebug() << result;
}

