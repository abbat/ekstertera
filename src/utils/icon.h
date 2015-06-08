/*!
 * \file
 * \brief Работа с провайдером иконок
 */

#ifndef _ekstertera_icon_h_
#define _ekstertera_icon_h_

#include "widgets/widget_disk_item.h"

/*!
 * \brief Работа с провайдером иконок
 */
class EteraIconProvider : public QObject
{
    Q_OBJECT

    public:

        /*!
         * \brief Инициализация провайдера иконок
         */
        static void init();

        /*!
         * \brief Освобождение ресурсов провайдера иконок
         */
        static void cleanup();

        /*!
         * \brief Singleton
         * \return Провайдер иконок
         */
        static EteraIconProvider* instance();

        /*!
         * \brief Получение иконки для элемента Яндекс.Диск
         * \param item Описатель элемента
         * \return Иконка для запрошенного элемента
         */
        QIcon icon(const EteraItem* item);

        /*!
         * \brief Получение превью или постановка в очередь на ожидание
         * \param item Описатель элемента
         * Если превью не найдено, будет установлена иконка по умолчанию
         * и элемент будет поставлен в очередь на ожидание, для отмены ожидания
         * элемент должен вызвать cancelPreview
         * \return true если установлено превью, false если поставлено в очередь
         */
        bool preview(WidgetDiskItem* item);

        /*!
         * \brief Отмена ожидания загрузки превью для всей очереди
         */
        void cancelPreview();

        /*!
         * \brief Отмена ожидания загрузки превью
         * \param item Описатель элемента
         */
        void cancelPreview(const WidgetDiskItem* item);

        /*!
         * \brief Получение списка поддерживаемых размеров иконок
         * \return Список размеров
         */
        const QList<int>* iconSizes() const { return &m_icon_sizes; }

        /*!
         * \brief Получение максимального размера иконки
         * \return Максимальный размер иконки
         */
        int maxIconSize() const { return m_icon_sizes[m_icon_sizes.count() - 1]; }

        /*!
         * \brief Получение индекса размера по умолчанию
         * \return Индекс в списке размеров
         */
        int defaultIconSizeIndex() const { return m_default_icon_size_index; }

    private:

        EteraIconProvider();
        ~EteraIconProvider();

        /*!
         * \brief Подготовка иконки поддерживаемых размеров
         * \param icon Базовая иконка
         * \param scale Фактор уменьшения
         * \param center Центрировать меньшее изображение вместо масштабирования
         * \param preview Рамка превью
         * \return Результирующая иконка
         */
        QIcon prepareIcon(const QIcon& icon, int scale = 1, bool center = false, bool preview = false);

        /*!
         * \brief Добавление к стандартной иконке символа симлинка
         * \param base_icon Иконка для наложения символа
         * Иконка должна содержать все поддерживаемые размеры (см. prepareIcon)
         * \return Результирующая иконка
         */
        QIcon addLinkIcon(const QIcon& base_icon);

#ifdef ETERA_WS_X11_OR_WIN
        /*!
         * \brief Получение иконки из кэша
         * \param icon Иконка
         * \param key Ключ кэша
         * \param shared Флаг публичного доступа
         * \return true, если иконка найдена
         */
        bool cachedIcon(QIcon& icon, const QString& key, bool shared);

        /*!
         * \brief Добавление иконки в кэш
         * \param icon Результирующая иконка
         * \param base_icon Базовая иконка для кэширования
         * Иконка должна содержать все поддерживаемые размеры (см. prepareIcon)
         * \param key Ключ кэша
         * \param shared Флаг публичного доступа
         * \return Всегда true
         */
        bool cacheIcon(QIcon& icon, const QIcon& base_icon, const QString& key, bool shared);
#endif

#ifdef ETERA_WS_WIN
        /*!
         * \brief Получение иконки по расширению
         * \param icon Иконка
         * \param ext Расширение
         * \param shared Флаг публичного доступа
         * \return true, если иконка найдена
         */
        bool extensionIcon(QIcon& icon, const QString& ext, bool shared);
#endif

#ifdef ETERA_WS_X11
        /*!
         * \brief Получение иконки по MIME типу
         * \param icon Иконка
         * \param mime MIME тип
         * \param shared Флаг публичного доступа
         * \return true, если иконка найдена
         */
        bool mimeIcon(QIcon& icon, const QString& mime, bool shared);
#endif

        /*!
         * \brief Получение иконки по медиа типу
         * \param icon Иконка
         * \param type Тип медиа
         * \param shared Флаг публичного доступа
         * \return true, если иконка найдена
         */
        bool mediaIcon(QIcon& icon, EteraItemMediaType type, bool shared);

    private:

        /*!
         * \brief Поддерживаемые размеры иконок
         */
        QList<int> m_icon_sizes;
        int        m_default_icon_size_index;   /*!< \brief Индекс размера иконки по умолчанию */

        QIcon m_link;        /*!< \brief Стандартная иконка публичности          */
        QIcon m_dir;         /*!< \brief Стандартная иконка директории           */
        QIcon m_dir_link;    /*!< \brief Стандартная иконка публичной директории */
        QIcon m_file;        /*!< \brief Стандартная иконка файла                */
        QIcon m_file_link;   /*!< \brief Стандартная иконка публичного файла     */

        QMap<EteraItemMediaType, QIcon> m_media_icon;        /*!< \brief Карта иконок по медиа типу           */
        QMap<EteraItemMediaType, QIcon> m_media_icon_link;   /*!< \brief Карта публичных иконок по медиа типу */

        /*!
         * \brief Элемент кэша превью
         */
        typedef struct {
            QIcon     Icon;    /*!< \brief Иконка                   */
            quint64   Size;    /*!< \brief Исходный объем           */
            QDateTime ATime;   /*!< \brief Время последнего доступа */
        } EteraPreviewCacheItem;

        /*!
         * \brief Кэш превью
         */
        typedef QMap<QUrl, EteraPreviewCacheItem*> EteraPreviewCache;

        /*!
         * \brief Кэш ожидающих превью
         */
        typedef QMultiMap<QUrl, WidgetDiskItem*> EteraPreviewWaitCache;

        QSet<QUrl>            m_preview_queue;        /*!< \brief Очередь ожидающих начала загрузки превью */
        EteraPreviewWaitCache m_preview_wait;         /*!< \brief Карта ожидающих превью элементов         */
        EteraPreviewCache     m_preview_cache;        /*!< \brief Карта иконок по превью                   */
        EteraPreviewCache     m_preview_cache_link;   /*!< \brief Карта публичных иконок по превью         */

        quint64 m_preview_queue_size;         /*!< \brief Размер очереди ожидания         */
        quint64 m_preview_cache_size;         /*!< \brief Размер иконок превью            */
        quint64 m_preview_cache_link_size;    /*!< \brief Размер публичных иконок превью  */
        quint64 m_preview_cache_size_limit;   /*!< \brief Максимальный размер кэша превью */

        /*!
         * \brief Загрузка следующего превью из очереди
         * \param api API
         */
        void loadNextPreview(EteraAPI* api);

        /*!
         * \brief Очистка кэша превью
         * \param cache Кэш
         */
        void clearPreviewCache(EteraPreviewCache* cache);

        /*!
         * \brief Уменьшение размера кэша превью
         */
        void gcPreviewCache();

#ifdef ETERA_WS_X11_OR_WIN
        QMap<QString, QIcon> m_cache_icon;        /*!< \brief Карта иконок             */
        QMap<QString, QIcon> m_cache_icon_link;   /*!< \brief Карта публичных иконок   */
        QSet<QString>        m_cache_icon_miss;   /*!< \brief Кэш отсутствующих иконок */
#endif

#ifdef ETERA_WS_WIN
        QSet<QString> m_jumbo_workaround;   /*!< \brief Кэш расширений иконок не имеющих большого размера */
#endif

    private slots:

        void task_on_get_preview_error(EteraAPI* api);
        void task_on_get_preview_success(EteraAPI* api, const QUrl& url, QIODevice* device);
};

#endif   // _ekstertera_icon_h_
