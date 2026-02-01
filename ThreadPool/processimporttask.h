#ifndef PROCESSIMPORTTASK_H
#define PROCESSIMPORTTASK_H

#include <QRunnable>
#include <QThread>
#include <QObject>
#include <QDebug>
#include <QSettings>
#include <atomic>

#include "cinifile.h"
#include "crecord.h"
#include "cimportfiles.h"

#define PROCESS_IMPORT_TASK_ID 11

class processImportTask : public QRunnable
{

public:
    processImportTask(int taskId, QObject* receiver);
    void run() override;

private:
    int m_taskId;
    QObject* m_receiver;
};

#endif // PROCESSIMPORTTASK_H
