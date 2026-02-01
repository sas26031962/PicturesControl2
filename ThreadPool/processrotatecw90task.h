#ifndef PROCESSROTATECW90TASK_H
#define PROCESSROTATECW90TASK_H

#include <QRunnable>
#include <QThread>
#include <QObject>
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>

#include "cinifile.h"

#define PROCESS_ROTATE_CW90_TASK_ID 16

class ProcessRotateCW90Task : public QRunnable
{
public:
    ProcessRotateCW90Task(int taskId, QObject* receiver);
    void run() override;

private:
    int m_taskId;
    QObject* m_receiver;
};

#endif // PROCESSROTATECW90TASK_H
