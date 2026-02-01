#ifndef PROCESSINGTASK_H
#define PROCESSINGTASK_H

#include <QRunnable>
#include <QThread>
#include <QObject>
#include <QDebug>

class ProcessingTask : public QRunnable
{
public:
    ProcessingTask(int taskId, QObject* receiver);
    void run() override;

private:
    int m_taskId;
    QObject* m_receiver;
};

#endif // PROCESSINGTASK_H
