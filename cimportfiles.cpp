#include "cimportfiles.h"

//int cImportFiles::CurrentIndex = 0;

int cImportFiles::MaxIndexValue = MAX_INDEX_DEFAULT_VALUE;

QString cImportFiles::labelExecStatusText = "";
bool cImportFiles::IslabelExecStatusTextChanged = false;

QString cImportFiles::labelFileNameText = "";
bool cImportFiles::IslabelFileNameTextChanged = false;


cImportFiles::cImportFiles(
        QListWidget * other,
        QListWidget * founded,
        QListWidget * keys,
        QObject *parent
        ) : QObject(parent)
{
    ListWidgetOther = other;
    ListWidgetFounded = founded;
    ListWidgetKeys = keys;
}

cImportFiles::~cImportFiles()
{

}

void cImportFiles::appEndItem(QListWidgetItem * item)
{
    ListWidgetOther->addItem(item);
    ListWidgetOther->setCurrentItem(item);
    ListWidgetOther->scrollToItem(item);
}


/*******************************************************************************
 * Функция создания списка файлов, не включённых в файл конфигурации
 ******************************************************************************/

void cImportFiles::execSearchNewFiles()
{

    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    QStringList Groups = settings.childGroups();//Загрузка полного списка групп

    qDebug() << "Groups count=" << Groups.count();

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
            qDebug() << "###Add section:" << groupName << " Path+FileName=" << path;
            slNewItems.append(path);
            iAddedFilesCounter++;

        }
        else
        {
            iSkippedFilesCounter++;

            qDebug() << "Skip existing section:" << groupName;
        }

    }//End of for(QList<cRecord>::iterator it = cRecord::RecordList->begin(); it != cRecord::RecordList->end(); ++it)

    qDebug() << "Result: added files counter=" << iAddedFilesCounter <<" skiped files couner=" << iSkippedFilesCounter;

    //Store data in file
    if(iAddedFilesCounter > 0)
    {
        if(cLoadFiles::saveStringListToFile(cIniFile::fileNewItems, slNewItems))
        {
             qDebug() << "New items list stored in file:" << cIniFile::fileNewItems;
        }
        else
        {
            qDebug() << "Store in file:" << cIniFile::fileNewItems << " process error";
        }
    }
    else
    {
        qDebug() << "New items not founded";
    }

}

//-------------------------------------------------------------------------
// Получить список групп
//-------------------------------------------------------------------------
void cImportFiles::getGroupsList()
{
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);
    *cIniFile::Groups = settings.childGroups();
    qDebug() << "cImportFiles::getGroupsList(): Groups count=" << cIniFile::Groups->count();

}//End of bool cImportFiles::getGroupsList()

bool cImportFiles::getKeysList()
{
    bool IsError = true;
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);
    *cIniFile::Keys = settings.allKeys();

    if(cIniFile::Keys->count() > 0)IsError = false;

    return IsError;//Возвращаем флаг ошибки
}

/******************************************************************************
 * Статическая функция bool cImportFiles::execImport() выполняет чтение
 * каталога - источника. Путь к каталогу задаётся исходя из типа
 * операционной системы
 * Целевым файлом функции является ini файл, имя которого задаётся исходя
 * из типа операционной системы.
 * Удалять файлы из каталога - источника запрещается!
 * Удалять секции из целевого ini файла запрещается!
 * Приведённая ниже функция производит добавление секций в целевой ini файл
 * СОДЕРЖИМОЕ КОНФИГУРАЦИОННОГО ФАЙЛА ОБНОВЛЯЕТСЯ, НО НЕ ПЕРЕЗАПИСЫВАЕТСЯ
 * Файлы с расширениями "mp4", "tif", "txt" в конфигурационный файл не добавляются
 *****************************************************************************/
void cImportFiles::execImport()
{
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    ListWidgetOther->addItem("==ExecImport==");

    QStringList Groups = settings.childGroups();//Загрузка полного списка групп

    ListWidgetOther->addItem("Initial groups count=" + QString::number(Groups.count()));

    //---Добавление идентификационной секции
    cImportFiles::MaxIndexValue = cRecord::RecordList->count();
    cIniFile::IniFile.addInitalSection(cImportFiles::MaxIndexValue);

    //---Подготовка к обработке файла данных
    iCurrentIndexGlobal.store(0);

    int iAddedFilesCounter = 0;
    int iSkippedFilesCounter = 0;
    int iErrorFilesCounter = 0;

    //---Начало обработки файла данных
    for(QList<cRecord>::iterator it = cRecord::RecordList->begin(); it != cRecord::RecordList->end(); ++it)
    {
        iCurrentIndexGlobal.fetch_add(1, std::memory_order_relaxed);
        int id = iCurrentIndexGlobal.load(std::memory_order_relaxed);

        const cRecord rec = *it;

        QString name = rec.qsName;
        int iDotPosition = name.indexOf('.');
        QString groupName = name.mid(0, iDotPosition);

        QString path = rec.qsPath;
        int iNamePosition = path.indexOf(name);
        QString PathWithoutName = path.mid(0, iNamePosition - 1);

        int size = rec.iSize;

        int iExtensionPosition = path.indexOf('.');
        QString qsExtension = path.mid(iExtensionPosition + 1);

        bool IsError = false;
        int width = 0;
        int height = 0;

        if(qsExtension.toLower() == "mp4")
        {
            qDebug() << "Id=" << iCurrentIndexGlobal.load(std::memory_order_relaxed) << "Extension: mp4";
            IsError = true;
            iErrorFilesCounter++;
        }
        else if(qsExtension.toLower() == "tif")
        {
            qDebug() << "Id=" << iCurrentIndexGlobal.load(std::memory_order_relaxed) << "Extension: tif";
            IsError = true;
            iErrorFilesCounter++;
        }
        else if(qsExtension.toLower() == "txt")
        {
            qDebug() << "Id=" << iCurrentIndexGlobal.load(std::memory_order_relaxed) << "Extension: txt";
            IsError = true;
            iErrorFilesCounter++;
        }
        else
        {
            //qDebug() << "Id=" << iCurrentIndexGlobal;

            //Фрагмент для обработки файлов изображений
            QImage image(path);//name
            if(image.isNull())
            {
                IsError = true;
                iErrorFilesCounter++;
            }
            else
            {
                width = image.width();
                height = image.height();
            }
        }

        //Добавление записи в конфигурационный файл, если её там нет
        if(!Groups.contains(groupName))
        {
            qDebug() << "###Add section:" << groupName;
            iAddedFilesCounter++;

            settings.beginGroup(groupName);
            settings.setValue("Id", id);
            settings.setValue("name", name);
            settings.setValue("path", PathWithoutName);
            settings.setValue("size", size);
            settings.setValue("new", true);//Признак новой записи
            if(IsError)
            {
                settings.setValue("error", true);
            }
            else
            {
                settings.setValue("width", width);
                settings.setValue("height", height);
            }
            settings.endGroup();

        }
        else
        {
            iSkippedFilesCounter++;

            qDebug() << "Skip existing section:" << groupName;
        }

    }//End of for(QList<cRecord>::iterator it = cRecord::RecordList->begin(); it != cRecord::RecordList->end(); ++it)

    //---Конец обработки файла данных

    settings.sync();

    qDebug() << "Result: added files counter=" << iAddedFilesCounter <<" skiped files couner=" << iSkippedFilesCounter;
    ListWidgetOther->addItem("Added groups count=" + QString::number(iAddedFilesCounter));
    ListWidgetOther->addItem("Skiped groups count=" + QString::number(iSkippedFilesCounter));
    ListWidgetOther->addItem("Error groups count=" + QString::number(iErrorFilesCounter));
    ListWidgetOther->addItem("*Final groups count=" + QString::number(Groups.count()) + "*");


}//End of void cImportFiles::execImport()

//-------------------------------------------------------------------------
// Выполнить импорт данных из каталога с файлами данных
//-------------------------------------------------------------------------
void cImportFiles::execActionImportInitial()
{

    QListWidgetItem * item0 = new QListWidgetItem("==ActionImportInitial==");
    item0->setForeground(Qt::blue);
    //ListWidgetOther->addItem(item0);
    appEndItem(item0);

    //---Создание рабочего списка
    std::unique_ptr<QList<cRecord> > ptrRecordList(new QList<cRecord>());
    cRecord::RecordList = ptrRecordList.get();

    //--- Очистка рабочего списка
    cRecord::RecordList->clear();

    //---Чтение содержимого каталога ---

    if(cRecord::readDirectory(cIniFile::IniFile.getDirectoryPah()) > 0)
    {
        QString path = cIniFile::iniFilePath;
        QListWidgetItem * itemX = new QListWidgetItem("Directory not found: " + path);
        itemX->setForeground(Qt::red);
        ListWidgetOther->addItem(itemX);
        return;
    }

    execImport();

    emit showExecStatus("File import complete!");

    cIniFile::qslDeletedSections.clear();

    QListWidgetItem * item1 = new QListWidgetItem("FileImport complete. Read " + QString::number(cRecord::RecordList->count()) + " files");
    appEndItem(item1);

    QMessageBox msgBox;
    msgBox.setText("execActionImportInitial");
    msgBox.setInformativeText("Ready");
    msgBox.exec();
    //QMessageBox::information(this, "Ready", "Import task completed");

    //---
    QString s = "ImportFilesInstance: execActionImportInitial()";
    emit showExecStatus(s);
    //---

}//End of void cImportFiles::execActionImportInitial()

//-------------------------------------------------------------------------
// Выполнить получение списка групп из файла конфигурации
//-------------------------------------------------------------------------

void cImportFiles::execActionGetGroupsList()
{
    QString s = "ImportFilesInstance: execActionGetGroupsList()";

    QListWidgetItem * item0 = new QListWidgetItem("==Action GetGroupsList==");
    item0->setForeground(Qt::blue);
    appEndItem(item0);

    ListWidgetFounded->clear();

    cImportFiles::getGroupsList();

    ListWidgetFounded->addItems(*cIniFile::Groups);

    //---
    emit showExecStatus(s);
    //---
}

//=============================================================================

void cImportFiles::execActionGetKeysList()
{
    QString s = "ImportFilesInstance:execActionGetKeysList()";

    QListWidgetItem * item0 = new QListWidgetItem("==Action GetKeysList==");
    item0->setForeground(Qt::blue);
    appEndItem(item0);

    ListWidgetKeys->clear();

    bool x = getKeysList();
    if(x)
    {
        s += ": error detected!";

        QListWidgetItem * itemX = new QListWidgetItem("getKeysList error!");
        itemX->setForeground(Qt::red);
        appEndItem(itemX);

    }
    else
    {
        s += ": sucsess!";

        //Удаление имени секции
        for(QList<QString>::iterator it = cIniFile::Keys->begin(); it != cIniFile::Keys->end(); ++it)
        {
            QString s = *it;
            int pos = s.lastIndexOf('/');
            if(pos > 0)
            {
                *it = s.mid(pos + 1);
            }
        }

        //---Удаление повторяющихся значений с сохранением порядка

        QSet<QString> seen;
        QStringList result;
        for(const QString &str : *cIniFile::Keys)
        {
            if(!seen.contains(str))
            {
                seen.insert(str);
                result.append(str);
            }
        }
        *cIniFile::Keys = result;

        //---
        ListWidgetKeys->addItems(*cIniFile::Keys);
        QListWidgetItem * item1 = new QListWidgetItem("GetKeysList: all keys=" + QString::number(cIniFile::Keys->count()));
        item1->setForeground(Qt::darkGreen);
        appEndItem(item1);
    }

    //---
    emit showExecStatus(s);
    //---
}

//=============================================================================

void cImportFiles::execActionLoad()
{
    //--- Начало функции загрузки
    qDebug() << "execActionLoad(): begin";

    QListWidgetItem * item0 = new QListWidgetItem("==ImportFilesInstance::Action Load==");
    item0->setForeground(Qt::blue);
    ListWidgetOther->addItem(item0);

    //Загрузка списка групп
    getGroupsList();
    cImportFiles::MaxIndexValue = cIniFile::Groups->count();

    // Загрузка списка ключей
    execActionGetKeysList();

    // Установка текущего индекса
    iCurrentIndexGlobal.store(cIniFile::getCurrentIndex());

    // Переход к следующему индексу
    //NavigationInstance->installNavigation();//20250709 Настройка навигации
    emit gotoInstallNavigation();

    //---
    QString s = "execActionLoad(), goto index:";
    s += QString::number(iCurrentIndexGlobal.load(std::memory_order_relaxed));
    emit showExecStatus(s);
    //---

}

//=============================================================================

