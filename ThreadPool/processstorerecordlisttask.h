#ifndef CPROCESSSTORERECORDLISTTASK_H
#define CPROCESSSTORERECORDLISTTASK_H

#include <QRunnable>
#include <QThread>
#include <QObject>
#include <QDebug>
#include <memory>

#include "crecord.h"
#include "cinifile.h"

#define PROCESS_STORE_RECORD_LIST_TASK_ID 12

class ProcessStoreRecordListTask : public QRunnable
{
public:
    ProcessStoreRecordListTask(int taskId, QObject* receiver);
    void run() override;

private:
    int m_taskId;
    QObject* m_receiver;
};

#endif // CPROCESSSTORERECORDLISTTASK_H
