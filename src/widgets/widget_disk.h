/*!
 * \file
 * \brief Виджет проводника Яндекс.Диск
 */

#ifndef _ekstertera_widgets_widget_disk_h_
#define _ekstertera_widgets_widget_disk_h_

#include "widget_tasks.h"
#include "widget_disk_item.h"

/*!
 * \brief Виджет проводника Яндекс.Диск
 */
class WidgetDisk : public QTabWidget
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
         * \brief Сменить отображаемый путь
         * \param path Путь
         */
        void changePath(const QString& path);

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
         * Увеличить рамер
         * \return Флаг возможности дальнейшего увеличения
         */
        bool zoomIn();

        /*!
         * Уменьшить рамер
         * \return Флаг возможности дальнейшего уменьшения
         */
        bool zoomOut();

        /*!
         * \brief Текущий размер иконок
         * \return Текущий размер иконок
         */
        int zoomFactor() { return m_icon_size_index; }

        /*!
         * \brief Установить текущий размер иконок
         * \factor Индекс размера
         * \return -1 если больше нельзя уменьшать, +1 если больше нельзя увеличить, 0 - в остальных случаях
         */
        int setZoomFactor(int factor);

        /*!
         * \brief Установка режима предпросмотра
         * \param mode Режим
         */
        void setPreviewMode(bool mode);

        /*!
         * \brief Пункты контекстного меню
         * \return Список пунктов
         */
        QList<QAction*> contextMenuActions() { return m_menu->actions(); }

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
         * \brief Текущий запрошенный путь
         * При неактивной смене пути равен m_path
         */
        QString m_required_path;

        /*!
         * \brief Текущий индекс размера иконок
         */
        int m_icon_size_index;

        /*!
         * \brief Режим предпросмотра
         */
        bool m_preview_mode;

        /*!
         * Виджет списка файлов и директорий
         */
        QListWidget* m_explorer;

        /*!
         * Виджет списка асинхронных задач
         */
        WidgetTasks* m_tasks;

        /*!
         * Блокиратор вызова QMessageBox::question для ситуаций, когда
         * сигнал посылается из разных потоков, а требуется только один ответ
         */
        QMutex m_question_mutex;

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
         * Поиск элемента в виджете файлов и директорий
         * \param path Путь к элементу
         * \return Виджет или NULL
         */
        WidgetDiskItem* findByPath(const QString& path);

        /*!
         * Удаление элемента в виджете файлов и директорий
         * \param path Путь к элементу
         */
        void removeByPath(const QString& path);

        /*!
         * Загрузка локального файла или директории на диск
         * \param path Имя файла или директории
         * \param parent ID родительской задачи
         */
        void putLocalObject(const QString& path, quint64 parent);

        /*!
         * Загрузка локального файла на диск
         * \param source Имя локального файла
         * \param target Имя файла на диске
         * \param overwrite Флаг принудительной перезаписи
         * \param parent ID родительской задачи
         */
        void putLocalFile(const QString& source, const QString& target, bool overwrite, quint64 parent);

        /*!
         * Загрузка локальной директории на диск
         * \param source Имя локальной директории
         * \param target Имя директории на диске
         * \param overwrite Флаг принудительной перезаписи
         * \param parent ID родительской задачи
         */
        void putLocalDir(const QString& source, const QString& target, bool overwrite, quint64 parent);

        /*!
         * Синхронизация локальной директории и директории на диске
         * \param source Имя локальной директории
         * \param target Имя директории на диске
         * \param overwrite Флаг принудительной перезаписи
         * \param parent ID родительской задачи
         */
        void syncLocalDir(const QString& source, const QString& target, bool overwrite, quint64 parent);

        /*!
         * Получение файла с диска
         * \param source Имя файла на диске
         * \param target Имя локального файла
         * \param parent ID родительской задачи
         */
        void getRemoteFile(const QString& source, const QString& target, quint64 parent);

        /*!
         * Получение директории с диска
         * \param source Имя директории на диске
         * \param target Имя локальной директории
         * \param parent ID родительской задачи
         */
        void getRemoteDir(const QString& source, const QString& target, quint64 parent);

        /*!
         * Рекурсивное удаление локальной директории
         * \param path Имя директории
         * \return Флаг успеха
         */
        bool removeDir(QDir dir);

        /*!
         * Публикация выделенных ресурсов
         * \param share Флаг публикации или закрытия доступа
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
        // обработчики виджета задач
        //

        void widget_tasks_on_change_count(int count);

        //
        // обработчики асинхронных задач
        //

        void task_on_start(quint64 id, const QString& message, const QVariantMap& args);

        void task_on_ls_error(EteraAPI* api);
        void task_on_ls_success(EteraAPI* api, const EteraItemList& list, const QString& path, const QString& preview, bool crop, quint64 offset, quint64 limit);

        void task_on_mkdir_stat_error(EteraAPI* api);
        void task_on_mkdir_stat_success(EteraAPI* api, const EteraItem& item);

        void task_on_mkdir_error(EteraAPI* api);
        void task_on_mkdir_success(EteraAPI* api, const QString& path);

        void task_on_rm_error(EteraAPI* api);
        void task_on_rm_success(EteraAPI* api, const QString& path);

        void task_on_rename_stat_error(EteraAPI* api);
        void task_on_rename_stat_success(EteraAPI* api, const EteraItem& item);

        void task_on_rename_error(EteraAPI* api);
        void task_on_rename_success(EteraAPI* api, const QString& source, const QString& target);

        void task_on_copy_paste_stat_error(EteraAPI* api);
        void task_on_copy_paste_stat_success(EteraAPI* api, const EteraItem& item);

        void task_on_copy_paste_error(EteraAPI* api);
        void task_on_copy_paste_success(EteraAPI* api, const QString& source, const QString& target);

        void task_on_cut_paste_stat_error(EteraAPI* api);
        void task_on_cut_paste_stat_success(EteraAPI* api, const EteraItem& item);

        void task_on_cut_paste_error(EteraAPI* api);
        void task_on_cut_paste_success(EteraAPI* api, const QString& source, const QString& target);

        void task_on_publish_error(EteraAPI* api);
        void task_on_publish_success(EteraAPI* api, const QString& path);
        void task_on_publish_success(EteraAPI* api, const EteraItem& item);

        void task_on_unpublish_error(EteraAPI* api);
        void task_on_unpublish_success(EteraAPI* api, const QString& path);
        void task_on_unpublish_success(EteraAPI* api, const EteraItem& item);

        void task_on_put_rm_error(EteraAPI* api);
        void task_on_put_rm_success(EteraAPI* api, const QString& path);

        void task_on_put_ensure_error(EteraAPI* api);
        void task_on_put_ensure_success(EteraAPI* api, const EteraItem& item);

        void task_on_put_stat_error(EteraAPI* api);
        void task_on_put_stat_success(EteraAPI* api, const EteraItem& item);

        void task_on_put_file_error(EteraAPI* api);
        void task_on_put_file_progress(EteraAPI* api, qint64 done, qint64 total);
        void task_on_put_file_success(EteraAPI* api, const QUrl& url, QIODevice* device);

        void task_on_put_dir_stat_error(EteraAPI* api);
        void task_on_put_dir_stat_success(EteraAPI* api, const EteraItem& item);

        void task_on_put_dir_error(EteraAPI* api);
        void task_on_put_dir_success(EteraAPI* api, const QString& path);

        void task_on_get_file_error(EteraAPI* api);
        void task_on_get_file_progress(EteraAPI* api, qint64 done, qint64 total);
        void task_on_get_file_success(EteraAPI* api, const QUrl& url, QIODevice* device);

        void task_on_get_dir_error(EteraAPI* api);
        void task_on_get_dir_success(EteraAPI* api, const EteraItemList& list, const QString& path, const QString& preview, bool crop, quint64 offset, quint64 limit);

    signals:

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
