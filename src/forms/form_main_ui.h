/*!
 * \file
 * \brief GUI главной формы приложения
 */

#ifndef _ekstertera_forms_form_main_ui_h_
#define _ekstertera_forms_form_main_ui_h_

#include "widget_disk.h"
#include "widget_disk_path.h"

/*!
 * \brief Класс для генерации GUI части главной формы
 */
class FormMainUI : public QMainWindow
{
    Q_OBJECT

    public:

        FormMainUI();

    protected:

        /*!
         * \brief Функция отработки закрытия формы
         */
        void virtual closeEvent(QCloseEvent* event);

        /*!
         * \brief Функция отработки смены состояния формы
         */
        void virtual changeEvent(QEvent* event);

    protected:

        /*!
         * \brief Главное меню
         */
        QMenuBar*    m_menubar;
          QMenu*     m_menu_file;            /*!< \brief Файл / ...       */
            QAction* m_menu_file_settings;   /*!< \brief Файл / Настройки */
            QAction* m_menu_file_exit;       /*!< \brief Файл / Выход     */

        /*!
         * \brief Тулбар
         */
        QToolBar*  m_toolbar;
          QAction* m_action_upload;     /*!< \brief Загрузить на диск   */
          QAction* m_action_download;   /*!< \brief Скопировать с диска */

        /*!
         * \brief Центральный виджет
         */
        QWidget*          m_widget_central;
          QVBoxLayout*    m_layout_central;   /*!< \brief Вертикальное расположение */
          WidgetDiskPath* m_widget_path;      /*!< \brief Виджет отображения пути   */
          WidgetDisk*     m_widget_disk;      /*!< \brief Виджет отображения диска  */

        /*!
         * \brief Статусбар
         */
        QStatusBar*     m_status_bar;
          QLabel*       m_label_operation;   /*!< \brief Статус операции (красный/желтый/зеленый) */
          QLabel*       m_label_used;        /*!< \brief Размер используемого пространства        */
          QLabel*       m_label_total;       /*!< \brief Общий размер диска                       */
          QProgressBar* m_progress;          /*!< \brief Прогресс                                 */

        /*!
         * \brief Функция перевода надписей на элементах управления
         */
        void retranslateUi();

    private:

        /*!
         * \brief Функция сохранения расположения и размеров элементов управления формы <при выходе>
         */
        void save();

        /*!
         * \brief Функция восстановления расположения и размеров элементов управления формы <при старте>
         */
        void restore();
};

#endif   // _ekstertera_forms_form_main_ui_h_
