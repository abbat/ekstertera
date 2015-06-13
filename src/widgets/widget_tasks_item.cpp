#include "widget_tasks_item.h"
//----------------------------------------------------------------------------------------------

WidgetTasksItem::WidgetTasksItem(quint64 id, QTreeWidget* parent) : QTreeWidgetItem(parent)
{
    init();
    m_id = id;
}
//----------------------------------------------------------------------------------------------

WidgetTasksItem::WidgetTasksItem(quint64 id, WidgetTasksItem* parent) : QTreeWidgetItem(parent)
{
    init();
    m_id = id;
}
//----------------------------------------------------------------------------------------------

WidgetTasksItem::~WidgetTasksItem()
{
}
//----------------------------------------------------------------------------------------------

void WidgetTasksItem::init()
{
    m_parent_id = 0;
    m_bar       = NULL;
    m_reply     = QMessageBox::NoButton;
    m_api       = NULL;
}
//----------------------------------------------------------------------------------------------
