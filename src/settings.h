/*!
 * \file
 * \brief Работа с настройками приложения
 */

#ifndef _ekstertera_settings_h_
#define _ekstertera_settings_h_

#include "sysheaders.h"

/*!
 * \brief Работа с настройками приложения
 */
class EteraSettings
{
    public:

        /*!
         * \brief Инициализация настроек приложения
         */
        static void init();

        /*!
         * \brief Освобождение ресурсов настроек приложения
         */
        static void cleanup();

        /*!
         * \brief Singleton
         * \return Настройки приложения
         */
        static EteraSettings* instance();

        /*!
         * \brief Перезагрузка настроек
         */
        void reload();

        /*!
         * \brief Получение текущего языка
         * \return Имя языка (ru, en, tr)
         */
        QString language() const { return m_language; }

        /*!
         * \brief Получение текущего OAuth токена
         * \return OAuth токен
         */
        QString token() const { return m_token; }

    private:

        EteraSettings();
        ~EteraSettings();

        QString m_language;   /*!< \brief Текущий язык */
        QString m_token;      /*!< \brief OAuth токен  */
};

#endif   // _ekstertera_settings_h_
