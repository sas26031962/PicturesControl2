#ifndef PROCESSSEARCHPATTERN2TASK_H
#define PROCESSSEARCHPATTERN2TASK_H

#include <QRunnable>
#include <QThread>
#include <QObject>
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>

#include "cinifile.h"

#define PROCESS_SEARCH_PATTERN2_TASK_ID 14

class ProcessSearchPattern2Task : public QRunnable
{
public:
    ProcessSearchPattern2Task(int taskId, QObject* receiver);
    void run() override;

private:
    int m_taskId;
    QObject* m_receiver;
};

#endif // PROCESSSEARCHPATTERN2TASK_H
