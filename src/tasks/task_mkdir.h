/*!
 * \file
 * \brief Асинхронная задача EteraAPI::mkdir
 */

#ifndef _ekstertera_tasks_task_mkdir_h_
#define _ekstertera_tasks_task_mkdir_h_

#include "task_sync.h"

/*!
 * \brief Асинхронная задача EteraAPI::mkdir
 */
class EteraTaskMKDIR : public EteraTaskSync
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param path Путь для создания директории
         */
        EteraTaskMKDIR(const QString& path);
        ~EteraTaskMKDIR();

        /*!
         * \brief Overloaded QRunnable::run
         */
        void run();

    signals:

        /*!
         * \brief Успешное завершение операции
         * \param id Идентификатор задачи
         * \param item Описатель созданной директории
         * \param args Дополнительные аргументы (аргументы конструктора)
         */
        void onSuccess(quint64 id, const EteraItem& item, const QVariantMap& args);
};

#endif   // _ekstertera_tasks_task_mkdir_h_
