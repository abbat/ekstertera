/*!
 * \file
 * \brief Асинхронная задача API
 */

#ifndef _ekstertera_tasks_task_h_
#define _ekstertera_tasks_task_h_

#include "utils/api.h"

/*!
 * \brief Базовый класс для асинхронных вызовов API в QThreadPool
 */
class EteraTask : public QObject
{
    Q_OBJECT

    public:

        EteraTask();
        virtual ~EteraTask();

        /*!
         * \brief Выполнение задачи
         */
        virtual void run() = 0;

        /*!
         * \brief Добавление нестандартного аргумента к списку
         * \param name Имя аргумента
         * \param value Значение аргумента
         */
        void addArg(const QString& name, const QVariant& value) { m_args[name] = value; }

        /*!
         * \brief Генерация следующего id задачи
         * \return Новый ID задачи
         */
        static quint64 nextID();

        /*!
         * \brief Добавление mutex, блокирующего вызов сигнала onSuccess
         * \param mutex Mutex
         */
        void setSuccessMutex(QMutex* mutex) { m_success_mutex = mutex; }

        /*!
         * \brief Блокировка onSuccess mutex (см. setSuccessMutex)
         */
        void successLock() { if (m_success_mutex != NULL) m_success_mutex->lock(); }

        /*!
         * \brief Разблокировка onSuccess mutex (см. setSuccessMutex)
         */
        void successUnlock() { if (m_success_mutex != NULL) m_success_mutex->unlock(); }

    protected:

        /*!
         * \brief OAuth токен
         */
        QString m_token;

        /*!
         * Идентификатор задачи
         */
        quint64 m_id;

        /*!
         * \brief API для асинхронных вызовов
         */
        EteraAPI* m_api;

        /*!
         * \brief Аргументы задачи
         */
        QVariantMap m_args;

        /*!
         * \brief Блокиратор сигнала onSuccess
         */
        QMutex* m_success_mutex;

        /*!
         * \brief Инициализация API
         */
        virtual void init() = 0;

        /*!
         * \brief Освобождение ресурсов API
         */
        virtual void cleanup() = 0;

    signals:

        /*!
         * \brief Старт/прогресс операции
         * \param id Идентификатор задачи
         * \param message Информационное сообщение
         * \param args Дополнительные аргументы (аргументы конструктора)
         */
        void onStart(quint64 id, const QString& message, const QVariantMap& args);
};

#endif   // _ekstertera_tasks_task_h_
