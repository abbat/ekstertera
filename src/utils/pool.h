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
class EteraThreadPool : public QObject
{
    Q_OBJECT

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
        void start(EteraTask* task, EteraTaskPriority priority = etpNormal);

    private:

        EteraThreadPool();
        ~EteraThreadPool();

        void gcThreads();     /*!< \brief Удаление завершившихся потоков */
        void spawnThread();   /*!< \brief Запуск нового потока           */

        EteraTaskQueue  m_queue;     /*!< \brief Очередь задач                        */
        QWaitCondition  m_wait;      /*!< \brief Блокировка на появление новой задачи */
        EteraThreadList m_threads;   /*!< \brief Список потоков                       */

    private slots:

        /*!
         * \brief Сигнал завершения рабочего потока
         * Поток не дождался новых задач в течении периода ожидания
         */
        void on_thread_finished();
};

#endif   // _ekstertera_pool_h_
