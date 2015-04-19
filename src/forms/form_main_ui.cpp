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

    //
    // меню
    //

    m_menubar = new QMenuBar(this);

    // верхний ряд меню
    m_menu_file = m_menubar->addMenu("");

    //
    // меню "Файл"
    //

    // Файл / Настройки
    m_menu_file_settings = m_menu_file->addAction(QIcon::fromTheme("preferences-system", QIcon(":/icons/settings16.png")), "");

    m_menu_file->addSeparator();

    // Файл / Выход
    m_menu_file_exit = m_menu_file->addAction(QIcon::fromTheme("application-exit", QIcon(":/icons/exit16.png")), "");
    m_menu_file_exit->setMenuRole(QAction::QuitRole);

    setMenuBar(m_menubar);

    //
    // тулбар
    //

    m_toolbar = addToolBar("");
    m_toolbar->setFloatable(false);

    m_action_upload = m_toolbar->addAction(QIcon(":/icons/upload32.png"), "");
    m_action_upload->setEnabled(false);

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

    m_label_operation = new QLabel(m_status_bar);
    m_status_bar->addWidget(m_label_operation);

    m_label_used = new QLabel(m_status_bar);
    m_status_bar->addWidget(m_label_used);

    m_label_total = new QLabel(m_status_bar);
    m_status_bar->addWidget(m_label_total);

    m_progress = new QProgressBar(m_status_bar);
    m_progress->setVisible(false);
    m_status_bar->addPermanentWidget(m_progress, 1);

    setStatusBar(m_status_bar);

    // локализация
    retranslateUi();

    // восстановление layout
    restore();
}
//----------------------------------------------------------------------------------------------

void FormMainUI::closeEvent(QCloseEvent* /*event*/)
{
    save();
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

    m_menu_file->setTitle(trUtf8("Файл"));

    m_menu_file_settings->setText(trUtf8("Настройки"));
    m_menu_file_exit->setText(trUtf8("Выход"));

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
