#include "pool.h"
//----------------------------------------------------------------------------------------------
#include "api.h"
//----------------------------------------------------------------------------------------------
static EteraThreadPool* g_thread_pool = NULL;
//----------------------------------------------------------------------------------------------

void EteraThreadPool::init()
{
    g_thread_pool = new EteraThreadPool();
}
//----------------------------------------------------------------------------------------------

void EteraThreadPool::cleanup()
{
    delete g_thread_pool;
}
//----------------------------------------------------------------------------------------------

EteraThreadPool* EteraThreadPool::instance()
{
    return g_thread_pool;
}
//----------------------------------------------------------------------------------------------

EteraThreadPool::EteraThreadPool()
{
    EteraAPI::init();

    for (int i = 0; i < QThread::idealThreadCount(); i++) {
        EteraThread* thread = new EteraThread(&m_queue, &m_wait);
        m_threads.append(thread);
        thread->start();
    }
}
//----------------------------------------------------------------------------------------------

EteraThreadPool::~EteraThreadPool()
{
    // очистка api
    EteraAPI::cleanup();

    // выставление флага остановки для всех потоков
    for (int i = 0; i < m_threads.count(); i++)
        m_threads[i]->stop();

    // очистка очереди
    m_queue.clear();

    // даем возможность завершиться потокам самостоятельно
    while (m_threads.isEmpty() == false) {
        m_wait.wakeAll();

        int i = 0;
        while (i < m_threads.count()) {
            EteraThread* thread = m_threads[i];
            if (thread->wait(50) == true) {
                delete thread;
                m_threads.removeAt(i);
            } else
                i++;
        }
    }
}
//----------------------------------------------------------------------------------------------

void EteraThreadPool::start(QRunnable* task, EteraTaskPriority priority)
{
    m_queue.enqueue(task, priority);
    m_wait.wakeOne();
}
//----------------------------------------------------------------------------------------------
