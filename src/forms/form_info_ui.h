/*!
 * \file
 * \brief GUI формы информации о файле или директории на Яндекс.Диске
 */

#ifndef _ekstertera_forms_form_info_ui_h_
#define _ekstertera_forms_form_info_ui_h_

#include "sysheaders.h"

/*!
 * \brief Класс для генерации GUI части формы информации о файле или директории на Яндекс.Диске
 */
class FormInfoUI : public QDialog
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param parent Родительский виджет
         */
        FormInfoUI(QWidget* parent);
        ~FormInfoUI();

    protected:

        /*!
         * \brief Функция отработки закрытия формы
         */
        void virtual closeEvent(QCloseEvent* event);

        /*!
         * \brief Функция перевода надписей на элементах управления
         */
        virtual void retranslateUi();

        /*!
         * \brief Функция сохранения расположения и размеров элементов управления формы (при выходе)
         */
        void save();

        /*!
         * \brief Функция восстановления расположения и размеров элементов управления формы (при старте)
         */
        void restore();

    protected:

        QFormLayout* m_layout;             /*!< \brief Общий layout     */
          QLabel*    m_label_name;         /*!< \brief Имя              */
          QLabel*    m_label_path;         /*!< \brief Путь             */
          QLabel*    m_label_type;         /*!< \brief Тип              */
          QLabel*    m_label_created;      /*!< \brief Дата создания    */
          QLabel*    m_label_modified;     /*!< \brief Дата изменения   */
          QLabel*    m_label_size;         /*!< \brief Размер           */
          QLabel*    m_label_md5;          /*!< \brief MD5              */
          QLabel*    m_label_sha256;       /*!< \brief SHA256           */
          QLabel*    m_label_media;        /*!< \brief Медиа            */
          QLabel*    m_label_mime;         /*!< \brief MIME             */
          QLabel*    m_label_public_url;   /*!< \brief Публичная ссылка */

          QLabel*    m_text_name;          /*!< \brief Имя              */
          QLabel*    m_text_path;          /*!< \brief Путь             */
          QLabel*    m_text_type;          /*!< \brief Тип              */
          QLabel*    m_text_created;       /*!< \brief Дата создания    */
          QLabel*    m_text_modified;      /*!< \brief Дата изменения   */
          QLabel*    m_text_size;          /*!< \brief Размер           */
          QLabel*    m_text_md5;           /*!< \brief MD5              */
          QLabel*    m_text_sha256;        /*!< \brief SHA256           */
          QLabel*    m_text_media;         /*!< \brief Медиа            */
          QLabel*    m_text_mime;          /*!< \brief MIME             */
          QLabel*    m_text_public_url;    /*!< \brief Публичная ссылка */

    private:

        /*!
         * \brief Хэлпер для подписей
         * \param str Исходная строка
         */
        QString label(const QString& str) { return QString("<b>%1:</b>").arg(str); }
};

#endif   // _ekstertera_forms_form_info_ui_h_
