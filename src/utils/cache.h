/*!
 * \file
 * \brief Кэш в памяти
 */

#ifndef _ekstertera_cache_h_
#define _ekstertera_cache_h_

#include "sysheaders.h"

/*!
 * \brief Кэш в памяти
 */
class EteraMemoryCache
{
    public:

        /*!
         * \brief Инициализация кэша
         */
        static void init();

        /*!
         * \brief Освобождение ресурсов кэша
         */
        static void cleanup();

        /*!
         * \brief Кэш в памяти для сетевых ресурсов
         * \return Кэш в памяти для сетевых ресурсов
         */
        static EteraMemoryCache* instance();

        /*!
         * \brief Очистка кэша
         */
        void clear();

        /*!
         * \brief Подготовка элемента кэша
         * \param url Ключ
         * \return Устройство для записи данных или NULL если ключ уже существует
         */
        QIODevice* prepare(const QUrl& url);

        /*!
         * \brief Удаление элемента кэша
         * \param url Ключ
         */
        void remove(const QUrl& url);

        /*!
         * \brief Пометка элемента как записанного
         * \param url Ключ
         */
        void insert(const QUrl& url);

    private:

        EteraMemoryCache();
        ~EteraMemoryCache();

        /*!
         * \brief Элемент кэша
         */
        typedef struct {
            QBuffer Device;   /*!< \brief Буфер                       */
            bool    Valid;    /*!< \brief Флаг завершенности загрузки */
        } EteraCacheItem;

        /*!
         * \brief Хэш таблица для кэша
         */
        typedef QHash<QUrl, EteraCacheItem*> EteraCacheHash;

        /*!
         * \brief Кэш
         */
        EteraCacheHash m_cache;

        /*!
         * \brief Блокировка
         */
        QMutex m_lock;
};

#endif   // _ekstertera_cache_h_
