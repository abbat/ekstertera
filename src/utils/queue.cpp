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

EteraTask* EteraTaskQueue::dequeue(EteraTaskPriority priority)
{
    m_mutex.lock();

    EteraTask* task = NULL;

    if (m_foreground.isEmpty() == false)
        task = m_foreground.dequeue();
    else if ((priority == etpIdle || priority == etpBackground) && m_background.isEmpty() == false)
        task = m_background.dequeue();
    else if (priority == etpIdle && m_idle.isEmpty() == false)
        task = m_idle.dequeue();

    m_mutex.unlock();

    return task;
}
//----------------------------------------------------------------------------------------------

void EteraTaskQueue::enqueue(EteraTask* task, EteraTaskPriority priority)
{
    m_mutex.lock();

    switch (priority) {
        case etpForeground:
            m_foreground.enqueue(task);
            break;
        case etpBackground:
            m_background.enqueue(task);
            break;
        case etpIdle:
            m_idle.enqueue(task);
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

    clearQueue(m_foreground);
    clearQueue(m_background);
    clearQueue(m_idle);

    m_mutex.unlock();
}
//----------------------------------------------------------------------------------------------
