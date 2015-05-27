/*!
 * \file
 * \brief Асинхронная задача EteraAPI::get
 */

#ifndef _ekstertera_tasks_task_get_h_
#define _ekstertera_tasks_task_get_h_

#include "task_progress.h"

/*!
 * \brief Асинхронная задача EteraAPI::get
 */
class EteraTaskGET : public EteraTaskProgress
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param source Имя файла на диске
         * \param target Имя локального файла (при пустом имени, данные сохраняются в кэш)
         */
        EteraTaskGET(const QString& source, const QString& target);
        ~EteraTaskGET();

        /*!
         * \brief Overloaded QRunnable::run
         */
        void run();

    signals:

        /*!
         * \brief Успешное завершение операции
         * \param id Идентификатор задачи
         * \param args Дополнительные аргументы (аргументы конструктора)
         */
        void onSuccess(quint64 id, const QVariantMap& args);
};

#endif   // _ekstertera_tasks_task_get_h_
