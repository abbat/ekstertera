/*!
 * \file
 * \brief Виджет прогресс-бара
 */

#ifndef _ekstertera_widgets_widget_progress_h_
#define _ekstertera_widgets_widget_progress_h_

#include "sysheaders.h"

/*!
 * \brief Виджет прогресс-бара с поддержкой qint64
 */
class WidgetProgressbar : public QWidget
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param parent Родительский виджет
         */
        WidgetProgressbar(QWidget* parent = NULL);
        ~WidgetProgressbar();

        //
        // слоты QProgressBar для подержки qint64
        //

        void setMaximum(qint64 maximum);
        void setValue(qint64 value);
        void reset();

    private:

        int       m_shift;     /*!< \brief Битовый сдвиг до необходимого значения */
        qint64    m_maximum;   /*!< \brief Максимальное значение                  */
        qint64    m_value;     /*!< \brief Текущее значение                       */
        QDateTime m_start;     /*!< \brief Время начала работы                    */
        qint64    m_elapsed;   /*!< \brief Количество секунд с начала работы      */

        /*!
         * \brief Получение номера старшего значимого бита (обратная ffs)
         * \param x Анализируемое значение
         * \return Старший значимый бит (счет с 1) или 0 для 0
         */
        static int msb(unsigned int x);

        /*!
         * \brief Получение номера старшего значимого бита (обратная ffsll)
         * \param x Анализируемое значение
         * \return Старший значимый бит (счет с 1) или 0 для 0
         */
        static int msbll(unsigned long long x);

        /*!
         * \brief Преобразование qint64 в int
         * \param Исходное значение
         * \return Результат
         */
        inline int shift(qint64 x) const { return (x > 0 ? (int)(x >> m_shift) : -1); }

        /*!
         * \brief Обновление текста прогресса операции
         */
        void updateProgressText();

        /*!
         * \brief Форматирование секунд в hh:mm:ss
         * \param secinds Количество секунд
         * \return Форматированная строка
         */
        static QString formatTime(qint64 seconds);

        QHBoxLayout*  m_layout;   /*!< \brief Layout для прогресса */
        QProgressBar*   m_bar;    /*!< \brief Прогресс             */
};

#endif   // _ekstertera_widgets_widget_progress_h_
