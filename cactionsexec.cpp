#include "cactionsexec.h"

cActionsExec::cActionsExec(QListWidget * other, QObject *parent) : QObject(parent)
{
    cIniFile::qslDeletedSections.clear();
    ListWidget = other;
}

void cActionsExec::appEndItem(QListWidgetItem * item)
{
    ListWidget->addItem(item);
    ListWidget->setCurrentItem(item);
    ListWidget->scrollToItem(item);
}

void cActionsExec::execActionRemoveMovie(bool x)
{
    QString s = "ActionRemoveMovie()";
    s += QString::number(x);

    QListWidgetItem * item0 = new QListWidgetItem("==ActionRemoveMovie==");
    item0->setForeground(Qt::blue);
    appEndItem(item0);

    //===
    // Создаем объект QSettings с указанием формата INI и пути к файлу
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    QStringList GroupsLocal = settings.childGroups();

    QListWidgetItem * item1 = new QListWidgetItem("childGroupsList length: " + QString::number(GroupsLocal.count()));
    appEndItem(item1);
    //qDebug() << "childGroupsList length: " << GroupsLocal.count();
    //qDebug() << "----------------------------";
    //---

    int iCount = 0;

    QStringList GroupsResult;//Список - результат
    GroupsResult.clear();

    QListIterator<QString> readIt(GroupsLocal);
    while (readIt.hasNext())
    {
        QString qsSection = readIt.next();

        //---
        bool IsSign = false;

        settings.beginGroup(qsSection);
        QList<QString> keys = settings.childKeys();

        QString qsName = settings.value("name", "noName").toString();
        QString qsPath = settings.value("path", "noPath").toString();
        QString qsWay = qsPath + "/" + qsName;

        if(qsName.toLower().indexOf(".mp4") >= 0)
        {
            iCount++;

            IsSign = true;

            GroupsResult.append(qsSection);//Добавление секции в список - результат

            //qDebug() << "Name=" << qsName << " iCount=" << iCount << " Keys count=" << iKeysCount;
            // Перебор всей ключей в секции
            QListIterator<QString> readKeyIt(keys);
            while (readKeyIt.hasNext())
            {
                QString qsKey = readKeyIt.next();
                qDebug() << qsKey;

                settings.remove(qsKey);
            }
            //qDebug() << "All keys in section " << qsSection << " removed!";
            QListWidgetItem * item2 = new QListWidgetItem("All keys in section " + qsSection + " removed!");
            appEndItem(item2);
        }

        settings.endGroup();

        //--- Удаление секции совсем ---
        if(IsSign)
        {
            settings.remove(qsSection);
            cIniFile::Groups->removeOne(qsSection);
            cIniFile::qslDeletedSections.append(qsWay);//#@
            //qDebug() << "Section " << qsSection << " removed!";
            QListWidgetItem * item3 = new QListWidgetItem("Section " + qsSection + " removed!");
            appEndItem(item3);
        }
        //---
    }//End of while (readIt.hasNext())

    // Выводим имена обрабатываемых файлов
    QListWidgetItem * item5 = new QListWidgetItem("RemovedItemsCount=" + QString::number(GroupsResult.count()));
    appEndItem(item5);
    QListWidgetItem * item6 = new QListWidgetItem("=RemovedItemsList=");
    appEndItem(item6);
    ListWidget->addItems(GroupsResult);
    QListWidgetItem * item7 = new QListWidgetItem("=RemovedItemsList tail=");
    item7->setForeground(Qt::darkGreen);
    appEndItem(item7);

    //===
    emit execShowExecStatus(s);
   //===

}//End of void cActionsExec::execActionRemoveMovie()

//=============================================================================

void cActionsExec::execActionRemoveText(bool x)
{
    QString s = "ActionRemoveText()";
    s += QString::number(x);

    QListWidgetItem * item0 = new QListWidgetItem("==ActionRemoveText==");
    item0->setForeground(Qt::blue);
    appEndItem(item0);

    //===
    // Создаем объект QSettings с указанием формата INI и пути к файлу
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    QStringList GroupsLocal = settings.childGroups();

    QListWidgetItem * item1 = new QListWidgetItem("childGroupsList length: " + QString::number(GroupsLocal.count()));
    appEndItem(item1);

    int iCount = 0;

    QStringList GroupsResult;//Список - результат
    GroupsResult.clear();

    QListIterator<QString> readIt(GroupsLocal);
    while (readIt.hasNext())
    {
        QString qsSection = readIt.next();

        bool IsSign = false;

        settings.beginGroup(qsSection);
        QList<QString> keys = settings.childKeys();

        QString qsName = settings.value("name", "noName").toString();
        QString qsPath = settings.value("path", "noPath").toString();
        QString qsWay = qsPath + "/" + qsName;

        if(qsName.toLower().indexOf(".txt") >= 0)
        {
            iCount++;

            IsSign = true;

            GroupsResult.append(qsSection);//Добавление секции в список - результат

            //qDebug() << "Name=" << qsName << " iCount=" << iCount << " Keys count=" << iKeysCount;

            //--- Удаление ключей секции: начало ---
            // Перебор всей ключей в секции
            QListIterator<QString> readKeyIt(keys);
            while (readKeyIt.hasNext())
            {
                QString qsKey = readKeyIt.next();
                qDebug() << qsKey;
                settings.remove(qsKey);
            }
            //qDebug() << "All keys in section " << qsSection << " removed!";
            QListWidgetItem * item2 = new QListWidgetItem("All keys in section " + qsSection + " removed!");
            appEndItem(item2);
            //--- Удаление ключей секции: конец ---
        }

        settings.endGroup();

        //--- Удаление секции совсем ---
        if(IsSign)
        {
            settings.remove(qsSection);
            cIniFile::Groups->removeOne(qsSection);
            cIniFile::qslDeletedSections.append(qsWay);//#@

            QListWidgetItem * item3 = new QListWidgetItem("Section " + qsSection + " removed!");
            appEndItem(item3);
            //qDebug() << "Section " << qsSection << " removed!";
        }

    }//End of while (readIt.hasNext())

    // Выводим имена обрабатываемых файлов
    QListWidgetItem * item5 = new QListWidgetItem("RemovedItemsCount=" + QString::number(GroupsResult.count()));
    appEndItem(item5);
    QListWidgetItem * item6 = new QListWidgetItem("=RemovedItemsList=");
    appEndItem(item6);
    ListWidget->addItems(GroupsResult);
    QListWidgetItem * item7 = new QListWidgetItem("=RemovedItemsList tail=");
    item7->setForeground(Qt::darkGreen);
    appEndItem(item7);

    //===
    emit execShowExecStatus(s);
   //===

}

//=============================================================================

void cActionsExec::execActionRemoveTif(bool x)
{

    QString s = "ActionRemoveTif()";
    s += QString::number(x);

    QListWidgetItem * item0 = new QListWidgetItem("==ActionRemoveTif==");
    item0->setForeground(Qt::blue);
    appEndItem(item0);

    //===
    // Создаем объект QSettings с указанием формата INI и пути к файлу
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    QStringList GroupsLocal = settings.childGroups();

    QListWidgetItem * item1 = new QListWidgetItem("childGroupsList length: " + QString::number(GroupsLocal.count()));
    appEndItem(item1);

    int iCount = 0;

    QStringList GroupsResult;//Список - результат
    GroupsResult.clear();

    QListIterator<QString> readIt(GroupsLocal);
    while (readIt.hasNext())
    {
        QString qsSection = readIt.next();

        bool IsSign = false;

        settings.beginGroup(qsSection);
        QList<QString> keys = settings.childKeys();

        QString qsName = settings.value("name", "noName").toString();
        QString qsPath = settings.value("path", "noPath").toString();
        QString qsWay = qsPath + "/" + qsName;

        if(qsName.toLower().indexOf(".tif") >= 0)
        {
            iCount++;

            IsSign = true;

            GroupsResult.append(qsSection);//Добавление секции в список - результат

            //qDebug() << "Name=" << qsName << " iCount=" << iCount << " Keys count=" << iKeysCount;
            // Перебор всей ключей в секции
            QListIterator<QString> readKeyIt(keys);
            while (readKeyIt.hasNext())
            {
                QString qsKey = readKeyIt.next();
                qDebug() << qsKey;
                settings.remove(qsKey);
            }
            //qDebug() << "All keys in section " << qsSection << " removed!";
            QListWidgetItem * item2 = new QListWidgetItem("All keys in section " + qsSection + " removed!");
            appEndItem(item2);
        }

        settings.endGroup();

        //--- Удаление секции совсем ---
        if(IsSign)
        {
            settings.remove(qsSection);
            cIniFile::Groups->removeOne(qsSection);
            cIniFile::qslDeletedSections.append(qsWay);//#@

            QListWidgetItem * item3 = new QListWidgetItem("Section " + qsSection + " removed!");
            appEndItem(item3);
            //qDebug() << "Section " << qsSection << " removed!";
        }

    }//End of while (readIt.hasNext())

    // Выводим имена обрабатываемых файлов
    QListWidgetItem * item5 = new QListWidgetItem("RemovedItemsCount=" + QString::number(GroupsResult.count()));
    appEndItem(item5);
    QListWidgetItem * item6 = new QListWidgetItem("=RemovedItemsList=");
    appEndItem(item6);
    ListWidget->addItems(GroupsResult);
    QListWidgetItem * item7 = new QListWidgetItem("=RemovedItemsList tail=");
    item7->setForeground(Qt::darkGreen);
    appEndItem(item7);

    //===
    emit execShowExecStatus(s);
   //===

}

//=============================================================================

void cActionsExec::execActionRemoveBin(bool x)
{

    QString s = "ActionRemoveBin()";
    s += QString::number(x);

    QListWidgetItem * item0 = new QListWidgetItem("==ActionRemoveBin==");
    item0->setForeground(Qt::blue);
    appEndItem(item0);

    //===
    // Создаем объект QSettings с указанием формата INI и пути к файлу
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    QStringList GroupsLocal = settings.childGroups();

    QListWidgetItem * item1 = new QListWidgetItem("childGroupsList length: " + QString::number(GroupsLocal.count()));
    appEndItem(item1);

    int iCount = 0;

    QStringList GroupsResult;//Список - результат
    GroupsResult.clear();

    QListIterator<QString> readIt(GroupsLocal);
    while (readIt.hasNext())
    {
        QString qsSection = readIt.next();

        bool IsSign = false;

        settings.beginGroup(qsSection);
        QList<QString> keys = settings.childKeys();

        QString qsName = settings.value("name", "noName").toString();
        QString qsPath = settings.value("path", "noPath").toString();
        QString qsWay = qsPath + "/" + qsName;

        if(qsName.toLower().indexOf(".bin") >= 0)
        {
            iCount++;

            IsSign = true;

            GroupsResult.append(qsSection);//Добавление секции в список - результат

            //qDebug() << "Name=" << qsName << " iCount=" << iCount << " Keys count=" << iKeysCount;
            // Перебор всей ключей в секции
            QListIterator<QString> readKeyIt(keys);
            while (readKeyIt.hasNext())
            {
                QString qsKey = readKeyIt.next();
                qDebug() << qsKey;
                settings.remove(qsKey);
            }
            //qDebug() << "All keys in section " << qsSection << " removed!";

            QListWidgetItem * item2 = new QListWidgetItem("All keys in section " + qsSection + " removed!");
            appEndItem(item2);
        }

        settings.endGroup();

        //--- Удаление секции совсем ---
        if(IsSign)
        {
            settings.remove(qsSection);
            cIniFile::Groups->removeOne(qsSection);
            cIniFile::qslDeletedSections.append(qsWay);//#@

            QListWidgetItem * item3 = new QListWidgetItem("Section " + qsSection + " removed!");
            appEndItem(item3);
            //qDebug() << "Section " << qsSection << " removed!";
        }

    }//End of while (readIt.hasNext())

    // Выводим имена обрабатываемых файлов
    QListWidgetItem * item5 = new QListWidgetItem("RemovedItemsCount=" + QString::number(GroupsResult.count()));
    appEndItem(item5);
    QListWidgetItem * item6 = new QListWidgetItem("=RemovedItemsList=");
    appEndItem(item6);
    ListWidget->addItems(GroupsResult);
    QListWidgetItem * item7 = new QListWidgetItem("=RemovedItemsList tail=");
    item7->setForeground(Qt::darkGreen);
    appEndItem(item7);

    //===
    emit execShowExecStatus(s);
   //===

}

//=============================================================================

void cActionsExec::execActionRemove3gp(bool x)
{
    QString s = "ActionRemove3gp()";
    s += QString::number(x);

    QListWidgetItem * item0 = new QListWidgetItem("==ActionRemove3gp==");
    item0->setForeground(Qt::blue);
    appEndItem(item0);

    //===
    // Создаем объект QSettings с указанием формата INI и пути к файлу
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    QStringList GroupsLocal = settings.childGroups();

    QListWidgetItem * item1 = new QListWidgetItem("childGroupsList length: " + QString::number(GroupsLocal.count()));
    appEndItem(item1);

    int iCount = 0;

    QStringList GroupsResult;//Список - результат
    GroupsResult.clear();

    QListIterator<QString> readIt(GroupsLocal);
    while (readIt.hasNext())
    {
        QString qsSection = readIt.next();

        bool IsSign = false;

        settings.beginGroup(qsSection);
        QList<QString> keys = settings.childKeys();

        QString qsName = settings.value("name", "noName").toString();
        QString qsPath = settings.value("path", "noPath").toString();
        QString qsWay = qsPath + "/" + qsName;

        if(qsName.toLower().indexOf(".3gp") >= 0)
        {
            iCount++;

            IsSign = true;

            GroupsResult.append(qsSection);//Добавление секции в список - результат

            //qDebug() << "Name=" << qsName << " iCount=" << iCount << " Keys count=" << iKeysCount;
            // Перебор всей ключей в секции
            QListIterator<QString> readKeyIt(keys);
            while (readKeyIt.hasNext())
            {
                QString qsKey = readKeyIt.next();
                qDebug() << qsKey;
                settings.remove(qsKey);
            }
            //qDebug() << "All keys in section " << qsSection << " removed!";
            QListWidgetItem * item2 = new QListWidgetItem("All keys in section " + qsSection + " removed!");
            appEndItem(item2);
        }

        settings.endGroup();

        //--- Удаление секции совсем ---
        if(IsSign)
        {
            settings.remove(qsSection);
            cIniFile::Groups->removeOne(qsSection);
            cIniFile::qslDeletedSections.append(qsWay);//#@

            QListWidgetItem * item3 = new QListWidgetItem("Section " + qsSection + " removed!");
            appEndItem(item3);
            //qDebug() << "Section " << qsSection << " removed!";
        }

    }//End of while (readIt.hasNext())

    // Выводим имена обрабатываемых файлов
    QListWidgetItem * item5 = new QListWidgetItem("RemovedItemsCount=" + QString::number(GroupsResult.count()));
    appEndItem(item5);
    QListWidgetItem * item6 = new QListWidgetItem("=RemovedItemsList=");
    appEndItem(item6);
    ListWidget->addItems(GroupsResult);
    QListWidgetItem * item7 = new QListWidgetItem("=RemovedItemsList tail=");
    item7->setForeground(Qt::darkGreen);
    appEndItem(item7);

}

//=============================================================================

void cActionsExec::execActionShowNewFiles(bool x)
{
    QString s = "execActionShowNewFiles()";
    s += QString::number(x);

    qDebug() << s;

    //---Создание рабочего списка
    std::unique_ptr<QList<cRecord> > ptrRecordList(new QList<cRecord>());
    cRecord::RecordList = ptrRecordList.get();

    //--- Очистка рабочего списка
    cRecord::RecordList->clear();

    //---Чтение содержимого каталога ---

    if(cRecord::readDirectory(cIniFile::IniFile.getDirectoryPah()) > 0)
    {
        QListWidgetItem * item0 = new QListWidgetItem("==ActionShowNewFiles==");
        item0->setForeground(Qt::blue);
        appEndItem(item0);

        QListWidgetItem * item1 = new QListWidgetItem("==Directory not found==");
        item0->setForeground(Qt::red);
        appEndItem(item1);

        QListWidgetItem * item2 = new QListWidgetItem("Path=" + cIniFile::IniFile.getDirectoryPah());
        appEndItem(item2);

        return;
    }

    //---

    cRecord::RecordsCount = cRecord::RecordList->count();
    qDebug() << "execActionShowNewFiles:RecordsCount=" << cRecord::RecordsCount;

    QListWidgetItem * item0 = new QListWidgetItem("==ActionShowNewFiles==");
    item0->setForeground(Qt::blue);
    appEndItem(item0);

    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    QStringList Groups = settings.childGroups();//Загрузка полного списка групп

    QListWidgetItem * item1 = new QListWidgetItem("AllGroupsListCount=" + QString::number(Groups.count()));
    appEndItem(item1);


    QStringList slNewItems;
    slNewItems.clear();

    int iAddedFilesCounter = 0;
    int iSkippedFilesCounter = 0;

    for(QList<cRecord>::iterator it = cRecord::RecordList->begin(); it != cRecord::RecordList->end(); ++it)
     {
        const cRecord rec = *it;

        QString name = rec.qsName;
        int iDotPosition = name.indexOf('.');
        QString groupName = name.mid(0, iDotPosition);

        QString path = rec.qsPath;
        int iNamePosition = path.indexOf(name);
        QString PathWithoutName = path.mid(0, iNamePosition - 1);

        int iExtensionPosition = path.indexOf('.');
        QString qsExtension = path.mid(iExtensionPosition + 1);

        //Добавление записи в конфигурационный файл, если её там нет
        if(!Groups.contains(groupName))
        {
            //qDebug() << "###Add section:" << groupName << " Path+FileName=" << path;
            slNewItems.append(path);
            iAddedFilesCounter++;

        }
        else
        {
            iSkippedFilesCounter++;

            //qDebug() << "Skip existing section:" << groupName;
        }

    }//End of for(QList<cRecord>::iterator it = cRecord::RecordList->begin(); it != cRecord::RecordList->end(); ++it)

    QListWidgetItem * item2 = new QListWidgetItem("AddedFilesCount=" + QString::number(iAddedFilesCounter));
    appEndItem(item2);

    QListWidgetItem * item3 = new QListWidgetItem("SkipedFilesCount=" + QString::number(iSkippedFilesCounter));
    appEndItem(item3);

    //qDebug() << "Result: added files counter=" << iAddedFilesCounter <<" skiped files couner=" << iSkippedFilesCounter;

    //Store data in file
    if(iAddedFilesCounter > 0)
    {
        if(cLoadFiles::saveStringListToFile(cIniFile::fileNewItems, slNewItems))
        {
             //qDebug() << "New items list stored in file:" << cIniFile::fileNewItems;
             QListWidgetItem * item4 = new QListWidgetItem("New items list stored in file:" + cIniFile::fileNewItems);
             appEndItem(item4);

        }
        else
        {
            //qDebug() << "Store in file:" << cIniFile::fileNewItems << " process error";
            QListWidgetItem * item5 = new QListWidgetItem("Store in file:" + cIniFile::fileNewItems + " process error");
            appEndItem(item5);
        }
    }
    else
    {
        QListWidgetItem * item6 = new QListWidgetItem("=New items not founded=");
        appEndItem(item6);
        //qDebug() << "New items not founded";
    }



    //---
    emit execShowExecStatus(s);
    //---
}

//=============================================================================

void cActionsExec::execActionRemove_Js(bool x)
{
    QString s = "ActionRemove_Js()";
    s += QString::number(x);

    QListWidgetItem * item0 = new QListWidgetItem("==ActionRemove_Js==");
    item0->setForeground(Qt::blue);
    appEndItem(item0);

    //===
    // Создаем объект QSettings с указанием формата INI и пути к файлу
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    QStringList GroupsLocal = settings.childGroups();

    QListWidgetItem * item1 = new QListWidgetItem("childGroupsList length: " + QString::number(GroupsLocal.count()));
    appEndItem(item1);
    //qDebug() << "childGroupsList length: " << GroupsLocal.count();
    //qDebug() << "----------------------------";
    //---

    int iCount = 0;

    QStringList GroupsResult;//Список - результат
    GroupsResult.clear();

    QListIterator<QString> readIt(GroupsLocal);
    while (readIt.hasNext())
    {
        QString qsSection = readIt.next();

        //---
        bool IsSign = false;

        settings.beginGroup(qsSection);
        QList<QString> keys = settings.childKeys();

        QString qsName = settings.value("name", "noName").toString();
        QString qsPath = settings.value("path", "noPath").toString();
        QString qsWay = qsPath + "/" + qsName;

        if(qsName.toLower().indexOf(".js") >= 0)
        {
            iCount++;

            IsSign = true;

            GroupsResult.append(qsSection);//Добавление секции в список - результат

            //qDebug() << "Name=" << qsName << " iCount=" << iCount << " Keys count=" << iKeysCount;
            // Перебор всей ключей в секции
            QListIterator<QString> readKeyIt(keys);
            while (readKeyIt.hasNext())
            {
                QString qsKey = readKeyIt.next();
                qDebug() << qsKey;

                settings.remove(qsKey);
            }
            //qDebug() << "All keys in section " << qsSection << " removed!";
            QListWidgetItem * item2 = new QListWidgetItem("All keys in section " + qsSection + " removed!");
            appEndItem(item2);
        }

        settings.endGroup();

        //--- Удаление секции совсем ---
        if(IsSign)
        {
            settings.remove(qsSection);
            cIniFile::Groups->removeOne(qsSection);
            cIniFile::qslDeletedSections.append(qsWay);//#@
            //qDebug() << "Section " << qsSection << " removed!";
            QListWidgetItem * item3 = new QListWidgetItem("Section " + qsSection + " removed!");
            appEndItem(item3);
        }
        //---
    }//End of while (readIt.hasNext())

    // Выводим имена обрабатываемых файлов
    QListWidgetItem * item5 = new QListWidgetItem("RemovedItemsCount=" + QString::number(GroupsResult.count()));
    appEndItem(item5);
    QListWidgetItem * item6 = new QListWidgetItem("=RemovedItemsList=");
    appEndItem(item6);
    ListWidget->addItems(GroupsResult);
    QListWidgetItem * item7 = new QListWidgetItem("=RemovedItemsList tail=");
    item7->setForeground(Qt::darkGreen);
    appEndItem(item7);

    //===
    emit execShowExecStatus(s);
   //===

}//End of void cActionsExec::execActionRemove_Js()

//=============================================================================

void cActionsExec::execActionRemove_Html(bool x)
{
    QString s = "ActionRemove_Html()";
    s += QString::number(x);

    QListWidgetItem * item0 = new QListWidgetItem("==ActionRemove_Html==");
    item0->setForeground(Qt::blue);
    appEndItem(item0);

    //===
    // Создаем объект QSettings с указанием формата INI и пути к файлу
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    QStringList GroupsLocal = settings.childGroups();

    QListWidgetItem * item1 = new QListWidgetItem("childGroupsList length: " + QString::number(GroupsLocal.count()));
    appEndItem(item1);
    //qDebug() << "childGroupsList length: " << GroupsLocal.count();
    //qDebug() << "----------------------------";
    //---

    int iCount = 0;

    QStringList GroupsResult;//Список - результат
    GroupsResult.clear();

    QListIterator<QString> readIt(GroupsLocal);
    while (readIt.hasNext())
    {
        QString qsSection = readIt.next();

        //---
        bool IsSign = false;

        settings.beginGroup(qsSection);
        QList<QString> keys = settings.childKeys();

        QString qsName = settings.value("name", "noName").toString();
        QString qsPath = settings.value("path", "noPath").toString();
        QString qsWay = qsPath + "/" + qsName;

        if(qsName.toLower().indexOf(".html") >= 0)
        {
            iCount++;

            IsSign = true;

            GroupsResult.append(qsSection);//Добавление секции в список - результат

            //qDebug() << "Name=" << qsName << " iCount=" << iCount << " Keys count=" << iKeysCount;
            // Перебор всей ключей в секции
            QListIterator<QString> readKeyIt(keys);
            while (readKeyIt.hasNext())
            {
                QString qsKey = readKeyIt.next();
                qDebug() << qsKey;

                settings.remove(qsKey);
            }
            //qDebug() << "All keys in section " << qsSection << " removed!";
            QListWidgetItem * item2 = new QListWidgetItem("All keys in section " + qsSection + " removed!");
            appEndItem(item2);
        }

        settings.endGroup();

        //--- Удаление секции совсем ---
        if(IsSign)
        {
            settings.remove(qsSection);
            cIniFile::Groups->removeOne(qsSection);
            cIniFile::qslDeletedSections.append(qsWay);//#@
            //qDebug() << "Section " << qsSection << " removed!";
            QListWidgetItem * item3 = new QListWidgetItem("Section " + qsSection + " removed!");
            appEndItem(item3);
        }
        //---
    }//End of while (readIt.hasNext())

    // Выводим имена обрабатываемых файлов
    QListWidgetItem * item5 = new QListWidgetItem("RemovedItemsCount=" + QString::number(GroupsResult.count()));
    appEndItem(item5);
    QListWidgetItem * item6 = new QListWidgetItem("=RemovedItemsList=");
    appEndItem(item6);
    ListWidget->addItems(GroupsResult);
    QListWidgetItem * item7 = new QListWidgetItem("=RemovedItemsList tail=");
    item7->setForeground(Qt::darkGreen);
    appEndItem(item7);

    //===
    emit execShowExecStatus(s);
   //===

}//End of void cActionsExec::execActionRemove_Html()

//=============================================================================


