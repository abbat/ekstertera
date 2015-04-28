/*!
 * \file
 * \brief Форма информации о файле или директории на Яндекс.Диске
 */

#ifndef _ekstertera_forms_form_info_h_
#define _ekstertera_forms_form_info_h_

#include "utils/api.h"
#include "form_info_ui.h"

/*!
 * \brief Класс формы информации о файле или директории на Яндекс.Диске
 */
class FormInfo : public FormInfoUI
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param item Описатель элемента на диске
         * \param parent Родительский виджет
         */
        FormInfo(const EteraItem& item, QWidget* parent);
        ~FormInfo();

    protected:

        /*!
         * \brief Функция отработки смены состояния формы
         */
        void virtual changeEvent(QEvent* event);

        /*!
         * \brief Функция перевода надписей на элементах управления
         */
        void retranslateUi();

    private:

        /*!
         * \brief Описатель отображаемых свойств
         */
        EteraItem m_item;
};

#endif   // _ekstertera_forms_form_info_h_
