/*!
 * \file
 * \brief Виджет проводника Яндекс.Диск
 */

#ifndef _ekstertera_widgets_widget_disk_h_
#define _ekstertera_widgets_widget_disk_h_

#include "clipboard.h"

/*!
 * \brief Виджет проводника Яндекс.Диск
 */
class WidgetDisk : public QWidget
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param parent Родительский виджет
         */
        WidgetDisk(QWidget* parent);
        ~WidgetDisk();

        /*!
         * \brief Установка токена
         * \param token Токен
         */
        void setToken(const QString& token);

        /*!
         * \brief Сменить отображаемый путь
         * \param path Путь
         * \return Флаг успеха
         */
        bool changePath(const QString& path);

        /*!
         * \brief Получение текущего пути
         * \return Текущий путь
         */
        QString path() const { return m_path; }

        /*!
         * Загрузка списка локальных файлов и директорий на диск
         * \param paths Список файлов и директорий
         */
        void putLocalObjects(const QStringList& paths);

        /*!
         * Загрузка локально выделенных в виджете файлов и директорий
         * \param path Локальный путь для загрузки
         */
        void getRemoteObjects(const QString& path);

        /*!
         * \brief Функция перевода надписей на элементах управления
         */
        void retranslateUi();

    protected:

        /*!
         * \brief Скролл мышью
         */
        virtual void wheelEvent(QWheelEvent* event);

        /*!
         * Изменение размера
         */
        virtual void resizeEvent(QResizeEvent* event);

        /*!
         * D&D
         */
        virtual void dragEnterEvent(QDragEnterEvent* event);

        /*!
         * D&D
         */
        virtual void dropEvent(QDropEvent* event);

    private:

        /*!
         * \brief Текущий путь
         */
        QString m_path;

        /*!
         * \brief API
         */
        EteraAPI m_api;

        /*!
         * \brief Буфер обмена
         */
        EteraClipboard* m_clipboard;

        /*!
         * \brief Последний ответ на вопрос
         */
        QMessageBox::StandardButton m_answer;

        /*!
         * Основной виджет списка
         */
        QListWidget* m_list;

        /*!
         * Контекстное меню
         */
        QMenu*   m_menu;
        QAction* m_menu_open;     /*!< \brief Открыть        */
        QAction* m_menu_new;      /*!< \brief Создать        */
        QAction* m_menu_cut;      /*!< \brief Вырезать       */
        QAction* m_menu_copy;     /*!< \brief Скопировать    */
        QAction* m_menu_paste;    /*!< \brief Вставить       */
        QAction* m_menu_delete;   /*!< \brief Удалить        */
        QAction* m_menu_rename;   /*!< \brief Переименовать  */
        QAction* m_menu_share;    /*!< \brief Открыть доступ */
        QAction* m_menu_revoke;   /*!< \brief Закрыть доступ */
        QAction* m_menu_info;     /*!< \brief Свойства       */

        /*!
         * Обновление списка в буфере обмена для cut/copy
         * \param copy_mode Флаг режима копирования
         */
        void updateBufferList(bool copy_mode);

        /*!
         * Обертка для обработчика фатальной ошибки API
         * (когда виджет переходит в неопределенное состояние)
         * \param clear Очистка виджета
         * \return Всегда false
         */
        bool fatalApiError(bool clear = true);

        /*!
         * Загрузка локального файла или директории на диск
         * \param path Имя файла или директории
         * \return Флаг успеха
         */
        bool putLocalObject(const QString& path);

        /*!
         * Загрузка локального файла на диск
         * \param source Имя локального файла
         * \param target Имя файла на диске
         * \return Флаг успеха
         */
        bool putLocalFile(const QString& source, const QString& target);

        /*!
         * Загрузка локальной директории на диск
         * \param source Имя локальной директории
         * \param target Имя директории на диске
         * \return Флаг успеха
         */
        bool putLocalDir(const QString& source, const QString& target);

        /*!
         * Получение файла с диска
         * \param source Имя файла на диске
         * \param target Имя локального файла
         * \return Флаг успеха
         */
        bool getRemoteFile(const QString& source, const QString& target);

        /*!
         * Получение директории с диска
         * \param source Имя директории на диске
         * \param target Имя локальной директории
         * \return Флаг успеха
         */
        bool getRemoteDir(const QString& source, const QString& target);

        /*!
         * Рекурсивное удаление локальной директории
         * \param path Имя директории
         * \return Флаг успеха
         */
        bool removeDir(QDir dir);

        /*!
         * Публикация выделенных ресурсов
         * \param share Флаг публикации или закрытия доступа
         * \return Флаг успеха
         */
        void shareObjects(bool share);

    private slots:

        /*!
         * \brief Открытие элемента
         */
        void item_activated(QListWidgetItem* item);

        /*!
         * \brief Изменение текста элемента
         * \param editor Редактор (обычно QLineEdit)
         */
        void item_end_edit(QWidget* editor, QAbstractItemDelegate::EndEditHint hint);

        /*!
         * \brief ПКМ, отображение меню
         * \param pos Точка
         */
        void custom_context_menu_requested(const QPoint& pos);

        /*!
         * \brief Изменение выделения
         */
        void on_item_selection_changed();

        //
        // обработчики контекстного меню
        //

        void menu_open_triggered();     /*!< \brief Открыть        */
        void menu_new_triggered();      /*!< \brief Создать        */
        void menu_cut_triggered();      /*!< \brief Вырезать       */
        void menu_copy_triggered();     /*!< \brief Скопировать    */
        void menu_paste_triggered();    /*!< \brief Вставить       */
        void menu_delete_triggered();   /*!< \brief Удалить        */
        void menu_rename_triggered();   /*!< \brief Переименовать  */
        void menu_share_triggered();    /*!< \brief Открыть доступ */
        void menu_revoke_triggered();   /*!< \brief Закрыть доступ */
        void menu_info_triggered();     /*!< \brief Свойства       */

        //
        // обработчики api
        //

        void api_on_progress(qint64 done, qint64 total);   /*!< \brief Прогресс операции */
        void api_on_error(const QString& error);           /*!< \brief Ошибки SSL        */

    signals:

        /*!
         * \brief Сигнал ошибки API
         * \param code Код ошибки
         * \param message Сообщение ошибки
         */
        void onApiError(int code, const QString& message);

        /*!
         * \brief Сигнал начала работы API
         * \param message Информационное сообщение
         */
        void onApiProgress(const QString& message);

        /*!
         * \brief Сигнал прогресса работы API
         * \param done Выполнено
         * \param total Всего
         */
        void onApiProgress(qint64 done, qint64 total);

        /*!
         * \brief Сигнал успешного окончания работы API
         */
        void onApiSuccess();

        /*!
         * \brief Сигнал смены пути
         * \param path Путь
         */
        void onPathChanged(const QString& path);

        /*!
         * \brief Сигнал смены возможных действий
         * \param download Возможность загрузить с диска
         */
        void onChangePossibleActions(bool download);
};

#endif   // _ekstertera_widgets_widget_disk_h_
