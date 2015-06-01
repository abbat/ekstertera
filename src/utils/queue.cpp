#include "queue.h"
//----------------------------------------------------------------------------------------------

EteraTaskQueue::EteraTaskQueue()
{
}
//----------------------------------------------------------------------------------------------

EteraTaskQueue::~EteraTaskQueue()
{
    clear();
}
//----------------------------------------------------------------------------------------------

EteraTask* EteraTaskQueue::dequeue()
{
    m_mutex.lock();

    EteraTask* task = NULL;

    if (m_high.isEmpty() == false)
        task = m_high.dequeue();
    else if (m_normal.isEmpty() == false)
        task = m_normal.dequeue();
    else if (m_low.isEmpty() == false)
        task = m_low.dequeue();

    m_mutex.unlock();

    return task;
}
//----------------------------------------------------------------------------------------------

void EteraTaskQueue::enqueue(EteraTask* task, EteraTaskPriority priority = etpNormal)
{
    m_mutex.lock();

    switch (priority) {
        case etpLow:
            m_low.enqueue(task);
            break;
        case etpNormal:
            m_normal.enqueue(task);
            break;
        case etpHigh:
            m_high.enqueue(task);
            break;
        default:
            delete task;
    }

    m_mutex.unlock();
}
//----------------------------------------------------------------------------------------------

void EteraTaskQueue::clearQueue(EteraRunnableQueue& queue)
{
    for (int i = 0; i < queue.count(); i++) {
        EteraTask* task = queue[i];
        delete task;
    }

    queue.clear();
}
//----------------------------------------------------------------------------------------------

void EteraTaskQueue::clear()
{
    m_mutex.lock();

    clearQueue(m_low);
    clearQueue(m_normal);
    clearQueue(m_high);

    m_mutex.unlock();
}
//----------------------------------------------------------------------------------------------
