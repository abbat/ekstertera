#include "widget_disk_item_delegate.h"
//----------------------------------------------------------------------------------------------

WidgetDiskItemDelegate::WidgetDiskItemDelegate(QObject* parent) : QStyledItemDelegate(parent)
{
}
//----------------------------------------------------------------------------------------------

WidgetDiskItemDelegate::~WidgetDiskItemDelegate()
{
}
//----------------------------------------------------------------------------------------------

void WidgetDiskItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItemV4 opt = *qstyleoption_cast<const QStyleOptionViewItemV4*>(&option);

    bool selected = opt.state & QStyle::State_Selected;

    QStyle* style  = (opt.widget != NULL ? opt.widget->style() : QApplication::style());
    QString text   = index.data(Qt::DisplayRole).toString();
    QIcon   icon   = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    QPixmap pixmap = icon.pixmap(opt.decorationSize, (selected == true ? QIcon::Selected : QIcon::Normal));

    int alignment = 0;
    switch (opt.decorationPosition) {
        case QStyleOptionViewItem::Left:
            alignment |= Qt::AlignLeft | Qt::AlignVCenter;
            break;
        case QStyleOptionViewItem::Right:
            alignment |= Qt::AlignRight | Qt::AlignVCenter;
            break;
        case QStyleOptionViewItem::Top:
            alignment |= Qt::AlignTop | Qt::AlignHCenter;
            break;
        case QStyleOptionViewItem::Bottom:
            alignment |= Qt::AlignBottom | Qt::AlignHCenter;
            break;
    }

    QRect icon_rect = style->itemPixmapRect(opt.rect, alignment, pixmap);
    style->drawItemPixmap(painter, opt.rect, alignment, pixmap);

    QRect free_rect;
    switch (opt.decorationPosition) {
        case QStyleOptionViewItem::Left:
            free_rect = opt.rect.adjusted(icon_rect.width() + 5, 0, -5, 0);
            break;
        case QStyleOptionViewItem::Right:
            free_rect = opt.rect.adjusted(0, 0, -icon_rect.width() - 5, 0);
            break;
        case QStyleOptionViewItem::Top:
            free_rect = opt.rect.adjusted(0, icon_rect.height() + 5, 0, 0);
            break;
        case QStyleOptionViewItem::Bottom:
            free_rect = opt.rect.adjusted(0, 0, -icon_rect.height() - 5, 0);
            break;
    }

    if (selected == true) {
        QRect text_rect = style->itemTextRect(opt.fontMetrics, free_rect, alignment, true, text);
        QRect high_rect = text_rect.adjusted(-5, -5, 5, 5);

        painter->save();

        painter->setRenderHint(QPainter::Antialiasing);

        painter->setPen(opt.palette.highlight().color());
        painter->setBrush(opt.palette.highlight());
        painter->drawRoundedRect(high_rect, 5, 5);

        painter->restore();
    }

    style->drawItemText(painter, free_rect, alignment, option.palette, true, text, (selected == true ? QPalette::HighlightedText : QPalette::WindowText));
}
//----------------------------------------------------------------------------------------------
