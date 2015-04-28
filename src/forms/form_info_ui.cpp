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

    setMinimumSize(360, 240);

    setWindowTitle(trUtf8("Свойства"));

    m_layout = new QFormLayout(this);
    m_layout->setLabelAlignment(Qt::AlignRight);

    m_label_name       = new QLabel(this);
    m_label_path       = new QLabel(this);
    m_label_type       = new QLabel(this);
    m_label_created    = new QLabel(this);
    m_label_modified   = new QLabel(this);
    m_label_size       = new QLabel(this);
    m_label_md5        = new QLabel(this);
    m_label_media      = new QLabel(this);
    m_label_mime       = new QLabel(this);
    m_label_public_url = new QLabel(this);

    Qt::TextInteractionFlags flags = Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard;

    m_text_name = new QLabel(this);
    m_text_name->setTextInteractionFlags(flags);
    m_layout->addRow(m_label_name, m_text_name);

    m_text_path = new QLabel(this);
    m_text_path->setTextInteractionFlags(flags);
    m_layout->addRow(m_label_path, m_text_path);

    m_text_type = new QLabel(this);
    m_text_type->setTextInteractionFlags(flags);
    m_layout->addRow(m_label_type, m_text_type);

    m_text_created = new QLabel(this);
    m_text_created->setTextInteractionFlags(flags);
    m_layout->addRow(m_label_created, m_text_created);

    m_text_modified = new QLabel(this);
    m_text_modified->setTextInteractionFlags(flags);
    m_layout->addRow(m_label_modified, m_text_modified);

    m_text_size = new QLabel(this);
    m_text_size->setTextInteractionFlags(flags);
    m_layout->addRow(m_label_size, m_text_size);

    m_text_md5 = new QLabel(this);
    m_text_md5->setTextInteractionFlags(flags);
    m_layout->addRow(m_label_md5, m_text_md5);

    m_text_media = new QLabel(this);
    m_text_media->setTextInteractionFlags(flags);
    m_layout->addRow(m_label_media, m_text_media);

    m_text_mime = new QLabel(this);
    m_text_mime->setTextInteractionFlags(flags);
    m_layout->addRow(m_label_mime, m_text_mime);

    m_text_public_url = new QLabel(this);
    m_text_public_url->setTextInteractionFlags(flags);
    m_layout->addRow(m_label_public_url, m_text_public_url);

    // восстановление layout
    restore();
}
//----------------------------------------------------------------------------------------------

FormInfoUI::~FormInfoUI()
{
}
//----------------------------------------------------------------------------------------------

void FormInfoUI::retranslateUi()
{
    setWindowTitle(trUtf8("Свойства"));

    m_label_name->setText(label(trUtf8("Имя")));
    m_label_path->setText(label(trUtf8("Путь")));
    m_label_type->setText(label(trUtf8("Тип")));
    m_label_created->setText(label(trUtf8("Создан")));
    m_label_modified->setText(label(trUtf8("Изменен")));
    m_label_size->setText(label(trUtf8("Размер")));
    m_label_md5->setText(label(trUtf8("MD5")));
    m_label_media->setText(label(trUtf8("Медиа")));
    m_label_mime->setText(label(trUtf8("MIME")));
    m_label_public_url->setText(label(trUtf8("URL")));
}
//----------------------------------------------------------------------------------------------

void FormInfoUI::closeEvent(QCloseEvent* /*event*/)
{
    save();

    deleteLater();
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
