/*!
 * \file
 * \brief Поток для пула асинхронных задач
 */

#ifndef _ekstertera_thread_h_
#define _ekstertera_thread_h_

#include "sysheaders.h"

/*!
 * \brief Очередь задач
 */
typedef QQueue<QRunnable*> EteraTaskQueue;

/*!
 * \brief Поток для пула асинхронных задач
 */
class EteraThread : public QThread
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param queue Общая очередь задач
         * \param mutex Блокировка для доступа к очереди
         * \param wait Блокировка на появление новой задачи
         */
        EteraThread(EteraTaskQueue* queue, QMutex* mutex, QWaitCondition* wait);
        ~EteraThread();

        /*!
         * \brief Остановка разбора очереди задач
         */
        void stop() { m_stopped = true; }

    protected:

        // QThread::run
        void run();

    private:

        /*!
         * \brief Получение следующей задачи
         * \return Следующая задача или NULL для пустой очереди
         */
        QRunnable* next();

        bool            m_stopped;       /*!< \brief Флаг необходимости завершения работы */
        QMutex          m_mutex;         /*!< \brief Блокировка для m_queue_wait          */
        EteraTaskQueue* m_queue;         /*!< \brief Очередь задач                        */
        QMutex*         m_queue_mutex;   /*!< \brief Блокировка очереди задач             */
        QWaitCondition* m_queue_wait;    /*!< \brief Блокировка на появление новой задачи */
};

#endif   // _ekstertera_thread_h_
