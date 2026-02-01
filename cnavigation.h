#ifndef CNAVIGATION_H
#define CNAVIGATION_H

#include <QObject>
#include <QListWidget>
#include <QSettings>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>
#include <QSpinBox>
#include <QProgressBar>
#include <QGroupBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "cinifile.h"
#include "cimportfiles.h"

//Значение, которое берётся за максимальное, если группа отображения пуста
#define DEFAULT_MAXIMUM_VAULE 100

class cNavigation : public QObject
{
    Q_OBJECT

    //Атрибуты
    QLabel * lbRow0;
    QLabel * lbRow1;
    QLabel * lbRow2;

    QGroupBox * Parent;

    //Методы
    void appEndItem(QListWidgetItem * item);
    void execNavigationCurrentIndex();

public:
    //Атрибуты
    QPushButton * pbBegin;
    QPushButton * pbPrevious;
    QPushButton * pbNext;
    QPushButton * pbEnd;
    QPushButton * pbGoTo;

    QPushButton * pbReload;
    QPushButton * pbErase;
    QPushButton * pbRemove;

    QListWidget * ListWidgetOther;
    QTableView * TableView;
    QSpinBox * SpinBoxIndex;
    QProgressBar * ProgressBarNavigation;

    //Конструкторы
    explicit cNavigation(
        QListWidget* list_widget,
        QTableView* table_view,
        QGroupBox* box,
        QObject *parent = 0
        );

    //Методы
    bool deleteSection(QString s);
    bool eraseSection(QString s);

signals:
    void draw(QString s);
    void showExecStatus(QString s);

public slots:
    void installNavigation();

    void loadRemovedSectionsList();
    void execActionGotoIndex();
    void execActionSelectImageBegin();
    void execActionSelectImageNext();
    void execActionSelectImagePrevious();
    void execActionSelectImageEnd();

    void execActionRemoveSection();
    void execActionEraseSection();

    void execShowCurrentIndexPicture();


};

#endif // CNAVIGATION_H
