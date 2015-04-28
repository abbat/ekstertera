/*!
 * \file
 * \brief Элемент виджета списка асинхронных задач
 */

#ifndef _ekstertera_widgets_widget_tasks_item_h_
#define _ekstertera_widgets_widget_tasks_item_h_

#include "sysheaders.h"

/*!
 * \brief Элемент виджета списка асинхронных задач
 */
class WidgetTasksItem : public QTreeWidgetItem
{
    public:

        /*!
         * \brief Конструктор
         * \param parent Родительский виджет
         */
        WidgetTasksItem(QTreeWidget* parent);

        /*!
         * \brief Конструктор
         * \param parent Родительский элемент
         */
        WidgetTasksItem(WidgetTasksItem* parent);

        ~WidgetTasksItem();
};

#endif   // _ekstertera_widgets_widget_tasks_item_h_
