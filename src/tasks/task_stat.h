/*!
 * \file
 * \brief Асинхронная задача EteraAPI::stat
 */

#ifndef _ekstertera_tasks_task_stat_h_
#define _ekstertera_tasks_task_stat_h_

#include "task_sync.h"

/*!
 * \brief Асинхронная задача EteraAPI::stat
 */
class EteraTaskSTAT : public EteraTaskSync
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param path Путь до объекта
         */
        EteraTaskSTAT(const QString& path);
        ~EteraTaskSTAT();

        /*!
         * \brief Overloaded QRunnable::run
         */
        void run();

    signals:

        /*!
         * \brief Успешное завершение операции
         * \param id Идентификатор задачи
         * \param item Описатель объекта
         * \param args Дополнительные аргументы (аргументы конструктора)
         */
        void onSuccess(quint64 id, const EteraItem& item, const QVariantMap& args);
};

#endif   // _ekstertera_tasks_task_stat_h_
