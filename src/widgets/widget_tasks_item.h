/*!
 * \file
 * \brief Элемент виджета списка асинхронных задач
 */

#ifndef _ekstertera_widgets_widget_tasks_item_h_
#define _ekstertera_widgets_widget_tasks_item_h_

#include "utils/api.h"

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

        quint64                     id()       const { return m_id;        }
        quint64                     parentID() const { return m_parent_id; }
        QProgressBar*               bar()      const { return m_bar;       }
        QMessageBox::StandardButton reply()    const { return m_reply;     }
        EteraAPI*                   api()      const { return m_api;       }

        void setParentID(quint64 parent_id)              { m_parent_id = parent_id; }
        void setBar(QProgressBar* bar)                   { m_bar       = bar;       }
        void setReply(QMessageBox::StandardButton reply) { m_reply     = reply;     }
        void setAPI(EteraAPI* api)                       { m_api       = api;       }

    private:

        quint64                     m_id;          /*!< \brief ID задачи           */
        quint64                     m_parent_id;   /*!< \brief ID родителя         */
        QProgressBar*               m_bar;         /*!< \brief Виджет прогресса    */
        QMessageBox::StandardButton m_reply;       /*!< \brief Стандартный ответ   */
        EteraAPI*                   m_api;         /*!< \brief API                 */

        /*!
         * \brief Инициализация полей в значение по умолчанию
         */
        void init();
};

#endif   // _ekstertera_widgets_widget_tasks_item_h_
