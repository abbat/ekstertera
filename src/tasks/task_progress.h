/*!
 * \file
 * \brief Асинхронная задача API, сигнализирующая о прогрессе операции
 */

#ifndef _ekstertera_tasks_task_progress_h_
#define _ekstertera_tasks_task_progress_h_

#include "task_sync.h"

/*!
 * \brief Базовый класс для асинхронных вызовов API в QThreadPool, выполняющихся последовательно и сигнализирующий о прогрессе операции
 */
class EteraTaskProgress : public EteraTaskSync
{
    Q_OBJECT

    public:

        EteraTaskProgress();
        virtual ~EteraTaskProgress();

        /*!
         * \brief Инициализация API
         */
        virtual void init();

        /*!
         * \brief Освобождение ресурсов API
         */
        virtual void cleanup();

    private slots:

        /*!
         * \brief Сигнал прогресса операции
         * \param done Выполнено
         * \param total Всего
         */
        void api_on_progress(qint64 done, qint64 total);

    signals:

        /*!
         * \brief Сигнал прогресса операции
         * \param id Идентификатор задачи
         * \param done Выполнено
         * \param total Всего
         */
        void onProgress(quint64 id, qint64 done, qint64 total);

};

#endif   // _ekstertera_tasks_task_progress_h_
