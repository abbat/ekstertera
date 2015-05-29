#include "form_main_ui.h"
//----------------------------------------------------------------------------------------------
#include "utils/settings.h"
//----------------------------------------------------------------------------------------------

FormMainUI::FormMainUI() : QMainWindow()
{
    QIcon icon;
    icon.addFile(":/icons/main16.png",  QSize(16, 16));
    icon.addFile(":/icons/main32.png",  QSize(32, 32));
    icon.addFile(":/icons/main48.png",  QSize(48, 48));
    icon.addFile(":/icons/main64.png",  QSize(64, 64));
    icon.addFile(":/icons/main128.png", QSize(128, 128));
    setWindowIcon(icon);

    setMinimumSize(280, 280);

    //
    // центральный виджет
    //

    m_widget_central = new QWidget(this);
    m_layout_central = new QVBoxLayout(m_widget_central);

    m_widget_path = new WidgetDiskPath(m_widget_central);
    m_layout_central->addWidget(m_widget_path);

    m_widget_disk = new WidgetDisk(m_widget_central);
    m_layout_central->addWidget(m_widget_disk);

    setCentralWidget(m_widget_central);

    //
    // меню
    //

    m_menubar = new QMenuBar(this);

    // верхний ряд меню
    m_menu_file = m_menubar->addMenu("");
    m_menu_edit = m_menubar->addMenu("");
    m_menu_view = m_menubar->addMenu("");
    m_menu_help = m_menubar->addMenu("");

    //
    // меню "Файл"
    //

    // Файл / Выход
    m_menu_file_exit = m_menu_file->addAction(QIcon::fromTheme("application-exit", QIcon(":/icons/tango/application-exit.svg")), "");
    m_menu_file_exit->setShortcut(QKeySequence(QKeySequence::Quit));
    m_menu_file_exit->setMenuRole(QAction::QuitRole);

    //
    // Меню "Правка"
    //

    m_menu_edit->addActions(m_widget_disk->contextMenuActions());

    m_menu_edit->addSeparator();

    m_menu_edit_settings = m_menu_edit->addAction(QIcon::fromTheme("preferences-system", QIcon(":/icons/tango/preferences-system.svg")), "");
    m_menu_edit_settings->setShortcut(QKeySequence(QKeySequence::Preferences));

    //
    // Меню "Вид"
    //

    m_menu_view_refresh = m_menu_view->addAction(QIcon::fromTheme("view-refresh", QIcon(":/icons/tango/view-refresh.svg")), "");
    m_menu_view_refresh->setShortcut(QKeySequence(QKeySequence::Refresh));

    m_menu_view->addSeparator();

    m_menu_view_zoom_in  = m_menu_view->addAction(QIcon::fromTheme("zoom-in", QIcon(":/icons/gnome/zoom-in32.png")), "");
    m_menu_view_zoom_in->setShortcut(QKeySequence(QKeySequence::ZoomIn));

    m_menu_view_zoom_out = m_menu_view->addAction(QIcon::fromTheme("zoom-out", QIcon(":/icons/gnome/zoom-out32.png")), "");
    m_menu_view_zoom_out->setShortcut(QKeySequence(QKeySequence::ZoomOut));

    //
    // меню "?"
    //

    m_menu_about = m_menu_help->addAction(QIcon::fromTheme("help-about", QIcon(":/icons/gnome/help-about.svg")), "");
    m_menu_about_qt = m_menu_help->addAction("");

    setMenuBar(m_menubar);

    //
    // тулбар
    //

    m_toolbar = addToolBar("");
    m_toolbar->setFloatable(false);

    m_toolbar->addAction(m_menu_view_refresh);

    m_action_upload = m_toolbar->addAction(QIcon(":/icons/upload32.png"), "");

    m_action_download = m_toolbar->addAction(QIcon(":/icons/download32.png"), "");
    m_action_download->setEnabled(false);

    m_toolbar->addAction(m_menu_view_zoom_out);
    m_toolbar->addAction(m_menu_view_zoom_in);

    //
    // статусбар
    //

    m_status_bar = new QStatusBar(this);

    m_label_used = new QLabel(m_status_bar);
    m_status_bar->addWidget(m_label_used);

    m_label_total = new QLabel(m_status_bar);
    m_status_bar->addWidget(m_label_total);

    setStatusBar(m_status_bar);

    // меню трея
    m_tray_menu = new QMenu(this);
    m_tray_menu->addAction(m_menu_file_exit);

    // иконка трея
    m_tray_icon = new QSystemTrayIcon(this);
    m_tray_icon->setContextMenu(m_tray_menu);
    m_tray_icon->setIcon(icon);
    m_tray_icon->setVisible(true);

    // локализация
    retranslateUi();

    // восстановление layout
    restore();
}
//----------------------------------------------------------------------------------------------

void FormMainUI::closeEvent(QCloseEvent* event)
{
    // сохранение layout
    if (m_tray_icon->isVisible() == true && EteraSettings::instance()->closeToTray() == true) {
        hide();
        event->ignore();
    } else {
        save();
        event->accept();
    }
}
//----------------------------------------------------------------------------------------------

void FormMainUI::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslateUi();
    else
        QWidget::changeEvent(event);
}
//----------------------------------------------------------------------------------------------

void FormMainUI::retranslateUi()
{
    setWindowTitle(trUtf8("Ekstertera"));

    m_menu_file->setTitle(trUtf8("&Файл"));

    m_menu_file_exit->setText(trUtf8("Выход"));

    m_menu_edit->setTitle(trUtf8("&Правка"));

    m_menu_edit_settings->setText(trUtf8("Параметры"));

    m_menu_view->setTitle(trUtf8("&Вид"));

    m_menu_view_refresh->setText(trUtf8("Обновить"));
    m_menu_view_zoom_in->setText(trUtf8("Увеличить"));
    m_menu_view_zoom_out->setText(trUtf8("Уменьшить"));

    m_menu_help->setTitle(trUtf8("&?"));

    m_menu_about->setText(trUtf8("О программе"));
    m_menu_about_qt->setText(trUtf8("О Qt"));

    m_toolbar->setWindowTitle(trUtf8("Панель инструментов"));

    m_action_upload->setText(trUtf8("Загрузить на Яндекс.Диск"));
    m_action_download->setText(trUtf8("Загрузить с Яндекс.Диска"));

    m_label_used->setToolTip(trUtf8("Используется"));
    m_label_total->setToolTip(trUtf8("Всего"));
}
//----------------------------------------------------------------------------------------------

void FormMainUI::save()
{
    QSettings settings;
    settings.setValue("layout/main", this->saveGeometry());
    settings.setValue("app/zoom", m_widget_disk->zoomFactor());
}
//----------------------------------------------------------------------------------------------

void FormMainUI::restore()
{
    QSettings settings;

    restoreGeometry(settings.value("layout/main").toByteArray());

    int zoom = m_widget_disk->setZoomFactor(settings.value("app/zoom", -1).toInt());
    if (zoom < 0)
        m_menu_view_zoom_out->setEnabled(false);
    else if (zoom > 0)
        m_menu_view_zoom_in->setEnabled(false);
}
//----------------------------------------------------------------------------------------------
