#include "widget_progress.h"
//----------------------------------------------------------------------------------------------
#include "utils/api.h"
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

WidgetProgressbar::WidgetProgressbar(QWidget* parent) : QWidget(parent)
{
    m_shift   = 0;
    m_maximum = 0;
    m_value   = 0;
    m_elapsed = 0;

    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(3, 0, 3, 0);

    m_bar = new QProgressBar(this);
    m_bar->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_bar->setMinimum(0);
    m_layout->addWidget(m_bar, Qt::AlignJustify);

    setVisible(true);

    m_start = QDateTime::currentDateTime();
}
//----------------------------------------------------------------------------------------------

WidgetProgressbar::~WidgetProgressbar()
{
}
//----------------------------------------------------------------------------------------------

void WidgetProgressbar::updateProgressText()
{
    qint64 elapsed = m_start.secsTo(QDateTime::currentDateTime());

    if (elapsed - m_elapsed < 3)
        return;

    QString text = "%p% ";

    if (m_maximum > 0 && m_value >= 0) {
        if (m_value == 0)
            text += QString("%1").arg("--:--:--");
        else {
            qint64 eta = (qint64)((double)elapsed * m_maximum / m_value) - elapsed;
            qint64 bps = m_value / elapsed;

            text += QString("%1 (%2)").arg(formatTime(eta)).arg(EteraAPI::humanSpeed(bps));
        }
    } else
        text = "";

    m_elapsed = elapsed;

    m_bar->setFormat(text);
}
//----------------------------------------------------------------------------------------------

QString WidgetProgressbar::formatTime(qint64 seconds)
{
    qint64 h = seconds / 60 / 60;
    seconds -= h * 60 * 60;
    qint64 m = seconds / 60;
    seconds -= m * 60;
    qint64 s = seconds;

    QString result;
    if (h < 10)
        result += QString("0%1:").arg(h);
    else
        result += QString("%1:").arg(h);

    if (h < 10)
        result += QString("0%1:").arg(m);
    else
        result += QString("%1:").arg(m);

    if (s < 10)
        result += QString("0%1").arg(s);
    else
        result += QString("%1").arg(s);

    return result;
}
//----------------------------------------------------------------------------------------------

void WidgetProgressbar::setMaximum(qint64 maximum)
{
    if (maximum == m_maximum)
        return;

    m_maximum = maximum;

    if (m_maximum > (qint64)INT_MAX)
        m_shift = msbll(m_maximum) - msb(INT_MAX);
    else
        m_shift = 0;

    m_bar->setMaximum(shift(m_maximum));

    updateProgressText();
}
//----------------------------------------------------------------------------------------------

void WidgetProgressbar::setValue(qint64 value)
{
    if (m_value == value)
        return;

    m_value = value;

    m_bar->setValue(shift(m_value));

    updateProgressText();
}
//----------------------------------------------------------------------------------------------
