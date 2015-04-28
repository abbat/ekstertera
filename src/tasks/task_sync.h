/*!
 * \file
 * \brief Асинхронная задача API, выполняющаяся последовательно
 */

#ifndef _ekstertera_tasks_task_sync_h_
#define _ekstertera_tasks_task_sync_h_

#include "task.h"

/*!
 * \brief Базовый класс для асинхронных вызовов API в QThreadPool, выполняющихся последовательно
 */
class EteraTaskSync : public EteraTask
{
    Q_OBJECT

    public:

        EteraTaskSync();
        virtual ~EteraTaskSync();

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
         * \brief Сигнал ошибки ssl
         * \param error Ошибки
         */
        void api_on_error(const QString& error);

    signals:

        /*!
         * \brief Сигнал ошибки
         * \param id Идентификатор задачи
         * \param code Код ошибки
         * \param error Описание ошибки
         * \param args Дополнительные аргументы (аргументы конструктора)
         */
        void onError(quint64 id, int code, const QString& error, const QVariantMap& args);
};

#endif   // _ekstertera_tasks_task_sync_h_
