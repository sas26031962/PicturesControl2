#ifndef CPROCESSSEARCHPATTERN1TASK_H
#define CPROCESSSEARCHPATTERN1TASK_H

#include <QRunnable>
#include <QThread>
#include <QObject>
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>

#include "cinifile.h"

#define PROCESS_SEARCH_PATTERN1_TASK_ID 13

class ProcessSearchPattern1Task : public QRunnable
{
public:
    ProcessSearchPattern1Task(int taskId, QObject* receiver);
    void run() override;

private:
    int m_taskId;
    QObject* m_receiver;
};

#endif // CPROCESSSEARCHPATTERN1TASK_H
