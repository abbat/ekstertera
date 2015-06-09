#include "widget_disk.h"
//----------------------------------------------------------------------------------------------
#include "widget_disk_item_delegate.h"
//----------------------------------------------------------------------------------------------
#include "utils/icon.h"
#include "utils/settings.h"
#include "utils/clipboard.h"
//----------------------------------------------------------------------------------------------
#include "forms/form_info.h"
//----------------------------------------------------------------------------------------------

WidgetDisk::WidgetDisk(QWidget* parent) : QTabWidget(parent)
{
    m_preview_mode = false;

    m_explorer = new QListWidget(this);

    m_explorer->setWrapping(true);
    m_explorer->setResizeMode(QListView::Adjust);
    m_explorer->setContextMenuPolicy(Qt::CustomContextMenu);
    m_explorer->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_explorer->setSortingEnabled(true);
    m_explorer->setItemDelegate(new WidgetDiskItemDelegate(m_explorer));

    setZoomFactor(EteraIconProvider::instance()->defaultIconSizeIndex());

    m_tasks = new WidgetTasks(this);

    setAcceptDrops(true);

    addTab(m_explorer, "");
    addTab(m_tasks, QIcon(":icons/green16.png"), "");

    int size = EteraIconProvider::instance()->maxIconSize();
    m_preview_arg = QString("%1x%2").arg(size).arg(size);

    // контекстное меню
    m_menu = new QMenu(m_explorer);

    m_menu_open = m_menu->addAction(QIcon::fromTheme("folder-open", QIcon(":/icons/tango/folder-open.svg")), "");
    m_menu_open->setShortcut(QKeySequence("Enter"));

    m_menu->addSeparator();

    m_menu_new = m_menu->addAction(QIcon::fromTheme("folder-new", QIcon(":/icons/tango/folder-new.svg")), "");
    m_menu_new->setShortcut(QKeySequence(QKeySequence::New));

    m_menu->addSeparator();

    m_menu_cut = m_menu->addAction(QIcon::fromTheme("edit-cut",   QIcon(":/icons/tango/edit-cut.svg")),   "");
    m_menu_cut->setShortcut(QKeySequence(QKeySequence::Cut));

    m_menu_copy = m_menu->addAction(QIcon::fromTheme("edit-copy",  QIcon(":/icons/tango/edit-copy.svg")),  "");
    m_menu_copy->setShortcut(QKeySequence(QKeySequence::Copy));

    m_menu_paste = m_menu->addAction(QIcon::fromTheme("edit-paste", QIcon(":/icons/tango/edit-paste.svg")), "");
    m_menu_paste->setShortcut(QKeySequence(QKeySequence::Paste));

    m_menu->addSeparator();

    m_menu_delete = m_menu->addAction(QIcon::fromTheme("edit-delete", QIcon(":/icons/tango/edit-delete.svg")), "");
    m_menu_delete->setShortcut(QKeySequence(QKeySequence::Delete));

    m_menu->addSeparator();

    m_menu_rename = m_menu->addAction("");
    m_menu_rename->setShortcut(QKeySequence("F2"));

    m_menu->addSeparator();

    m_menu_share = m_menu->addAction(QIcon::fromTheme("emblem-shared",    QIcon(":/icons/gnome/emblem-shared16.png")),   "");
    m_menu_revoke = m_menu->addAction(QIcon::fromTheme("document-revert", QIcon(":/icons/gnome/document-revert16.png")), "");

    m_menu->addSeparator();

    m_menu_info = m_menu->addAction(QIcon::fromTheme("document-properties", QIcon(":/icons/tango/document-properties.svg")), "");

    connect(m_menu_open,   SIGNAL(triggered()), this, SLOT(menu_open_triggered()));
    connect(m_menu_new,    SIGNAL(triggered()), this, SLOT(menu_new_triggered()));
    connect(m_menu_cut,    SIGNAL(triggered()), this, SLOT(menu_cut_triggered()));
    connect(m_menu_copy,   SIGNAL(triggered()), this, SLOT(menu_copy_triggered()));
    connect(m_menu_paste,  SIGNAL(triggered()), this, SLOT(menu_paste_triggered()));
    connect(m_menu_delete, SIGNAL(triggered()), this, SLOT(menu_delete_triggered()));
    connect(m_menu_rename, SIGNAL(triggered()), this, SLOT(menu_rename_triggered()));
    connect(m_menu_share,  SIGNAL(triggered()), this, SLOT(menu_share_triggered()));
    connect(m_menu_revoke, SIGNAL(triggered()), this, SLOT(menu_revoke_triggered()));
    connect(m_menu_info,   SIGNAL(triggered()), this, SLOT(menu_info_triggered()));

    // события виджета проводника диска
    connect(m_explorer, SIGNAL(itemSelectionChanged()), this, SLOT(on_item_selection_changed()));
    connect(m_explorer, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(item_activated(QListWidgetItem*)));
    connect(m_explorer, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(custom_context_menu_requested(const QPoint&)));
    connect(m_explorer->itemDelegate(), SIGNAL(closeEditor(QWidget*, QAbstractItemDelegate::EndEditHint)), this, SLOT(item_end_edit(QWidget*, QAbstractItemDelegate::EndEditHint)));

    // события виджета асинхронных задач
    connect(m_tasks, SIGNAL(onChangeCount(int)), this, SLOT(widget_tasks_on_change_count(int)));

    // локализация
    retranslateUi();

    // включение / выключение пунктов контекстного меню
    on_item_selection_changed();
}
//----------------------------------------------------------------------------------------------

WidgetDisk::~WidgetDisk()
{
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::retranslateUi()
{
    m_menu_open->setText(trUtf8("Открыть"));
    m_menu_new->setText(trUtf8("Создать"));
    m_menu_cut->setText(trUtf8("Вырезать"));
    m_menu_copy->setText(trUtf8("Копировать"));
    m_menu_paste->setText(trUtf8("Вставить"));
    m_menu_delete->setText(trUtf8("Удалить"));
    m_menu_rename->setText(trUtf8("Переименовать"));
    m_menu_share->setText(trUtf8("Открыть доступ"));
    m_menu_revoke->setText(trUtf8("Закрыть доступ"));
    m_menu_info->setText(trUtf8("Свойства"));

    setTabText(0, trUtf8("Проводник"));
    setTabText(1, trUtf8("Задачи"));

    //
    // информационные сообщения
    //

    ERROR_MESSAGE                       = trUtf8("Ошибка!");
    ATTENTION_MESSAGE                   = trUtf8("Внимание!");
    ERROR_MESSAGE_LS                    = trUtf8("Ошибка чтения %1:\n%2");
    START_MESSAGE_LS                    = trUtf8("Чтение %1");
    ERROR_MESSAGE_MKDIR                 = trUtf8("Ошибка создания %1:\n%2");
    START_MESSAGE_MKDIR_CAPTION         = trUtf8("Создать директорию");
    START_MESSAGE_MKDIR_TEXT            = trUtf8("Введите имя новой директории");
    START_MESSAGE_MKDIR_VALUE           = trUtf8("Новая папка");
    ERROR_MESSAGE_MKDIR_ALREADY_EXISTS  = trUtf8("Директория %1 уже существует");
    START_MESSAGE_MKDIR                 = trUtf8("Создание %1");
    ERROR_MESSAGE_STAT                  = trUtf8("Ошибка чтения информации о %1:\n%2");
    START_MESSAGE_STAT                  = trUtf8("Чтение информации о %1");
    ROOT_MESSAGE_CP                     = trUtf8("Копирование");
    ERROR_MESSAGE_CP                    = trUtf8("Ошибка копирования %1 в %2:\n%3");
    ERROR_MESSAGE_CP_ALREADY_EXISTS     = trUtf8("Ошибка копирования %1:\n%2 уже существует");
    START_MESSAGE_CP                    = trUtf8("Копирование %1 в %2");
    ROOT_MESSAGE_MV                     = trUtf8("Перемещение");
    ERROR_MESSAGE_MV                    = trUtf8("Ошибка перемещения %1 в %2:\n%3");
    ERROR_MESSAGE_MV_ALREADY_EXISTS     = trUtf8("Ошибка перемещения %1:\n%2 уже существует");
    START_MESSAGE_MV                    = trUtf8("Перемещение %1 в %2");
    ERROR_MESSAGE_CPMV_SAME             = trUtf8("Источник и приемник совпадают");
    ROOT_MESSAGE_RM                     = trUtf8("Удаление");
    ASK_DELETE_MESSAGE                  = trUtf8("Вы уверены, что хотите удалить выбранные элементы?");
    ERROR_MESSAGE_RM                    = trUtf8("Ошибка удаления %1:\n%2");
    START_MESSAGE_RM                    = trUtf8("Удаление %1");
    ERROR_MESSAGE_RENAME_INVALID_CHAR   = trUtf8("Недопустимый символ \"%1\" в имени");
    ERROR_MESSAGE_RENAME_IVALID_NAME    = trUtf8("Недопустимое имя");
    ERROR_MESSAGE_RENAME                = trUtf8("Ошибка переименования %1 в %2:\n%3");
    ERROR_MESSAGE_RENAME_ALREADY_EXISTS = trUtf8("Ошибка переименования %1:\n%2 же существует");
    START_MESSAGE_RENAME                = trUtf8("Переименование %1 в %2");
    ROOT_MESSAGE_PUBLISH                = trUtf8("Открытие доступа");
    ERROR_MESSAGE_PUBLISH               = trUtf8("Ошибка открытия доступа к %1:\n%2");
    START_MESSAGE_PUBLISH               = trUtf8("Открытие доступа к %1");
    ROOT_MESSAGE_UNPUBLISH              = trUtf8("Закрытие доступа");
    ERROR_MESSAGE_UNPUBLISH             = trUtf8("Ошибка закрытия доступа к %1:\n%2");
    START_MESSAGE_UNPUBLISH             = trUtf8("Закрытие доступа к %1");
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::wheelEvent(QWheelEvent* event)
{
    m_explorer->horizontalScrollBar()->event(event);
}
//----------------------------------------------------------------------------------------------

EteraAPI* WidgetDisk::createAPI()
{
    EteraAPI* api = new EteraAPI(this);
    api->setToken(EteraSettings::instance()->token());
    return api;
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::updateBufferList(bool copy_mode)
{
    QList<QListWidgetItem*> selected = m_explorer->selectedItems();

    if (selected.isEmpty() == true)
        return;

    EteraClipboard* clipboard = EteraClipboard::instance();

    clipboard->clear();

    if (copy_mode == true)
        clipboard->setCopyMode();
    else
        clipboard->setCutMode();

    for (int i = 0; i < selected.count(); i++)
        clipboard->append(*((static_cast<WidgetDiskItem*>(selected[i]))->item()));
}
//----------------------------------------------------------------------------------------------

WidgetDiskItem* WidgetDisk::findByPath(const QString& path)
{
    // оптимизировать O(n) при необходимости
    for (int i = 0; i < m_explorer->count(); i++) {
        WidgetDiskItem*  witem = static_cast<WidgetDiskItem*>(m_explorer->item(i));
        const EteraItem* eitem = witem->item();
        if (eitem->path() == path)
            return witem;
    }

    return NULL;
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::removeByPath(const QString& path)
{
    // оптимизировать O(n) при необходимости
    for (int i = 0; i < m_explorer->count(); i++) {
        WidgetDiskItem*  witem = static_cast<WidgetDiskItem*>(m_explorer->item(i));
        const EteraItem* eitem = witem->item();
        if (eitem->path() == path) {
            m_explorer->takeItem(i);
            delete witem;
            break;
        }
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::widget_tasks_on_change_count(int count)
{
    if (count > 0) {
        setTabIcon(1, QIcon(":icons/yellow16.png"));
        setTabText(1, trUtf8("Задачи (%1)").arg(count));
    } else {
        setTabIcon(1, QIcon(":icons/green16.png"));
        setTabText(1, trUtf8("Задачи"));
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::changePath(const QString& path)
{
    m_explorer->setCursor(Qt::BusyCursor);

    m_path = "";
    m_required_path = path;

    m_explorer->clear();

    QString _path = path;
    if (_path.endsWith("/") == false) {
        _path += "/";
        m_required_path += "/";
    }

    EteraAPI* api = createAPI();

    ETERA_API_TASK_LS(api, task_on_ls_success, task_on_ls_error);

    m_tasks->addSimpleTask(api->id(), START_MESSAGE_LS.arg(_path));

    api->ls(_path, m_preview_arg, true);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_ls_error(EteraAPI* api)
{
    QMessageBox::critical(this, ERROR_MESSAGE, ERROR_MESSAGE_LS.arg(api->path()).arg(api->lastErrorMessage()));

    m_explorer->setCursor(Qt::ArrowCursor);

    m_tasks->removeSimpleTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_ls_success(EteraAPI* api, const EteraItemList& list, quint64 limit)
{
    // если загружался большой список, но в это время сменили текущий путь - останавливаем предыдущую работу
    if (api->path() != m_required_path) {
        m_tasks->removeSimpleTask(api->id());
        api->deleteLater();
        return;
    }

    // добавление элементов
    for (int i = 0; i < list.count(); i++)
        new WidgetDiskItem(m_explorer, list[i], m_preview_mode);

    // проверка необходимости остановки
    if ((quint64)list.count() < limit) {
        m_tasks->removeSimpleTask(api->id());

        m_path = api->path();

        m_explorer->setCursor(Qt::ArrowCursor);

        emit onPathChanged(api->path());

        api->deleteLater();
    } else {
        quint64 offset = api->offset() + limit;
        api->ls(api->path(), api->preview(), api->crop(), offset, limit);
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::item_activated(QListWidgetItem* item)
{
    WidgetDiskItem*  witem = static_cast<WidgetDiskItem*>(item);
    const EteraItem* eitem = witem->item();

    if (eitem->isDir() == false)
        return;

    QString path = eitem->path();

    changePath(path);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::custom_context_menu_requested(const QPoint& pos)
{
    if (m_path.isEmpty() == true)
        return;

    m_menu->exec(m_explorer->viewport()->mapToGlobal(pos));
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_open_triggered()
{
    WidgetDiskItem* witem = static_cast<WidgetDiskItem*>(m_explorer->currentItem());
    if (witem == NULL)
        return;

    const EteraItem* eitem = witem->item();

    if (eitem->isDir() == true)
        changePath(eitem->path());
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_new_triggered()
{
    if (m_path.isEmpty() == true)
        return;

    bool ok;
    QString value = QInputDialog::getText(this, START_MESSAGE_MKDIR_CAPTION, START_MESSAGE_MKDIR_TEXT, QLineEdit::Normal, START_MESSAGE_MKDIR_VALUE, &ok);
    if (ok == false || value.isEmpty() == true)
        return;

    QString path = m_path + value;

    EteraAPI* api = createAPI();

    ETERA_API_TASK_MKDIR(api, task_on_mkdir_success, task_on_mkdir_error);

    m_tasks->addSimpleTask(api->id(), START_MESSAGE_MKDIR.arg(path));

    api->mkdir(path);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_mkdir_error(EteraAPI* api)
{
    // CONFLICT, директория уже существует
    if (api->lastErrorCode() == 409)
        QMessageBox::warning(this, ATTENTION_MESSAGE, ERROR_MESSAGE_MKDIR_ALREADY_EXISTS.arg(api->path()));
    else
        QMessageBox::critical(this, ERROR_MESSAGE, ERROR_MESSAGE_MKDIR.arg(api->path()).arg(api->lastErrorMessage()));

    m_tasks->removeSimpleTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_mkdir_success(EteraAPI* api)
{
    ETERA_API_CONTINUE_TASK_STAT(api, task_on_mkdir_stat_success, task_on_mkdir_stat_error, task_on_mkdir_error);

    m_tasks->addSimpleTask(api->id(), START_MESSAGE_STAT.arg(api->path()));

    api->stat(api->path());
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_mkdir_stat_error(EteraAPI* api)
{
    QMessageBox::critical(this, ERROR_MESSAGE, ERROR_MESSAGE_STAT.arg(api->path()).arg(api->lastErrorMessage()));

    m_tasks->removeSimpleTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_mkdir_stat_success(EteraAPI* api, const EteraItem& item)
{
    if (item.parentPath() == m_path)
        m_explorer->setCurrentItem(new WidgetDiskItem(m_explorer, item, m_preview_mode), QItemSelectionModel::ClearAndSelect);

    m_tasks->removeSimpleTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_cut_triggered()
{
    updateBufferList(false);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_copy_triggered()
{
    updateBufferList(true);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_paste_triggered()
{
    EteraClipboard* clipboard = EteraClipboard::instance();

    if (clipboard->isEmpty() == true || m_path.isEmpty() == true)
        return;

    quint64 parent = 0;
    if (clipboard->count() > 1) {
        parent = EteraAPI::nextID();
        if (clipboard->copyMode() == true)
            m_tasks->addSimpleTask(parent, ROOT_MESSAGE_CP);
        else
            m_tasks->addSimpleTask(parent, ROOT_MESSAGE_MV);
    }

    for (int i = 0; i < clipboard->count(); i++) {
        EteraItem src = clipboard->at(i);
        QString   dst = m_path + src.name();

        // нет смысла перемещать само в себя
        if (src.path() == dst) {
            QMessageBox::warning(this, ATTENTION_MESSAGE, ERROR_MESSAGE_CPMV_SAME);
            return;
        }

        EteraAPI* api = createAPI();

        if (clipboard->copyMode() == true) {
            ETERA_API_TASK_CP(api, task_on_copy_paste_success, task_on_copy_paste_error);

            m_tasks->addChildTask(parent, api->id(), START_MESSAGE_CP.arg(src.path()).arg(dst));

            api->cp(src.path(), dst, false);
        } else {
            ETERA_API_TASK_MV(api, task_on_cut_paste_success, task_on_cut_paste_error);

            m_tasks->addChildTask(parent, api->id(), START_MESSAGE_MV.arg(src.path()).arg(dst));

            api->mv(src.path(), dst, false);
        }
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_copy_paste_error(EteraAPI* api)
{
    // CONFLICT, объект уже существует
    if (api->lastErrorCode() == 409)
        QMessageBox::warning(this, ATTENTION_MESSAGE, ERROR_MESSAGE_CP_ALREADY_EXISTS.arg(api->source()).arg(api->target()));
    else
        QMessageBox::critical(this, ERROR_MESSAGE, ERROR_MESSAGE_CP.arg(api->source()).arg(api->target()).arg(api->lastErrorMessage()));

    // TODO: обработать асинхронную ошибку, т.к. копирование могло быть все же успешным
    m_tasks->removeChildTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_copy_paste_success(EteraAPI* api)
{
    EteraClipboard::instance()->removeByPath(api->source());

    ETERA_API_CONTINUE_TASK_STAT(api, task_on_copy_cut_paste_stat_success, task_on_copy_cut_paste_stat_error, task_on_copy_paste_error);

    m_tasks->addChildTask(api->parentId(), api->id(), START_MESSAGE_STAT.arg(api->target()));

    api->stat(api->target(), m_preview_arg, true);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_cut_paste_error(EteraAPI* api)
{
    // CONFLICT, объект уже существует
    if (api->lastErrorCode() == 409)
        QMessageBox::warning(this, ATTENTION_MESSAGE, ERROR_MESSAGE_MV_ALREADY_EXISTS.arg(api->source()).arg(api->target()));
    else
        QMessageBox::critical(this, ERROR_MESSAGE, ERROR_MESSAGE_MV.arg(api->source()).arg(api->target()).arg(api->lastErrorMessage()));

    // TODO: обработать асинхронную ошибку, т.к. вставка могла быть все же успешной
    m_tasks->removeChildTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_cut_paste_success(EteraAPI* api)
{
    EteraClipboard::instance()->removeByPath(api->source());

    ETERA_API_CONTINUE_TASK_STAT(api, task_on_copy_cut_paste_stat_success, task_on_copy_cut_paste_stat_error, task_on_copy_paste_error);

    m_tasks->addChildTask(api->parentId(), api->id(), START_MESSAGE_STAT.arg(api->target()));

    api->stat(api->target(), m_preview_arg, true);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_copy_cut_paste_stat_error(EteraAPI* api)
{
    QMessageBox::critical(this, ERROR_MESSAGE, ERROR_MESSAGE_STAT.arg(api->target()).arg(api->lastErrorMessage()));

    m_tasks->removeChildTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_copy_cut_paste_stat_success(EteraAPI* api, const EteraItem& item)
{
    if (item.parentPath() == m_path)
        m_explorer->setCurrentItem(new WidgetDiskItem(m_explorer, item, m_preview_mode), QItemSelectionModel::ClearAndSelect);

    m_tasks->removeChildTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_delete_triggered()
{
    QList<QListWidgetItem*> selected = m_explorer->selectedItems();

    if (selected.count() == 0)
        return;

    if (QMessageBox::question(this, ATTENTION_MESSAGE, ASK_DELETE_MESSAGE, QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes)
        return;

    quint64 parent = 0;
    if (selected.count() > 1) {
        parent = EteraAPI::nextID();
        m_tasks->addSimpleTask(parent, ROOT_MESSAGE_RM);
    }

    for (int i = 0; i < selected.count(); i++) {
        WidgetDiskItem*  witem = static_cast<WidgetDiskItem*>(selected[i]);
        const EteraItem* eitem = witem->item();

        EteraAPI* api = createAPI();

        ETERA_API_TASK_RM(api, task_on_rm_success, task_on_rm_error);

        m_tasks->addChildTask(parent, api->id(), START_MESSAGE_RM.arg(eitem->path()));

        api->rm(eitem->path(), true);
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_rm_error(EteraAPI* api)
{
    // если объект не существует, то и удалять нечего
    if (api->lastErrorCode() != 404)
        QMessageBox::critical(this, ERROR_MESSAGE, ERROR_MESSAGE_RM.arg(api->path()).arg(api->lastErrorMessage()));

    // если ошибка во время ожидания операции, то скорее всего объект будет удален
    // если объект был удален ранее, то его нужно удалить из виджета
    if (/* TODO: async == true ||*/ api->lastErrorCode() == 404) {
        removeByPath(api->path());
        EteraClipboard::instance()->removeByPath(api->path());
    }

    m_tasks->removeChildTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_rm_success(EteraAPI* api)
{
    removeByPath(api->path());

    EteraClipboard::instance()->removeByPath(api->path());

    m_tasks->removeChildTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_rename_triggered()
{
    QList<QListWidgetItem*> selected = m_explorer->selectedItems();

    if (selected.count() == 1) {
        WidgetDiskItem* witem = static_cast<WidgetDiskItem*>(m_explorer->currentItem());
        witem->setFlags(witem->flags() | Qt::ItemIsEditable);
        m_explorer->editItem(witem);
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::item_end_edit(QWidget* editor, QAbstractItemDelegate::EndEditHint /*hint*/)
{
    WidgetDiskItem* witem = static_cast<WidgetDiskItem*>(m_explorer->currentItem());
    if (witem == NULL)
        return;

    witem->setFlags(witem->flags() ^ Qt::ItemIsEditable);

    const EteraItem* eitem = witem->item();

    QString value = reinterpret_cast<QLineEdit*>(editor)->text();

    if (value == eitem->name())
        return;

    // базовые проверки имени
    QStringList chars;
    chars << "<" << ">" << ":" << "\"" << "/" << "\\" << "|" << "?" << "*" << "\r" << "\n";

    for (int i = 0; i < chars.count(); i++)
        if (value.contains(chars[i]) == true) {
            witem->revertText();
            QMessageBox::warning(this, ATTENTION_MESSAGE, ERROR_MESSAGE_RENAME_INVALID_CHAR.arg(chars[i]));
            return;
        }

    if (value == "." || value == "..") {
        witem->revertText();
        QMessageBox::warning(this, ATTENTION_MESSAGE, ERROR_MESSAGE_RENAME_IVALID_NAME);
        return;
    }

    QString path = m_path + value;

    EteraAPI* api = createAPI();

    ETERA_API_TASK_MV(api, task_on_rename_success, task_on_rename_error);

    m_tasks->addSimpleTask(api->id(), START_MESSAGE_RENAME.arg(eitem->path()).arg(path));

    api->mv(eitem->path(), path, false);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_rename_error(EteraAPI* api)
{
    // откат переименования
    WidgetDiskItem* witem = findByPath(api->source());
    if (witem != NULL)
        witem->revertText();

    if (api->lastErrorCode() == 409)
        QMessageBox::critical(this, ERROR_MESSAGE, ERROR_MESSAGE_RENAME_ALREADY_EXISTS.arg(api->source()).arg(api->target()));
    else
        QMessageBox::critical(this, ERROR_MESSAGE, ERROR_MESSAGE_RENAME.arg(api->source()).arg(api->target()).arg(api->lastErrorMessage()));

    m_tasks->removeSimpleTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_rename_success(EteraAPI* api)
{
    EteraClipboard::instance()->removeByPath(api->source());

    ETERA_API_CONTINUE_TASK_STAT(api, task_on_rename_stat_success, task_on_rename_stat_error, task_on_rename_error);

    m_tasks->addSimpleTask(api->id(), START_MESSAGE_STAT.arg(api->target()));

    api->stat(api->target(), m_preview_arg, true);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_rename_stat_error(EteraAPI* api)
{
    QMessageBox::critical(this, ERROR_MESSAGE, ERROR_MESSAGE_STAT.arg(api->target()).arg(api->lastErrorMessage()));

    m_tasks->removeSimpleTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_rename_stat_success(EteraAPI* api, const EteraItem& item)
{
    WidgetDiskItem* witem = findByPath(api->source());
    if (witem != NULL)
        witem->replaceItem(item, m_preview_mode);

    m_tasks->removeSimpleTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_share_triggered()
{
    shareObjects(true);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_revoke_triggered()
{
    shareObjects(false);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::shareObjects(bool share)
{
    QList<QListWidgetItem*> selected = m_explorer->selectedItems();

    int count = selected.count();

    if (count == 0)
        return;

    quint64 parent = 0;
    if (count > 1) {
        parent = EteraAPI::nextID();
        if (share == true)
            m_tasks->addSimpleTask(parent, ROOT_MESSAGE_PUBLISH);
        else
            m_tasks->addSimpleTask(parent, ROOT_MESSAGE_UNPUBLISH);
    }

    for (int i = 0; i < count; i++) {
        WidgetDiskItem*  witem = static_cast<WidgetDiskItem*>(selected[i]);
        const EteraItem* eitem = witem->item();

        EteraAPI* api = createAPI();

        if (share == true) {
            ETERA_API_TASK_PUBLISH(api, task_on_publish_success, task_on_publish_error);

            m_tasks->addChildTask(parent, api->id(), START_MESSAGE_PUBLISH.arg(eitem->path()));

            api->publish(eitem->path());
        } else {
            ETERA_API_TASK_UNPUBLISH(api, task_on_unpublish_success, task_on_unpublish_error);

            m_tasks->addChildTask(parent, api->id(), START_MESSAGE_UNPUBLISH.arg(eitem->path()));

            api->unpublish(eitem->path());
        }
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_publish_error(EteraAPI* api)
{
    QMessageBox::critical(this, ERROR_MESSAGE, ERROR_MESSAGE_PUBLISH.arg(api->path()).arg(api->lastErrorMessage()));

    m_tasks->removeChildTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_publish_success(EteraAPI* api)
{
    WidgetDiskItem* witem = findByPath(api->path());

    if (witem == NULL) {
        m_tasks->removeChildTask(api->id());
        api->deleteLater();
    } else {
        ETERA_API_CONTINUE_TASK_STAT(api, task_on_publish_stat_success, task_on_publish_stat_error, task_on_publish_error);

        m_tasks->addChildTask(api->parentId(), api->id(), START_MESSAGE_STAT.arg(api->path()));

        api->stat(api->path());
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_unpublish_error(EteraAPI* api)
{
    QMessageBox::critical(this, ERROR_MESSAGE, ERROR_MESSAGE_UNPUBLISH.arg(api->path()).arg(api->lastErrorMessage()));

    m_tasks->removeChildTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_unpublish_success(EteraAPI* api)
{
    WidgetDiskItem* witem = findByPath(api->path());

    if (witem == NULL) {
        m_tasks->removeChildTask(api->id());
        api->deleteLater();
    } else {
        ETERA_API_CONTINUE_TASK_STAT(api, task_on_unpublish_stat_success, task_on_unpublish_stat_error, task_on_unpublish_error);

        m_tasks->addChildTask(api->parentId(), api->id(), START_MESSAGE_STAT.arg(api->path()));

        api->stat(api->path());
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_publish_stat_error(EteraAPI* api)
{
    QMessageBox::critical(this, ERROR_MESSAGE, ERROR_MESSAGE_STAT.arg(api->path()).arg(api->lastErrorMessage()));

    m_tasks->removeChildTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_publish_stat_success(EteraAPI* api, const EteraItem& item)
{
    WidgetDiskItem* witem = findByPath(item.path());

    if (witem != NULL)
        witem->replaceItem(item, m_preview_mode);

    m_tasks->removeChildTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_unpublish_stat_error(EteraAPI* api)
{
    QMessageBox::critical(this, ERROR_MESSAGE, ERROR_MESSAGE_STAT.arg(api->path()).arg(api->lastErrorMessage()));

    m_tasks->removeChildTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_unpublish_stat_success(EteraAPI* api, const EteraItem& item)
{
    WidgetDiskItem* witem = findByPath(item.path());

    if (witem != NULL)
        witem->replaceItem(item, m_preview_mode);

    m_tasks->removeChildTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_info_triggered()
{
    WidgetDiskItem* witem = static_cast<WidgetDiskItem*>(m_explorer->currentItem());
    if (witem == NULL)
        return;

    const EteraItem* eitem = witem->item();

    FormInfo* info = new FormInfo(*eitem, this);
    info->show();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/uri-list") == true && m_path.isEmpty() == false)
        event->acceptProposedAction();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::dropEvent(QDropEvent* event)
{
    if (m_path.isEmpty() == true)
        return;

    if (event->mimeData()->hasFormat("text/uri-list") == true) {
        QStringList list;

        QList<QUrl> urls = event->mimeData()->urls();
        for (int i = 0; i < urls.count(); i++)
            if (urls[i].scheme() == "file")
                list.append(urls[i].toLocalFile());

        if (list.isEmpty() == false)
            putLocalObjects(list);

        event->acceptProposedAction();
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::putLocalObjects(const QStringList& paths)
{
    if (m_path.isEmpty() == true || paths.isEmpty() == true)
        return;

    quint64 fakeid = EteraAPI::nextID();

    // FIX ME
    /*QVariantMap map;
    map["answer"] = QMessageBox::NoButton;*/

    m_tasks->addSimpleTask(fakeid, trUtf8("Загрузка на Диск")/*, map*/);

    for (int i = 0; i < paths.count(); i++)
        putLocalObject(paths[i], fakeid);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::putLocalObject(const QString& path, quint64 parent)
{
    if (m_path.isEmpty() == true)
        return;

    QFileInfo info(path);

    // пропускаем симлинки и bundle
    if (info.isDir() == false && info.isFile() == false)
        return;

    QString target = m_path + info.fileName();

    if (info.isDir() == true)
        putLocalDir(path, target, false, parent);
    else if (info.isFile() == true)
        putLocalFile(path, target, false, parent);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_rm_error(EteraAPI* api)
{
    QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка удаления %1:\n%2").arg(api->path()).arg(api->lastErrorMessage()));

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_rm_success(EteraAPI* api)
{
    if (api->ensure() == eitDir)
        putLocalDir(api->source(), api->target(), api->overwrite(), api->parentId());
    else if (api->ensure() == eitFile)
        putLocalFile(api->source(), api->target(), api->overwrite(), api->parentId());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_ensure_error(EteraAPI* api)
{
    QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка чтения информации о %1:\n%2").arg(api->path()).arg(api->lastErrorMessage()));

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_ensure_success(EteraAPI* api, const EteraItem& item)
{
    api->deleteLater();

    if (api->ensure() == eitDir) {
        if (item.isDir() == true) {
            syncLocalDir(api->source(), api->target(), api->overwrite(), api->parentId());
            return;
        } else if (item.isFile() == true) {
            //quint64     rootid   = m_tasks->rootID(api->parentId());
            //QVariantMap rootargs = m_tasks->args(rootid); FIXME

            QMessageBox::StandardButton answer;
            // (QMessageBox::StandardButton)rootargs.value("answer", QMessageBox::NoButton).toInt();

            // пропускаем если ранее было указано не перезаписывать файлы
            if (answer != QMessageBox::NoToAll) {
                // что делать с конфликтами?
                if (answer != QMessageBox::YesToAll) {
                    answer = QMessageBox::question(this, trUtf8("Файл уже существует!"), trUtf8("Файл %1 уже существует, перезаписать?").arg(api->target()), QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll);

                    // обновляем ответ FIXME
                    /*rootargs["answer"] = answer;
                    m_tasks->setArgs(rootid, rootargs);*/
                }

                // удалить и перезаписать директорией при положительном ответе
                if (answer == QMessageBox::Yes || answer == QMessageBox::YesToAll) {
                    bool overwrite = (api->overwrite() == true || answer == QMessageBox::YesToAll);

                    api->setOverwrite(overwrite);

                    ETERA_API_CONTINUE_TASK_RM(api, task_on_put_rm_success, task_on_put_rm_error, task_on_put_ensure_error);

                    api->rm(api->target(), true);
                }
            }
        }
    } else if (api->ensure() == eitFile) {
        //quint64     rootid   = m_tasks->rootID(api->parentId());
        //QVariantMap rootargs = m_tasks->args(rootid); FIXME

        QMessageBox::StandardButton answer;
        // (QMessageBox::StandardButton)rootargs.value("answer", QMessageBox::NoButton).toInt();

        // пропускаем если ранее было указано не перезаписывать файлы
        if (answer != QMessageBox::NoToAll) {
            // что делать с конфликтами?
            if (answer != QMessageBox::YesToAll) {
                answer = QMessageBox::question(this, trUtf8("Файл уже существует!"), trUtf8("Файл %1 уже существует, перезаписать?").arg(api->target()), QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll);

                // обновляем ответ
                /*rootargs["answer"] = answer; FIXME
                m_tasks->setArgs(rootid, rootargs);*/
            }

            // удалить и перезаписать файлом при положительном ответе
            if (answer == QMessageBox::Yes || answer == QMessageBox::YesToAll) {
                bool overwrite = (api->overwrite() == true || answer == QMessageBox::YesToAll);

                if (item.isFile() == true) {
                    putLocalFile(api->source(), api->target(), overwrite, api->parentId());
                } else if (item.isDir() == true) {
                    api->setOverwrite(overwrite);

                    ETERA_API_CONTINUE_TASK_RM(api, task_on_put_rm_success, task_on_put_rm_error, task_on_put_ensure_error);

                    api->rm(api->target(), true);
                }
            }
        }
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_stat_error(EteraAPI* api)
{
    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_stat_success(EteraAPI* api, const EteraItem& item)
{
    if (item.parentPath() == m_path) {
        WidgetDiskItem* witem = findByPath(item.path());

        if (witem == NULL)
            m_explorer->setCurrentItem(new WidgetDiskItem(m_explorer, item, m_preview_mode), QItemSelectionModel::ClearAndSelect);
        else
            witem->replaceItem(item, m_preview_mode);
    }

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_file_progress(EteraAPI* api, qint64 done, qint64 total)
{
    m_tasks->setProgress(api->id(), done, total);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_file_error(EteraAPI* api)
{
    // если объект существует, нужно убедиться, что это файл и тогда можно продолжить работу
    if (api->lastErrorCode() == 409) {
        //quint64     rootid   = m_tasks->rootID(api->id());
        //QVariantMap rootargs = m_tasks->args(rootid); FIXME

        QMessageBox::StandardButton answer;
        // (QMessageBox::StandardButton)rootargs.value("answer", QMessageBox::NoButton).toInt();

        // пропускаем если ранее было указано не перезаписывать файлы
        if (answer != QMessageBox::NoToAll) {
            EteraAPI* api = createAPI();

            api->setEnsure(eitFile);

            ETERA_API_TASK_STAT(api, task_on_put_ensure_success, task_on_put_ensure_error);

            api->stat(api->target());
        }
    } else {
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка загрузки %1 в %2:\n%3").arg(api->source()).arg(api->target()).arg(api->lastErrorMessage()));
        m_tasks->removeChildTask(api->id());
        api->deleteLater();
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_file_success(EteraAPI* api)
{
    api->device()->deleteLater();

    ETERA_API_CONTINUE_TASK_STAT(api, task_on_put_stat_success, task_on_put_stat_error, task_on_put_file_error);

    api->stat(api->target());
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::putLocalFile(const QString& source, const QString& target, bool overwrite, quint64 parent)
{
    EteraAPI* api = createAPI();

    api->setParentId(parent);

    m_tasks->addChildTask(parent, api->id(), source);

    ETERA_API_TASK_PUT(api, task_on_put_file_success, task_on_put_file_error, task_on_put_file_progress);

    api->put(source, target, overwrite);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_dir_stat_error(EteraAPI* api)
{
    QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка создания %1:\n%2").arg(api->path()).arg(api->lastErrorMessage()));

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_dir_stat_success(EteraAPI* api, const EteraItem& item)
{
    if (item.parentPath() == m_path)
        m_explorer->setCurrentItem(new WidgetDiskItem(m_explorer, item, m_preview_mode), QItemSelectionModel::ClearAndSelect);

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_dir_error(EteraAPI* api)
{
    // если объект существует, нужно убедиться, что это директория и тогда можно продолжить работу
    if (api->lastErrorCode() == 409) {
        EteraAPI* api = createAPI();

        api->setEnsure(eitDir);

        ETERA_API_TASK_STAT(api, task_on_put_ensure_success, task_on_put_ensure_error);

        api->stat(api->target());
    } else {
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка создания %1:\n%2").arg(api->target()).arg(api->lastErrorMessage()));
        api->deleteLater();
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_dir_success(EteraAPI* api)
{
    ETERA_API_CONTINUE_TASK_STAT(api, task_on_put_dir_stat_success, task_on_put_dir_stat_error, task_on_put_dir_error);

    api->stat(api->path());

    syncLocalDir(api->source(), api->target(), api->overwrite(), api->parentId());
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::putLocalDir(const QString& source, const QString& target, bool overwrite, quint64 parent)
{
    EteraAPI* api = createAPI();

    api->setSource(source);
    api->setTarget(target);
    api->setParentId(parent);
    api->setOverwrite(overwrite);

    ETERA_API_TASK_MKDIR(api, task_on_put_dir_success, task_on_put_dir_error);

    api->mkdir(target);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::syncLocalDir(const QString& source, const QString& target, bool overwrite, quint64 parent)
{
    QFileInfoList list = QDir(source).entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);

    for (int i = 0; i < list.count(); i++) {
        QFileInfo info = list[i];

        if (info.isDir() == true)
            putLocalDir(info.absoluteFilePath(), target + "/" + info.fileName(), overwrite, parent);
        else if (info.isFile() == true)
            putLocalFile(info.absoluteFilePath(), target + "/" + info.fileName(), overwrite, parent);
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::on_item_selection_changed()
{
    if (m_explorer->selectedItems().count() == 0)
        emit onChangePossibleActions(false);
    else
        emit onChangePossibleActions(true);

    QList<QListWidgetItem*> selected = m_explorer->selectedItems();

    bool e = (selected.isEmpty() == false);
    m_menu_cut->setEnabled(e);
    m_menu_copy->setEnabled(e);
    m_menu_delete->setEnabled(e);

    if (selected.count() == 1) {
        WidgetDiskItem*  witem = static_cast<WidgetDiskItem*>(selected[0]);
        const EteraItem* eitem = witem->item();
        if (eitem->isDir() == true)
            m_menu_open->setEnabled(true);
        else
            m_menu_open->setEnabled(false);

        m_menu_rename->setEnabled(true);
        m_menu_info->setEnabled(true);
    } else {
        m_menu_open->setEnabled(false);
        m_menu_rename->setEnabled(false);
        m_menu_info->setEnabled(false);
    }

    bool can_share  = false;
    bool can_revoke = false;
    for (int i = 0; i < selected.count(); i++) {
        WidgetDiskItem*  witem = static_cast<WidgetDiskItem*>(selected[i]);
        const EteraItem* eitem = witem->item();

        if (eitem->isPublic() == true)
            can_revoke = true;
        else
            can_share = true;

        if (can_share == true && can_revoke == true)
            break;
    }

    m_menu_share->setEnabled(can_share);
    m_menu_revoke->setEnabled(can_revoke);

    EteraClipboard* clipboard = EteraClipboard::instance();

    m_menu_paste->setEnabled(clipboard->isEmpty() == false);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::getRemoteObjects(const QString& path)
{
    QList<QListWidgetItem*> selected = m_explorer->selectedItems();

    int count = selected.count();

    if (count == 0)
        return;

    quint64 fakeid = EteraAPI::nextID();

    /*QVariantMap map; FIXME
    map["answer"] = QMessageBox::NoButton;*/

    m_tasks->addSimpleTask(fakeid, trUtf8("Сохранение с Диска")/*, map*/);

    for (int i = 0; i < count; i++) {
        WidgetDiskItem*  witem = static_cast<WidgetDiskItem*>(selected[i]);
        const EteraItem* eitem = witem->item();

        if (eitem->isDir() == true)
            getRemoteDir(eitem->path(), path + "/" + eitem->name(), fakeid);
        else if (eitem->isFile() == true)
            getRemoteFile(eitem->path(), path + "/" + eitem->name(), fakeid);
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_get_file_progress(EteraAPI* api, qint64 done, qint64 total)
{
    m_tasks->setProgress(api->id(), done, total);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_get_file_error(EteraAPI* api)
{
    QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка загрузки %1 в %2:\n%3").arg(api->source()).arg(api->target()).arg(api->lastErrorMessage()));

    m_tasks->removeChildTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_get_file_success(EteraAPI* api)
{
    m_tasks->removeChildTask(api->id());

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::getRemoteFile(const QString& source, const QString& target, quint64 parent)
{
    QFileInfo info(target);

    if (info.exists() == true) {
        //quint64     rootid   = m_tasks->rootID(parent);
        //QVariantMap rootargs = m_tasks->args(rootid); FIXME

        QMessageBox::StandardButton answer;
        // (QMessageBox::StandardButton)rootargs.value("answer", QMessageBox::NoButton).toInt();

        // ранее было указано не перезаписывать файлы
        if (answer == QMessageBox::NoToAll)
            return;

        // что делать с конфликтами?
        if (answer != QMessageBox::YesToAll) {
            answer = QMessageBox::question(this, trUtf8("Файл уже существует!"), trUtf8("Файл %1 уже существует, перезаписать?").arg(target), QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll);

            // обновляем ответ FIXME
            /*rootargs["answer"] = answer;
            m_tasks->setArgs(rootid, rootargs);*/

            // оставить локальную копию
            if (answer == QMessageBox::No || answer == QMessageBox::NoToAll)
                return;
        }

        // QMessageBox::YesToAll
        if (info.isDir() == true) {
            removeDir(target);
        } else if (info.isFile() == true || info.isSymLink() == true) {
            if (info.dir().remove(info.absoluteFilePath()) == false) {
                QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка удаления %1").arg(info.absoluteFilePath()));
                return;
            }
        } else {
            QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Неизвестный объект %1").arg(info.absoluteFilePath()));
            return;
        }
    }

    EteraAPI* api = createAPI();

    api->setParentId(parent);

    ETERA_API_TASK_GET(api, task_on_get_file_success, task_on_get_file_error, task_on_get_file_progress);

    m_tasks->addChildTask(parent, api->id(), source);

    api->get(source, target);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_get_dir_error(EteraAPI* api)
{
    QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка чтения %1:\n%2").arg(api->path()).arg(api->lastErrorMessage()));

    api->deleteLater();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_get_dir_success(EteraAPI* api, const EteraItemList& list, quint64 limit)
{
    for (int i = 0; i < list.count(); i++) {
        EteraItem item = list[i];
        if (item.isDir() == true)
            getRemoteDir(item.path(), api->target() + "/" + item.name(), api->parentId());
        else if (item.isFile() == true)
            getRemoteFile(item.path(), api->target() + "/" + item.name(), api->parentId());
    }

    if ((quint64)list.count() < limit)
        api->deleteLater();
    else {
        quint64 offset = api->offset() + limit;
        api->ls(api->path(), api->preview(), api->crop(), offset, limit);
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::getRemoteDir(const QString& source, const QString& target, quint64 parent)
{
    QFileInfo info(target);

    if (info.exists() == false && info.dir().mkdir(info.absoluteFilePath()) == false) {
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка создания %1").arg(info.absoluteFilePath()));
        return;
    } else if (info.exists() == true && info.isDir() == false) {
        //quint64     rootid   = m_tasks->rootID(parent);
        //QVariantMap rootargs = m_tasks->args(rootid); FIXME

        QMessageBox::StandardButton answer;
        // (QMessageBox::StandardButton)rootargs.value("answer", QMessageBox::NoButton).toInt();

        // ранее было указано не перезаписывать файлы
        if (answer == QMessageBox::NoToAll)
            return;

        // что делать с конфликтами?
        if (answer != QMessageBox::YesToAll) {
            answer = QMessageBox::question(this, trUtf8("Файл уже существует!"), trUtf8("Файл %1 уже существует, перезаписать?").arg(target), QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll);

            // обновляем ответ FIXME
            /*rootargs["answer"] = answer;
            m_tasks->setArgs(rootid, rootargs);*/

            // оставить локальную копию
            if (answer == QMessageBox::No || answer == QMessageBox::NoToAll)
                return;
        }

        // QMessageBox::YesToAll
        if (info.dir().remove(info.absoluteFilePath()) == false) {
            QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка удаления %1").arg(info.absoluteFilePath()));
            return;
        }
    }

    EteraAPI* api = createAPI();

    api->setTarget(target);
    api->setParentId(parent);

    ETERA_API_TASK_LS(api, task_on_get_dir_success, task_on_get_dir_error);

    api->ls(source);
}
//----------------------------------------------------------------------------------------------

bool WidgetDisk::removeDir(QDir dir)
{
    if (dir.exists() == false)
        return false;

    QFileInfoList list = dir.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);

    for (int i = 0; i < list.count(); i++) {
        QFileInfo info = list[i];

        if (info.isFile() == true || info.isSymLink() == true) {
            if (dir.remove(info.absoluteFilePath()) == false) {
                QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка удаления %1").arg(info.absoluteFilePath()));
                return false;
            }
        } else if (info.isDir() == true) {
            if (removeDir(info.absoluteFilePath()) == false)
                return false;
        }
    }

    if (dir.rmdir(dir.absolutePath()) == false) {
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка удаления %1").arg(dir.absolutePath()));
        return false;
    }

    return true;
}
//----------------------------------------------------------------------------------------------

int WidgetDisk::setZoomFactor(int factor)
{
    const QList<int>* sizes = EteraIconProvider::instance()->iconSizes();

    if (factor < 0 || factor >= sizes->count())
        factor = EteraIconProvider::instance()->defaultIconSizeIndex();

    m_icon_size_index = factor;

    int size = sizes->at(m_icon_size_index);

    m_explorer->setIconSize(QSize(size, size));

    if (m_icon_size_index == 0)
        return -1;
    else if (m_icon_size_index == sizes->count() - 1)
        return 1;

    return 0;
}
//----------------------------------------------------------------------------------------------

bool WidgetDisk::zoomIn()
{
    const QList<int>* sizes = EteraIconProvider::instance()->iconSizes();

    if (m_icon_size_index < sizes->count() - 1)
        m_icon_size_index++;

    int size = sizes->at(m_icon_size_index);

    m_explorer->setIconSize(QSize(size, size));

    return (m_icon_size_index == sizes->count() - 1 ? false : true);
}
//----------------------------------------------------------------------------------------------

bool WidgetDisk::zoomOut()
{
    const QList<int>* sizes = EteraIconProvider::instance()->iconSizes();

    if (m_icon_size_index != 0)
        m_icon_size_index--;

    int size = sizes->at(m_icon_size_index);

    m_explorer->setIconSize(QSize(size, size));

    return (m_icon_size_index == 0 ? false : true);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::setPreviewMode(bool mode)
{
    m_preview_mode = mode;

    if (m_preview_mode == false)
        EteraIconProvider::instance()->cancelPreview();

    for (int i = 0; i < m_explorer->count(); i++) {
        WidgetDiskItem* item = static_cast<WidgetDiskItem*>(m_explorer->item(i));
        item->update(m_preview_mode);
    }
}
//----------------------------------------------------------------------------------------------
