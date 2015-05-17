#include "form_main_ui.h"
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
    // меню
    //

    m_menubar = new QMenuBar(this);

    // верхний ряд меню
    m_menu_file = m_menubar->addMenu("");
    m_menu_help = m_menubar->addMenu("");

    //
    // меню "Файл"
    //

    // Файл / Настройки
    m_menu_file_settings = m_menu_file->addAction(QIcon::fromTheme("preferences-system", QIcon(":/icons/tango/preferences-system.svg")), "");

    m_menu_file->addSeparator();

    // Файл / Выход
    m_menu_file_exit = m_menu_file->addAction(QIcon::fromTheme("application-exit", QIcon(":/icons/tango/application-exit.svg")), "");
    m_menu_file_exit->setMenuRole(QAction::QuitRole);

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

    m_action_upload = m_toolbar->addAction(QIcon(":/icons/upload32.png"), "");

    m_action_download = m_toolbar->addAction(QIcon(":/icons/download32.png"), "");
    m_action_download->setEnabled(false);

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
    if (m_tray_icon->isVisible() == true) {
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

    m_menu_file_settings->setText(trUtf8("Настройки"));
    m_menu_file_exit->setText(trUtf8("Выход"));

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
}
//----------------------------------------------------------------------------------------------

void FormMainUI::restore()
{
    QSettings settings;
    restoreGeometry(settings.value("layout/main").toByteArray());
}
//----------------------------------------------------------------------------------------------
