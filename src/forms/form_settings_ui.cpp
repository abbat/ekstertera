#include "form_settings_ui.h"
//----------------------------------------------------------------------------------------------

FormSettingsUI::FormSettingsUI(QWidget* parent) : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint)
{
    QIcon icon;
    icon.addFile(":/icons/settings16.png",  QSize(16, 16));
    icon.addFile(":/icons/settings32.png",  QSize(32, 32));
    icon.addFile(":/icons/settings48.png",  QSize(48, 48));
    icon.addFile(":/icons/settings64.png",  QSize(64, 64));
    icon.addFile(":/icons/settings128.png", QSize(128, 128));
    setWindowIcon(QIcon::fromTheme("preferences-system", icon));

    resize(520, 150);
    setFixedSize(width(), height());

    //
    // layout
    //

    m_layout_spacer = new QVBoxLayout(this);

    m_layout = new QHBoxLayout();
    m_layout_spacer->addLayout(m_layout);

    //
    // подписи
    //

    m_layout_label = new QVBoxLayout();
    m_layout->addLayout(m_layout_label);

    m_label_language = new QLabel(this);
    m_layout_label->addWidget(m_label_language);

    m_label_token = new QLabel(this);
    m_layout_label->addWidget(m_label_token);

    //
    // контролы
    //

    m_layout_control = new QVBoxLayout();
    m_layout->addLayout(m_layout_control);

    m_combo_language = new QComboBox(this);
    m_combo_language->addItem(QString::fromUtf8("Русский"), "ru");
    m_combo_language->addItem(QString::fromUtf8("English"), "en");
    m_layout_control->addWidget(m_combo_language);

    m_layout_token = new QHBoxLayout();

    m_text_token = new QLineEdit(this);
    m_layout_token->addWidget(m_text_token);

    m_button_token = new QPushButton(this);
    m_button_token->setIcon(QIcon(":/icons/oauth16.png"));
    m_button_token->setMaximumSize(m_text_token->height(), m_text_token->height());
    m_layout_token->addWidget(m_button_token);

    m_layout_control->addLayout(m_layout_token);

    //
    // кнопки
    //

    m_layout_button = new QVBoxLayout();
    m_layout->addLayout(m_layout_button);

    m_button_ok = new QPushButton(this);
    m_button_ok->setShortcut(QKeySequence("Return"));
    m_button_ok->setDefault(true);
    m_layout_button->addWidget(m_button_ok);

    m_button_cancel = new QPushButton(this);
    m_button_cancel->setShortcut(QKeySequence("Esc"));
    m_layout_button->addWidget(m_button_cancel);

    m_spacer_button = new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::Expanding);
    m_layout_button->addItem(m_spacer_button);

    //
    // выравнивание
    //

    m_spacer_layout = new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::Expanding);
    m_layout_spacer->addItem(m_spacer_layout);

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
    setWindowTitle(trUtf8("Настройки"));

    m_label_language->setText(trUtf8("Язык"));
    m_label_token->setText(trUtf8("OAuth токен"));

    m_button_token->setToolTip(trUtf8("Получить OAuth токен"));

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
