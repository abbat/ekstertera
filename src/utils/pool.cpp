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

    m_max_idle_threads = QThread::idealThreadCount();
    if (m_max_idle_threads < 4)
        m_max_idle_threads = 4;
    else if (m_max_idle_threads > 8)
        m_max_idle_threads = 8;

    m_max_background_threads = m_max_idle_threads / 2;
    m_max_foreground_threads = m_max_idle_threads / 4;

    m_foreground_threads = 0;
    m_background_threads = 0;
    m_idle_threads       = 0;
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
        m_idle_wait.wakeAll();
        m_background_wait.wakeAll();
        m_foreground_wait.wakeAll();

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

void EteraThreadPool::spawnThread(EteraTaskPriority destiny)
{
    QWaitCondition* wait = NULL;

    switch (destiny) {
        case etpForeground:
            wait = &m_foreground_wait;
            m_foreground_threads++;
            break;
        case etpBackground:
            wait = &m_background_wait;
            m_background_threads++;
            break;
        case etpIdle:
            wait = &m_idle_wait;
            m_idle_threads++;
            break;
    }

    EteraThread* thread = new EteraThread(&m_queue, wait, destiny);

    connect(thread, SIGNAL(finished()), this, SLOT(on_thread_finished()));

    m_threads.append(thread);

    thread->start();
}
//----------------------------------------------------------------------------------------------

void EteraThreadPool::start(EteraTask* task, EteraTaskPriority priority)
{
    m_queue.enqueue(task, priority);

    if (m_idle_threads < m_max_idle_threads)
        spawnThread(etpIdle);
    else if (priority == etpForeground) {
        if (m_foreground_threads < m_max_foreground_threads)
            spawnThread(etpForeground);
        else
            m_foreground_wait.wakeOne();
    } else if (priority == etpBackground) {
        if (m_background_threads < m_max_background_threads)
            spawnThread(etpBackground);
        else
            m_background_wait.wakeOne();
    } else
        m_idle_wait.wakeOne();
}
//----------------------------------------------------------------------------------------------

void EteraThreadPool::gcThreads()
{
    int i = 0;
    while (i < m_threads.count()) {
        EteraThread* thread = m_threads[i];
        if (thread->isFinished() == true) {
            switch (thread->destiny()) {
                case etpForeground: m_foreground_threads--; break;
                case etpBackground: m_background_threads--; break;
                case etpIdle:       m_idle_threads--;       break;
            }

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
