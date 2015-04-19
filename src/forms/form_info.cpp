#include "form_info.h"
//----------------------------------------------------------------------------------------------

FormInfo::FormInfo(const EteraItem& item, QWidget* parent) : FormInfoUI(parent)
{
    m_item = item;

    m_text_name->setText(m_item.name());
    m_text_path->setText(m_item.path());

    if (m_item.isDir() == true)
        m_text_type->setText(trUtf8("Директория"));
    else if (m_item.isFile() == true)
        m_text_type->setText(trUtf8("Файл"));

    QString format = "yyyy-MM-dd hh:mm:ss";
    m_text_created->setText(m_item.created().toLocalTime().toString(format));
    m_text_modified->setText(m_item.modified().toLocalTime().toString(format));

    m_text_size->setText(EteraAPI::humanBytes(m_item.size()));
    m_text_md5->setText(m_item.md5());
    m_text_media->setText(m_item.mediaString());
    m_text_mime->setText(m_item.mimeType());
    m_text_public_url->setText(m_item.publicURL());
}
//----------------------------------------------------------------------------------------------

FormInfo::~FormInfo()
{
}
//----------------------------------------------------------------------------------------------

void FormInfo::changeEvent(QEvent* event)
{
    FormInfoUI::changeEvent(event);
}
//----------------------------------------------------------------------------------------------
