/*!
 * \file
 * \brief Элемент виджета списка асинхронных задач
 */

#ifndef _ekstertera_widgets_widget_tasks_item_h_
#define _ekstertera_widgets_widget_tasks_item_h_

#include "widget_progress.h"

/*!
 * \brief Элемент виджета списка асинхронных задач
 */
class WidgetTasksItem : public QTreeWidgetItem
{
    public:

        /*!
         * \brief Конструктор
         * \param id ID задачи
         * \param parent Родительский виджет
         */
        WidgetTasksItem(quint64 id, QTreeWidget* parent);

        /*!
         * \brief Конструктор
         * \param id ID задачи
         * \param parent Родительский элемент
         */
        WidgetTasksItem(quint64 id, WidgetTasksItem* parent);

        ~WidgetTasksItem();

        /*!
         * \brief ID задачи
         * \return ID задачи
         */
        quint64 id() const { return m_id; }

        /*!
         * \brief Виджет прогресса задачи
         * \return Виджет прогресса задачи
         */
        WidgetProgressbar* bar() const { return m_bar; }

        /*!
         * \brief Установка виджета прогресса задачи
         * \param bar Виджет прогресса
         */
        void setBar(WidgetProgressbar* bar) { m_bar = bar; }

    private:

        quint64            m_id;    /*!< \brief ID задачи        */
        WidgetProgressbar* m_bar;   /*!< \brief Виджет прогресса */
};

#endif   // _ekstertera_widgets_widget_tasks_item_h_
