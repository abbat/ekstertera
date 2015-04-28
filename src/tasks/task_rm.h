/*!
 * \file
 * \brief Асинхронная задача EteraAPI::rm
 */

#ifndef _ekstertera_tasks_task_rm_h_
#define _ekstertera_tasks_task_rm_h_

#include "task_async.h"

/*!
 * \brief Асинхронная задача EteraAPI::rm
 */
class EteraTaskRM : public EteraTaskAsync
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param path Путь для удаления директории
         * \param permanently Флаг полного удаления (false - удаление в корзину)
         */
        EteraTaskRM(const QString& path, bool permanently);
        ~EteraTaskRM();

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

#endif   // _ekstertera_tasks_task_rm_h_
