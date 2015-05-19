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
        QIcon addLinkIcon(QIcon base_icon);

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
};

#endif   // _ekstertera_icon_h_
