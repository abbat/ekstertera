#include "form_settings_ui.h"
//----------------------------------------------------------------------------------------------

FormSettingsUI::FormSettingsUI(QWidget* parent) : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint)
{
    setWindowIcon(QIcon::fromTheme("preferences-system", QIcon(":/icons/tango/preferences-system.svg")));

    resize(520, 150);
    setFixedSize(width(), height());

    m_layout = new QHBoxLayout(this);

    //
    // форма
    //

    m_layout_settings = new QFormLayout();

    m_label_language = new QLabel(this);
    m_combo_language = new QComboBox(this);
    m_combo_language->addItem(QString::fromUtf8("Русский"), "ru");
    m_combo_language->addItem(QString::fromUtf8("English"), "en");
    m_layout_settings->addRow(m_label_language, m_combo_language);

    m_layout_token = new QHBoxLayout();
    m_label_token  = new QLabel(this);
    m_text_token   = new QLineEdit(this);
    m_button_token = new QPushButton(this);
    m_button_token->setIcon(QIcon(":/icons/gnome/dialog-password16.png"));
    m_button_token->setMaximumSize(m_text_token->height(), m_text_token->height());
    m_layout_token->addWidget(m_text_token);
    m_layout_token->addWidget(m_button_token);
    m_layout_settings->addRow(m_label_token, m_layout_token);

    m_close_to_tray = new QCheckBox(this);
    m_layout_settings->addRow("", m_close_to_tray);

    m_layout->addLayout(m_layout_settings);

    //
    // кнопки
    //

    m_layout_button = new QVBoxLayout();

    m_button_ok = new QPushButton(this);
    m_button_ok->setShortcut(QKeySequence("Return"));
    m_button_ok->setDefault(true);
    m_layout_button->addWidget(m_button_ok);

    m_button_cancel = new QPushButton(this);
    m_button_cancel->setShortcut(QKeySequence("Esc"));
    m_layout_button->addWidget(m_button_cancel);

    m_spacer_button = new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::Expanding);
    m_layout_button->addItem(m_spacer_button);

    m_layout->addLayout(m_layout_button);

    // локализация
    retranslateUi();

    // восстановление layout
    restore();
}
//----------------------------------------------------------------------------------------------

FormSettingsUI::~FormSettingsUI()
{
}
//----------------------------------------------------------------------------------------------

void FormSettingsUI::closeEvent(QCloseEvent* /*event*/)
{
    save();
}
//----------------------------------------------------------------------------------------------

void FormSettingsUI::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslateUi();
    else
        QWidget::changeEvent(event);
}
//----------------------------------------------------------------------------------------------

void FormSettingsUI::retranslateUi()
{
    setWindowTitle(trUtf8("Параметры"));

    m_label_language->setText(trUtf8("Язык"));
    m_label_token->setText(trUtf8("OAuth токен"));

    m_button_token->setToolTip(trUtf8("Получить OAuth токен"));

    m_close_to_tray->setText(trUtf8("Скрывать в трее"));
    m_close_to_tray->setToolTip(trUtf8("Скрывать в значок в области уведомления вместо закрытия при нажатии на кнопку закрытия окна"));

    m_button_ok->setText(trUtf8("OK"));
    m_button_cancel->setText(trUtf8("Отмена"));
}
//----------------------------------------------------------------------------------------------

void FormSettingsUI::save()
{
    QSettings settings;
    settings.setValue("layout/settings", this->saveGeometry());
}
//----------------------------------------------------------------------------------------------

void FormSettingsUI::restore()
{
    QSettings settings;
    this->restoreGeometry(settings.value("layout/settings").toByteArray());
}
//----------------------------------------------------------------------------------------------
