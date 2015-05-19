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
    // директория
    m_dir      = QIcon::fromTheme("folder", QIcon(":/icons/tango/folder.svg"));
    m_dir_link = addLinkIcon(m_dir);

    // файл
    m_file      = QIcon::fromTheme("text-x-generic", QIcon(":/icons/tango/text-x-generic.svg"));
    m_file_link = addLinkIcon(m_file);

    /*!
     * \brief Описатель соответствий типа медиа и иконки
     */
    typedef struct {
        EteraItemMediaType Type;           /*!< \brief Тип медиа              */
        const char*        ThemeIcon;      /*!< \brief Имя иконки из темы     */
        const char*        ResourceIcon;   /*!< \brief Имя иконки из ресурсов */
    } EteraMediaIconMap;

    /*!
     * \brief Карта соответствий типа медиа и иконки
     */
    const EteraMediaIconMap media_icon_map[] = {
        { eimtAudio,       "audio-x-generic",          ":/icons/tango/audio-x-generic.svg"          },
        { eimtBackup,      "package-x-generic",        ":/icons/tango/package-x-generic.svg"        },
        { eimtBook,        NULL,                       NULL                                         },
        { eimtCompressed,  "package-x-generic",        ":/icons/tango/package-x-generic.svg"        },
        { eimtData,        NULL,                       NULL                                         },
        { eimtDevelopment, "text-x-script",            ":/icons/tango/text-x-script.svg"            },
        { eimtDiskimage,   "package-x-generic",        ":/icons/tango/package-x-generic.svg"        },
        { eimtDocument,    "x-office-document",        ":/icons/tango/x-office-document.svg"        },
        { eimtEncoded,     NULL,                       NULL                                         },
        { eimtExecutable,  "application-x-executable", ":/icons/tango/application-x-executable.svg" },
        { eimtFlash,       "video-x-generic",          ":/icons/tango/video-x-generic.svg"          },
        { eimtFont,        "font-x-generic",           ":/icons/tango/font-x-generic.svg"           },
        { eimtImage,       "image-x-generic",          ":/icons/tango/image-x-generic.svg"          },
        { eimtSettings,    NULL,                       NULL                                         },
        { eimtSpreadsheet, "x-office-spreadsheet",     ":/icons/tango/x-office-spreadsheet.svg"     },
        { eimtText,        "text-x-generic",           ":/icons/tango/text-x-generic.svg"           },
        { eimtVideo,       "video-x-generic",          ":/icons/tango/video-x-generic.svg"          },
        { eimtWeb,         "text-html",                ":/icons/tango/text-html.svg"                },
        { eimtUnknown,     NULL,                       NULL                                         }
    };

    const EteraMediaIconMap* map = media_icon_map;

    while (map->Type != eimtUnknown) {
        if (map->ThemeIcon != NULL || map->ResourceIcon != NULL) {
            QIcon icon_base;

            if (map->ThemeIcon != NULL)
                icon_base = QIcon::fromTheme(map->ThemeIcon, QIcon(map->ResourceIcon));
            else
                icon_base = QIcon(map->ResourceIcon);

            QIcon icon_link = addLinkIcon(icon_base);

            m_media_icon[map->Type]      = icon_base;
            m_media_icon_link[map->Type] = icon_link;
        }

        map++;
    }
}
//----------------------------------------------------------------------------------------------

EteraIconProvider::~EteraIconProvider()
{
}
//----------------------------------------------------------------------------------------------

QIcon EteraIconProvider::addLinkIcon(QIcon base_icon)
{
    QIcon result;
    QIcon link_icon = QIcon::fromTheme("emblem-symbolic-link", QIcon(":/icons/tango/emblem-symbolic-link.svg"));

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
//----------------------------------------------------------------------------------------------

bool EteraIconProvider::mediaIcon(QIcon& icon, EteraItemMediaType type, bool shared)
{
    if (shared == true) {
        if (m_media_icon_link.contains(type) == false)
            return false;

        icon = m_media_icon_link[type];

        return true;
    }

    if (m_media_icon.contains(type) == false)
        return false;

    icon = m_media_icon[type];

    return true;
}
//----------------------------------------------------------------------------------------------

QIcon EteraIconProvider::icon(const EteraItem& item)
{
    if (item.isDir() == true)
        return (item.isPublic() == true ? m_dir_link : m_dir);
    else if (item.isFile() == true) {
        QIcon result;

        if (mediaIcon(result, item.mediaType(), item.isPublic()) == true)
            return result;

        return (item.isPublic() == true ? m_file_link : m_file);
    }

    return QIcon();
}
//----------------------------------------------------------------------------------------------
