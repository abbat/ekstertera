/*!
 * \file
 * \brief Пул потоков для асинхронных задач
 */

#ifndef _ekstertera_pool_h_
#define _ekstertera_pool_h_

#include "sysheaders.h"

/*!
 * \brief Пул потоков для асинхронных задач
 */
class EteraThreadPool : public QThreadPool
{
    public:

        /*!
         * \brief Инициализация пула
         */
        static void init();

        /*!
         * \brief Освобождение ресурсов пула
         */
        static void cleanup();

        /*!
         * \brief Аналог QThreadPool::globalInstance()
         * \return Пул потоков
         */
        static QThreadPool* globalInstance();

    private:

        EteraThreadPool();
        ~EteraThreadPool();

        /*!
         * \brief Пул потоков
         */
        QThreadPool* m_pool;
};

#endif   // _ekstertera_pool_h_
