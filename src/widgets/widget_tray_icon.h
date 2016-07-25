/*!
 * \file
 * \brief Виджет иконки в трее
 */

#ifndef _ekstertera_widgets_widget_tray_icon_h_
#define _ekstertera_widgets_widget_tray_icon_h_

#include "sysheaders.h"

#ifndef ETERA_CUSTOM_TRAY_ICON
typedef QSystemTrayIcon WidgetTrayIcon;
#else

#ifdef signals
    #define __signals__
    #undef  signals
#endif

#include <gtk/gtk.h>
#include <glib-object.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#ifdef ETERA_CUSTOM_TRAY_ICON_UNITY
    #include <libappindicator/app-indicator.h>
#endif

#ifdef __signals__
    #define signals Q_SIGNALS
    #undef  __signals__
#endif

/*!
 * \brief В Qt 5.x сломана иконка трея, необходима собственная реализация
 * так же необходима реализация отдельного поведения для Ubuntu Unity
 */
class WidgetTrayIcon : public QObject
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param parent Родительский виджет
         */
        WidgetTrayIcon(QObject* parent);
        ~WidgetTrayIcon();

        /*!
         * \brief Функция перевода надписей на элементах управления
         */
        void retranslateUi();

    // QSystemTrayIcon
    public:

        void setContextMenu(QMenu* menu);   /*!< \brief QSystemTrayIcon::setContextMenu */
        void setIcon(const QIcon& icon);    /*!< \brief QSystemTrayIcon::setIcon        */
        void setVisible(bool visible);      /*!< \brief QSystemTrayIcon::setVisible     */
        bool isVisible() const;             /*!< \brief QSystemTrayIcon::isVisible      */

    signals:

        void activated(QSystemTrayIcon::ActivationReason reason);   /*!< \brief QSystemTrayIcon::activated */

    private:

        /*!
         * \brief Тип десктопа
         */
        typedef enum {
            Qt,      /*!< \brief Qt (по умолчанию) */
            Gtk,     /*!< \brief GTK (xfce, gnome) */
            Unity,   /*!< \brief Unity (ubuntu)    */
        } DesktopType;

        DesktopType m_type;   /*!< \brief Тип десктопа */

        bool m_visible;   /*!< \brief Флаг отображения иконки */

        QSystemTrayIcon* m_icon;   /*!< \brief Стандартная иконка Qt */

        /*!
         * \brief Элемент соответствия меню Qt и Gtk
         */
        typedef struct {
            QAction*     Action;     /*!< \brief Элемент меню Qt  */
            GtkMenuItem* MenuItem;   /*!< \brief Элемент меню Gtk */
        } EteraGtkActionMenuItem;

        /*!
         * \brief Список соответствий элементов меню Qt и Gtk
         */
        QList<EteraGtkActionMenuItem> m_gtk_action_menu_item_list;

        GtkWidget* m_gtk_menu;   /*!< \brief https://developer.gnome.org/gtk3/stable/GtkMenu.html */

        /*!
         * \brief Callback уничтожения буфера pixels, созданного gdk_pixbuf_new_from_data (см. gdk_pixbuf_new_from_qicon)
         */
        static void gdk_pixbuf_destroy_notify(guchar* pixels, gpointer data);

        /*!
         * \brief Преобразование иконки Qt в иконку Gtk
         * \param icon Иконка Qt
         * \return Иконка Gtk
         */
        static GdkPixbuf* gdk_pixbuf_new_from_qicon(const QIcon& icon);

        /*!
         * \brief Активация элемента меню Gtk и трансляция ее в сигнал activate для QAction
         */
        static void gtk_item_activate(GtkMenuItem* menuitem, gpointer user_data);

#ifdef ETERA_CUSTOM_TRAY_ICON_UNITY
        AppIndicator* m_app_indicator;   /*!< \brief https://developer.ubuntu.com/api/devel/ubuntu-12.04/c/appindicator/libappindicator-app-indicator.html */
#endif

#ifdef ETERA_CUSTOM_TRAY_ICON_GTK
        GtkStatusIcon* m_gtk_icon;   /*!< \brief https://developer.gnome.org/gtk3/stable/GtkStatusIcon.html */

        /*!
         * \brief Активация иконки трея Gtk и трансляция ее в сигнал activated для WidgetTrayIcon
         */
        static void gtk_icon_activate(GtkStatusIcon* status_icon, gpointer user_data);

        /*!
         * \brief Активация иконки трея Gtk средней кнопкой и трансляция ее в сигнал activated для WidgetTrayIcon
         */
        static void gtk_icon_button_release_event(GtkStatusIcon* status_icon, GdkEvent* event, gpointer user_data);

        /*!
         * \brief Отображение меню иконки трея для Gtk
         */
        static void gtk_icon_popup_menu(GtkStatusIcon* status_icon, guint button, guint activate_time, gpointer user_data);
#endif

    private slots:

        void icon_activated(QSystemTrayIcon::ActivationReason reason);   /*!< \brief QSystemTrayIcon::activated */
};
#endif   // ETERA_CUSTOM_TRAY_ICON

#endif   // _ekstertera_widgets_widget_tray_icon_h_
