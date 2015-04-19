/*!
 * \file
 * \brief Главная форма приложения
 */

#ifndef _ekstertera_forms_form_main_h_
#define _ekstertera_forms_form_main_h_

#include "form_main_ui.h"

/*!
 * \brief Класс главной формы приложения
 */
class FormMain : public FormMainUI
{
    Q_OBJECT

    public:

        FormMain();
        ~FormMain();

    protected:

        /*!
         * \brief Функция отработки смены состояния формы
         */
        void virtual changeEvent(QEvent* event);

    private:

        /*!
         * \brief Статус операции
         */
        typedef enum
        {
            opGreen,    /*!< \brief Зеленый (нет ошибки, готов) */
            opYellow,   /*!< \brief Желтый (получение данных)   */
            opRed       /*!< \brief Красный (ошибка)            */
        } OperationStatus;

        /*!
         * \brief Обновление статуса в статус-баре
         */
        void updateInfoStatus();

        /*!
         * \brief Обновление статуса операции
         * \param status Статус
         * \param tooltip Всплывающее сообщение
         */
        void setOperationStatus(OperationStatus status, const QString& tooltip = "");

    private slots:

        //
        // меню "Файл"
        //

        void menu_file_settings_triggered();   /*!< \brief Файл / Настройки */
        void menu_file_exit_triggered();       /*!< \brief Файл / Выход     */

        //
        // тулбар
        //

        void action_upload_triggered();     /*!< \brief Загрузить на Яндекс.Диск */
        void action_download_triggered();   /*!< \brief Загрузить с Яндекс.Диска */

        //
        // обработчики виджета диска
        //

        void widget_disk_on_api_error(int code, const QString& message);   /*!< \brief Ошибка API                     */
        void widget_disk_on_api_progress(const QString& message);          /*!< \brief Начало работы API              */
        void widget_disk_on_api_progress(qint64 done, qint64 total);       /*!< \brief Прогресс работы API            */
        void widget_disk_on_api_success();                                 /*!< \brief Успешное завершение работы API */
        void widget_disk_on_path_changed(const QString& path);             /*!< \brief Смена пути в виджете           */
        void widget_disk_on_change_possible_actions(bool download);        /*!< \brief Смена возможных действий       */

        //
        // обработчики виджета путей
        //

        void widget_path_on_path_change_request(const QString& path);   /*!< \brief Запрос на смену пути */
};

#endif   // _ekstertera_forms_form_main_h_
