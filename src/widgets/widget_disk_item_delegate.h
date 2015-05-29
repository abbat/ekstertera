/*!
 * \file
 * \brief Делегат для ручной отрисовки элемента виджета проводника Яндекс.Диск
 */

#ifndef _ekstertera_widgets_widget_disk_item_delegate_h_
#define _ekstertera_widgets_widget_disk_item_delegate_h_

#include "sysheaders.h"

/*!
 * \brief Делегат для ручной отрисовки элемента виджета проводника Яндекс.Диск
 */
class WidgetDiskItemDelegate : public QStyledItemDelegate
{
    public:

        WidgetDiskItemDelegate(QObject* parent = NULL);
        ~WidgetDiskItemDelegate();

    // QStyledItemDelegate
    public:

        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif   // _ekstertera_widgets_widget_disk_item_delegate_h_
