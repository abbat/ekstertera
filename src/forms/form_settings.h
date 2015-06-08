/*!
 * \file
 * \brief Форма настроек приложения
 */

#ifndef _ekstertera_forms_form_settings_h_
#define _ekstertera_forms_form_settings_h_

#include "utils/api.h"
#include "form_settings_ui.h"

/*!
 * \brief Класс формы настроек приложения
 */
class FormSettings : public FormSettingsUI
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param parent Родительский виджет
         */
        FormSettings(QWidget* parent);
        ~FormSettings();

    protected:

        /*!
         * \brief Функция отработки смены состояния формы
         */
        void virtual changeEvent(QEvent* event);

    private:

        QString m_language;   /*!< \brief Язык локализации на момент открытия формы */

    private slots:

        /*!
         * \brief OK
         */
        void button_ok_clicked();

        /*!
         * \brief Отмена
         */
        void button_cancel_clicked();

        /*!
         * \brief Смена языка
         */
        void combo_language_current_index_changed(int index);

        /*!
         * \brief Получение OAuth токена
         */
        void button_token_clicked();

        /*!
         * \brief Ошибка получения OAuth токена
         * \param api API
         */
        void task_on_token_error(EteraAPI* api);

        /*!
         * \brief Получение OAuth токена
         * \param api API
         * \param token OAuth токен
         */
        void task_on_token_success(EteraAPI* api, const QString& token);
};

#endif   // _ekstertera_forms_form_settings_h_
