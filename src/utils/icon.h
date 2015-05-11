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
         * \brief Провайдер стандартных иконок
         */
        QCommonStyle m_style;

        /*!
         * \brief Получение иконки стандартной иконки, аналог QStyle::standardIcon
         * Необходим для Windows, где нет тем и могут подставляться иконки малого размера
         * \param pixmap Имя стандартной иконки
         * \return Иконка
         */
        QIcon standardIcon(QStyle::StandardPixmap pixmap);

#ifdef Q_WS_WIN
        /*!
         * \brief Получение иконки стандартной иконки из темы Tango
         * \param pixmap Имя стандартной иконки
         * \return Иконка
         */
        QIcon tangoIcon(QStyle::StandardPixmap pixmap);

        /*!
         * \brief Добавление к стандартной иконке символа симлинка
         * \param base Имя стандартной иконки в ресурсах
         * \return Иконка
         */
        QIcon tangoAddLink(const QString& base);
#endif
};

#endif   // _ekstertera_icon_h_
