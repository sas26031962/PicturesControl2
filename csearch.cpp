#include "csearch.h"

cSearch::cSearch(QListWidget * founded, QListWidget * other, QListWidget * keys, QLineEdit * pattern, QLineEdit * search_all_keys, QObject *parent) : QObject(parent)
{
    QChar qcFill = QChar('-');
    qsFoundedDelimiter = QString(20, qcFill);

    ListWidgetFounded = founded;
    ListWidgetOther = other;
    ListWidgetKeys = keys;
    LineEditPattern = pattern;
    LineEditSearchAllKeys = search_all_keys;

    connect(ListWidgetFounded, &QListWidget::itemClicked, this, &cSearch::execListWidgetFoundedItemClicked);
    connect(LineEditSearchAllKeys, &QLineEdit::textChanged, this, &cSearch::execLineEditSearchAllKeysTextChanched);

}

void cSearch::appEndItem(QListWidgetItem * item)
{
    ListWidgetOther->addItem(item);
    ListWidgetOther->setCurrentItem(item);
    ListWidgetOther->scrollToItem(item);
}


//=============================================================================

//
// Отобразить содержимое списка cIniFile::Groups в элементе ui->listWidgetFounded
//
void cSearch::showGroupsList()
{
    ListWidgetFounded->clear();
    QListIterator<QString> readIt(*cIniFile::Groups);
    while (readIt.hasNext())
    {
        QString qsSection = readIt.next();
        ListWidgetFounded->addItem(qsSection);
    }
}

//=============================================================================

void cSearch::execListWidgetFoundedItemClicked()
{
    QString s = "execListWidgetFoundedItemClicked()";
    QString value = ListWidgetFounded->currentItem()->text();

    if(value != qsFoundedDelimiter)
    {
        int FoundedIndex = -1;

        if(cIniFile::Groups->contains(value))
        {
            FoundedIndex = cIniFile::Groups->indexOf(value);


            // Модификация индекса
            iCurrentIndexGlobal.store(FoundedIndex, std::memory_order_relaxed);
            // Отобразить картинку
            emit showCurrentIndexPicture();

        }
        else
        {
            QListWidgetItem * item0 = new QListWidgetItem("!!!Item:" + value + " not found!!!");
            item0->setForeground(Qt::red);
            ListWidgetOther->addItem(item0);
        }

        s += ": ";
        s += value;
    }
    else
    {
        s += ": delimiter >> nothing to show";
    }
    //---
    emit showExecStatus(s);
    //---
}

//=============================================================================

bool cSearch::searchFreshRecords()
{
    QListWidgetItem * item0 = new QListWidgetItem("==execSearchFreshRecords==");
    item0->setForeground(Qt::blue);
    ListWidgetOther->addItem(item0);

    // Создаем объект QSettings с указанием формата INI и пути к файлу
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    // Читаем значения из INI-файла

    QStringList TotalGroups = settings.childGroups();//Загрузка полного списка групп

    QListWidgetItem * item1 = new QListWidgetItem("TotalGroups length: " + QString::number(TotalGroups.count()));
    ListWidgetOther->addItem(item1);

    cIniFile::Groups->clear();//Очистка результата

    int iCountYes = 0;// Очистка счётчика найденных объектов
    int iCountNo = 0;
    QListIterator<QString> readIt(TotalGroups);
    while (readIt.hasNext())
    {
        QString qsSection = readIt.next();
        //qDebug() << qsSection;
        //Проверка условия
        settings.beginGroup(qsSection);
        QStringList keys = settings.childKeys();
        settings.endGroup();

        bool match = (keys.count() <= 6);//true;

        //Запись результата в список
        if(match)
        {
            iCountYes++;//Подсчёт удачных попыток
            cIniFile::Groups->append(qsSection);
        }
        else
        {
            iCountNo++;//Подсчёт неудачных попыток
        }
    }

    QString qsItem2;
    qsItem2 = "Count NO in ";
    qsItem2 += QString::number(iCountNo);
    qsItem2 += " files";

    QString qsItem3;
    qsItem3 = "Count YES in ";
    qsItem3 += QString::number(iCountYes);
    qsItem3 += " files";

    QListWidgetItem * item2 = new QListWidgetItem(qsItem2);
    ListWidgetOther->addItem(item2);

    QListWidgetItem * item3 = new QListWidgetItem(qsItem3);
    ListWidgetOther->addItem(item3);

    //---
    bool x = cLoadFiles::saveStringListToFile(cIniFile::pattern1StringListFilePath, *cIniFile::Groups);
    //---
    return x;
}

//=============================================================================

bool cSearch::searchNamePattern(const QString& pattern)
{
    QRegularExpression re(pattern);

    QListWidgetItem * item0 = new QListWidgetItem("==execSearchByNamePattern==");
    item0->setForeground(Qt::blue);
    ListWidgetOther->addItem(item0);

    QListWidgetItem * item1 = new QListWidgetItem("<Pattern>=" + pattern);
    ListWidgetOther->addItem(item1);

    //qDebug() << "Search pattern:" << pattern;

    // Создаем объект QSettings с указанием формата INI и пути к файлу
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    // Читаем значения из INI-файла

    QStringList TotalGroups = settings.childGroups();//Загрузка полного списка групп

    QListWidgetItem * item2 = new QListWidgetItem("TotalGroups length: " + QString::number(TotalGroups.count()));
    ListWidgetOther->addItem(item2);

    cIniFile::Groups->clear();//Очистка результата

    int iCount = 0;// Очистка счётчика найденных объектов
    QListIterator<QString> readIt(TotalGroups);
    while (readIt.hasNext())
    {
        QString qsSection = readIt.next();
        //qDebug() << qsSection;
        bool match = re.match(qsSection.toLower()).hasMatch();
        if(match)
        {
            iCount++;
            cIniFile::Groups->append(qsSection);
            //qDebug() << "iterator: section=" << qsSection << " contain pattern:" << pattern << " count=" << iCount;
        }
    }

    QString qsItem3;
    if(iCount > 0)
    {
        qsItem3 = "<Pattern> in file names detected in ";
        qsItem3 += QString::number(iCount);
        qsItem3 += " files";
    }
    else
    {
        qsItem3 = "No <Pattern> in file names detected";
    }

    QListWidgetItem * item3 = new QListWidgetItem(qsItem3);
    ListWidgetOther->addItem(item3);

    //---
    bool x = cLoadFiles::saveStringListToFile(cIniFile::pattern1StringListFilePath, *cIniFile::Groups);
    //---
    return x;
}

//=============================================================================

/******************************************************************************
 * Функция выбирает из полного списка файлов те файлы, которые удовлетворяют
 * любому из списка условий, приведённых в списке yes, передаваемому как параметр.
 * Результат работы функции - список cIniFile::Groups
 *****************************************************************************/
void cSearch::execLoadFilesByConditionOrYes(QStringList yes)
{
    //--- Начало функции загрузки

    QListWidgetItem * item0 = new QListWidgetItem("==execLoadFilesByConditionOrYes==");
    item0->setForeground(Qt::blue);
    ListWidgetOther->addItem(item0);

    // Создаем объект QSettings с указанием формата INI и пути к файлу
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    // Читаем значения из INI-файла

    QStringList TotalGroups = settings.childGroups();//Загрузка полного списка групп
    cIniFile::Groups->clear();//Очистка результата

    // Выводим значения
    QListWidgetItem * item1 = new QListWidgetItem("TotalGroups length: " + QString::number(TotalGroups.count()));
    ListWidgetOther->addItem(item1);

    int iCountYes = 0;// Очистка счётчика записей, удовлетворяющих условию
    int iCountNo = 0;// Очистка счётчика записей, не удовлетворяющих условию

    //============== ОПРОС ПОЛНОГО СПИСКА ===================
    QListIterator<QString> readItTotal(TotalGroups);
    while (readItTotal.hasNext())
    {
        QString qsSection = readItTotal.next();
        //qDebug() << qsSection;
        settings.beginGroup(qsSection);

        QString qsIsRotated;

        QStringList keys = settings.childKeys();
        QListIterator<QString> readItKeys(keys);

        //------ ОПРОС СПИСКА КЛЮЧЕЙ ТЕКУЩЕЙ ЗАПИСИ ---------
        while(readItKeys.hasNext())
        {
            QString key = readItKeys.next();
            QString value = settings.value(key,"0").toString();

            QListIterator<QString> readItYes(yes);
            //~~~ ПЕРЕБОР УСЛОВИЙ ПРИ ПРОВЕРКЕ ТЕКУЩЕГО КЛЮЧА ~~~

            while (readItYes.hasNext())
            {
                QString key_yes = readItYes.next();
                //qDebug() << "iteratorYes: key=" << key_yes;// << " count=" << iCount;
                QString value_yes = settings.value(key_yes,"0").toString();
                if(key == key_yes)
                {
                    qsIsRotated = value_yes;
                    iCountYes++;
                    cIniFile::Groups->append(qsSection);
                    //qDebug() << "iterator: section=" << qsSection << " key=" << key << " count=" << iCount;
                }
                else
                {
                    iCountNo++;
                }
            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        }//End of while(readItKeys.hasNext())

        //---------------------------------------------------
        settings.endGroup();

    }//End of while (readIt.hasNext())
    //=======================================================

    QString qsItem2;
    if(iCountYes > 0)
    {
        qsItem2 = "Signed by condition OrYes key detected in ";
        qsItem2 += QString::number(iCountYes);
        qsItem2 += " files";
    }
    else
    {
        qsItem2 = "No signed by condition OrYes key detected";
    }

    QListWidgetItem * item2 = new QListWidgetItem(qsItem2);
    ListWidgetOther->addItem(item2);

    //--- Окончание функции загрузки

    settings.sync();

}//End of void cSearch::execLoadFilesByConditionOrYes(QStringList yes)

//=============================================================================

/******************************************************************************
 * Функция выбирает из полного списка файлов те файлы, которые удовлетворяют
 * каждому из списка условий, приведённых в списке yes, передаваемому как параметр.
 * Результат работы функции - список cIniFile::Groups
 *****************************************************************************/
void cSearch::execLoadFilesByConditionYesYes(QStringList yes)
{
    //--- Начало функции загрузки

    QListWidgetItem * item0 = new QListWidgetItem("==execLoadFilesByConditionYesYes==");
    item0->setForeground(Qt::blue);
    ListWidgetOther->addItem(item0);

    // Создаем объект QSettings с указанием формата INI и пути к файлу
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    // Читаем значения из INI-файла

    QStringList TotalGroups = settings.childGroups();//Загрузка полного списка групп

    // Выводим значения
    QListWidgetItem * item1 = new QListWidgetItem("TotalGroups length: " + QString::number(TotalGroups.count()));
    ListWidgetOther->addItem(item1);

    int iCountYes = 0;// Очистка счётчика записей, удовлетворяющих условию
    int iCountNo = 0;// Очистка счётчика записей, не удовлетворяющих условию

    //============= ОПРОС ПЕРВОГО ЭЛЕМЕНТА ==================
    qDebug() << "#Check sections and keys for first elemen of StringList YES: BEGIN";
    QStringList FirstGroups;
    FirstGroups.clear();

    QListIterator<QString> readItTotal0(TotalGroups);
    while (readItTotal0.hasNext())
    {
        QString qsSection = readItTotal0.next();
        //qDebug() << qsSection;

        //Перебор всех ключей данной секции
        settings.beginGroup(qsSection);

        QStringList keys = settings.childKeys();
        QListIterator<QString> readItKeys(keys);

        //------ ОПРОС СПИСКА КЛЮЧЕЙ ТЕКУЩЕЙ ЗАПИСИ ---------
        QString key_yes = yes.at(0);

        while(readItKeys.hasNext())
        {
            QString key = readItKeys.next();

            //qDebug() << "iteratorYes: key=" << key_yes;// << " count=" << iCount;
            if(key == key_yes)
            {
                iCountYes++;
                FirstGroups.append(qsSection);
                qDebug() << "Inser section=" << qsSection << " becouse found key=" << key_yes;
            }
            else
            {
                iCountNo++;
            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        }//End of while(readItKeys.hasNext())

        //---------------------------------------------------
        settings.endGroup();

    }//End of while (readIt.hasNext())
    qDebug() << "#Check sections and keys for first elemen of StringList YES: END";
    qDebug() << "#Check sections and keys for first elemen of StringList YES: Found recors count=" << iCountYes;

    //Первая итерация результата
    //*cIniFile::Groups = FirstGroups;
    cIniFile::Groups->clear();
    cIniFile::Groups->append(FirstGroups);
    qDebug() << "ResultGroups count=" << cIniFile::Groups->count();

    if((yes.count() > 1))
    {
        //============ ОПРОС ОСТАВШЕГОСЯ СПИСКА =================
        qDebug() << "#Check sections and keys for tail elemens of StringList YES: BEGIN";

        QListIterator<QString> readItFirst(FirstGroups);
        while (readItFirst.hasNext())
        {
            QString qsSection = readItFirst.next();

            settings.beginGroup(qsSection);

            QListIterator<QString> readItYes(yes);

            readItYes.next();//Пропускаем первую секцию из StringList YES

            while (readItYes.hasNext())
            {
                QString key_yes = readItYes.next();

                QStringList keys = settings.childKeys();
                QListIterator<QString> readItKeys(keys);

                //------ ОПРОС СПИСКА КЛЮЧЕЙ ТЕКУЩЕЙ ЗАПИСИ ---------
                bool IsKeyFound = false;
                while(readItKeys.hasNext())
                {
                    QString key = readItKeys.next();


                    //~~~ ПЕРЕБОР УСЛОВИЙ ПРИ ПРОВЕРКЕ ТЕКУЩЕГО КЛЮЧА ~~~

                    if(key == key_yes)
                    {
                        IsKeyFound = true;
                    }

                    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

                }//End of while(readItKeys.hasNext())

                if(!IsKeyFound)
                {
                    //qDebug() << "Section=" << qsSection << " must be removed becouse not found key=";// << key_yes;
                    if(cIniFile::Groups->contains(qsSection))
                    {
                        iCountNo++;
                        iCountYes--;
                        cIniFile::Groups->removeOne(qsSection);//Удаление секции из списка, если не удовлетворяет условию
                        qDebug() << "Remove section=" << qsSection << " becouse not found key=";// << key_yes;
                    }
                }
                else
                {
                    qDebug() << "In section=" << qsSection << " found key=";// << key_yes;
                }

            }//End of while (readItYes.hasNext())

            //---------------------------------------------------
            settings.endGroup();

        }//End of while (readIt.hasNext())
        //=======================================================

        qDebug() << "#Check sections and keys for tail elemens of StringList YES: END";
    }

    QString qsItem2;
    if(iCountYes > 0)
    {
        qsItem2 = "Signed by condition OrYes key detected in ";
        qsItem2 += QString::number(iCountYes);
        qsItem2 += " files";
    }
    else
    {
        qsItem2 = "No signed by condition OrYes key detected";
    }

    QListWidgetItem * item2 = new QListWidgetItem(qsItem2);
    ListWidgetOther->addItem(item2);

    //--- Окончание функции загрузки

    settings.sync();

}//End of void cSearch::execLoadFilesByConditionYesYes(QStringList yes)

//=============================================================================

/******************************************************************************
 * Функция выбирает из полного списка файлов те файлы, которые содержат признак
 * IsRotated.
 * Результат работы функции - список cIniFile::Groups
 *****************************************************************************/
void cSearch::execLoadFilesSignedIsRotated()
{
    //--- Начало функции загрузки

    QListWidgetItem * item0 = new QListWidgetItem("==execLoadFilesSignedIsRotated==");
    item0->setForeground(Qt::blue);
    ListWidgetOther->addItem(item0);

    // Создаем объект QSettings с указанием формата INI и пути к файлу
    QSettings settings(cIniFile::iniFilePath, QSettings::IniFormat);

    // Читаем значения из INI-файла

    QStringList TotalGroups = settings.childGroups();//Загрузка полного списка групп
    cIniFile::Groups->clear();//Очистка результата

    // Выводим значения
    QListWidgetItem * item1 = new QListWidgetItem("TotalGroups length: " + QString::number(TotalGroups.count()));
    ListWidgetOther->addItem(item1);

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
    ListWidgetOther->addItem(item2);

    //--- Окончание функции загрузки

    settings.sync();

}//End of void cLoadFiles::execLoadFilesSignedIsRotated()

//=============================================================================

void cSearch::execActionSearchRotated()
{
    QString s = "SearchInstance::execActionSearchRotated()";

    execLoadFilesSignedIsRotated();

    emit gotoInstallNavigation();//Настройка навигации

    s += ": find ";
    s += QString::number(cImportFiles::MaxIndexValue);
    s += " records";
    //---
    emit showExecStatus(s);
    //---
}

//=============================================================================

void cSearch::execActionSearchYesYes()
{
    QString s = "SearchInstance::execActionSearchYesYes()";

    if(cIniFile::SearchKeys->count() > 0)
    {
        execLoadFilesByConditionYesYes(*cIniFile::SearchKeys);

        //NavigationInstance->installNavigation();//Настройка навигации
        emit gotoInstallNavigation();//Настройка навигации

        s += ": find ";
        s += QString::number(cImportFiles::MaxIndexValue);
        s += " records";
    }
    else
    {
        s += ": empy search task, nothing to do!!!";
    }
    //---
    emit showExecStatus(s);
    //---
}

//=============================================================================

void cSearch::execActionSearchOrYes()
{
    QString s = "SearchInstance::execActionSearchOrYes()";

    if(cIniFile::SearchKeys->count() > 0)
    {
        execLoadFilesByConditionOrYes(*cIniFile::SearchKeys);

        emit gotoInstallNavigation();//Настройка навигации

        s += ": find ";
        s += QString::number(cImportFiles::MaxIndexValue);
        s += " records";
    }
    else
    {
        s += ": empy search task, nothing to do!!!";
    }
    //---
    emit showExecStatus(s);
    //---
}

//=============================================================================

void cSearch::execActionSearchFreshRecords()
{
    QString s = "SearchInstance::execActionSearchFreshRecords()";

    bool x = searchFreshRecords();
    if(x)
    {
        s += ": successfull write result to file";
    }
    else
    {
        s += ": fault write result to file";
    }

    emit gotoInstallNavigation();//Настройка навигации

    //---
    emit showExecStatus(s);
    //---
}

//=============================================================================

void cSearch::execActionSearchNamePattern1()
{
    QString s = "SearchInstance::execActionSearchNamePattern1()";
    QString pattern = "^20[0-9]{6}_[0-9]{6}";

    bool x = searchNamePattern(pattern);

    showGroupsList();

    QString qsFileName = "./data/StringListPattern1.txt";//cIniFile::pattern1StringListFilePath;
    qDebug() << "Pattern1 file path=" << qsFileName;

    bool y = cLoadFiles::saveStringListToFile(
        qsFileName,
        *cIniFile::Groups
        );

    if(y)
        qDebug() << "Save to file: " << qsFileName << " success";
    else
        qDebug() << "Save to file: " << qsFileName << " error!!!";

    //---
    s += ": iCount=";
    s += QString::number(cIniFile::Groups->count());
    //---
    if(x)
    {
        s += "pattern1StringList stored in file";
    }
    //---
    emit showExecStatus(s);
    //---
}

//=============================================================================

void cSearch::execActionSearchNamePattern2()
{
    QString s = "SearchInstance::execActionSearchNamePattern2()";
    QString pattern = "^20[0-9]{2}-[0-9]{2}-[0-9]{2} [0-9]{2}-[0-9]{2}-[0-9]{2}";

    bool x = searchNamePattern(pattern);

    showGroupsList();

    QString qsFileName = "./data/StringListPattern2.txt";//cIniFile::pattern2StringListFilePath;
    qDebug() << "Pattern1 file path=" << qsFileName;

    bool y = cLoadFiles::saveStringListToFile(
        qsFileName,
        *cIniFile::Groups
        );

    if(y)
        qDebug() << "Save to file: " << qsFileName << " success";
    else
        qDebug() << "Can't open file: " << qsFileName << ", save to this file error!!!";

    //---
    s += ": iCount=";
    s += QString::number(cIniFile::Groups->count());
    //---
    if(x)
    {
        s += "pattern1StringList stored in file";
    }
    //---
    emit showExecStatus(s);
    //---
}

//=============================================================================

void cSearch::execActionSearchNamePattern()
{
    QString s = "SearchInstance::execActionSearchNamePattern()";
    //QString pattern = "^IMG_20[0-9]{6}_[0-9]{6}";
    //QString pattern = "IMG_20[0-9]{6}_[0-9]{6}";
    QString pattern = LineEditPattern->text();

    searchNamePattern(pattern);

    showGroupsList();

    //---
    s += ": iCount=";
    s += QString::number(cIniFile::Groups->count());
    //---
    bool x = cLoadFiles::saveStringListToFile(cIniFile::patternXStringListFilePath, *cIniFile::Groups);

    if(x)
    {
        s += "pattern1StringList stored in file";
    }
    //---
    emit showExecStatus(s);
    //---
}

//=============================================================================

/******************************************************************************
 * Поиск пересечения списков qslPattern1 и qslPattern2 по признаку совпадения
 * даты.
 * Исходные данных содержатся в файлах с именами:
 * cIniFile::pattern1StringListFilePath и
 * cIniFile::pattern2StringListFilePath
 *
 ******************************************************************************/
void cSearch::execActionSearchNamePatterns12Intersection()
{
    QString s = "SearchInstance::execActionSearchNamePattens12Intersection()";

    QListWidgetItem * item0 = new QListWidgetItem("==ActionSearchNamePatterns12Intersection==");
    item0->setForeground(Qt::blue);
    appEndItem(item0);

    QListWidgetItem * item1 = new QListWidgetItem("Pattern1Path=" + cIniFile::pattern1StringListFilePath);
    appEndItem(item1);

    QListWidgetItem * item2 = new QListWidgetItem("Pattern2Path=" + cIniFile::pattern2StringListFilePath);
    appEndItem(item2);

    QStringList qslPattern1 = cLoadFiles::loadStringListFromFile(cIniFile::pattern1StringListFilePath);
    QStringList qslPattern2 = cLoadFiles::loadStringListFromFile(cIniFile::pattern2StringListFilePath);

    QListWidgetItem * item3 = new QListWidgetItem("Pattern1_ItemsCount=" + QString::number(qslPattern1.count()));
    appEndItem(item3);

    QListWidgetItem * item4 = new QListWidgetItem("Pattern2_ItemsCount=" + QString::number(qslPattern2.count()));
    appEndItem(item4);


    // Очистка результата
    int iCount = 0;
    ListWidgetFounded->clear();

    int Year = 1900;
    int Month = 1;
    int Day = 1;
    int Hour = 0;
    int Min = 0;
    int Sec = 0;
    QString qsMirror = "1900-00-00 00-00-00";
    QString sX;

    QListIterator<QString> readIt(qslPattern1);
    while (readIt.hasNext())
    {
        QString qsSection = readIt.next();
        //qDebug() << qsSection;

        //Проверка строки на соответствие шаблону 1
        QString pattern1 = "^20[0-9]{6}_[0-9]{6}";
        QRegularExpression re(pattern1);
        bool match = re.match(qsSection.toLower()).hasMatch();
        if (match)
        {
            //Извлечение даты
            Year = qsSection.mid(0, 4).toInt();
            Month = qsSection.mid(4, 2).toInt();
            Day = qsSection.mid(6, 2).toInt();
            Hour = qsSection.mid(9, 2).toInt();
            Min = qsSection.mid(11, 2).toInt();
            Sec = qsSection.mid(13, 2).toInt();
            //qDebug() << "Строка " << qsSection << " is Ok for:" << pattern1 << ": Year=" << Year << " Month=" << Month << " Day=" << Day << " Hour=" << Hour << " Min=" << Min << " Sec=" << Sec;

            sX = "Pattern1_Item=";
            sX += qsSection;
            sX += " Data=";
            sX += QString::number(Year);
            sX += ".";
            sX += QString::number(Month);
            sX += ".";
            sX += QString::number(Day);
            sX += "_";
            sX += QString::number(Hour);
            sX += ":";
            sX += QString::number(Min);
            sX += ":";
            sX += QString::number(Sec);

//            QListWidgetItem * itemX = new QListWidgetItem(sX);
//            appEndItem(itemX);

            //Формирование строки данных по стандарту шаблона 2
            qsMirror = QString::number(Year);
            qsMirror += "-";
            if(Month < 10)qsMirror += "0";
            qsMirror += QString::number(Month);
            qsMirror += "-";
            if(Day < 10)qsMirror += "0";
            qsMirror += QString::number(Day);
            qsMirror += " ";
            if(Hour < 10)qsMirror += "0";
            qsMirror += QString::number(Hour);
            qsMirror += "-";
            if(Min < 10)qsMirror += "0";
            qsMirror += QString::number(Min);
            qsMirror += "-";
            if(Sec < 10)qsMirror += "0";
            qsMirror += QString::number(Sec);

            //qDebug() << "Строка " << qsSection << " is Ok for:" << pattern1 << ": Mirror=" << qsMirror;

            //Поиск сформированной строки в списке 2
            if(qslPattern2.contains(qsMirror))
            {
                //Подсчёт совпадений
                iCount++;
                //qDebug() << "String " << qsSection << " has mirror:" << qsMirror;

                sX += " has mirror: ";
                sX += qsMirror;

                //ListWidgetFounded->addItem(qsFoundedDelimiter);
                ListWidgetFounded->addItem(qsSection);
                //ListWidgetFounded->addItem(qsMirror);

                //Удаление найденной строки из конфигурационного файла
                //NavigationInstance->deleteSection(qsMirror);//!!!
            }
        }
        else
        {
            sX = "Pattern1 not match for " + qsSection + " Mirror=" + qsMirror;
        }

        QListWidgetItem * itemX = new QListWidgetItem(sX);
        appEndItem(itemX);
    }
    s += ": mirrors count=";
    s += QString::number(iCount);
    //---
    emit showExecStatus(s);
    //---
}

//=============================================================================

void cSearch::execActionSearchNamePatterns1XIntersection()
{
    QString s = "SearchInstance::execActionSearchNamePattens1XIntersection()";

    QString pattern1 = "^20[0-9]{6}_[0-9]{6}";

    QStringList qslPattern1 = cLoadFiles::loadStringListFromFile(cIniFile::pattern1StringListFilePath);
    QStringList qslPatternX = cLoadFiles::loadStringListFromFile(cIniFile::patternXStringListFilePath);

    qDebug() << "ListPattern1 count=" << qslPattern1.count() << " ListPatternX count=" << qslPatternX.count();

    int iCount = 0;// Очистка счётчика найденных объектов
    ListWidgetFounded->clear();
    QListIterator<QString> readIt(qslPattern1);
    while (readIt.hasNext())
    {
        QString qsSection = readIt.next();
        //qDebug() << qsSection;
        QRegularExpression re(pattern1);
        bool match = re.match(qsSection.toLower()).hasMatch();
        int Year, Month, Day, Hour, Min, Sec;
        QString qsMirror;
        if (match)
        {
            Year = qsSection.mid(0, 4).toInt();
            Month = qsSection.mid(4, 2).toInt();
            Day = qsSection.mid(6, 2).toInt();
            Hour = qsSection.mid(9, 2).toInt();
            Min = qsSection.mid(11, 2).toInt();
            Sec = qsSection.mid(13, 2).toInt();
            //qDebug() << "Строка " << qsSection << " is Ok for:" << pattern1 << ": Year=" << Year << " Month=" << Month << " Day=" << Day << " Hour=" << Hour << " Min=" << Min << " Sec=" << Sec;
            //Формирование шаблона по типу 3
            qsMirror = "IMG";
            qsMirror += QString::number(Year);
            if(Month < 10)qsMirror += "0";
            qsMirror += QString::number(Month);
            if(Day < 10)qsMirror += "0";
            qsMirror += QString::number(Day);
            qsMirror += "_";
            if(Hour < 10)qsMirror += "0";
            qsMirror += QString::number(Hour);
            if(Min < 10)qsMirror += "0";
            qsMirror += QString::number(Min);
            if(Sec < 10)qsMirror += "0";
            qsMirror += QString::number(Sec);
            qDebug() << "Строка " << qsSection << " is Ok for:" << pattern1 << ": Mirror=" << qsMirror;
            if(qslPatternX.contains(qsMirror))
            {
                iCount++;
                qDebug() << "String " << qsSection << " has mirror:" << qsMirror;
                ListWidgetFounded->addItem(qsSection);

                //NavigationInstance->deleteSection(qsMirror);//!!!
            }
        }
    }
    s += ": mirrors count=";
    s += QString::number(iCount);
    //---
    emit showExecStatus(s);
    //---
}

//=============================================================================

void cSearch::execLineEditSearchAllKeysTextChanched(QString s)
{
    QString result = "SearchInstance: LineEditSearchAllKeysTextChanged=";
    result += s;
    if(ListWidgetKeys->count() > 0)
    {
        int iKeys = 0;
        QListWidgetItem * qlswLine;
        QString qsLine;

        for(int i = 0; i < ListWidgetKeys->count(); i++)
        {
            qlswLine = ListWidgetKeys->item(i);
            qsLine = qlswLine->text();

            if(qsLine.contains(s))
            {
                iKeys++;
                ListWidgetKeys->setCurrentRow(i);
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

//=============================================================================

