/*!
 * \file
 * \brief Пул потоков для асинхронных задач
 */

#ifndef _ekstertera_pool_h_
#define _ekstertera_pool_h_

#include "thread.h"

/*!
 * \brief Пул потоков для асинхронных задач
 */
class EteraThreadPool
{
    public:

        /*!
         * \brief Инициализация пула
         */
        static void init();

        /*!
         * \brief Освобождение ресурсов пула
         */
        static void cleanup();

        /*!
         * \brief Singleton
         * \return Пул потоков
         */
        static EteraThreadPool* instance();

        /*!
         * \brief Добавление задачи в очередь
         * \param task Задача
         */
        void start(QRunnable* task);

    private:

        EteraThreadPool();
        ~EteraThreadPool();

        QList<EteraThread*> m_threads;   /*!< \brief Список потоков */

        EteraTaskQueue  m_queue;         /*!< \brief Очередь задач                        */
        QMutex          m_queue_mutex;   /*!< \brief Блокировка очереди задач             */
        QWaitCondition  m_queue_wait;    /*!< \brief Блокировка на появление новой задачи */
};

#endif   // _ekstertera_pool_h_
