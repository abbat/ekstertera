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
class WidgetProgressbar : public QProgressBar
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param parent Родительский виджет
         */
        WidgetProgressbar(QWidget* parent = NULL);
        ~WidgetProgressbar();

    private:

        int    m_shift;     /*!< \brief Битовый сдвиг до необходимого значения */
        qint64 m_maximum;   /*!< \brief Максимальное значение                  */
        qint64 m_minimum;   /*!< \brief Минимальное значение                   */
        qint64 m_value;     /*!< \brief Текущее значение                       */

        /*!
         * \brief Обновление значения сдвига на основании значений m_minimum / m_maximum
         */
        void updateShift();

    // перегрузка слотов QProgressBar для подержки qint64
    public slots:

        void setMaximum(qint64 maximum);
        void setMinimum(qint64 minimum);
        void setRange(qint64 minimum, qint64 maximum);
        void setValue(qint64 value);
};

#endif   // _ekstertera_widgets_widget_progress_h_
