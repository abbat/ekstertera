/*!
 * \file
 * \brief Асинхронная задача EteraAPI::put
 */

#ifndef _ekstertera_tasks_task_put_h_
#define _ekstertera_tasks_task_put_h_

#include "task_progress.h"

/*!
 * \brief Асинхронная задача EteraAPI::put
 */
class EteraTaskPUT : public EteraTaskProgress
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param source Имя локального файла
         * \param target Путь назначения
         * \param overwrite Признак перезаписи (false - перезапись запрещена)
         */
        EteraTaskPUT(const QString& source, const QString& target, bool overwrite);
        ~EteraTaskPUT();

        /*!
         * \brief Overloaded QRunnable::run
         */
        void run();

    signals:

        /*!
         * \brief Успешное завершение операции (с установленным флагом stat)
         * \param id Идентификатор задачи
         * \param item Описатель загруженного объекта
         * \param args Дополнительные аргументы (аргументы конструктора)
         */
        void onSuccess(quint64 id, const EteraItem& item, const QVariantMap& args);
};

#endif   // _ekstertera_tasks_task_put_h_
