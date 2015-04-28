/*!
 * \file
 * \brief Асинхронная задача EteraAPI::info
 */

#ifndef _ekstertera_tasks_task_info_h_
#define _ekstertera_tasks_task_info_h_

#include "task_sync.h"

/*!
 * \brief Асинхронная задача EteraAPI::info
 */
class EteraTaskINFO : public EteraTaskSync
{
    Q_OBJECT

    public:

        EteraTaskINFO();
        ~EteraTaskINFO();

        /*!
         * \brief Overloaded QRunnable::run
         */
        void run();

    signals:

        /*!
         * \brief Успешное завершение операции
         * \param id Идентификатор задачи
         * \param info Информация о диске
         * \param args Дополнительные аргументы (аргументы конструктора)
         */
        void onSuccess(quint64 id, const EteraInfo& info, const QVariantMap& args);
};

#endif   // _ekstertera_tasks_task_info_h_
