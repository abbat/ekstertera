#include "icon.h"
//----------------------------------------------------------------------------------------------
#include "settings.h"
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

EteraIconProvider::EteraIconProvider() : QObject()
{
    m_icon_sizes << 24 << 32 << 48 << 64 << 96 << 128 << 256;
    m_default_icon_size_index = 2 /* 48 */;

#ifdef ETERA_WS_WIN
    // данные типы файлов не имеют больших иконок под windows
    m_jumbo_workaround << "cer" << "chm" << "css" << "js" << "msi" << "prf" << "py" << "sh" << "wsf";
#endif

    // публичность
    m_link = prepareIcon(QIcon::fromTheme("emblem-symbolic-link", QIcon(":/icons/tango/emblem-symbolic-link.svg")), 2);

    // директория
    m_dir      = prepareIcon(QIcon::fromTheme("folder", QIcon(":/icons/tango/folder.svg")));
    m_dir_link = addLinkIcon(m_dir);

    // файл
    m_file      = prepareIcon(QIcon::fromTheme("text-x-generic", QIcon(":/icons/tango/text-x-generic.svg")));
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

            icon_base = prepareIcon(icon_base);

            QIcon icon_link = addLinkIcon(icon_base);

            m_media_icon[map->Type]      = icon_base;
            m_media_icon_link[map->Type] = icon_link;
        }

        map++;
    }

    m_preview_queue_size       = 0;
    m_preview_cache_size       = 0;
    m_preview_cache_link_size  = 0;
    m_preview_cache_size_limit = 100 * 1024 * 1024; // TODO: сделать настраиваемым, 100MB ~> 3 тыс. превьюшек
}
//----------------------------------------------------------------------------------------------

EteraIconProvider::~EteraIconProvider()
{
    clearPreviewCache(&m_preview_cache);
    clearPreviewCache(&m_preview_cache_link);
}
//----------------------------------------------------------------------------------------------

QIcon EteraIconProvider::prepareIcon(const QIcon& icon, int scale, bool center, bool preview)
{
    QIcon result;

    for (int i = 0; i < m_icon_sizes.count(); i++) {
        int size = m_icon_sizes[i] / scale;

        QPixmap pixmap = icon.pixmap(size);
        if (pixmap.width() < size) {
            if (center == false)
                pixmap = pixmap.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            else {
                int dx = (size - pixmap.width()) / 2;

                QPixmap base_pixmap(size, size);
                base_pixmap.fill(Qt::transparent);

                QPainter painter(&base_pixmap);

                painter.drawPixmap(dx, dx, pixmap.width(), pixmap.width(), pixmap);

                pixmap = base_pixmap;
            }
        }

        if (preview == true) {
            QPainter painter(&pixmap);

            painter.setPen(Qt::black);
            painter.drawRect(0, 0, size - 1, size - 1);

            painter.setPen(QColor(0xbb, 0xbb, 0xbb));
            painter.drawRect(1, 1, size - 3, size - 3);
        }

        result.addPixmap(pixmap);
    }

    return result;
}
//----------------------------------------------------------------------------------------------

QIcon EteraIconProvider::addLinkIcon(const QIcon& base_icon)
{
    QIcon result;

    for (int i = 0; i < m_icon_sizes.count(); i++) {
        int size = m_icon_sizes[i];
        int link_size = size / 2;

        QPixmap base_pixmap = base_icon.pixmap(size);
        QPixmap link_pixmap = m_link.pixmap(link_size);

        QPainter painter(&base_pixmap);

        painter.drawPixmap(link_size, link_size, link_pixmap);

        result.addPixmap(base_pixmap);
    }

    return result;
}
//----------------------------------------------------------------------------------------------
#ifdef ETERA_WS_X11_OR_WIN
bool EteraIconProvider::cachedIcon(QIcon& icon, const QString& key, bool shared)
{
    if (m_cache_icon_miss.contains(key) == true)
        return false;

    if (shared == true) {
        if (m_cache_icon_link.contains(key) == true) {
            icon = m_cache_icon_link[key];
            return true;
        }

        // непубличная иконка могла уже быть закэширована
        if (m_cache_icon.contains(key) == true) {
            QIcon link_icon = addLinkIcon(m_cache_icon[key]);
            m_cache_icon_link[key] = link_icon;
            icon = link_icon;
            return true;
        }

        return false;
    }

    if (m_cache_icon.contains(key) == true) {
        icon = m_cache_icon[key];
        return true;
    }

    return false;
}
#endif
//----------------------------------------------------------------------------------------------
#ifdef ETERA_WS_X11_OR_WIN
bool EteraIconProvider::cacheIcon(QIcon& icon, const QIcon& base_icon, const QString& key, bool shared)
{
    m_cache_icon[key] = base_icon;

    if (shared == true) {
        QIcon link_icon = addLinkIcon(base_icon);
        m_cache_icon_link[key] = link_icon;
        icon = link_icon;
    } else
        icon = base_icon;

    return true;
}
#endif
//----------------------------------------------------------------------------------------------
#ifdef ETERA_WS_WIN
bool EteraIconProvider::extensionIcon(QIcon& icon, const QString& ext, bool shared)
{
    if (cachedIcon(icon, ext, shared) == true)
        return true;

    // получение индекса иконки по расширению
    SHFILEINFO sfi;
    if (SHGetFileInfo((LPCTSTR)("." + ext).utf16(), FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES) == 0)
        return false;

    if (sfi.iIcon == 0) {
        m_cache_icon_miss.insert(ext);
        return false;
    }

    // генерация иконки по размерам
    QIcon base_icon;

    QList<int> sizes;
    sizes << SHIL_LARGE << SHIL_EXTRALARGE;

    bool center = m_jumbo_workaround.contains(ext);
    if (center == false)
        sizes << SHIL_JUMBO;

    for (int i = 0; i < sizes.size(); i++) {
        int size = sizes[i];

        IImageList* ilist;
        if (FAILED(SHGetImageList(size, IID_IImageList, (void**)&ilist)))
            return false;

        HICON   hicon;
        HRESULT result = ilist->GetIcon(sfi.iIcon, ILD_TRANSPARENT, &hicon);

        ilist->Release();

        if (result != S_OK)
            return false;

        QPixmap pixmap = QPixmap::fromWinHICON(hicon);

        DestroyIcon(hicon);

        if (pixmap.isNull() == true)
            return false;

        base_icon.addPixmap(pixmap);
    }

    return cacheIcon(icon, prepareIcon(base_icon, 1, center), ext, shared);
}
#endif
//----------------------------------------------------------------------------------------------
#ifdef ETERA_WS_X11
bool EteraIconProvider::mimeIcon(QIcon& icon, const QString& mime, bool shared)
{
    if (cachedIcon(icon, mime, shared) == true)
        return true;

    QMimeType mime_type = QMimeDatabase().mimeTypeForName(mime);

    if (mime_type.isValid() == false || mime_type.isDefault() == true) {
        m_cache_icon_miss.insert(mime);
        return false;
    }

    QIcon base_icon;
    if (QIcon::hasThemeIcon(mime_type.iconName()) == true)
        base_icon = QIcon::fromTheme(mime_type.iconName());
    else if (QIcon::hasThemeIcon(mime_type.genericIconName()) == true)
        base_icon = QIcon::fromTheme(mime_type.genericIconName());
    else {
        m_cache_icon_miss.insert(mime);
        return false;
    }

    return cacheIcon(icon, prepareIcon(base_icon), mime, shared);
}
#endif
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

QIcon EteraIconProvider::icon(const EteraItem* item)
{
    if (item->isDir() == true)
        return (item->isPublic() == true ? m_dir_link : m_dir);
    else if (item->isFile() == true) {
        QIcon result;

#ifdef ETERA_WS_WIN
        if (extensionIcon(result, item->extension(), item->isPublic()) == true)
            return result;
#endif

#ifdef ETERA_WS_X11
        if (mimeIcon(result, item->mimeType(), item->isPublic()) == true)
            return result;
#endif

        if (mediaIcon(result, item->mediaType(), item->isPublic()) == true)
            return result;

        return (item->isPublic() == true ? m_file_link : m_file);
    }

    return QIcon();
}
//----------------------------------------------------------------------------------------------

bool EteraIconProvider::preview(WidgetDiskItem* item)
{
    QDateTime now = QDateTime::currentDateTime();

    const EteraItem* eitem = item->item();

    QString preview = eitem->preview();

    if (eitem->isPublic() == true) {
        EteraPreviewCacheItem* citem = m_preview_cache_link.value(preview, NULL);

        if (citem != NULL) {
            citem->ATime = now;
            item->setIcon(citem->Icon);
            return true;
        }

        citem = m_preview_cache.value(preview, NULL);

        if (citem != NULL) {
            citem->ATime = now;

            EteraPreviewCacheItem* litem = new EteraPreviewCacheItem();

            litem->Icon  = addLinkIcon(citem->Icon);
            litem->Size  = citem->Size;
            litem->ATime = now;

            m_preview_cache_link[preview] = litem;
            m_preview_cache_link_size += litem->Size;

            item->setIcon(litem->Icon);

            return true;
        }
    } else {
        EteraPreviewCacheItem* citem = m_preview_cache.value(preview, NULL);

        if (citem != NULL) {
            citem->ATime = now;
            item->setIcon(citem->Icon);
            return true;
        }
    }

    item->setIcon(icon(eitem));

    m_preview_wait.insert(preview, item);

    if (m_preview_queue_size >= 8 /* TODO: #3 */) {
        m_preview_queue.insert(preview);
        return false;
    }

    m_preview_queue_size++;

    EteraAPI* api = new EteraAPI(this);

    api->setToken(EteraSettings::instance()->token());

    ETERA_API_CONNECT_GET(api, task_on_get_preview_success);
    ETERA_API_CONNECT_ERROR(api, task_on_get_preview_error);

    api->get(preview);

    return false;
}
//----------------------------------------------------------------------------------------------

void EteraIconProvider::cancelPreview()
{
    m_preview_wait.clear();
    m_preview_queue.clear();
}
//----------------------------------------------------------------------------------------------

void EteraIconProvider::cancelPreview(const WidgetDiskItem* item)
{
    QString preview = item->item()->preview();

    if (m_preview_wait.contains(preview) == true)
        m_preview_wait.remove(preview);

    if (m_preview_queue.contains(preview) == true)
        m_preview_queue.remove(preview);
}
//----------------------------------------------------------------------------------------------

void EteraIconProvider::task_on_get_preview_success(EteraAPI* api)
{
    m_preview_queue_size--;

    QString url = api->url();

    const QByteArray& data = static_cast<QBuffer*>(api->device())->buffer();

    QPixmap pixmap;
    pixmap.loadFromData(data);

    api->device()->deleteLater();

    if (pixmap.isNull() == true) {
        if (m_preview_wait.contains(url) == true)
            m_preview_wait.remove(url);

        loadNextPreview(api);

        return;
    }

    QIcon icon;
    icon.addPixmap(pixmap);

    icon = prepareIcon(icon, 1, false, true);

    EteraPreviewCacheItem* citem = new EteraPreviewCacheItem();

    citem->Icon  = icon;
    citem->Size  = data.size();
    citem->ATime = QDateTime::currentDateTime();

    m_preview_cache[url]  = citem;
    m_preview_cache_size += citem->Size;

    WidgetDiskItem* item = m_preview_wait.value(url, NULL);
    if (item != NULL) {
        m_preview_wait.remove(url);

        if (item->item()->isPublic() == false)
            item->setIcon(citem->Icon);
        else {
            EteraPreviewCacheItem* litem = new EteraPreviewCacheItem();

            litem->Icon  = addLinkIcon(citem->Icon);
            litem->Size  = citem->Size;
            litem->ATime = citem->ATime;

            m_preview_cache_link[url]  = litem;
            m_preview_cache_link_size += litem->Size;

            item->setIcon(litem->Icon);
        }
    }

    if (m_preview_cache_size + m_preview_cache_link_size > m_preview_cache_size_limit)
        gcPreviewCache();

    loadNextPreview(api);
}
//----------------------------------------------------------------------------------------------

void EteraIconProvider::task_on_get_preview_error(EteraAPI* api)
{
    m_preview_queue_size--;

    QString  url    = api->url();
    QBuffer* device = static_cast<QBuffer*>(api->device());

    device->deleteLater();

    if (m_preview_wait.contains(url) == true)
        m_preview_wait.remove(url);

    if (m_preview_cache_size + m_preview_cache_link_size > m_preview_cache_size_limit)
        gcPreviewCache();

    loadNextPreview(api);
}
//----------------------------------------------------------------------------------------------

void EteraIconProvider::loadNextPreview(EteraAPI* api)
{
    if (m_preview_queue.count() == 0)
        api->deleteLater();
    else {
        m_preview_queue_size++;

        QSet<QString>::const_iterator i = m_preview_queue.constBegin();
        QString preview = *i;
        m_preview_queue.remove(preview);

        api->get(preview);
    }
}
//----------------------------------------------------------------------------------------------

void EteraIconProvider::clearPreviewCache(EteraPreviewCache* cache)
{
    EteraPreviewCache::const_iterator i = cache->constBegin();
    while (i != cache->constEnd()) {
        EteraPreviewCacheItem* item = i.value();
        delete item;
        ++i;
    }

    cache->clear();

    if (cache == &m_preview_cache)
        m_preview_cache_size = 0;
    else if (cache == &m_preview_cache_link)
        m_preview_cache_link_size = 0;
}
//----------------------------------------------------------------------------------------------

void EteraIconProvider::gcPreviewCache()
{
    // пока что простой LRU
    QMap<QDateTime, QString> map;

    EteraPreviewCache::const_iterator i = m_preview_cache.constBegin();
    while (i != m_preview_cache.constEnd()) {
        map[i.value()->ATime] = i.key();
        ++i;
    }

    i = m_preview_cache_link.constBegin();
    while (i != m_preview_cache_link.constEnd()) {
        map[i.value()->ATime] = i.key();
        ++i;
    }

    quint64 threshold = m_preview_cache_size_limit / 4 * 3;

    QMap<QDateTime, QString>::const_iterator j = map.constBegin();
    while (m_preview_cache_size + m_preview_cache_link_size > threshold) {
        QString url = j.value();

        EteraPreviewCacheItem* item = m_preview_cache.value(url, NULL);
        if (item != NULL) {
            m_preview_cache_size -= item->Size;
            delete item;
            m_preview_cache.remove(url);
        }

        item = m_preview_cache_link.value(url, NULL);
        if (item != NULL) {
            m_preview_cache_link_size -= item->Size;
            delete item;
            m_preview_cache_link.remove(url);
        }

        ++j;
    }
}
//----------------------------------------------------------------------------------------------
