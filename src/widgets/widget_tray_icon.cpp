#include "widget_tray_icon.h"
//----------------------------------------------------------------------------------------------
#ifdef ETERA_CUSTOM_TRAY_ICON
//----------------------------------------------------------------------------------------------

WidgetTrayIcon::WidgetTrayIcon(QObject* parent) : QObject(parent)
{
    m_visible  = false;
    m_icon     = NULL;
    m_gtk_menu = NULL;

#ifdef ETERA_CUSTOM_TRAY_ICON_UNITY
    m_app_indicator = NULL;
#endif

#ifdef ETERA_CUSTOM_TRAY_ICON_GTK
    m_gtk_icon = NULL;
#endif

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString desktop = env.value("XDG_CURRENT_DESKTOP", "").toLower();
    if (desktop.isEmpty() == true)
        desktop = env.value("DESKTOP_SESSION", "").toLower();

#ifdef ETERA_CUSTOM_TRAY_ICON_UNITY
    if (desktop == "unity")
        m_type = Unity;
    else
#endif
#ifdef ETERA_CUSTOM_TRAY_ICON_GTK
    if (desktop == "xfce" || desktop.contains("gnome") || desktop == "mate" || desktop == "x-cinnamon")
        m_type = Gtk;
    else
#endif
        m_type = Qt;

#ifdef ETERA_CUSTOM_TRAY_ICON_UNITY
    if (m_type == Unity) {
        if (gtk_init_check(NULL, NULL) == TRUE) {
            QString icon_path = QDir::homePath() + "/.local/share/icons";

            QDir dir;
            if (dir.exists(icon_path) == false && dir.mkpath(icon_path) == false)
                m_type = Qt;

            if (m_type != Qt) {
                m_app_indicator = app_indicator_new_with_path(ETERA_APP_NAME, ETERA_APP_NAME, APP_INDICATOR_CATEGORY_APPLICATION_STATUS, icon_path.toUtf8().constData());
                if (m_app_indicator == NULL)
                    m_type = Qt;
            }
        } else
            m_type = Qt;
    } else
#endif
#ifdef ETERA_CUSTOM_TRAY_ICON_GTK
    if (m_type == Gtk) {
        if (gtk_init_check(NULL, NULL) == TRUE) {
            m_gtk_icon = gtk_status_icon_new();
            if (m_gtk_icon != NULL) {
                m_visible = true;

                g_signal_connect(m_gtk_icon, "activate",             G_CALLBACK(gtk_icon_activate),             this);
                g_signal_connect(m_gtk_icon, "button-release-event", G_CALLBACK(gtk_icon_button_release_event), this);
                g_signal_connect(m_gtk_icon, "popup-menu",           G_CALLBACK(gtk_icon_popup_menu),           this);
            } else
                m_type = Qt;
        } else
            m_type = Qt;
    }
#endif

    if (m_type == Qt) {
        m_icon = new QSystemTrayIcon(this);
        connect(m_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(icon_activated(QSystemTrayIcon::ActivationReason)));
    }
}
//----------------------------------------------------------------------------------------------

WidgetTrayIcon::~WidgetTrayIcon()
{
    if (m_gtk_menu != NULL)
        gtk_widget_destroy(m_gtk_menu);

#ifdef ETERA_CUSTOM_TRAY_ICON_UNITY
    if (m_app_indicator != NULL)
        g_object_unref(G_OBJECT(m_app_indicator));
#endif

#ifdef ETERA_CUSTOM_TRAY_ICON_GTK
    if (m_gtk_icon !=  NULL)
        g_object_unref(m_gtk_icon);
#endif
}
//----------------------------------------------------------------------------------------------

void WidgetTrayIcon::retranslateUi()
{
    if (m_type == Gtk || m_type == Unity)
        for (int i = 0; i < m_gtk_action_menu_item_list.count(); i++) {
            const EteraGtkActionMenuItem& action_menu_item = m_gtk_action_menu_item_list[i];
            gtk_menu_item_set_label(action_menu_item.MenuItem, action_menu_item.Action->text().toUtf8().constData());
        }
}
//----------------------------------------------------------------------------------------------

bool WidgetTrayIcon::isVisible() const
{
    return m_visible;
}
//----------------------------------------------------------------------------------------------

void WidgetTrayIcon::icon_activated(QSystemTrayIcon::ActivationReason reason)
{
    emit activated(reason);
}
//----------------------------------------------------------------------------------------------

void WidgetTrayIcon::setContextMenu(QMenu* menu)
{
    if (m_type == Gtk || m_type == Unity) {
        m_gtk_action_menu_item_list.clear();

        if (m_gtk_menu != NULL)
            gtk_widget_destroy(m_gtk_menu);

        m_gtk_menu = gtk_menu_new();

        if (m_gtk_menu != NULL) {
            QList<QAction*> actions = menu->actions();
            for (int i = 0; i < actions.count(); i++) {
                QAction* action = actions[i];

                GtkWidget* item;
                if (action->isSeparator() == true)
                    item = gtk_separator_menu_item_new();
                else if (action->icon().isNull() == true)
                    item = gtk_menu_item_new_with_label(action->text().toUtf8().constData());
                else {
                    item = gtk_image_menu_item_new_with_label(action->text().toUtf8().constData());

                    if (item != NULL) {
                        GdkPixbuf* pixbuf = gdk_pixbuf_new_from_qicon(action->icon());
                        if (pixbuf != NULL) {
                            gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item), gtk_image_new_from_pixbuf(pixbuf));
                            g_object_unref(pixbuf);
                        }
                    }
                }

                if (item != NULL) {
                    if (action->isSeparator() == false) {
                        EteraGtkActionMenuItem action_menu_item;
                        action_menu_item.Action   = action;
                        action_menu_item.MenuItem = GTK_MENU_ITEM(item);
                        m_gtk_action_menu_item_list.append(action_menu_item);

                        g_signal_connect(item, "activate", G_CALLBACK(gtk_item_activate), action);
                    }

                    gtk_menu_shell_append(GTK_MENU_SHELL(m_gtk_menu), item);

                    gtk_widget_show(item);
                }
            }

#ifdef ETERA_CUSTOM_TRAY_ICON_UNITY
            if (m_type == Unity)
                app_indicator_set_menu(m_app_indicator, GTK_MENU(m_gtk_menu));
#endif
        }
    } else if (m_type == Qt)
        m_icon->setContextMenu(menu);
}
//----------------------------------------------------------------------------------------------

void WidgetTrayIcon::setIcon(const QIcon& icon)
{
#ifdef ETERA_CUSTOM_TRAY_ICON_UNITY
    if (m_type == Unity) {
        QString icon_path = QDir::homePath() + QString("/.local/share/icons/%1.png").arg(ETERA_APP_NAME);
        QSize size = icon.actualSize(QSize(256, 256));
        icon.pixmap(size).save(icon_path);
    } else
#endif
#ifdef ETERA_CUSTOM_TRAY_ICON_GTK
    if (m_type == Gtk) {
        GdkPixbuf* pixbuf = gdk_pixbuf_new_from_qicon(icon);
        if (pixbuf != NULL) {
            gtk_status_icon_set_from_pixbuf(m_gtk_icon, pixbuf);
            g_object_unref(pixbuf);
        }
    } else
#endif
    if (m_type == Qt)
        m_icon->setIcon(icon);
}
//----------------------------------------------------------------------------------------------

void WidgetTrayIcon::setVisible(bool visible)
{
    if (m_visible == visible)
        return;

    m_visible = visible;

#ifdef ETERA_CUSTOM_TRAY_ICON_UNITY
    if (m_type == Unity)
        app_indicator_set_status(m_app_indicator, (m_visible == true ? APP_INDICATOR_STATUS_ACTIVE : APP_INDICATOR_STATUS_PASSIVE));
    else
#endif
#ifdef ETERA_CUSTOM_TRAY_ICON_GTK
    if (m_type == Gtk)
        gtk_status_icon_set_visible(m_gtk_icon, m_visible);
    else
#endif
    if (m_type == Qt)
        m_icon->setVisible(m_visible);
}
//----------------------------------------------------------------------------------------------

void WidgetTrayIcon::gdk_pixbuf_destroy_notify(guchar* pixels, gpointer /*data*/)
{
    delete[] pixels;
}
//----------------------------------------------------------------------------------------------

GdkPixbuf* WidgetTrayIcon::gdk_pixbuf_new_from_qicon(const QIcon& icon)
{
    // TODO: Получить актуальный размер
    QImage image = icon.pixmap(16, 16).toImage();

#if QT_VERSION >= 0x050000
    if (image.format() != QImage::Format_RGBA8888_Premultiplied)
        image = image.convertToFormat(QImage::Format_RGBA8888_Premultiplied);
#else
    if (image.format() != QImage::Format_ARGB32_Premultiplied)
        image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
#endif

    guchar* buf = new guchar[image.byteCount()];

    memcpy(buf, image.bits(), image.byteCount());

#if QT_VERSION < 0x050000
    // BGRA -> RGBA
    for (int i = 0; i < image.byteCount(); i += 4)
        qSwap(buf[i], buf[i + 2]);
#endif

    return gdk_pixbuf_new_from_data(buf, GDK_COLORSPACE_RGB, (image.hasAlphaChannel() == true ? TRUE : FALSE), 8, image.width(), image.height(), image.bytesPerLine(), gdk_pixbuf_destroy_notify, NULL);
}
//----------------------------------------------------------------------------------------------

void WidgetTrayIcon::gtk_item_activate(GtkMenuItem* /*menuitem*/, gpointer user_data)
{
    static_cast<QAction*>(user_data)->activate(QAction::Trigger);
}
//----------------------------------------------------------------------------------------------

#ifdef ETERA_CUSTOM_TRAY_ICON_GTK
void WidgetTrayIcon::gtk_icon_activate(GtkStatusIcon* /*status_icon*/, gpointer user_data)
{
    static_cast<WidgetTrayIcon*>(user_data)->activated(QSystemTrayIcon::Trigger);
}
#endif
//----------------------------------------------------------------------------------------------
#ifdef ETERA_CUSTOM_TRAY_ICON_GTK
void WidgetTrayIcon::gtk_icon_button_release_event(GtkStatusIcon* /*status_icon*/, GdkEvent* event, gpointer user_data)
{
    if (event->button.button == 2)
        static_cast<WidgetTrayIcon*>(user_data)->activated(QSystemTrayIcon::MiddleClick);
}
#endif
//----------------------------------------------------------------------------------------------
#ifdef ETERA_CUSTOM_TRAY_ICON_GTK
void WidgetTrayIcon::gtk_icon_popup_menu(GtkStatusIcon* /*status_icon*/, guint button, guint activate_time, gpointer user_data)
{
    WidgetTrayIcon* self = static_cast<WidgetTrayIcon*>(user_data);
    if (self->m_gtk_menu != NULL)
        gtk_menu_popup(GTK_MENU(self->m_gtk_menu), NULL, NULL, NULL, NULL, button, activate_time);
}
#endif
//----------------------------------------------------------------------------------------------
#endif   // ETERA_CUSTOM_TRAY_ICON
//----------------------------------------------------------------------------------------------
