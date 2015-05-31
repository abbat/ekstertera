/*!
 * \file
 * \brief Очередь задач для пула асинхронных задач
 */

#ifndef _ekstertera_queue_h_
#define _ekstertera_queue_h_

#include "sysheaders.h"

/*!
 * \brief Приоритет задачи
 */
typedef enum {
    etpLow,      /*!< \brief Низкий (загрузка превью)          */
    etpNormal,   /*!< \brief Нормальный (загрузка файлов)      */
    etpHigh      /*!< \brief Высокий (получение списка файлов) */
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
         * \return Задача или NULL если очередь пуста
         * Задачи отдаются согласно приоритету
         */
        QRunnable* dequeue();

        /*!
         * \brief Постановка задачи в очередь
         * \param task Задача
         * \param priority Приоритет
         */
        void enqueue(QRunnable* task, EteraTaskPriority priority);

        /*!
         * \brief Очистка очереди
         */
        void clear();

    private:

        /*!
         * \brief Тип для очереди задач
         */
        typedef QQueue<QRunnable*> EteraRunnableQueue;

        /*!
         * \brief Очистка очереди
         */
        void clearQueue(EteraRunnableQueue& queue);

        QMutex             m_mutex;    /*!< \brief Блокировка для доступа                 */
        EteraRunnableQueue m_low;      /*!< \brief Очередь задач с низким приоритетом     */
        EteraRunnableQueue m_normal;   /*!< \brief Очередь задач с нормальным приоритетом */
        EteraRunnableQueue m_high;     /*!< \brief Очередь задач с высоким приоритетом    */
};

#endif   // _ekstertera_queue_h_
