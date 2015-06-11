#include "widget_tasks_item.h"
//----------------------------------------------------------------------------------------------

WidgetTasksItem::WidgetTasksItem(QTreeWidget* parent) : QTreeWidgetItem(parent)
{
    init();
}
//----------------------------------------------------------------------------------------------

WidgetTasksItem::WidgetTasksItem(WidgetTasksItem* parent) : QTreeWidgetItem(parent)
{
    init();
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
