/*!
 * \file
 * \brief Асинхронная задача EteraAPI::mv
 */

#ifndef _ekstertera_tasks_task_mv_h_
#define _ekstertera_tasks_task_mv_h_

#include "task_async.h"

/*!
 * \brief Асинхронная задача EteraAPI::mv
 */
class EteraTaskMV : public EteraTaskAsync
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param source Исходный путь
         * \param target Путь назначения
         * \param overwrite Флаг перезаписи (false - перезапись запрещена)
         */
        EteraTaskMV(const QString& source, const QString& target, bool overwrite);
        ~EteraTaskMV();

        /*!
         * \brief Overloaded QRunnable::run
         */
        void run();

    signals:

        /*!
         * \brief Успешное завершение операции
         * \param id Идентификатор задачи
         * \param item Описатель перемещенного объекта
         * \param args Дополнительные аргументы (аргументы конструктора)
         */
        void onSuccess(quint64 id, const EteraItem& item, const QVariantMap& args);
};

#endif   // _ekstertera_tasks_task_mv_h_
