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
         * \param priority Приоритет
         */
        void start(QRunnable* task, EteraTaskPriority priority = etpNormal);

    private:

        EteraThreadPool();
        ~EteraThreadPool();

        QList<EteraThread*> m_threads;   /*!< \brief Список потоков */

        EteraTaskQueue m_queue;   /*!< \brief Очередь задач                        */
        QWaitCondition m_wait;    /*!< \brief Блокировка на появление новой задачи */
};

#endif   // _ekstertera_pool_h_
