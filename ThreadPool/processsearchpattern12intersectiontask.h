#ifndef PROCESSSEARCHPATTERN12INTERSECTIONTASK_H
#define PROCESSSEARCHPATTERN12INTERSECTIONTASK_H

#include <QRunnable>
#include <QThread>
#include <QObject>
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>

#include "cinifile.h"
#include "cloadfiles.h"

#define PROCESS_SEARCH_PATTERN12_INTERSECTION_TASK_ID 15

class ProcessSearchPattern12IntersectionTask : public QRunnable
{
public:
    ProcessSearchPattern12IntersectionTask(int taskId, QObject* receiver);
    void run() override;

private:
    int m_taskId;
    QObject* m_receiver;
};

#endif // PROCESSSEARCHPATTERN12INTERSECTIONTASK_H
