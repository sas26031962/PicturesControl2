#ifndef PROCESSROTATECCW90TASK_H
#define PROCESSROTATECCW90TASK_H

#include <QRunnable>
#include <QThread>
#include <QObject>
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>

#include "cinifile.h"

#define PROCESS_ROTATE_CCW90_TASK_ID 17

class ProcessRotateCCW90Task : public QRunnable
{
public:
    ProcessRotateCCW90Task(int taskId, QObject* receiver);
    void run() override;

private:
    int m_taskId;
    QObject* m_receiver;
};

#endif // PROCESSROTATECCW90TASK_H
