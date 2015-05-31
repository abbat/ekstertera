#include "thread.h"
//----------------------------------------------------------------------------------------------

EteraThread::EteraThread(EteraTaskQueue* queue, QWaitCondition* wait)
{
    m_stopped = false;
    m_queue   = queue;
    m_wait    = wait;
}
//----------------------------------------------------------------------------------------------

EteraThread::~EteraThread()
{
}
//----------------------------------------------------------------------------------------------

void EteraThread::run()
{
    while (m_stopped == false) {
        QRunnable* task = m_queue->dequeue();

        if (task == NULL) {
            m_mutex.lock();
            m_wait->wait(&m_mutex);
            m_mutex.unlock();

            task = m_queue->dequeue();
        }

        if (task != NULL) {
            task->run();
            delete task;
        }
    }
}
//----------------------------------------------------------------------------------------------
