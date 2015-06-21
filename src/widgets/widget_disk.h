/*!
 * \file
 * \brief Виджет проводника Яндекс.Диск
 */

#ifndef _ekstertera_widgets_widget_disk_h_
#define _ekstertera_widgets_widget_disk_h_

#include "utils/api.h"
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
         * \brief Функция перевода надписей на элементах управления
         */
        void retranslateUi();

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
         * \brief Загрузка списка локальных файлов и директорий на диск
         * \param paths Список файлов и директорий
         */
        void putLocalObjects(const QStringList& paths);

        /*!
         * \brief Загрузка локально выделенных в виджете файлов и директорий
         * \param path Локальный путь для загрузки
         */
        void getRemoteObjects(const QString& path);

        /*!
         * \brief Увеличить рамер
         * \return Флаг возможности дальнейшего увеличения
         */
        bool zoomIn();

        /*!
         * \brief Уменьшить рамер
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
         * \param factor Индекс размера
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

    protected:

        /*!
         * \brief Скролл мышью
         */
        virtual void wheelEvent(QWheelEvent* event);

        /*!
         * \brief D&D
         */
        virtual void dragEnterEvent(QDragEnterEvent* event);

        /*!
         * \brief D&D
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
         * \brief Формат запроса на превьюшки
         */
        QString m_preview_arg;

        /*!
         * \brief Виджет списка файлов и директорий
         */
        QListWidget* m_explorer;

        /*!
         * \brief Виджет списка асинхронных задач
         */
        WidgetTasks* m_tasks;

        //
        // контекстное меню
        //

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
         * \brief Создание экземпляра API и установка токена
         * \param id ID задачи
         * \return Экземпляр api
         */
        EteraAPI* createAPI(quint64 id = 0);

        /*!
         * \brief Освобождение экземпляра API и удаление id задачи из списка задач
         * \param api API (может быть NULL)
         */
        void releaseAPI(EteraAPI* api);

        /*!
         * \brief Обновление списка в буфере обмена для cut/copy
         * \param copy_mode Флаг режима копирования
         */
        void updateBufferList(bool copy_mode);

        /*!
         * \brief Поиск элемента в виджете файлов и директорий
         * \param path Путь к элементу
         * \return Виджет или NULL
         */
        WidgetDiskItem* findByPath(const QString& path);

        /*!
         * \brief Удаление элемента в виджете файлов и директорий
         * \param path Путь к элементу
         */
        void removeByPath(const QString& path);

        /*!
         * \brief Загрузка локального файла или директории на диск
         * \param path Имя файла или директории
         * \param parent ID родительской задачи
         */
        void putLocalObject(const QString& path, quint64 parent);

        /*!
         * \brief Загрузка локального файла на диск
         * \param source Имя локального файла
         * \param target Имя файла на диске
         * \param overwrite Флаг принудительной перезаписи
         * \param parent ID родительской задачи
         */
        void putLocalFile(const QString& source, const QString& target, bool overwrite, quint64 parent);

        /*!
         * \brief Загрузка локальной директории на диск
         * \param source Имя локальной директории
         * \param target Имя директории на диске
         * \param overwrite Флаг принудительной перезаписи
         * \param parent ID родительской задачи
         */
        void putLocalDir(const QString& source, const QString& target, bool overwrite, quint64 parent);

        /*!
         * \brief Синхронизация локальной директории и директории на диске
         * \param source Имя локальной директории
         * \param target Имя директории на диске
         * \param overwrite Флаг принудительной перезаписи
         * \param parent ID родительской задачи
         */
        void syncLocalDir(const QString& source, const QString& target, bool overwrite, quint64 parent);

        /*!
         * \brief Получение файла с диска
         * \param source Имя файла на диске
         * \param target Имя локального файла
         * \param parent ID родительской задачи
         * \return QMessageBox::NoButton в случае успешного выполнения
         * другие в случае ошибки (QMessageBox::Abort)
         */
        QMessageBox::StandardButton getRemoteFile(const QString& source, const QString& target, quint64 parent);

        /*!
         * \brief Получение директории с диска
         * \param source Имя директории на диске
         * \param target Имя локальной директории
         * \param parent ID родительской задачи
         * \return QMessageBox::NoButton в случае успешного выполнения
         * другие в случае ошибки (QMessageBox::Abort)
         */
        QMessageBox::StandardButton getRemoteDir(const QString& source, const QString& target, quint64 parent);

        /*!
         * \brief Рекурсивное удаление локальной директории
         * \param path Имя директории
         * \return QMessageBox::NoButton в случае успешного выполнения
         * другие в случае ошибки (QMessageBox::Abort)
         */
        QMessageBox::StandardButton removeDir(QDir dir);

        /*!
         * \brief Публикация выделенных ресурсов
         * \param share Флаг публикации или закрытия доступа
         */
        void shareObjects(bool share);

        /*!
         * \brief Связь id задачи с активным API
         */
        typedef QMap<quint64, EteraAPI*> EteraAPIMap;

        /*!
         * \brief Описатель активности отправки объектов
         */
        typedef struct {
            quint64 ID;          /*!< \brief ID задачи              */
            quint64 Parent;      /*!< \brief ID родительской задачи */
            QString Source;      /*!< \brief Источник               */
            QString Target;      /*!< \brief Приемник               */
            bool    Overwrite;   /*!< \brief Флаг перезаписи        */
        } EteraPutActivityItem;

        /*!
         * \brief Очередь задач отправки объектов (put)
         */
        typedef QQueue<EteraPutActivityItem> EteraPutActivityQueue;

        /*!
         * \brief Тип активности отправки объекта
         */
        typedef enum {
            epatDir,      /*!< \brief Отправка директории (mkdir) */
            epatFile,     /*!< \brief Отправка файла (put)        */
            epatUnknown   /*!< \brief Неизвестная активность      */
        } EteraPutActivityType;

        int                   m_put_activity_limit;     /*!< \brief Масимальное количество запросов в активности */
        EteraPutActivityQueue m_put_queue_mkdir;        /*!< \brief Очередь mkdir для отправки файлов            */
        EteraPutActivityQueue m_put_queue_put;          /*!< \brief Очередь put для отправки файлов              */
        EteraAPIMap           m_put_active_api_mkdir;   /*!< \brief Активные mkdir задачи API                    */
        EteraAPIMap           m_put_active_api_put;     /*!< \brief Активные put задачи API                      */

        /*!
         * \brief Добавление put активности в очередь ожидания
         * \param type Тип активности
         * \param parent ID родительской задачи
         * \param source Источник
         * \param target Приемник
         * \param overwrite Флаг перезаписи
         */
        void addPutActivity(EteraPutActivityType type, quint64 parent, const QString& source, const QString& target, bool overwrite);

        /*!
         * \brief Запуск get активностей
         * \param type Тип активности, которая была завершена параметром api
         * \param api API для удаления или NULL для активности apatUnknown
         */
        void spawnPutActivity(EteraPutActivityType type, EteraAPI* api = NULL);

        /*!
         * \brief Остановка put активностей
         * \param id ID активности
         * \param full Флаг полного удаления дерева активностей
         */
        void abortPutActivity(quint64 id, bool full = false);

        /*!
         * \brief Удаление put активностей из очереди ожидания
         * \param queue Очередь ожидания
         * \param ids ID задач для удаления
         */
        void removePutActivity(EteraPutActivityQueue& queue, QList<quint64>& aborted);

        /*!
         * \brief Описатель активности получения файла
         */
        typedef struct {
            quint64 ID;       /*!< \brief ID задачи              */
            quint64 Parent;   /*!< \brief ID родительской задачи */
            QString Source;   /*!< \brief Источник               */
            QString Target;   /*!< \brief Приемник               */
        } EteraGetActivityItem;

        /*!
         * \brief Очередь задач получения файлов (get)
         */
        typedef QQueue<EteraGetActivityItem> EteraGetActivityQueue;

        /*!
         * \brief Тип активности получения файла
         */
        typedef enum {
            egatList,     /*!< \brief Получение списка (ls)  */
            agatGet,      /*!< \brief Загрузка файла (get)   */
            agatUnknown   /*!< \brief Неизвестная активность */
        } EteraGetActivityType;

        int                   m_get_activity_limit;   /*!< \brief Масимальное количество запросов в активности */
        EteraGetActivityQueue m_get_queue_ls;         /*!< \brief Очередь ls для получения файлов              */
        EteraGetActivityQueue m_get_queue_get;        /*!< \brief Очередь get для получения файлов             */
        EteraAPIMap           m_get_active_api_ls;    /*!< \brief Активные ls задачи API                       */
        EteraAPIMap           m_get_active_api_get;   /*!< \brief Активные get задачи API                      */

        /*!
         * \brief Добавление get активности в очередь ожидания
         * \param type Тип активности
         * \param parent ID родительской задачи
         * \param source Источник
         * \param target Приемник
         */
        void addGetActivity(EteraGetActivityType type, quint64 parent, const QString& source, const QString& target);

        /*!
         * \brief Запуск get активностей
         * \param type Тип активности, которая была завершена параметром api
         * \param api API для удаления или NULL для активности agatUnknown
         */
        void spawnGetActivity(EteraGetActivityType type, EteraAPI* api = NULL);

        /*!
         * \brief Остановка get активностей
         * \param id ID активности
         * \param full Флаг полного удаления дерева активностей
         */
        void abortGetActivity(quint64 id, bool full = false);

        /*!
         * \brief Удаление get активностей из очереди ожидания
         * \param queue Очередь ожидания
         * \param ids ID задач для удаления
         */
        void removeGetActivity(EteraGetActivityQueue& queue, QList<quint64>& aborted);

        /*!
         * \brief Таймер для отложенных сигналов
         */
        QTimer* m_emit_timer;

        /*!
         * \brief Флаг отображения пользователю QMessageBox
         */
        bool m_message_box_active;

        /*!
         * \brief Проверка, что UI заблокирован QMessageBox
         * \return true, если UI заблокирован, откладываем выполнение
         * сигнала через вызов delayEmit
         */
        bool messageBoxLocked() const { return m_message_box_active; }

        /*!
         * \brief Блокирование UI перед вызовом QMessageBox
         */
        void messageBoxLock() { m_message_box_active = true; }

        /*!
         * \brief Разблокирование UI после вызова QMessageBox
         */
        void messageBoxUnlock() { m_message_box_active = false; }

        /*!
         * \brief Имя слота для вызова отложенных сигналов
         */
        typedef enum {
            slot_task_on_ls_error,                    /*!< \brief task_on_ls_error                  */
            slot_task_on_mkdir_error,                 /*!< \brief task_on_mkdir_error               */
            slot_task_on_mkdir_stat_error,            /*!< \brief task_on_mkdir_stat_error          */
            slot_task_on_copy_paste_error,            /*!< \brief task_on_copy_paste_error          */
            slot_task_on_cut_paste_error,             /*!< \brief task_on_cut_paste_error           */
            slot_task_on_copy_cut_paste_stat_error,   /*!< \brief task_on_copy_cut_paste_stat_error */
            slot_task_on_rm_error,                    /*!< \brief task_on_rm_error                  */
            slot_task_on_publish_error,               /*!< \brief task_on_publish_error             */
            slot_task_on_unpublish_error,             /*!< \brief task_on_publish_error             */
            slot_task_on_publish_stat_error,          /*!< \brief task_on_publish_stat_error        */
            slot_task_on_unpublish_stat_error,        /*!< \brief task_on_unpublish_stat_error      */
            slot_task_on_put_mkdir_error,             /*!< \brief task_on_put_mkdir_error           */
            slot_task_on_put_file_error,              /*!< \brief task_on_put_file_error            */
            slot_task_on_put_stat_error,              /*!< \brief task_on_put_stat_error            */
            slot_task_on_put_ensure_error,            /*!< \brief task_on_put_ensure_error          */
            slot_task_on_put_rm_error,                /*!< \brief task_on_put_rm_error              */
            slot_task_on_get_dir_error,               /*!< \brief task_on_get_dir_error             */
            slot_task_on_get_file_error               /*!< \brief task_on_get_file_error            */
        } EteraTaskSlot;

        /*!
         * \brief Описатель сигнала для отложенного вызова
         */
        typedef struct {
            EteraTaskSlot Slot;   /*!< \brief Имя слота */
            EteraAPI*     API;    /*!< \brief API       */
        } EteraTaskSignal;

        /*!
         * \brief Тип для очереди отложенных сигналов
         */
        typedef QQueue<EteraTaskSignal> EteraSignalQueue;

        /*!
         * \brief Очередь отложенных сигналов
         */
        EteraSignalQueue m_delayed_queue;

        /*!
         * \brief Имя слота для вызова отложенных сигналов
         */
        typedef enum {
            slot_task_on_put_ensure_success   /*!< \brief task_on_put_ensure_success */
        } EteraTaskSlotStat;

        /*!
         * \brief Описатель сигнала для отложенного вызова stat
         */
        typedef struct {
            EteraTaskSlotStat Slot;   /*!< \brief Имя слота */
            EteraAPI*         API;    /*!< \brief API       */
            EteraItem         Item;   /*!< \brief Элемент   */
        } EteraTaskSignalStat;

        /*!
         * \brief Тип для очереди отложенных сигналов stat
         */
        typedef QQueue<EteraTaskSignalStat> EteraSignalStatQueue;

        /*!
         * \brief Очередь отложенных сигналов
         */
        EteraSignalStatQueue m_delayed_stat_queue;

        /*!
         * \brief Поставить сигнал в очередь
         * \param slot Слот для отложенного вызова
         * \param api API
         */
        void delayEmit(EteraTaskSlot slot, EteraAPI* api);

        /*!
         * \brief Поставить сигнал в очередь stat
         * \param slot Слот для отложенного вызова
         * \param api API
         * \param item Элемент
         */
        void delayEmit(EteraTaskSlotStat slot, EteraAPI* api, const EteraItem& item);

        /*!
         * \brief Очередь отложенных задач
         */
        EteraSignalQueue m_delayed_task_queue;

        /*!
         * \brief Поставить задачу в очередь отложенных задач
         * \param slot Слот для отложенного вызова
         * \param api API
         */
        void delayTask(EteraTaskSlot slot, EteraAPI* api);

        /*!
         * \brief Удаление отложенных сигналов
         * \param api API
         */
        void removeDelayed(const EteraAPI* api);

        /*!
         * \brief Проверка, что код ошибки позволяет отложить задачу
         * \param code Код HTTP
         * \return true, если задачу можно отложить (восстановимая ошибка)
         */
        bool canDelayTask(int code);

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

        /*
         * putLocalObject +-> putLocalFile -> EteraAPI::put +-> EteraAPI::stat (?)
         *                |       ^   ^                     |
         *                |       |   |                     +-> EteraAPI::stat (HTTP-409) --+
         *                |       |   |                               |                     |
         *                |       |   +-------------------------------+                     |
         *                |       |                                                         v
         *                |       +-----------------------------------+-------------- EteraAPI::rm
         *                |       |                                   |                     ^
         *                |       v                                   |                     |
         *                +-> putLocalDir -> EteraAPI::mkdir +-> syncLocalDir               |
         *                                                   |                              |
         *                                                   +-> EteraAPI::stat (HTTP-409) -+
         *                                                   |
         *                                                   +-> EteraAPI::stat (?)
         */

        void task_on_put_mkdir_error(EteraAPI* api);
        void task_on_put_mkdir_success(EteraAPI* api);

        void task_on_put_file_error(EteraAPI* api);
        void task_on_put_file_success(EteraAPI* api);
        void task_on_put_file_progress(EteraAPI* api, qint64 done, qint64 total);

        void task_on_put_stat_error(EteraAPI* api);
        void task_on_put_stat_success(EteraAPI* api, const EteraItem& item);

        void task_on_put_ensure_error(EteraAPI* api);
        void task_on_put_ensure_success(EteraAPI* api, const EteraItem& item);

        void task_on_put_rm_error(EteraAPI* api);
        void task_on_put_rm_success(EteraAPI* api);

        /*
         * getRemoteObjects +-> getRemoteFile (@removeDir) -> EteraAPI::get
         *                  |         ^
         *                  |         |
         *                  |         +-----------------------+
         *                  |         |                       |
         *                  |         v                       |
         *                  +-> getRemoteDir -> EteraAPI::ls -+
         */

        void task_on_get_dir_error(EteraAPI* api);
        void task_on_get_dir_success(EteraAPI* api, const EteraItemList& list, quint64 limit);

        void task_on_get_file_error(EteraAPI* api);
        void task_on_get_file_success(EteraAPI* api);
        void task_on_get_file_progress(EteraAPI* api, qint64 done, qint64 total);

        // отложенные сигналы
        void emit_delayed_signals();

        // отложенные задачи
        void emit_delayed_tasks();

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

        /*!
         * \brief Смена выделения
         * \param file Количество выделенных файлов
         * \param dirs Количество выделенных директорий
         * \param size Размер выделенных файлов
         */
        void onSelectionChanged(int files, int dirs, quint64 size);
};

#endif   // _ekstertera_widgets_widget_disk_h_
