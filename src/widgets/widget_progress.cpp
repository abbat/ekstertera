#include "widget_progress.h"
//----------------------------------------------------------------------------------------------
/*!
 * \brief Получение номера старшего значимого бита (обратная ffs)
 * \param x Анализируемое значение
 * \return Старший значимый бит (счет с 1) или 0 для 0
 */
int msb(unsigned int x)
{
    int ret = sizeof(unsigned int) * CHAR_BIT;
    return x ? ret - __builtin_clz(x) : ret;
}
//----------------------------------------------------------------------------------------------
/*!
 * \brief Получение номера старшего значимого бита (обратная ffsll)
 * \param x Анализируемое значение
 * \return Старший значимый бит (счет с 1) или 0 для 0
 */
int msbll(unsigned long long x)
{
    int ret = sizeof(unsigned long long) * CHAR_BIT;
    return x ? ret - __builtin_clzll(x) : ret;
}
//----------------------------------------------------------------------------------------------

WidgetProgressbar::WidgetProgressbar(QWidget* parent) : QProgressBar(parent)
{
    m_shift   = 0;
    m_maximum = 0;
    m_minimum = 0;
    m_value   = 0;
}
//----------------------------------------------------------------------------------------------

WidgetProgressbar::~WidgetProgressbar()
{
}
//----------------------------------------------------------------------------------------------

void WidgetProgressbar::updateShift()
{
    qint64 val = qMax(m_minimum, m_maximum);

    if (val > (qint64)INT_MAX)
        m_shift = msbll(val) - msb(INT_MAX);
    else
        m_shift = 0;
}
//----------------------------------------------------------------------------------------------

void WidgetProgressbar::setMaximum(qint64 maximum)
{
    if (maximum == m_maximum)
        return;

    m_maximum = maximum;

    updateShift();

    QProgressBar::setRange(m_minimum >> m_shift, m_maximum >> m_shift);
}
//----------------------------------------------------------------------------------------------

void WidgetProgressbar::setMinimum(qint64 minimum)
{
    if (minimum == m_minimum)
        return;

    m_minimum = minimum;

    updateShift();

    QProgressBar::setRange(m_minimum >> m_shift, m_maximum >> m_shift);
}
//----------------------------------------------------------------------------------------------

void WidgetProgressbar::setRange(qint64 minimum, qint64 maximum)
{
    if (minimum == m_minimum && maximum == m_maximum)
        return;

    m_minimum = minimum;
    m_maximum = maximum;

    updateShift();

    QProgressBar::setRange(m_minimum >> m_shift, m_maximum >> m_shift);
}
//----------------------------------------------------------------------------------------------

void WidgetProgressbar::setValue(qint64 value)
{
    if (m_value == value)
        return;

    m_value = value;

    QProgressBar::setValue(m_value >> m_shift);
}
//----------------------------------------------------------------------------------------------
