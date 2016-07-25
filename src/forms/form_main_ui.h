/*!
 * \file
 * \brief GUI главной формы приложения
 */

#ifndef _ekstertera_forms_form_main_ui_h_
#define _ekstertera_forms_form_main_ui_h_

#include "widgets/widget_disk.h"
#include "widgets/widget_disk_path.h"
#include "widgets/widget_tray_icon.h"

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
          QMenu*     m_menu_file;            /*!< \brief Файл / ...         */
            QAction* m_menu_file_exit;       /*!< \brief Файл / Выход       */
          QMenu*     m_menu_edit;            /*!< \brief Правка / ...       */
            QAction* m_menu_edit_settings;   /*!< \brief Правка / Параметры */
          QMenu*     m_menu_view;            /*!< \brief Вид / ...          */
            QAction* m_menu_view_refresh;    /*!< \brief Вид / Обновить     */
            QAction* m_menu_view_zoom_in;    /*!< \brief Вид / Увеличить    */
            QAction* m_menu_view_zoom_out;   /*!< \brief Вид / Уменьшить    */
            QAction* m_menu_view_preview;    /*!< \brief Вид / Предпросмотр */
          QMenu*     m_menu_help;            /*!< \brief ? / ...            */
            QAction* m_menu_about;           /*!< \brief ? / О программе    */
            QAction* m_menu_about_qt;        /*!< \brief ? / О Qt           */

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
        QStatusBar* m_status_bar;
          QLabel*   m_label_used;       /*!< \brief Размер используемого пространства */
          QLabel*   m_label_total;      /*!< \brief Общий размер диска                */
          QLabel*   m_label_selected;   /*!< \brief Статус выделения                  */

        /*!
         * \brief Иконка в трее
         */
        WidgetTrayIcon* m_tray_icon;
          QAction*      m_tray_menu_show;   /*!< \brief Показать */

        /*!
         * \brief Меню в трее
         */
        QMenu* m_tray_menu;

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
