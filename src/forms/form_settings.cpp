#include "form_settings.h"
//----------------------------------------------------------------------------------------------
#include "api.h"
#include "settings.h"
#include "translator.h"
//----------------------------------------------------------------------------------------------

FormSettings::FormSettings(QWidget* parent) : FormSettingsUI(parent)
{
    EteraSettings* settings = EteraSettings::instance();

    m_language = EteraTranslator::instance()->language();

    m_text_token->setText(settings->token());

    int index = m_combo_language->findData(m_language);
    if (index != -1)
        m_combo_language->setCurrentIndex(index);

    connect(m_button_cancel, SIGNAL(clicked()), this, SLOT(button_cancel_clicked()));
    connect(m_button_ok,     SIGNAL(clicked()), this, SLOT(button_ok_clicked()));

    connect(m_combo_language, SIGNAL(currentIndexChanged(int)), this, SLOT(combo_language_current_index_changed(int)));
    connect(m_button_token,   SIGNAL(clicked()), this, SLOT(button_token_clicked()));
}
//----------------------------------------------------------------------------------------------

FormSettings::~FormSettings()
{
}
//----------------------------------------------------------------------------------------------

void FormSettings::changeEvent(QEvent* event)
{
    FormSettingsUI::changeEvent(event);
}
//----------------------------------------------------------------------------------------------

void FormSettings::combo_language_current_index_changed(int index)
{
    QString language = m_combo_language->itemData(index).toString();

    EteraTranslator::instance()->changeTranslator(language);
}
//----------------------------------------------------------------------------------------------

void FormSettings::button_cancel_clicked()
{
    // возврат локализации
    QString language = m_combo_language->itemData(m_combo_language->currentIndex()).toString();
    if (language != m_language)
        EteraTranslator::instance()->changeTranslator(m_language);

    save();
    reject();
}
//----------------------------------------------------------------------------------------------

void FormSettings::button_ok_clicked ()
{
    QSettings settings;

    settings.setValue("app/language", m_combo_language->itemData(m_combo_language->currentIndex()).toString());
    settings.setValue("api/token", m_text_token->text());

    EteraSettings::instance()->reload();

    save();
    accept();
}
//----------------------------------------------------------------------------------------------

void FormSettings::button_token_clicked()
{
    EteraAPI api;
    api.getToken();

    bool ok;
    QString auth_code = QInputDialog::getText(this, trUtf8("Получение OAuth токена"), trUtf8("Введите код подтверждения"), QLineEdit::Normal, "", &ok).trimmed();
    if (ok == false || auth_code.isEmpty() == true) {
        QMessageBox::warning(this, trUtf8("Внимание!"), trUtf8("Необходимо ввести код подтверждения!"));
        return;
    }

    QString token = api.getToken(auth_code);
    if (token.isEmpty() == true) {
        QMessageBox::critical(this, trUtf8("Ошибка!"), api.lastErrorMessage());
        return;
    }

    m_text_token->setText(token);
}
//----------------------------------------------------------------------------------------------
