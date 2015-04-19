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

QIcon EteraIconProvider::icon(const EteraItem& item)
{
    if (item.isDir() == true) {
        if (item.isPublic() == true)
            return icon(QStyle::SP_DirLinkIcon);
        else
            return icon(QStyle::SP_DirIcon);
    } else if (item.isFile() == true) {
        if (item.isPublic() == true)
            return icon(QStyle::SP_FileLinkIcon);
        else
            return icon(QStyle::SP_FileIcon);
    }

    return QIcon();
}
//----------------------------------------------------------------------------------------------
