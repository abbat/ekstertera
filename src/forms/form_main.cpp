#include "form_main.h"
//----------------------------------------------------------------------------------------------
#include "utils/pool.h"
#include "utils/settings.h"
//----------------------------------------------------------------------------------------------
#include "form_settings.h"
//----------------------------------------------------------------------------------------------

FormMain::FormMain() : FormMainUI()
{
    // меню "Файл"
    connect(m_menu_file_exit, SIGNAL(triggered()), this, SLOT(menu_file_exit_triggered()));

    // меню "Параметры"
    connect(m_menu_edit_settings, SIGNAL(triggered()), this, SLOT(menu_edit_settings_triggered()));

    // меню "Вид"
    connect(m_menu_view_refresh,  SIGNAL(triggered()), this, SLOT(menu_view_refresh_triggered()));
    connect(m_menu_view_zoom_in,  SIGNAL(triggered()), this, SLOT(menu_view_zoom_in_triggered()));
    connect(m_menu_view_zoom_out, SIGNAL(triggered()), this, SLOT(menu_view_zoom_out_triggered()));
    connect(m_menu_view_preview,  SIGNAL(changed()),   this, SLOT(menu_view_preview_changed()));

    // меню "?"
    connect(m_menu_about,    SIGNAL(triggered()), this, SLOT(menu_help_about_triggered()));
    connect(m_menu_about_qt, SIGNAL(triggered()), this, SLOT(menu_help_about_qt_triggered()));

    // иконка трея
    connect(m_tray_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(tray_icon_activated(QSystemTrayIcon::ActivationReason)));

    // тулбар
    connect(m_action_upload,   SIGNAL(triggered()), this, SLOT(action_upload_triggered()));
    connect(m_action_download, SIGNAL(triggered()), this, SLOT(action_download_triggered()));

    // виджет пути
    connect(m_widget_path, SIGNAL(onPathChangeRequest(const QString&)), this, SLOT(widget_path_on_path_change_request(const QString&)));

    // виджет диска
    connect(m_widget_disk, SIGNAL(onPathChanged(const QString&)),   this, SLOT(widget_disk_on_path_changed(const QString&)));
    connect(m_widget_disk, SIGNAL(onChangePossibleActions(bool)),   this, SLOT(widget_disk_on_change_possible_actions(bool)));

    // обновление статуса
    updateInfoStatus();

    // получение корня диска
    m_widget_disk->changePath("disk:/");
}
//----------------------------------------------------------------------------------------------

FormMain::~FormMain()
{
}
//----------------------------------------------------------------------------------------------

void FormMain::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        updateInfoStatus();
        m_widget_disk->retranslateUi();
    }

    FormMainUI::changeEvent(event);
}
//----------------------------------------------------------------------------------------------

void FormMain::menu_file_exit_triggered()
{
    m_tray_icon->setVisible(false);
    close();
}
//----------------------------------------------------------------------------------------------

void FormMain::menu_edit_settings_triggered()
{
    QString token = EteraSettings::instance()->token();

    FormSettings form(this);
    if (form.exec() == QDialog::Accepted) {
        if (token != EteraSettings::instance()->token()) {
            updateInfoStatus();
            m_widget_disk->changePath("disk:/");
        }
    }
}
//----------------------------------------------------------------------------------------------

void FormMain::menu_view_refresh_triggered()
{
    m_widget_disk->changePath(m_widget_disk->path());
}
//----------------------------------------------------------------------------------------------

void FormMain::menu_view_zoom_in_triggered()
{
    bool result = m_widget_disk->zoomIn();

    if (result == false)
        m_menu_view_zoom_in->setEnabled(false);

    m_menu_view_zoom_out->setEnabled(true);
}
//----------------------------------------------------------------------------------------------

void FormMain::menu_view_zoom_out_triggered()
{
    bool result = m_widget_disk->zoomOut();

    if (result == false)
        m_menu_view_zoom_out->setEnabled(false);

    m_menu_view_zoom_in->setEnabled(true);
}
//----------------------------------------------------------------------------------------------

void FormMain::menu_view_preview_changed()
{
    m_widget_disk->setPreviewMode(m_menu_view_preview->isChecked());
}
//----------------------------------------------------------------------------------------------

void FormMain::menu_help_about_triggered()
{
    QString text;
    text += QString("<b>Ekstertera v%1</b><br><br>").arg(ETERA_VERSION);
    text += QString("<a href=\"%1\">%1</a><br>").arg("https://github.com/abbat/ekstertera");

    QMessageBox::about(this, trUtf8("О программе"), text);
}
//----------------------------------------------------------------------------------------------

void FormMain::menu_help_about_qt_triggered()
{
    QMessageBox::aboutQt(this, trUtf8("О Qt"));
}
//----------------------------------------------------------------------------------------------

void FormMain::tray_icon_activated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
        setVisible(!isVisible());
}
//----------------------------------------------------------------------------------------------

void FormMain::task_on_info_error(EteraAPI* api)
{
    m_label_used->setText("");
    m_label_total->setText("");

    m_label_used->setVisible(false);
    m_label_total->setVisible(false);

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void FormMain::task_on_info_success(EteraAPI* api, const EteraInfo& info)
{
    m_label_used->setText(EteraAPI::humanBytes(info.used()));
    m_label_total->setText(EteraAPI::humanBytes(info.total()));

    m_label_used->setVisible(true);
    m_label_total->setVisible(true);

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void FormMain::updateInfoStatus()
{
    QString token = EteraSettings::instance()->token();

    if (token.isEmpty() == true)
        return;

    EteraAPI* api = new EteraAPI();

    api->setToken(token);

    connect(api, SIGNAL(onError(EteraAPI*)), this, SLOT(task_on_info_error(EteraAPI*)));
    connect(api, SIGNAL(onInfo(EteraAPI*, const EteraInfo&)), this, SLOT(task_on_info_success(EteraAPI*, const EteraInfo&)));

    api->info();
}
//----------------------------------------------------------------------------------------------

void FormMain::widget_path_on_path_change_request(const QString& path)
{
    m_widget_disk->changePath(path);
}
//----------------------------------------------------------------------------------------------

void FormMain::widget_disk_on_path_changed(const QString& path)
{
    m_widget_path->changePath(path);
}
//----------------------------------------------------------------------------------------------

void FormMain::widget_disk_on_change_possible_actions(bool download)
{
    m_action_download->setEnabled(download);
}
//----------------------------------------------------------------------------------------------

void FormMain::action_upload_triggered()
{
    QStringList list = QFileDialog::getOpenFileNames(this, trUtf8("Загрузка файлов на Яндекс.Диск"));
    m_widget_disk->putLocalObjects(list);
}
//----------------------------------------------------------------------------------------------

void FormMain::action_download_triggered()
{
    QString path = QFileDialog::getExistingDirectory(this, trUtf8("Загрузка файлов с Яндекс.Диска"));
    if (path == "")
        return;

    m_widget_disk->getRemoteObjects(path);
}
//----------------------------------------------------------------------------------------------
