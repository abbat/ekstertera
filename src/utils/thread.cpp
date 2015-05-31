#include "thread.h"
//----------------------------------------------------------------------------------------------

EteraThread::EteraThread(EteraTaskQueue* queue, QMutex* mutex, QWaitCondition* wait)
{
    m_stopped     = false;
    m_queue       = queue;
    m_queue_mutex = mutex;
    m_queue_wait  = wait;
}
//----------------------------------------------------------------------------------------------

EteraThread::~EteraThread()
{
}
//----------------------------------------------------------------------------------------------

QRunnable* EteraThread::next()
{
    m_queue_mutex->lock();

    if (m_queue->isEmpty() == true) {
        m_queue_mutex->unlock();
        return NULL;
    }

    QRunnable* result = m_queue->dequeue();

    m_queue_mutex->unlock();

    return result;
}
//----------------------------------------------------------------------------------------------

void EteraThread::run()
{
    while (m_stopped == false) {
        QRunnable* task = next();

        if (task == NULL) {
            m_mutex.lock();
            m_queue_wait->wait(&m_mutex);
            m_mutex.unlock();

            task = next();
        }

        if (task != NULL) {
            task->run();
            delete task;
        }
    }
}
//----------------------------------------------------------------------------------------------
