/*!
 * \file
 * \brief Работа с буфером обмена
 */

#ifndef _ekstertera_clipboard_h_
#define _ekstertera_clipboard_h_

#include "api.h"

/*!
 * \brief Работа с буфером обмена
 */
class EteraClipboard : public EteraItemList
{
    public:

        /*!
         * \brief Инициализация буфера обмена
         */
        static void init();

        /*!
         * \brief Освобождение ресурсов буфера обмена
         */
        static void cleanup();

        /*!
         * \brief Singleton
         * \return Буфер обмена
         */
        static EteraClipboard* instance();

        /*!
         * \brief Получение режима копирования
         * \return true если установлен режим копирования
         */
        bool copyMode() const { return m_copy_mode; }

        /*!
         * \brief Установка режима копирования
         */
        void setCopyMode() { m_copy_mode = true; }

        /*!
         * \brief Установка режима вырезания
         */
        void setCutMode() { m_copy_mode = false; }

        /*!
         * \brief Удаление объекта по имени пути
         * \param path Путь
         */
        void removeByPath(const QString& path);

    private:

        EteraClipboard();
        ~EteraClipboard();

        /*!
         * \brief Флаг режима копирования
         */
        bool m_copy_mode;
};

#endif   // _ekstertera_clipboard_h_
