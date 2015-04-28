/*!
 * \file
 * \brief Асинхронная задача EteraAPI::unpublish
 */

#ifndef _ekstertera_tasks_task_unpublish_h_
#define _ekstertera_tasks_task_unpublish_h_

#include "task_sync.h"

/*!
 * \brief Асинхронная задача EteraAPI::unpublish
 */
class EteraTaskUNPUBLISH : public EteraTaskSync
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param path Путь для закрытия доступа
         */
        EteraTaskUNPUBLISH(const QString& path);
        ~EteraTaskUNPUBLISH();

        /*!
         * \brief Overloaded QRunnable::run
         */
        void run();

    signals:

        /*!
         * \brief Успешное завершение операции
         * \param id Идентификатор задачи
         * \param item Описатель объекта к которому закрыт доступ
         * \param args Дополнительные аргументы (аргументы конструктора)
         */
        void onSuccess(quint64 id, const EteraItem& item, const QVariantMap& args);
};

#endif   // _ekstertera_tasks_task_unpublish_h_
