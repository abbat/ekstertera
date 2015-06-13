#include "widget_tasks_item.h"
//----------------------------------------------------------------------------------------------

WidgetTasksItem::WidgetTasksItem(quint64 id, QTreeWidget* parent) : QTreeWidgetItem(parent)
{
    m_id  = id;
    m_bar = NULL;
}
//----------------------------------------------------------------------------------------------

WidgetTasksItem::WidgetTasksItem(quint64 id, WidgetTasksItem* parent) : QTreeWidgetItem(parent)
{
    m_id  = id;
    m_bar = NULL;
}
//----------------------------------------------------------------------------------------------

WidgetTasksItem::~WidgetTasksItem()
{
}
//----------------------------------------------------------------------------------------------
