/*!
 * \file
 * \brief Очередь задач для пула асинхронных задач
 */

#ifndef _ekstertera_queue_h_
#define _ekstertera_queue_h_

#include "tasks/task.h"

/*!
 * \brief Приоритет задачи
 */
typedef enum {
    etpForeground,   /*!< \brief Задачи отзывчивости UI (загрузка списка файлов и директорий) */
    etpBackground,   /*!< \brief Фоновые задачи UI (загрузка превью)                          */
    etpIdle          /*!< \brief Задачи некритичные для UI (upload / download)                */
} EteraTaskPriority;

/*!
 * \brief Очередь задач для пула асинхронных задач
 */
class EteraTaskQueue
{
    public:

        EteraTaskQueue();
        ~EteraTaskQueue();

        /*!
         * \brief Получение задачи из очереди
         * \param priority Минимально требуемый приоритет задачи
         * \return Задача или NULL если очередь с требуемым приоритетом пуста
         */
        EteraTask* dequeue(EteraTaskPriority priority);

        /*!
         * \brief Постановка задачи в очередь
         * \param task Задача
         * \param priority Приоритет
         */
        void enqueue(EteraTask* task, EteraTaskPriority priority);

        /*!
         * \brief Очистка очереди
         */
        void clear();

        /*!
         * \brief Очистка очереди по приоритету
         * \param priority Приоритет
         */
        void purge(EteraTaskPriority priority);

    private:

        /*!
         * \brief Тип для очереди задач
         */
        typedef QQueue<EteraTask*> EteraRunnableQueue;

        /*!
         * \brief Очистка очереди
         */
        void clearQueue(EteraRunnableQueue& queue);

        QMutex             m_mutex;        /*!< \brief Блокировка для доступа           */
        EteraRunnableQueue m_foreground;   /*!< \brief Очередь задач критичных для UI   */
        EteraRunnableQueue m_background;   /*!< \brief Очередь фоновых задач UI         */
        EteraRunnableQueue m_idle;         /*!< \brief Очередь задач некритичных для UI */
};

#endif   // _ekstertera_queue_h_
