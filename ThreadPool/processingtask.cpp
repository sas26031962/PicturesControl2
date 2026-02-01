#include "processingtask.h"

ProcessingTask::ProcessingTask(int taskId, QObject* receiver)
    : m_taskId(taskId), m_receiver(receiver)
{
    setAutoDelete(true); // Автоматическое удаление после выполнения
}

void ProcessingTask::run()
{
    QString qsStartMessage = "Task:";
    qsStartMessage += QString::number(m_taskId);
    qsStartMessage += " run in thread";
    //qsStartMessage += QThread::currentThreadId();

    qDebug() << "Задача" << m_taskId << "запущена в потоке" << QThread::currentThreadId();

    QMetaObject::invokeMethod(m_receiver, "execBeginMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsStartMessage));

    // Имитация длительной обработки
    for (int i = 0; i < 100; ++i)
    {
        QThread::msleep(30); // Задержка 30 мс

        // Отправка прогресса (каждые 5%)
        if (i % 5 == 0)
        {
            QMetaObject::invokeMethod(m_receiver, "updateProgress",
                                      Qt::QueuedConnection,
                                      Q_ARG(int, 0));
        }
    }

    // Уведомление о завершении задачи
    QMetaObject::invokeMethod(
                m_receiver, "updateProgress",
                Qt::QueuedConnection,
                Q_ARG(int, 1)
                );

    QString qsFinishMessage = "Task:";
    qsFinishMessage += QString::number(m_taskId);
    qsFinishMessage += " completed";
    QMetaObject::invokeMethod(m_receiver, "execEndMessage",
                              Qt::QueuedConnection,
                              Q_ARG(QString, qsFinishMessage));
}

