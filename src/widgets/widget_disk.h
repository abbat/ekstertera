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

        QString ERROR_MESSAGE;                         /*!< \brief "Ошибка!"                                                   */
        QString ATTENTION_MESSAGE;                     /*!< \brief "Внимание!"                                                 */
        QString ERROR_MESSAGE_QT;                      /*!< \brief "QTBUG-1158"                                                */
        QString ERROR_MESSAGE_LS;                      /*!< \brief "Ошибка чтения %1:\n%2" для ls                              */
        QString START_MESSAGE_LS;                      /*!< \brief "Чтение %1" для ls                                          */
        QString ERROR_MESSAGE_MKDIR;                   /*!< \brief "Ошибка создания %1:\n%2" для mkdir                         */
        QString START_MESSAGE_MKDIR;                   /*!< \brief "Создание %1" для mkdir                                     */
        QString START_MESSAGE_MKDIR_CAPTION;           /*!< \brief "Создать директорию" для mkdir                              */
        QString START_MESSAGE_MKDIR_TEXT;              /*!< \brief "Введите имя новой директории" для mkdir                    */
        QString START_MESSAGE_MKDIR_VALUE;             /*!< \brief "Новая папка" для mkdir                                     */
        QString ERROR_MESSAGE_MKDIR_ALREADY_EXISTS;    /*!< \brief "Директория %1 уже существует" для mkdir                    */
        QString ERROR_MESSAGE_STAT;                    /*!< \brief "Ошибка чтения информации о %1:\n%2" для stat               */
        QString START_MESSAGE_STAT;                    /*!< \brief "Чтение информации о %1" для stat                           */
        QString ROOT_MESSAGE_CP;                       /*!< \brief "Копирование" для cp                                        */
        QString ERROR_MESSAGE_CP;                      /*!< \brief "Ошибка копирования %1 в %2:\n%3" для cp                    */
        QString ERROR_MESSAGE_CP_ALREADY_EXISTS;       /*!< \brief "Ошибка копирования %1:\n%2 уже существует" для cp          */
        QString START_MESSAGE_CP;                      /*!< \brief "Копирование %1 в %2" для cp                                */
        QString ROOT_MESSAGE_MV;                       /*!< \brief "Перемещение" для mv                                        */
        QString ERROR_MESSAGE_MV;                      /*!< \brief "Ошибка перемещения %1 в %2:\n%3" для mv                    */
        QString ERROR_MESSAGE_MV_ALREADY_EXISTS;       /*!< \brief "Ошибка перемещения %1:\n%2 уже существует" для mv          */
        QString START_MESSAGE_MV;                      /*!< \brief "Перемещение %1 в %2" для mv                                */
        QString ERROR_MESSAGE_CPMV_SAME;               /*!< \brief "Источник и приемник совпадают" для mv                      */
        QString ROOT_MESSAGE_RM;                       /*!< \brief "Удаление" для rm                                           */
        QString ASK_DELETE_MESSAGE;                    /*!< \brief "Вы уверены, что хотите удалить выбранные элементы?" для rm */
        QString ERROR_MESSAGE_RM;                      /*!< \brief "Ошибка удаления %1:\n%2" для rm                            */
        QString START_MESSAGE_RM;                      /*!< \brief "Удаление %1" для rm                                        */
        QString ERROR_MESSAGE_RENAME_INVALID_CHAR;     /*!< \brief "Недопустимый символ \"%1\" в имени" для rename             */
        QString ERROR_MESSAGE_RENAME_IVALID_NAME;      /*!< \brief "Недопустимое имя" для rename                               */
        QString ERROR_MESSAGE_RENAME;                  /*!< \brief "Ошибка переименования %1 в %2:\n%3" для rename             */
        QString ERROR_MESSAGE_RENAME_ALREADY_EXISTS;   /*!< \brief "Ошибка переименования %1:\n%2 же существует" для rename    */
        QString START_MESSAGE_RENAME;                  /*!< \brief "Переименование %1 в %2" для rename                         */
        QString ROOT_MESSAGE_PUBLISH;                  /*!< \brief "Открытие доступа" для publish                              */
        QString ERROR_MESSAGE_PUBLISH;                 /*!< \brief "Ошибка открытия доступа к %1:\n%2" для publish             */
        QString START_MESSAGE_PUBLISH;                 /*!< \brief "Открытие доступа к %1" для publish                         */
        QString ROOT_MESSAGE_UNPUBLISH;                /*!< \brief "Закрытие доступа" для unpublish                            */
        QString ERROR_MESSAGE_UNPUBLISH;               /*!< \brief "Ошибка закрытия доступа к %1:\n%2" для unpublish           */
        QString START_MESSAGE_UNPUBLISH;               /*!< \brief "Закрытие доступа к %1" для unpublish                       */
        QString ROOT_MESSAGE_UPLOAD;                   /*!< \brief "Отправка на Диск" для put                                  */
        QString ERROR_MESSAGE_UPLOAD;                  /*!< \brief "Ошибка отправки %1 в %2:\n%3" для put                      */
        QString START_MESSAGE_UPLOAD;                  /*!< \brief "Отправка %1 в %2" для put                                  */
        QString START_MESSAGE_UPLOAD_CAPTION;          /*!< \brief "Файл уже существует!" для put                              */
        QString START_MESSAGE_UPLOAD_TEXT;             /*!< \brief "Файл %1 уже существует, перезаписать?" для put             */
        QString ROOT_MESSAGE_DOWNLOAD;                 /*!< \brief "Загрузка с Диска" для get                                  */
        QString ERROR_MESSAGE_DOWNLOAD;                /*!< \brief "Ошибка загрузки %1 в %2:\n%3" для get                      */
        QString START_MESSAGE_DOWNLOAD;                /*!< \brief "Загрузка %1 в %2" для get                                  */
        QString START_MESSAGE_DOWNLOAD_CAPTION;        /*!< \brief "Файл уже существует!" для get                              */
        QString START_MESSAGE_DOWNLOAD_TEXT;           /*!< \brief "Файл %1 уже существует, перезаписать?" для get             */
        QString ERROR_MESSAGE_UNKNOWN_OBJECT;          /*!< \brief "Неизвестный объект %1" для get                             */

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
         * Формат запроса на превьюшки
         */
        QString m_preview_arg;

        /*!
         * Виджет списка файлов и директорий
         */
        QListWidget* m_explorer;

        /*!
         * Виджет списка асинхронных задач
         */
        WidgetTasks* m_tasks;

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
         * Создание экземпляра API и установка токена
         * \return Экземпляр api
         */
        EteraAPI* createAPI();

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

        void task_on_ls_error(EteraAPI* api);
        void task_on_ls_success(EteraAPI* api, const EteraItemList& list, quint64 limit);

        void task_on_mkdir_error(EteraAPI* api);
        void task_on_mkdir_success(EteraAPI* api);

        void task_on_mkdir_stat_error(EteraAPI* api);
        void task_on_mkdir_stat_success(EteraAPI* api, const EteraItem& item);

        void task_on_copy_paste_error(EteraAPI* api);
        void task_on_copy_paste_success(EteraAPI* api);

        void task_on_cut_paste_error(EteraAPI* api);
        void task_on_cut_paste_success(EteraAPI* api);

        void task_on_copy_cut_paste_stat_error(EteraAPI* api);
        void task_on_copy_cut_paste_stat_success(EteraAPI* api, const EteraItem& item);

        void task_on_rm_error(EteraAPI* api);
        void task_on_rm_success(EteraAPI* api);

        void task_on_rename_error(EteraAPI* api);
        void task_on_rename_success(EteraAPI* api);

        void task_on_rename_stat_error(EteraAPI* api);
        void task_on_rename_stat_success(EteraAPI* api, const EteraItem& item);

        void task_on_publish_error(EteraAPI* api);
        void task_on_publish_success(EteraAPI* api);

        void task_on_unpublish_error(EteraAPI* api);
        void task_on_unpublish_success(EteraAPI* api);

        void task_on_publish_stat_error(EteraAPI* api);
        void task_on_publish_stat_success(EteraAPI* api, const EteraItem& item);

        void task_on_unpublish_stat_error(EteraAPI* api);
        void task_on_unpublish_stat_success(EteraAPI* api, const EteraItem& item);

        void task_on_put_dir_error(EteraAPI* api);
        void task_on_put_dir_success(EteraAPI* api);

        void task_on_put_dir_stat_error(EteraAPI* api);
        void task_on_put_dir_stat_success(EteraAPI* api, const EteraItem& item);

        void task_on_put_file_error(EteraAPI* api);
        void task_on_put_file_success(EteraAPI* api);
        void task_on_put_file_progress(EteraAPI* api, qint64 done, qint64 total);

        void task_on_put_ensure_error(EteraAPI* api);
        void task_on_put_ensure_success(EteraAPI* api, const EteraItem& item);

        void task_on_get_dir_error(EteraAPI* api);
        void task_on_get_dir_success(EteraAPI* api, const EteraItemList& list, quint64 limit);

        void task_on_get_file_error(EteraAPI* api);
        void task_on_get_file_success(EteraAPI* api);
        void task_on_get_file_progress(EteraAPI* api, qint64 done, qint64 total);



        void task_on_put_rm_error(EteraAPI* api);
        void task_on_put_rm_success(EteraAPI* api);

        void task_on_put_stat_error(EteraAPI* api);
        void task_on_put_stat_success(EteraAPI* api, const EteraItem& item);

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
