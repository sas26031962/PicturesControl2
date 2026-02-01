#include "cloadfiles.h"

cLoadFiles::cLoadFiles(QListWidget *other)
{
    ListWidget = other;
}

//void cLoadFiles::install(QListWidget * other)
//{
//    ListWidget = other;
//}

//=============================================================================

void cLoadFiles::appEndItem(QListWidgetItem * item)
{
    ListWidget->addItem(item);
    ListWidget->setCurrentItem(item);
    ListWidget->scrollToItem(item);
}

//=============================================================================

/******************************************************************************
 * Функция выбирает из полного списка файлов те файлы, которые содержат признак
 * IsRotated.
 * Результат работы функции - список cIniFile::Groups
 *****************************************************************************/
void cLoadFiles::execLoadFilesSignedIsRotated()
{
    //--- Начало функции загрузки

    QListWidgetItem * item0 = new QListWidgetItem("==execLoadFilesSignedIsRotated==");
    item0->setForeground(Qt::blue);
    ListWidget->addItem(item0);

    // Создаем объект QSettings с указанием формата INI и пути к файлу
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    // Читаем значения из INI-файла

    QStringList TotalGroups = settings.childGroups();//Загрузка полного списка групп
    cIniFile::Groups->clear();//Очистка результата

    // Выводим значения
    QListWidgetItem * item1 = new QListWidgetItem("TotalGroups length: " + QString::number(TotalGroups.count()));
    ListWidget->addItem(item1);

    int iCount = 0;// Очистка счётчика найденных объектов

    QListIterator<QString> readIt(TotalGroups);
    while (readIt.hasNext())
    {
        QString qsSection = readIt.next();
        //qDebug() << qsSection;
        settings.beginGroup(qsSection);
        //===
        QString qsIsRotated;

        QStringList keys = settings.childKeys();
        QListIterator<QString> readIt(keys);
        while(readIt.hasNext())
        {
            QString key = readIt.next();
            QString value = settings.value(key,"0").toString();

            if(key == "IsRotated")
            {
                qsIsRotated = value;
                iCount++;
                cIniFile::Groups->append(qsSection);
                qDebug() << "iterator: section=" << qsSection << " key=" << key << " count=" << iCount;
            }
        }
        //===
        settings.endGroup();
    }

    QString qsItem2;
    if(iCount > 0)
    {
        qsItem2 = "IsRotated key detected in ";
        qsItem2 += QString::number(iCount);
        qsItem2 += " files";
        //qDebug() << "IsRotated key detected in " << iCount << " files";
    }
    else
    {
        qsItem2 = "No IsRotated key detected";
        //qDebug() << "No IsRotated key detected";
    }

    QListWidgetItem * item2 = new QListWidgetItem(qsItem2);
    ListWidget->addItem(item2);

    //--- Окончание функции загрузки

    settings.sync();

}//End of void cLoadFiles::execLoadFilesSignedIsRotated()

//=============================================================================


bool cLoadFiles::saveStringListToFile(const QString& fileName, const QStringList& list)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }
    else
    {
        // Установка кодировки
        QTextStream out(&file);
        if(cEnvironment::iSystemType == WINDOWS_SYSTEM_TYPE)
            out.setCodec("Windows-1251");
        else
            out.setCodec("UTF-8");

        for (const QString& str : list) {
            out << str << "\n";
        }

        file.close();
        return true;
    }
}

//=============================================================================

QStringList cLoadFiles::loadStringListFromFile(const QString& fileName)
{
    QStringList list;
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");

        while (!in.atEnd()) {
            list << in.readLine();
        }

        file.close();
    }
    return list;
}

//=============================================================================

