/*!
 * \file
 * \brief Асинхронная задача EteraAPI::publish
 */

#ifndef _ekstertera_tasks_task_publish_h_
#define _ekstertera_tasks_task_publish_h_

#include "task_sync.h"

/*!
 * \brief Асинхронная задача EteraAPI::publish
 */
class EteraTaskPUBLISH : public EteraTaskSync
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param path Путь для открытия доступа
         */
        EteraTaskPUBLISH(const QString& path);
        ~EteraTaskPUBLISH();

        /*!
         * \brief Overloaded QRunnable::run
         */
        void run();

    signals:

        /*!
         * \brief Успешное завершение операции
         * \param id Идентификатор задачи
         * \param item Описатель объекта к которому открыт доступ
         * \param args Дополнительные аргументы (аргументы конструктора)
         */
        void onSuccess(quint64 id, const EteraItem& item, const QVariantMap& args);
};

#endif   // _ekstertera_tasks_task_publish_h_
