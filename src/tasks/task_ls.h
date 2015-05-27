/*!
 * \file
 * \brief Асинхронная задача EteraAPI::ls
 */

#ifndef _ekstertera_tasks_task_ls_h_
#define _ekstertera_tasks_task_ls_h_

#include "task_sync.h"

/*!
 * \brief Асинхронная задача EteraAPI::ls
 */
class EteraTaskLS : public EteraTaskSync
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param path Путь для получения списка файлов и директорий
         * \param preview Размер превью
         * \param crop Параметр для обрезания превью
         */
        EteraTaskLS(const QString& path, const QString& preview = "", bool crop = false);
        ~EteraTaskLS();

        /*!
         * \brief Overloaded QRunnable::run
         */
        void run();

    signals:

        /*!
         * \brief Успешное завершение операции
         * \param id Идентификатор задачи
         * \param list Список файлов и директорий
         * \param args Дополнительные аргументы (аргументы конструктора)
         */
        void onSuccess(quint64 id, const EteraItemList& list, const QVariantMap& args);
};

#endif   // _ekstertera_tasks_task_ls_h_
