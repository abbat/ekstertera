/*!
 * \file
 * \brief Работа с провайдером иконок
 */

#ifndef _ekstertera_icon_h_
#define _ekstertera_icon_h_

#include "api.h"

/*!
 * \brief Работа с провайдером иконок
 */
class EteraIconProvider
{
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
        QIcon icon(const EteraItem& item);

    private:

        EteraIconProvider();
        ~EteraIconProvider();

        /*!
         * \brief Добавление к стандартной иконке символа симлинка
         * \param base_icon Иконка для наложения символа
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

        QIcon m_dir;         /*!< \brief Стандартная иконка директории           */
        QIcon m_dir_link;    /*!< \brief Стандартная иконка публичной директории */
        QIcon m_file;        /*!< \brief Стандартная иконка файла                */
        QIcon m_file_link;   /*!< \brief Стандартная иконка публичного файла     */

        QMap<EteraItemMediaType, QIcon> m_media_icon;        /*!< \brief Карта иконок по медиа типу           */
        QMap<EteraItemMediaType, QIcon> m_media_icon_link;   /*!< \brief Карта публичных иконок по медиа типу */

#ifdef ETERA_WS_X11_OR_WIN
        QMap<QString, QIcon> m_cache_icon;        /*!< \brief Карта иконок             */
        QMap<QString, QIcon> m_cache_icon_link;   /*!< \brief Карта публичных иконок   */
        QSet<QString>        m_cache_icon_miss;   /*!< \brief Кэш отсутствующих иконок */
#endif
};

#endif   // _ekstertera_icon_h_
