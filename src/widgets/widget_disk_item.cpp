#include "widget_disk_item.h"
//----------------------------------------------------------------------------------------------
#include "icon.h"
//----------------------------------------------------------------------------------------------

WidgetDiskItem::WidgetDiskItem(QListWidget* parent, const EteraItem& item) : QListWidgetItem(parent, QListWidgetItem::UserType)
{
    replaceItem(item);
}
//----------------------------------------------------------------------------------------------

WidgetDiskItem::~WidgetDiskItem()
{
}
//----------------------------------------------------------------------------------------------

bool WidgetDiskItem::operator<(const QListWidgetItem& other) const
{
    const EteraItem* item = (static_cast<const WidgetDiskItem*>(&other))->item();

    if (m_item.type() == item->type())
        return QListWidgetItem::operator<(other);

    if (m_item.isDir() == true)
        return true;

    return false;
}
//----------------------------------------------------------------------------------------------

void WidgetDiskItem::update()
{
    EteraIconProvider* icon_provider = EteraIconProvider::instance();

    setText(m_item.name());

    setIcon(icon_provider->icon(m_item));
}
//----------------------------------------------------------------------------------------------

void WidgetDiskItem::revertText()
{
    setText(m_item.name());
}
//----------------------------------------------------------------------------------------------

void WidgetDiskItem::replaceItem(const EteraItem& item)
{
    m_item = item;
    update();
}
//----------------------------------------------------------------------------------------------
