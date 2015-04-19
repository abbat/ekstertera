/*!
 * \file
 * \brief Виджет отображения текущего пути на Яндекс.Диск
 */

#ifndef _ekstertera_widgets_widget_disk_path_h_
#define _ekstertera_widgets_widget_disk_path_h_

#include "sysheaders.h"

/*!
 * \brief Виджет отображения текущего пути на Яндекс.Диск
 */
class WidgetDiskPath : public QWidget
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param parent Родительский виджет
         */
        WidgetDiskPath(QWidget* parent);
        ~WidgetDiskPath();

        /*!
         * \brief Сменить отображаемый путь
         * \param path Путь
         */
        void changePath(const QString& path);

    private:

        /*!
         * \brief Список кнопок
         */
        QList<QToolButton*> m_buttons;

    public slots:

        void button_clicked();   /*!< \brief Нажатие на кнопку пути */

    signals:

        /*!
         * \brief Сигнал необходимости смены пути
         */
        void onPathChangeRequest(const QString& path);
};

#endif   // _ekstertera_widgets_widget_disk_path_h_
