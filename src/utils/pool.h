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
        void start(EteraTask* task, EteraTaskPriority priority = etpIdle);

    private:

        EteraThreadPool();
        ~EteraThreadPool();

        /*!
         * \brief Удаление завершившихся потоков
         */
        void gcThreads();

        /*!
         * \brief Запуск нового потока
         * \param destiny Предпочитаемый приоритет задач
         */
        void spawnThread(EteraTaskPriority destiny = etpIdle);

        int m_max_threads;          /*!< \brief Максимальное число потоков    */
        int m_foreground_threads;   /*!< \brief Количество foreground потоков */
        int m_background_threads;   /*!< \brief Количество background потоков */
        int m_idle_threads;         /*!< \brief Количество idle потоков       */

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
