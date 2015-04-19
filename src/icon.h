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
         * \brief Получение станадтной иконки
         * \param pixmap Тип иконки
         * \return Иконка для запрошенного типа
         */
        QIcon icon(QStyle::StandardPixmap pixmap) { return m_style.standardIcon(pixmap); }

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
};

#endif   // _ekstertera_icon_h_
