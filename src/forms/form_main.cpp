#include "form_main.h"
//----------------------------------------------------------------------------------------------
#include "settings.h"
//----------------------------------------------------------------------------------------------
#include "form_settings.h"
//----------------------------------------------------------------------------------------------

FormMain::FormMain() : FormMainUI()
{
    // меню "Файл"
    connect(m_menu_file_settings, SIGNAL(triggered()), this, SLOT(menu_file_settings_triggered()));
    connect(m_menu_file_exit,     SIGNAL(triggered()), this, SLOT(menu_file_exit_triggered()));

    // тулбар
    connect(m_action_upload,   SIGNAL(triggered()), this, SLOT(action_upload_triggered()));
    connect(m_action_download, SIGNAL(triggered()), this, SLOT(action_download_triggered()));

    // виджет пути
    connect(m_widget_path, SIGNAL(onPathChangeRequest(const QString&)), this, SLOT(widget_path_on_path_change_request(const QString&)));

    // виджет диска
    connect(m_widget_disk, SIGNAL(onApiError(int, const QString&)), this, SLOT(widget_disk_on_api_error(int, const QString&)));
    connect(m_widget_disk, SIGNAL(onApiProgress(const QString&)),   this, SLOT(widget_disk_on_api_progress(const QString&)));
    connect(m_widget_disk, SIGNAL(onApiProgress(qint64, qint64)),   this, SLOT(widget_disk_on_api_progress(qint64, qint64)));
    connect(m_widget_disk, SIGNAL(onApiSuccess()),                  this, SLOT(widget_disk_on_api_success()));
    connect(m_widget_disk, SIGNAL(onPathChanged(const QString&)),   this, SLOT(widget_disk_on_path_changed(const QString&)));
    connect(m_widget_disk, SIGNAL(onChangePossibleActions(bool)),   this, SLOT(widget_disk_on_change_possible_actions(bool)));

    // обновление статуса
    updateInfoStatus();

    // получение корня диска
    m_widget_disk->setToken(EteraSettings::instance()->token());
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
    }
}
//----------------------------------------------------------------------------------------------

void FormMain::menu_file_exit_triggered()
{
    close();
}
//----------------------------------------------------------------------------------------------

void FormMain::setOperationStatus(OperationStatus status, const QString& tooltip)
{
    switch (status) {
        case opGreen:
            m_action_upload->setEnabled(true);
            m_label_operation->setPixmap(QPixmap(":icons/green16.png"));
            break;
        case opYellow:
            m_action_upload->setEnabled(false);
            m_label_operation->setPixmap(QPixmap(":icons/yellow16.png"));
            break;
        case opRed:
            m_action_upload->setEnabled(false);
            m_label_operation->setPixmap(QPixmap(":icons/red16.png"));
            break;
    }

    m_label_operation->setToolTip(tooltip);
}
//----------------------------------------------------------------------------------------------

void FormMain::updateInfoStatus()
{
    setOperationStatus(opYellow);

    m_label_used->setText("");
    m_label_total->setText("");

    QString token = EteraSettings::instance()->token();

    if (token.isEmpty() == true) {
        setOperationStatus(opGreen);
        return;
    }

    EteraAPI api;
    api.setToken(token);

    EteraInfo info;
    if (api.info(info) == false) {
        setOperationStatus(opRed);
        return;
    }

    m_label_used->setText(EteraAPI::humanBytes(info.used()));
    m_label_total->setText(EteraAPI::humanBytes(info.total()));

    setOperationStatus(opGreen);
}
//----------------------------------------------------------------------------------------------

void FormMain::widget_path_on_path_change_request(const QString& path)
{
    m_widget_disk->changePath(path);
}
//----------------------------------------------------------------------------------------------

void FormMain::widget_disk_on_api_error(int /*code*/, const QString& message)
{
    m_progress->setVisible(false);

    setOperationStatus(opRed, message);
}
//----------------------------------------------------------------------------------------------

void FormMain::widget_disk_on_api_progress(const QString& message)
{
    m_progress->setMaximum(0);
    m_progress->setMinimum(0);
    m_progress->setValue(0);
    m_progress->setVisible(true);

    setOperationStatus(opYellow, message);
}
//----------------------------------------------------------------------------------------------

void FormMain::widget_disk_on_api_progress(qint64 done, qint64 total)
{
    m_progress->setMaximum(total);
    m_progress->setValue(done);
}
//----------------------------------------------------------------------------------------------

void FormMain::widget_disk_on_api_success()
{
    m_progress->setVisible(false);

    setOperationStatus(opGreen, trUtf8("OK"));
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
