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

EteraThreadPool::EteraThreadPool() : QObject()
{
    EteraAPI::init();
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

void EteraThreadPool::spawnThread()
{
    EteraThread* thread = new EteraThread(&m_queue, &m_wait);

    connect(thread, SIGNAL(finished()), this, SLOT(on_thread_finished()));

    m_threads.append(thread);

    thread->start();
}
//----------------------------------------------------------------------------------------------

void EteraThreadPool::start(EteraTask* task, EteraTaskPriority priority)
{
    m_queue.enqueue(task, priority);

    if (m_threads.count() < QThread::idealThreadCount() || priority == etpHigh)
        spawnThread();

    m_wait.wakeOne();
}
//----------------------------------------------------------------------------------------------

void EteraThreadPool::gcThreads()
{
    int i = 0;
    while (i < m_threads.count()) {
        EteraThread* thread = m_threads[i];
        if (thread->isFinished() == true) {
            delete thread;
            m_threads.removeAt(i);
        } else
            i++;
    }
}
//----------------------------------------------------------------------------------------------

void EteraThreadPool::on_thread_finished()
{
    gcThreads();
}
//----------------------------------------------------------------------------------------------
