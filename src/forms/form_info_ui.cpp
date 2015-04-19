#include "form_info_ui.h"
//----------------------------------------------------------------------------------------------

FormInfoUI::FormInfoUI(QWidget* parent) : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint)
{
    QIcon icon;
    icon.addFile(":/icons/info16.png",  QSize(16, 16));
    icon.addFile(":/icons/info32.png",  QSize(32, 32));
    icon.addFile(":/icons/info48.png",  QSize(48, 48));
    icon.addFile(":/icons/info64.png",  QSize(64, 64));
    icon.addFile(":/icons/info128.png", QSize(128, 128));
    setWindowIcon(QIcon::fromTheme("document-properties", icon));

    resize(350, 300);
    setFixedSize(width(), height());

    m_layout = new QHBoxLayout(this);

    //
    // подписи
    //

    m_layout_label = new QVBoxLayout();
    m_layout->addLayout(m_layout_label);

    Qt::Alignment alignment = Qt::AlignRight | Qt::AlignVCenter;

    m_label_name = new QLabel(this);
    m_label_name->setAlignment(alignment);
    m_layout_label->addWidget(m_label_name);

    m_label_path = new QLabel(this);
    m_label_path->setAlignment(alignment);
    m_layout_label->addWidget(m_label_path);

    m_label_type = new QLabel(this);
    m_label_type->setAlignment(alignment);
    m_layout_label->addWidget(m_label_type);

    m_label_created = new QLabel(this);
    m_label_created->setAlignment(alignment);
    m_layout_label->addWidget(m_label_created);

    m_label_modified = new QLabel(this);
    m_label_modified->setAlignment(alignment);
    m_layout_label->addWidget(m_label_modified);

    m_label_size = new QLabel(this);
    m_label_size->setAlignment(alignment);
    m_layout_label->addWidget(m_label_size);

    m_label_md5 = new QLabel(this);
    m_label_md5->setAlignment(alignment);
    m_layout_label->addWidget(m_label_md5);

    m_label_media = new QLabel(this);
    m_label_media->setAlignment(alignment);
    m_layout_label->addWidget(m_label_media);

    m_label_mime = new QLabel(this);
    m_label_mime->setAlignment(alignment);
    m_layout_label->addWidget(m_label_mime);

    m_label_public_url = new QLabel(this);
    m_label_public_url->setAlignment(alignment);
    m_layout_label->addWidget(m_label_public_url);

    //
    // контролы
    //

    m_layout_control = new QVBoxLayout();
    m_layout->addLayout(m_layout_control);

    Qt::TextInteractionFlags flags = Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard;

    m_text_name = new QLabel(this);
    m_text_name->setTextInteractionFlags(flags);
    m_layout_control->addWidget(m_text_name);

    m_text_path = new QLabel(this);
    m_text_path->setTextInteractionFlags(flags);
    m_layout_control->addWidget(m_text_path);

    m_text_type = new QLabel(this);
    m_text_type->setTextInteractionFlags(flags);
    m_layout_control->addWidget(m_text_type);

    m_text_created = new QLabel(this);
    m_text_created->setTextInteractionFlags(flags);
    m_layout_control->addWidget(m_text_created);

    m_text_modified = new QLabel(this);
    m_text_modified->setTextInteractionFlags(flags);
    m_layout_control->addWidget(m_text_modified);

    m_text_size = new QLabel(this);
    m_text_size->setTextInteractionFlags(flags);
    m_layout_control->addWidget(m_text_size);

    m_text_md5 = new QLabel(this);
    m_text_md5->setTextInteractionFlags(flags);
    m_layout_control->addWidget(m_text_md5);

    m_text_media = new QLabel(this);
    m_text_media->setTextInteractionFlags(flags);
    m_layout_control->addWidget(m_text_media);

    m_text_mime = new QLabel(this);
    m_text_mime->setTextInteractionFlags(flags);
    m_layout_control->addWidget(m_text_mime);

    m_text_public_url = new QLabel(this);
    m_text_public_url->setTextInteractionFlags(flags);
    m_layout_control->addWidget(m_text_public_url);

    m_layout->setStretchFactor(m_layout_control, 2);

    // локализация
    retranslateUi();

    // восстановление layout
    restore();
}
//----------------------------------------------------------------------------------------------

FormInfoUI::~FormInfoUI()
{
}
//----------------------------------------------------------------------------------------------

void FormInfoUI::closeEvent(QCloseEvent* /*event*/)
{
    save();
}
//----------------------------------------------------------------------------------------------

void FormInfoUI::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslateUi();
    else
        QWidget::changeEvent(event);
}
//----------------------------------------------------------------------------------------------

void FormInfoUI::retranslateUi()
{
    setWindowTitle(trUtf8("Свойства"));

    m_label_name->setText("<b>" + trUtf8("Имя") + ":</b>");
    m_label_path->setText("<b>" + trUtf8("Путь") + ":</b>");
    m_label_type->setText("<b>" + trUtf8("Тип") + ":</b>");
    m_label_created->setText("<b>" + trUtf8("Создан") + ":</b>");
    m_label_modified->setText("<b>" + trUtf8("Изменен") + ":</b>");
    m_label_size->setText("<b>" + trUtf8("Размер") + ":</b>");
    m_label_md5->setText("<b>" + trUtf8("MD5") + ":</b>");
    m_label_media->setText("<b>" + trUtf8("Медиа") + ":</b>");
    m_label_mime->setText("<b>" + trUtf8("MIME") + ":</b>");
    m_label_public_url->setText("<b>" + trUtf8("URL") + ":</b>");
}
//----------------------------------------------------------------------------------------------

void FormInfoUI::save()
{
    QSettings settings;

    settings.setValue("layout/info", this->saveGeometry());
}
//----------------------------------------------------------------------------------------------

void FormInfoUI::restore()
{
    QSettings settings;

    this->restoreGeometry(settings.value("layout/info").toByteArray());
}
//----------------------------------------------------------------------------------------------
