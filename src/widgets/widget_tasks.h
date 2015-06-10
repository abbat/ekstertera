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
         */
        void addTask(quint64 id, const QString& text);

        /*!
         * \brief Добавление подчиненной задачи
         * \param parent ID родительской задачи
         * \param id ID задачи
         * \param text Текст задачи
         */
        void addChildTask(quint64 parent, quint64 id, const QString& text);

        /*!
         * \brief Удаление задачи
         * \param id ID задачи
         */
        void removeTask(quint64 id);

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
        QMessageBox::StandardButton answer(quint64 id);

        /*!
         * \brief Установка стандартного ответа
         * \param id ID задачи
         * \param answer Ответ
         */
        void setAnswer(quint64 id, QMessageBox::StandardButton answer);

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
         * \brief Элемент карты задач
         */
        typedef struct {
            quint64                     Parent;   /*!< \brief ID родителя         */
            WidgetTasksItem*            Item;     /*!< \brief Графический элемент */
            QProgressBar*               Bar;      /*!< \brief Виджет прогресса    */
            QMessageBox::StandardButton Answer;   /*!< \brief Стандартный ответ   */
        } TasksItem;

        /*!
         * \brief Карта задач по id задачи
         */
        QMap<quint64, TasksItem*> m_tasks;

    signals:

        /*!
         * \brief Сигнал смены количества задач
         * \param count Количество задач
         */
        void onChangeCount(int count);
};

#endif   // _ekstertera_widgets_widget_tasks_h_
