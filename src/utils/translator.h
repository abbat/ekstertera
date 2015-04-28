/*!
 * \file
 * \brief Работа с локализацией
 */

#ifndef _ekstertera_translator_h_
#define _ekstertera_translator_h_

#include "sysheaders.h"

/*!
 * \brief Транслятор для смены языка приложения
 */
class EteraTranslator
{
    public:

        /*!
         * \brief Инициализация локализации
         */
        static void init();

        /*!
         * \brief Освобождение ресурсов локализации
         */
        static void cleanup();

        /*!
         * \brief Singleton
         * \return Транслятор для смены языка
         */
        static EteraTranslator* instance();

        /*!
         * \brief Смена языка приложения
         * \return Имя языка (ru, en, tr)
         */
        void changeTranslator(const QString& language);

        /*!
         * \brief Получение текущего языка приложения
         * \return Имя языка (ru, en, tr)
         */
        QString language() const { return m_language; }

    private:

        EteraTranslator();
        ~EteraTranslator();

        QString     m_language;         /*!< \brief Текущий язык                     */
        QTranslator m_qt_translator;    /*!< \brief Транслятор стандартных сообщений */
        QTranslator m_app_translator;   /*!< \brief Транслятор приложения            */
};

#endif   // _ekstertera_translator_h_
