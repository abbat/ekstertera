#include "form_main.h"
//----------------------------------------------------------------------------------------------
#include "utils/pool.h"
#include "utils/settings.h"
#include "tasks/task_info.h"
//----------------------------------------------------------------------------------------------
#include "form_settings.h"
//----------------------------------------------------------------------------------------------

FormMain::FormMain() : FormMainUI()
{
    // меню "Файл"
    connect(m_menu_file_settings, SIGNAL(triggered()), this, SLOT(menu_file_settings_triggered()));
    connect(m_menu_file_exit,     SIGNAL(triggered()), this, SLOT(menu_file_exit_triggered()));

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

void FormMain::menu_file_settings_triggered()
{
    FormSettings form(this);
    if (form.exec() == QDialog::Accepted) {
        updateInfoStatus();
        m_widget_disk->changePath("disk:/");
    }
}
//----------------------------------------------------------------------------------------------

void FormMain::menu_file_exit_triggered()
{
    m_tray_icon->setVisible(false);
    close();
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

void FormMain::task_on_info_success(quint64 /*id*/, const EteraInfo& info, const QVariantMap& /*args*/)
{
    m_label_used->setText(EteraAPI::humanBytes(info.used()));
    m_label_total->setText(EteraAPI::humanBytes(info.total()));
}
//----------------------------------------------------------------------------------------------

void FormMain::task_on_info_error(quint64 /*id*/, int /*code*/, const QString& /*error*/, const QVariantMap& /*args*/)
{
    m_label_used->setText("");
    m_label_total->setText("");
}
//----------------------------------------------------------------------------------------------

void FormMain::updateInfoStatus()
{
    QString token = EteraSettings::instance()->token();

    if (token.isEmpty() == true)
        return;

    EteraTaskINFO* info = new EteraTaskINFO();

    connect(info, SIGNAL(onError(quint64, int, const QString&, const QVariantMap&)), this, SLOT(task_on_info_error(quint64, int, const QString&, const QVariantMap&)));
    connect(info, SIGNAL(onSuccess(quint64, const EteraInfo&, const QVariantMap&)), this, SLOT(task_on_info_success(quint64, const EteraInfo&, const QVariantMap&)));

    EteraThreadPool::globalInstance()->start(info);
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
