#include "thread.h"
//----------------------------------------------------------------------------------------------

EteraThread::EteraThread(EteraTaskQueue* queue, QWaitCondition* wait, EteraTaskPriority destiny)
{
    m_stopped = false;
    m_queue   = queue;
    m_wait    = wait;
    m_destiny = destiny;
}
//----------------------------------------------------------------------------------------------

EteraThread::~EteraThread()
{
}
//----------------------------------------------------------------------------------------------

void EteraThread::run()
{
    while (m_stopped == false) {
        EteraTask* task = m_queue->dequeue(m_destiny);

        if (task == NULL) {
            m_mutex.lock();
            bool timedout = m_wait->wait(&m_mutex, 30000);
            m_mutex.unlock();

            if (timedout == false)
                break;
        } else {
            task->run();
            delete task;
        }
    }
}
//----------------------------------------------------------------------------------------------
