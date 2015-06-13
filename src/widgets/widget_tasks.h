/*!
 * \file
 * \brief Виджет списка асинхронных задач
 */

#ifndef _ekstertera_widgets_widget_tasks_h_
#define _ekstertera_widgets_widget_tasks_h_

#include "widget_tasks_item.h"

/*!
 * \brief Виджет списка асинхронных задач
 */
class WidgetTasks : public QTreeWidget
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param parent Родительский виджет
         */
        WidgetTasks(QWidget* parent);
        ~WidgetTasks();

        /*!
         * \brief Создание экземпляра API и установка токена
         * \param id ID задачи
         * \return Экземпляр api
         */
        EteraAPI* createAPI(quint64 id = 0);

        /*!
         * \brief Получение экземпляра API из имеющегося
         * \param api API или NULL для создания нового экземпляра
         * \param id ID задачи
         * \return Экземпляр api
         */
        EteraAPI* resetAPI(EteraAPI* api, quint64 id);

        /*!
         * \brief Освобождение экземпляра API и удаление id задачи из списка задач
         * \param api API (может быть NULL)
         */
        void releaseAPI(EteraAPI* api);

        /*!
         * \brief Добавление задачи
         * \param id ID задачи
         * \param text Текст задачи
         * \param tooltip Всплывающая подсказка
         * \param api API
         */
        void addTask(quint64 id, const QString& text, const QString& tooltip = "", EteraAPI* api = NULL);

        /*!
         * \brief Добавление подчиненной задачи
         * \param parent ID родительской задачи
         * \param id ID задачи
         * \param text Текст задачи
         * \param tooltip Всплывающая подсказка
         * \param api API
         */
        void addChildTask(quint64 parent, quint64 id, const QString& text, const QString& tooltip = "", EteraAPI* api = NULL);

        /*!
         * \brief Проверка, что у задачи есть дочерние задачи
         * и удаление ее в случае отсуствия дочерних задач
         * \param id ID задачи
         */
        void checkTask(quint64 id);

        /*!
         * \brief Удаление задачи
         * \param id ID задачи
         */
        void removeTask(quint64 id);

        /*!
         * \brief Остановка и удаление задачи
         * \param id ID задачи
         * \param aborted Список id остановленных задач
         */
        void abortTask(quint64 id, QList<quint64>& aborted);

        /*!
         * \brief Получение ID корневой задачи
         * \param id ID дочерней задачи
         * \return ID корневой задачи или 0
         */
        quint64 rootID(quint64 id);

        /*!
         * \brief Получение стандартного ответа
         * \param id ID задачи
         */
        QMessageBox::StandardButton reply(quint64 id);

        /*!
         * \brief Установка стандартного ответа
         * \param id ID задачи
         * \param answer Ответ
         */
        void setReply(quint64 id, QMessageBox::StandardButton answer);

        /*!
         * \brief Установка API
         * \param id ID задачи
         * \param api API
         */
        void setAPI(quint64 id, EteraAPI* api);

        /*!
         * \brief Обновление прогресса задачи
         * \param id ID задачи
         * \param done Завершено
         * \param total Всего
         */
        void setProgress(quint64 id, qint64 done, qint64 total);

    protected:

        /*!
         * \brief QWidget::resizeEvent
         * Масштабирование колонок
         */
        void resizeEvent(QResizeEvent* event);

    private:

        /*!
         * \brief Карта задач по id задачи
         */
        QMap<quint64, WidgetTasksItem*> m_tasks;

        /*!
         * \brief Остановка и удаление задачи
         * \param elemnt Элемент дерева
         * \param aborted Список id остановленных задач
         */
        void abortTask(WidgetTasksItem* element, QList<quint64>& aborted);

    signals:

        /*!
         * \brief Сигнал смены количества задач
         * \param count Количество задач
         */
        void onChangeCount(int count);
};

#endif   // _ekstertera_widgets_widget_tasks_h_
