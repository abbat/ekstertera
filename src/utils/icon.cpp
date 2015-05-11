#include "icon.h"
//----------------------------------------------------------------------------------------------
static EteraIconProvider* g_icon_provider = NULL;
//----------------------------------------------------------------------------------------------

void EteraIconProvider::init()
{
    g_icon_provider = new EteraIconProvider();
}
//----------------------------------------------------------------------------------------------

void EteraIconProvider::cleanup()
{
    delete g_icon_provider;
}
//----------------------------------------------------------------------------------------------

EteraIconProvider* EteraIconProvider::instance()
{
    return g_icon_provider;
}
//----------------------------------------------------------------------------------------------

EteraIconProvider::EteraIconProvider()
{
}
//----------------------------------------------------------------------------------------------

EteraIconProvider::~EteraIconProvider()
{
}
//----------------------------------------------------------------------------------------------

QIcon EteraIconProvider::standardIcon(QStyle::StandardPixmap pixmap)
{
#ifndef Q_WS_WIN
    return m_style.standardIcon(pixmap);
#else
    return tangoIcon(pixmap);
#endif
}
//----------------------------------------------------------------------------------------------

QIcon EteraIconProvider::icon(const EteraItem& item)
{
    if (item.isDir() == true) {
        if (item.isPublic() == true)
            return standardIcon(QStyle::SP_DirLinkIcon);
        else
            return standardIcon(QStyle::SP_DirIcon);
    } else if (item.isFile() == true) {
        if (item.isPublic() == true)
            return standardIcon(QStyle::SP_FileLinkIcon);
        else
            return standardIcon(QStyle::SP_FileIcon);
    }

    return QIcon();
}
//----------------------------------------------------------------------------------------------
#ifdef Q_WS_WIN
QIcon EteraIconProvider::tangoIcon(QStyle::StandardPixmap pixmap)
{
    switch (pixmap) {
        case QStyle::SP_FileIcon:
            return QIcon(":/icons/tango/text-x-generic.svg");
        case QStyle::SP_FileLinkIcon:
            return tangoAddLink(":/icons/tango/text-x-generic.svg");
        case QStyle::SP_DirIcon:
            return QIcon(":/icons/tango/folder.svg");
        case QStyle::SP_DirLinkIcon:
            return tangoAddLink(":/icons/tango/folder.svg");
        default:
            return m_style.standardIcon(pixmap);
    }
}
#endif
//----------------------------------------------------------------------------------------------
#ifdef Q_WS_WIN
QIcon EteraIconProvider::tangoAddLink(const QString& base)
{
    QIcon result;

    QIcon base_icon = QIcon(base);
    QIcon link_icon = QIcon(":/icons/tango/emblem-symbolic-link.svg");

    QList<int> sizes;
    sizes << 16 << 32 << 48 << 64;

    for (int i = 0; i < sizes.size(); i++) {
        int size = sizes[i];

        QPixmap base_pixmap = base_icon.pixmap(size);
        QPixmap link_pixmap = link_icon.pixmap(size / 2);

        QPainter painter(&base_pixmap);

        painter.drawPixmap(size / 2, size / 2, link_pixmap);

        result.addPixmap(base_pixmap);
    }

    return result;
}
#endif
//----------------------------------------------------------------------------------------------
