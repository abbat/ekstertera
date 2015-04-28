/*!
 * \file
 * \brief Асинхронная задача API, требующая ожидания асинхронной операции
 */

#ifndef _ekstertera_tasks_task_async_h_
#define _ekstertera_tasks_task_async_h_

#include "task.h"

/*!
 * \brief Базовый класс для асинхронных вызовов API в QThreadPool, требующих ожидания асинхронной операции
 */
class EteraTaskAsync : public EteraTask
{
    Q_OBJECT

    public:

        EteraTaskAsync();
        virtual ~EteraTaskAsync();

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
         * \param async Флаг того, что ошибка произошла во время асинхронной операции ожидания
         * \param args Дополнительные аргументы (аргументы конструктора)
         */
        void onError(quint64 id, int code, const QString& error, bool async, const QVariantMap& args);
};

#endif   // _ekstertera_tasks_task_async_h_
