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
         * \brief Добавление задачи
         * \param id ID задачи
         * \param text Текст задачи
         * \param tooltip Всплывающая подсказка
         */
        void addTask(quint64 id, const QString& text, const QString& tooltip = "");

        /*!
         * \brief Добавление подчиненной задачи
         * \param parent ID родительской задачи
         * \param id ID задачи
         * \param text Текст задачи
         * \param tooltip Всплывающая подсказка
         */
        void addChildTask(quint64 parent, quint64 id, const QString& text, const QString& tooltip = "");

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
         * \brief Получение списка ID дочерних задач
         * \param id ID задачи
         * \param ids Список ID дочерних задач (включительно)
         */
        void childIDs(quint64 id, QList<quint64>& ids);

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
         * \brief Обновление прогресса задачи
         * \param id ID задачи
         * \param done Завершено
         * \param total Всего
         */
        void setProgress(quint64 id, qint64 done, qint64 total);

        /*!
         * \brief Сброс прогресса задачи
         * \param id ID задачи
         */
        void resetProgress(quint64 id);

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
         * \brief Карта ответов по id задачи
         */
        QMap<quint64, QMessageBox::StandardButton> m_reply;

        /*!
         * \brief Удаление задачи
         * \param item Элемент задачи
         */
        void removeTask(WidgetTasksItem* item);

        /*!
         * \brief Получение списка ID дочерних задач
         * \param elemnt Задача
         * \param ids Список ID дочерних задач (включительно)
         */
        void childIDs(WidgetTasksItem* element, QList<quint64>& ids);

    signals:

        /*!
         * \brief Сигнал смены количества задач
         * \param count Количество задач
         */
        void onChangeCount(int count);
};

#endif   // _ekstertera_widgets_widget_tasks_h_
