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
        EteraThread* thread = new EteraThread(&m_queue, &m_queue_mutex, &m_queue_wait);
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
    m_queue_mutex.lock();

    for (int i = 0; i < m_queue.count(); i++) {
        QRunnable* task = m_queue[i];
        delete task;
    }

    m_queue.clear();

    m_queue_mutex.unlock();

    // даем возможность завершиться потокам самостоятельно
    while (m_threads.isEmpty() == false) {
        m_queue_wait.wakeAll();

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

void EteraThreadPool::start(QRunnable* task)
{
    m_queue_mutex.lock();
    m_queue.enqueue(task);
    m_queue_mutex.unlock();

    m_queue_wait.wakeOne();
}
//----------------------------------------------------------------------------------------------
