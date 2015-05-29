#include "widget_disk.h"
//----------------------------------------------------------------------------------------------
#include "widget_disk_item_delegate.h"
//----------------------------------------------------------------------------------------------
#include "tasks/all.h"
#include "utils/pool.h"
#include "utils/icon.h"
#include "utils/settings.h"
#include "utils/clipboard.h"
//----------------------------------------------------------------------------------------------
#include "forms/form_info.h"
//----------------------------------------------------------------------------------------------

WidgetDisk::WidgetDisk(QWidget* parent) : QTabWidget(parent)
{
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
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::wheelEvent(QWheelEvent* event)
{
    m_explorer->horizontalScrollBar()->event(event);
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

void WidgetDisk::task_on_start(quint64 id, const QString& message, const QVariantMap& args)
{
    quint64 parent = args.value("parent", 0).toULongLong();

    if (parent == 0)
        m_tasks->addSimpleTask(id, message, args);
    else
        m_tasks->addChildTask(parent, id, message, args);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_ls_error(quint64 id, int /*code*/, const QString& error, const QVariantMap& args)
{
    QString path = args["path"].toString();

    QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка чтения %1:\n%2").arg(path).arg(error));

    m_tasks->removeSimpleTask(id);

    m_explorer->setCursor(Qt::ArrowCursor);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_ls_success(quint64 id, const EteraItemList& list, const QVariantMap& args)
{
    QString path = args["path"].toString();

    m_explorer->clear();

    for (int i = 0; i < list.count(); i++)
        new WidgetDiskItem(m_explorer, list[i]);

    emit onPathChanged(path);

    m_path = path;

    m_tasks->removeSimpleTask(id);

    m_explorer->setCursor(Qt::ArrowCursor);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::changePath(const QString& path)
{
    m_explorer->setCursor(Qt::BusyCursor);

    m_path = "";
    m_explorer->clear();

    QString _path = path;
    if (_path.endsWith("/") == false)
        _path += "/";

    int     size    = EteraIconProvider::instance()->maxIconSize();
    QString preview = QString("%1x%2").arg(size).arg(size);

    EteraTaskLS* ls = new EteraTaskLS(_path, preview, true);

    connect(ls, SIGNAL(onStart(quint64, const QString&, const QVariantMap&)), this, SLOT(task_on_start(quint64, const QString&, const QVariantMap&)));
    connect(ls, SIGNAL(onSuccess(quint64, const EteraItemList&, const QVariantMap&)), this, SLOT(task_on_ls_success(quint64, const EteraItemList&, const QVariantMap&)));
    connect(ls, SIGNAL(onError(quint64, int, const QString&, const QVariantMap&)), this, SLOT(task_on_ls_error(quint64, int, const QString&, const QVariantMap&)));

    EteraThreadPool::globalInstance()->start(ls);
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

void WidgetDisk::task_on_mkdir_error(quint64 id, int code, const QString& error, const QVariantMap& args)
{
    QString path = args["path"].toString();

    // CONFLICT, директория уже существует
    if (code == 409)
        QMessageBox::warning(this, trUtf8("Внимание!"), trUtf8("Директория %1 уже существует").arg(path));
    else
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка создания %1:\n%2").arg(path).arg(error));

    m_tasks->removeSimpleTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_mkdir_success(quint64 id, const EteraItem& item, const QVariantMap& /*args*/)
{
    if (item.parentPath() == m_path)
        m_explorer->setCurrentItem(new WidgetDiskItem(m_explorer, item), QItemSelectionModel::ClearAndSelect);

    m_tasks->removeSimpleTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_new_triggered()
{
    if (m_path.isEmpty() == true)
        return;

    bool ok;
    QString value = QInputDialog::getText(this, trUtf8("Создать директорию"), trUtf8("Введите имя новой директории"), QLineEdit::Normal, trUtf8("Новая папка"), &ok);
    if (ok == false || value.isEmpty() == true)
        return;

    QString path = m_path + value;

    EteraTaskMKDIR* mkdir = new EteraTaskMKDIR(path);

    connect(mkdir, SIGNAL(onStart(quint64, const QString&, const QVariantMap&)), this, SLOT(task_on_start(quint64, const QString&, const QVariantMap&)));
    connect(mkdir, SIGNAL(onSuccess(quint64, const EteraItem&, const QVariantMap&)), this, SLOT(task_on_mkdir_success(quint64, const EteraItem&, const QVariantMap&)));
    connect(mkdir, SIGNAL(onError(quint64, int, const QString&, const QVariantMap&)), this, SLOT(task_on_mkdir_error(quint64, int, const QString&, const QVariantMap&)));

    EteraThreadPool::globalInstance()->start(mkdir);
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

void WidgetDisk::task_on_copy_paste_error(quint64 id, int code, const QString& error, bool /*async*/, const QVariantMap& args)
{
    QString source = args["source"].toString();
    QString target = args["target"].toString();

    // CONFLICT, объект уже существует
    if (code == 409)
        QMessageBox::warning(this, trUtf8("Внимание!"), trUtf8("Ошибка копирования %1:\n%2 уже существует").arg(source).arg(target));
    else
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка копирования %1 в %2:\n%3").arg(source).arg(target).arg(error));

    // TODO: обработать асинхронную ошибку, т.к. копирование могло быть все же успешным

    m_tasks->removeSimpleTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_copy_paste_success(quint64 id, const EteraItem& item, const QVariantMap& args)
{
    if (item.parentPath() == m_path)
        m_explorer->setCurrentItem(new WidgetDiskItem(m_explorer, item), QItemSelectionModel::ClearAndSelect);

    QString source = args["source"].toString();

    EteraClipboard* clipboard = EteraClipboard::instance();

    clipboard->removeByPath(source);

    m_tasks->removeSimpleTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_cut_paste_error(quint64 id, int code, const QString& error, bool /*async*/, const QVariantMap& args)
{
    QString source = args["source"].toString();
    QString target = args["target"].toString();

    // CONFLICT, объект уже существует
    if (code == 409)
        QMessageBox::warning(this, trUtf8("Внимание!"), trUtf8("Ошибка перемещения %1:\n%2 уже существует").arg(source).arg(target));
    else
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка перемещения %1 в %2:\n%3").arg(source).arg(target).arg(error));

    // TODO: обработать асинхронную ошибку, т.к. вставка могла быть все же успешной
    m_tasks->removeSimpleTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_cut_paste_success(quint64 id, const EteraItem& item, const QVariantMap& args)
{
    if (item.parentPath() == m_path)
        m_explorer->setCurrentItem(new WidgetDiskItem(m_explorer, item), QItemSelectionModel::ClearAndSelect);

    QString source = args["source"].toString();

    EteraClipboard* clipboard = EteraClipboard::instance();

    clipboard->removeByPath(source);

    m_tasks->removeSimpleTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_paste_triggered()
{
    EteraClipboard* clipboard = EteraClipboard::instance();

    if (clipboard->isEmpty() == true || m_path.isEmpty() == true)
        return;

    for (int i = 0; i < clipboard->count(); i++) {
        EteraItem src = clipboard->at(i);
        QString   dst = m_path + src.name();

        // нет смысла перемещать само в себя
        if (src.path() == dst) {
            QMessageBox::warning(this, trUtf8("Внимание!"), trUtf8("Источник и приемник совпадают"));
            return;
        }

        if (clipboard->copyMode() == true) {
            EteraTaskCP* cp = new EteraTaskCP(src.path(), dst, false);

            connect(cp, SIGNAL(onStart(quint64, const QString&, const QVariantMap&)), this, SLOT(task_on_start(quint64, const QString&, const QVariantMap&)));
            connect(cp, SIGNAL(onSuccess(quint64, const EteraItem&, const QVariantMap&)), this, SLOT(task_on_copy_paste_success(quint64, const EteraItem&, const QVariantMap&)));
            connect(cp, SIGNAL(onError(quint64, int, const QString&, bool, const QVariantMap&)), this, SLOT(task_on_copy_paste_error(quint64, int, const QString&, bool, const QVariantMap&)));

            EteraThreadPool::globalInstance()->start(cp);
        } else {
            EteraTaskMV* mv = new EteraTaskMV(src.path(), dst, false);

            connect(mv, SIGNAL(onStart(quint64, const QString&, const QVariantMap&)), this, SLOT(task_on_start(quint64, const QString&, const QVariantMap&)));
            connect(mv, SIGNAL(onSuccess(quint64, const EteraItem&, const QVariantMap&)), this, SLOT(task_on_cut_paste_success(quint64, const EteraItem&, const QVariantMap&)));
            connect(mv, SIGNAL(onError(quint64, int, const QString&, bool, const QVariantMap&)), this, SLOT(task_on_cut_paste_error(quint64, int, const QString&, bool, const QVariantMap&)));

            EteraThreadPool::globalInstance()->start(mv);
        }
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_rm_error(quint64 id, int code, const QString& error, bool async, const QVariantMap& args)
{
    QString path = args["path"].toString();

    // Если объект не существует, то и удалять нечего
    if (code != 404)
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка удаления %1:\n%2").arg(path).arg(error));

    // если ошибка во время ожидания операции, то скорее всего объект будет удален
    // если объект был удален ранее, то его нужно удалить из виджета
    if (async == true || code == 404) {
        // удаление из виджета
        removeByPath(path);

        // удаление из буфера обмена
        EteraClipboard* clipboard = EteraClipboard::instance();

        clipboard->removeByPath(path);
    }

    m_tasks->removeSimpleTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_rm_success(quint64 id, const QVariantMap& args)
{
    QString path = args["path"].toString();

    // удаление из виджета
    removeByPath(path);

    // удаление из буфера обмена
    EteraClipboard* clipboard = EteraClipboard::instance();

    clipboard->removeByPath(path);

    m_tasks->removeSimpleTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_delete_triggered()
{
    QList<QListWidgetItem*> selected = m_explorer->selectedItems();

    if (selected.count() == 0)
        return;

    if (QMessageBox::question(this, trUtf8("Внимание!"), trUtf8("Вы уверены, что хотите удалить выбранные файлы?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes)
        return;

    for (int i = 0; i < selected.count(); i++) {
        WidgetDiskItem*  witem = static_cast<WidgetDiskItem*>(selected[i]);
        const EteraItem* eitem = witem->item();

        EteraTaskRM* rm = new EteraTaskRM(eitem->path(), true);

        connect(rm, SIGNAL(onStart(quint64, const QString&, const QVariantMap&)), this, SLOT(task_on_start(quint64, const QString&, const QVariantMap&)));
        connect(rm, SIGNAL(onSuccess(quint64, const QVariantMap&)), this, SLOT(task_on_rm_success(quint64, const QVariantMap&)));
        connect(rm, SIGNAL(onError(quint64, int, const QString&, bool, const QVariantMap&)), this, SLOT(task_on_rm_error(quint64, int, const QString&, bool, const QVariantMap&)));

        EteraThreadPool::globalInstance()->start(rm);
    }
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

void WidgetDisk::task_on_rename_error(quint64 id, int code, const QString& error, bool /*async*/, const QVariantMap& args)
{
    QString source = args["source"].toString();
    QString target = args["target"].toString();

    // откат переименования
    WidgetDiskItem* witem = findByPath(source);
    if (witem != NULL)
        witem->revertText();

    if (code == 409)
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка переименования %1 в %2:\nПриемник уже существует").arg(source).arg(target));
    else
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка переименования %1 в %2:\n%3").arg(source).arg(target).arg(error));

    m_tasks->removeSimpleTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_rename_success(quint64 id, const EteraItem& item, const QVariantMap& args)
{
    QString source = args["source"].toString();

    // удаление из буфера обмена
    EteraClipboard* clipboard = EteraClipboard::instance();

    clipboard->removeByPath(source);

    // замена описателя
    WidgetDiskItem* witem = findByPath(source);
    if (witem != NULL)
        witem->replaceItem(item);

    m_tasks->removeSimpleTask(id);
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
            QMessageBox::warning(this, trUtf8("Внимание!"), trUtf8("Недопустимый символ \"%1\" в имени").arg(chars[i]));
            return;
        }

    if (value == "." || value == "..") {
        witem->revertText();
        QMessageBox::warning(this, trUtf8("Внимание!"), trUtf8("Недопустимое имя"));
        return;
    }

    QString path = m_path + value;

    EteraTaskMV* mv = new EteraTaskMV(eitem->path(), path, false);

    connect(mv, SIGNAL(onStart(quint64, const QString&, const QVariantMap&)), this, SLOT(task_on_start(quint64, const QString&, const QVariantMap&)));
    connect(mv, SIGNAL(onSuccess(quint64, const EteraItem&, const QVariantMap&)), this, SLOT(task_on_rename_success(quint64, const EteraItem&, const QVariantMap&)));
    connect(mv, SIGNAL(onError(quint64, int, const QString&, bool, const QVariantMap&)), this, SLOT(task_on_rename_error(quint64, int, const QString&, bool, const QVariantMap&)));

    EteraThreadPool::globalInstance()->start(mv);
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

    quint64 fakeid = EteraTask::nextID();

    QVariantMap map;
    map["answer"] = QMessageBox::NoButton;

    m_tasks->addSimpleTask(fakeid, trUtf8("Загрузка на Диск"), map);

    for (int i = 0; i < paths.count(); i++)
        putLocalObject(paths[i], fakeid);

    m_tasks->checkWaitTask(fakeid);
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

void WidgetDisk::task_on_put_rm_error(quint64 id, int /*code*/, const QString& error, bool /*async*/, const QVariantMap& args)
{
    QString path = args["path"].toString();

    QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка удаления %1:\n%2").arg(path).arg(error));

    m_tasks->removeChildTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_rm_success(quint64 id, const QVariantMap& args)
{
    QString source    = args["source"].toString();
    QString target    = args["target"].toString();
    quint64 parent    = args["parent"].toULongLong();
    bool    overwrite = args["overwrite"].toBool();
    QString ensure    = args["ensure"].toString();

    if (ensure == "dir")
        putLocalDir(source, target, overwrite, parent);
    else if (ensure == "file")
        putLocalFile(source, target, overwrite, parent);

    m_tasks->removeChildTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_ensure_error(quint64 id, int /*code*/, const QString& error, const QVariantMap& args)
{
    QString path = args["path"].toString();

    QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка чтения информации о %1:\n%2").arg(path).arg(error));

    m_tasks->removeChildTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_ensure_success(quint64 id, const EteraItem& item, const QVariantMap& args)
{
    QString source    = args["source"].toString();
    QString target    = args["target"].toString();
    quint64 parent    = args["parent"].toULongLong();
    bool    overwrite = args["overwrite"].toBool();
    QString ensure    = args["ensure"].toString();

    if (ensure == "dir") {
        if (item.isDir() == true) {
            m_tasks->addChildTask(parent, id, trUtf8("Обработка %1").arg(source), args);
            syncLocalDir(source, target, overwrite, id);
            m_tasks->checkWaitTask(id);
            return;
        } else if (item.isFile() == true) {
            quint64     rootid   = m_tasks->rootID(id);
            QVariantMap rootargs = m_tasks->args(rootid);

            QMessageBox::StandardButton answer = (QMessageBox::StandardButton)rootargs.value("answer", QMessageBox::NoButton).toInt();

            // пропускаем если ранее было указано не перезаписывать файлы
            if (answer != QMessageBox::NoToAll) {
                // что делать с конфликтами?
                if (answer != QMessageBox::YesToAll) {
                    answer = QMessageBox::question(this, trUtf8("Файл уже существует!"), trUtf8("Файл %1 уже существует, перезаписать?").arg(target), QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll);

                    // обновляем ответ
                    rootargs["answer"] = answer;
                    m_tasks->setArgs(rootid, rootargs);
                }

                // удалить и перезаписать директорией при положительном ответе
                if (answer == QMessageBox::Yes || answer == QMessageBox::YesToAll) {
                    overwrite = (overwrite == true || answer == QMessageBox::YesToAll);

                    EteraTaskRM* rm = new EteraTaskRM(target, true);

                    rm->addArg("source", source);
                    rm->addArg("target", target);
                    rm->addArg("parent", parent);
                    rm->addArg("overwrite", overwrite);
                    rm->addArg("ensure", ensure);

                    m_tasks->addWaitTask(parent);

                    connect(rm, SIGNAL(onStart(quint64, const QString&, const QVariantMap&)), this, SLOT(task_on_start(quint64, const QString&, const QVariantMap&)));
                    connect(rm, SIGNAL(onSuccess(quint64, const QVariantMap&)), this, SLOT(task_on_put_rm_success(quint64, const QVariantMap&)));
                    connect(rm, SIGNAL(onError(quint64, int, const QString&, bool, const QVariantMap&)), this, SLOT(task_on_put_rm_error(quint64, int, const QString&, bool, const QVariantMap&)));

                    EteraThreadPool::globalInstance()->start(rm);
                }
            }
        }
    } else if (ensure == "file") {
        quint64     rootid   = m_tasks->rootID(id);
        QVariantMap rootargs = m_tasks->args(rootid);

        QMessageBox::StandardButton answer = (QMessageBox::StandardButton)rootargs.value("answer", QMessageBox::NoButton).toInt();

        // пропускаем если ранее было указано не перезаписывать файлы
        if (answer != QMessageBox::NoToAll) {
            // что делать с конфликтами?
            if (answer != QMessageBox::YesToAll) {
                answer = QMessageBox::question(this, trUtf8("Файл уже существует!"), trUtf8("Файл %1 уже существует, перезаписать?").arg(target), QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll);

                // обновляем ответ
                rootargs["answer"] = answer;
                m_tasks->setArgs(rootid, rootargs);
            }

            // удалить и перезаписать файлом при положительном ответе
            if (answer == QMessageBox::Yes || answer == QMessageBox::YesToAll) {
                overwrite = (overwrite == true || answer == QMessageBox::YesToAll);

                if (item.isFile() == true) {
                    putLocalFile(source, target, overwrite, parent);
                } else if (item.isDir() == true) {
                    EteraTaskRM* rm = new EteraTaskRM(target, true);

                    rm->addArg("source", source);
                    rm->addArg("target", target);
                    rm->addArg("parent", parent);
                    rm->addArg("overwrite", overwrite);
                    rm->addArg("ensure", ensure);

                    m_tasks->addWaitTask(parent);

                    connect(rm, SIGNAL(onStart(quint64, const QString&, const QVariantMap&)), this, SLOT(task_on_start(quint64, const QString&, const QVariantMap&)));
                    connect(rm, SIGNAL(onSuccess(quint64, const QVariantMap&)), this, SLOT(task_on_put_rm_success(quint64, const QVariantMap&)));
                    connect(rm, SIGNAL(onError(quint64, int, const QString&, bool, const QVariantMap&)), this, SLOT(task_on_put_rm_error(quint64, int, const QString&, bool, const QVariantMap&)));

                    EteraThreadPool::globalInstance()->start(rm);
                }
            }
        }
    }

    m_tasks->removeChildTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_file_progress(quint64 id, qint64 done, qint64 total)
{
    m_tasks->setProgress(id, done, total);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_file_error(quint64 id, int code, const QString& error, const QVariantMap& args)
{
    QString source    = args["source"].toString();
    QString target    = args["target"].toString();
    quint64 parent    = args["parent"].toULongLong();
    bool    overwrite = args["overwrite"].toBool();

    // если объект существует, нужно убедиться, что это файл и тогда можно продолжить работу
    if (code == 409) {
        quint64     rootid   = m_tasks->rootID(id);
        QVariantMap rootargs = m_tasks->args(rootid);

        QMessageBox::StandardButton answer = (QMessageBox::StandardButton)rootargs.value("answer", QMessageBox::NoButton).toInt();

        // пропускаем если ранее было указано не перезаписывать файлы
        if (answer != QMessageBox::NoToAll) {
            EteraTaskSTAT* stat = new EteraTaskSTAT(target);

            stat->addArg("source", source);
            stat->addArg("target", target);
            stat->addArg("parent", parent);
            stat->addArg("overwrite", overwrite);
            stat->addArg("ensure", "file");

            m_tasks->addWaitTask(parent);

            stat->setSuccessMutex(&m_question_mutex);

            connect(stat, SIGNAL(onStart(quint64, const QString&, const QVariantMap&)), this, SLOT(task_on_start(quint64, const QString&, const QVariantMap&)));
            connect(stat, SIGNAL(onSuccess(quint64, const EteraItem&, const QVariantMap&)), this, SLOT(task_on_put_ensure_success(quint64, const EteraItem&, const QVariantMap&)), Qt::BlockingQueuedConnection);
            connect(stat, SIGNAL(onError(quint64, int, const QString&, const QVariantMap&)), this, SLOT(task_on_put_ensure_error(quint64, int, const QString&, const QVariantMap&)));

            EteraThreadPool::globalInstance()->start(stat);
        }
    } else
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка загрузки %1 в %2:\n%3").arg(source).arg(target).arg(error));

    m_tasks->removeChildTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_file_success(quint64 id, const EteraItem& item, const QVariantMap& /*args*/)
{
    if (item.parentPath() == m_path) {
        WidgetDiskItem* witem = findByPath(item.path());

        if (witem == NULL)
            m_explorer->setCurrentItem(new WidgetDiskItem(m_explorer, item), QItemSelectionModel::ClearAndSelect);
        else
            witem->replaceItem(item);
    }

    m_tasks->removeChildTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::putLocalFile(const QString& source, const QString& target, bool overwrite, quint64 parent)
{
    EteraTaskPUT* put = new EteraTaskPUT(source, target, overwrite);

    put->addArg("parent", parent);

    m_tasks->addWaitTask(parent);

    connect(put, SIGNAL(onStart(quint64, const QString&, const QVariantMap&)), this, SLOT(task_on_start(quint64, const QString&, const QVariantMap&)));
    connect(put, SIGNAL(onProgress(quint64, qint64, qint64)), this, SLOT(task_on_put_file_progress(quint64, qint64, qint64)));
    connect(put, SIGNAL(onSuccess(quint64, const EteraItem&, const QVariantMap&)), this, SLOT(task_on_put_file_success(quint64, const EteraItem&, const QVariantMap&)));
    connect(put, SIGNAL(onError(quint64, int, const QString&, const QVariantMap&)), this, SLOT(task_on_put_file_error(quint64, int, const QString&, const QVariantMap&)));

    EteraThreadPool::globalInstance()->start(put);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_dir_error(quint64 id, int code, const QString& error, const QVariantMap& args)
{
    QString source    = args["source"].toString();
    QString target    = args["target"].toString();
    quint64 parent    = args["parent"].toULongLong();
    bool    overwrite = args["overwrite"].toBool();

    // если объект существует, нужно убедиться, что это директория и тогда можно продолжить работу
    if (code == 409) {
        EteraTaskSTAT* stat = new EteraTaskSTAT(target);

        stat->addArg("source", source);
        stat->addArg("target", target);
        stat->addArg("parent", parent);
        stat->addArg("overwrite", overwrite);
        stat->addArg("ensure", "dir");

        m_tasks->addWaitTask(parent);

        stat->setSuccessMutex(&m_question_mutex);

        connect(stat, SIGNAL(onStart(quint64, const QString&, const QVariantMap&)), this, SLOT(task_on_start(quint64, const QString&, const QVariantMap&)));
        connect(stat, SIGNAL(onSuccess(quint64, const EteraItem&, const QVariantMap&)), this, SLOT(task_on_put_ensure_success(quint64, const EteraItem&, const QVariantMap&)), Qt::BlockingQueuedConnection);
        connect(stat, SIGNAL(onError(quint64, int, const QString&, const QVariantMap&)), this, SLOT(task_on_put_ensure_error(quint64, int, const QString&, const QVariantMap&)));

        EteraThreadPool::globalInstance()->start(stat);
    } else
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка создания %1:\n%2").arg(target).arg(error));

    m_tasks->removeChildTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_put_dir_success(quint64 id, const EteraItem& item, const QVariantMap& args)
{
    if (item.parentPath() == m_path)
        m_explorer->setCurrentItem(new WidgetDiskItem(m_explorer, item), QItemSelectionModel::ClearAndSelect);

    QString source    = args["source"].toString();
    QString target    = args["target"].toString();
    quint64 parent    = args["parent"].toULongLong();
    bool    overwrite = args["overwrite"].toBool();

    m_tasks->addChildTask(parent, id, trUtf8("Обработка %1").arg(source), args);

    syncLocalDir(source, target, overwrite, id);

    m_tasks->checkWaitTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::putLocalDir(const QString& source, const QString& target, bool overwrite, quint64 parent)
{
    EteraTaskMKDIR* mkdir = new EteraTaskMKDIR(target);

    mkdir->addArg("source", source);
    mkdir->addArg("target", target);
    mkdir->addArg("parent", parent);
    mkdir->addArg("overwrite", overwrite);

    m_tasks->addWaitTask(parent);

    connect(mkdir, SIGNAL(onStart(quint64, const QString&, const QVariantMap&)), this, SLOT(task_on_start(quint64, const QString&, const QVariantMap&)));
    connect(mkdir, SIGNAL(onSuccess(quint64, const EteraItem&, const QVariantMap&)), this, SLOT(task_on_put_dir_success(quint64, const EteraItem&, const QVariantMap&)));
    connect(mkdir, SIGNAL(onError(quint64, int, const QString&, const QVariantMap&)), this, SLOT(task_on_put_dir_error(quint64, int, const QString&, const QVariantMap&)));

    EteraThreadPool::globalInstance()->start(mkdir);
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

    quint64 fakeid = EteraTask::nextID();

    QVariantMap map;
    map["answer"] = QMessageBox::NoButton;

    m_tasks->addSimpleTask(fakeid, trUtf8("Сохранение с Диска"), map);

    for (int i = 0; i < count; i++) {
        WidgetDiskItem*  witem = static_cast<WidgetDiskItem*>(selected[i]);
        const EteraItem* eitem = witem->item();

        if (eitem->isDir() == true)
            getRemoteDir(eitem->path(), path + "/" + eitem->name(), fakeid);
        else if (eitem->isFile() == true)
            getRemoteFile(eitem->path(), path + "/" + eitem->name(), fakeid);
    }

    m_tasks->checkWaitTask(fakeid);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_get_file_progress(quint64 id, qint64 done, qint64 total)
{
    m_tasks->setProgress(id, done, total);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_get_file_error(quint64 id, int /*code*/, const QString& error, const QVariantMap& args)
{
    QString source = args["source"].toString();
    QString target = args["target"].toString();

    QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка загрузки %1 в %2:\n%3").arg(source).arg(target).arg(error));

    m_tasks->removeChildTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_get_file_success(quint64 id, const QVariantMap& /*args*/)
{
    m_tasks->removeChildTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::getRemoteFile(const QString& source, const QString& target, quint64 parent)
{
    QFileInfo info(target);

    if (info.exists() == true) {
        quint64     rootid   = m_tasks->rootID(parent);
        QVariantMap rootargs = m_tasks->args(rootid);

        QMessageBox::StandardButton answer = (QMessageBox::StandardButton)rootargs.value("answer", QMessageBox::NoButton).toInt();

        // ранее было указано не перезаписывать файлы
        if (answer == QMessageBox::NoToAll)
            return;

        // что делать с конфликтами?
        if (answer != QMessageBox::YesToAll) {
            answer = QMessageBox::question(this, trUtf8("Файл уже существует!"), trUtf8("Файл %1 уже существует, перезаписать?").arg(target), QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll);

            // обновляем ответ
            rootargs["answer"] = answer;
            m_tasks->setArgs(rootid, rootargs);

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

    EteraTaskGET* get = new EteraTaskGET(source, target);

    get->addArg("parent", parent);

    m_tasks->addWaitTask(parent);

    connect(get, SIGNAL(onStart(quint64, const QString&, const QVariantMap&)), this, SLOT(task_on_start(quint64, const QString&, const QVariantMap&)));
    connect(get, SIGNAL(onProgress(quint64, qint64, qint64)), this, SLOT(task_on_get_file_progress(quint64, qint64, qint64)));
    connect(get, SIGNAL(onSuccess(quint64, const QVariantMap&)), this, SLOT(task_on_get_file_success(quint64, const QVariantMap&)));
    connect(get, SIGNAL(onError(quint64, int, const QString&, const QVariantMap&)), this, SLOT(task_on_get_file_error(quint64, int, const QString&, const QVariantMap&)));

    EteraThreadPool::globalInstance()->start(get);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_get_dir_error(quint64 id, int /*code*/, const QString& error, const QVariantMap& args)
{
    QString source = args["source"].toString();

    QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка чтения %1:\n%2").arg(source).arg(error));

    m_tasks->removeChildTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_get_dir_success(quint64 id, const EteraItemList& list, const QVariantMap& args)
{
    QString source = args["source"].toString();
    QString target = args["target"].toString();
    quint64 parent = args["parent"].toULongLong();

    m_tasks->addChildTask(parent, id, trUtf8("Обработка %1").arg(source), args);

    for (int i = 0; i < list.count(); i++) {
        EteraItem item = list[i];
        if (item.isDir() == true)
            getRemoteDir(item.path(), target + "/" + item.name(), id);
        else if (item.isFile() == true)
            getRemoteFile(item.path(), target + "/" + item.name(), id);
    }

    m_tasks->checkWaitTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::getRemoteDir(const QString& source, const QString& target, quint64 parent)
{
    QFileInfo info(target);
    if (info.exists() == false && info.dir().mkdir(info.absoluteFilePath()) == false) {
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка создания %1").arg(info.absoluteFilePath()));
        return;
    } else if (info.exists() == true && info.isDir() == false) {
        quint64     rootid   = m_tasks->rootID(parent);
        QVariantMap rootargs = m_tasks->args(rootid);

        QMessageBox::StandardButton answer = (QMessageBox::StandardButton)rootargs.value("answer", QMessageBox::NoButton).toInt();

        // ранее было указано не перезаписывать файлы
        if (answer == QMessageBox::NoToAll)
            return;

        // что делать с конфликтами?
        if (answer != QMessageBox::YesToAll) {
            answer = QMessageBox::question(this, trUtf8("Файл уже существует!"), trUtf8("Файл %1 уже существует, перезаписать?").arg(target), QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll);

            // обновляем ответ
            rootargs["answer"] = answer;
            m_tasks->setArgs(rootid, rootargs);

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

    EteraTaskLS* ls = new EteraTaskLS(source);

    ls->addArg("source", source);
    ls->addArg("target", target);
    ls->addArg("parent", parent);

    m_tasks->addWaitTask(parent);

    ls->setSuccessMutex(&m_question_mutex);

    connect(ls, SIGNAL(onStart(quint64, const QString&, const QVariantMap&)), this, SLOT(task_on_start(quint64, const QString&, const QVariantMap&)));
    connect(ls, SIGNAL(onSuccess(quint64, const EteraItemList&, const QVariantMap&)), this, SLOT(task_on_get_dir_success(quint64, const EteraItemList&, const QVariantMap&)), Qt::BlockingQueuedConnection);
    connect(ls, SIGNAL(onError(quint64, int, const QString&, const QVariantMap&)), this, SLOT(task_on_get_dir_error(quint64, int, const QString&, const QVariantMap&)));

    EteraThreadPool::globalInstance()->start(ls);
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

void WidgetDisk::task_on_share_error(quint64 id, int /*code*/, const QString& error, const QVariantMap& args)
{
    QString path = args["path"].toString();

    QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка открытия доступа к %1:\n%2").arg(path).arg(error));

    m_tasks->removeSimpleTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_share_success(quint64 id, const EteraItem& item, const QVariantMap& /*args*/)
{
    WidgetDiskItem* witem = findByPath(item.path());

    if (witem != NULL)
        witem->replaceItem(item);

    m_tasks->removeSimpleTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_revoke_error(quint64 id, int /*code*/, const QString& error, const QVariantMap& args)
{
    QString path = args["path"].toString();

    QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка закрытия доступа к %1:\n%2").arg(path).arg(error));

    m_tasks->removeSimpleTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::task_on_revoke_success(quint64 id, const EteraItem& item, const QVariantMap& /*args*/)
{
    WidgetDiskItem* witem = findByPath(item.path());

    if (witem != NULL)
        witem->replaceItem(item);

    m_tasks->removeSimpleTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::shareObjects(bool share)
{
    QList<QListWidgetItem*> selected = m_explorer->selectedItems();

    int count = selected.count();

    if (count == 0)
        return;

    for (int i = 0; i < count; i++) {
        WidgetDiskItem*  witem = static_cast<WidgetDiskItem*>(selected[i]);
        const EteraItem* eitem = witem->item();

        if (share == true) {
            EteraTaskPUBLISH* publish = new EteraTaskPUBLISH(eitem->path());

            connect(publish, SIGNAL(onStart(quint64, const QString&, const QVariantMap&)), this, SLOT(task_on_start(quint64, const QString&, const QVariantMap&)));
            connect(publish, SIGNAL(onSuccess(quint64, const EteraItem&, const QVariantMap&)), this, SLOT(task_on_revoke_success(quint64, const EteraItem&, const QVariantMap&)));
            connect(publish, SIGNAL(onError(quint64, int, const QString&, const QVariantMap&)), this, SLOT(task_on_revoke_error(quint64, int, const QString&, const QVariantMap&)));

            EteraThreadPool::globalInstance()->start(publish);
        } else {
            EteraTaskUNPUBLISH* unpublish = new EteraTaskUNPUBLISH(eitem->path());

            connect(unpublish, SIGNAL(onStart(quint64, const QString&, const QVariantMap&)), this, SLOT(task_on_start(quint64, const QString&, const QVariantMap&)));
            connect(unpublish, SIGNAL(onSuccess(quint64, const EteraItem&, const QVariantMap&)), this, SLOT(task_on_revoke_success(quint64, const EteraItem&, const QVariantMap&)));
            connect(unpublish, SIGNAL(onError(quint64, int, const QString&, const QVariantMap&)), this, SLOT(task_on_revoke_error(quint64, int, const QString&, const QVariantMap&)));

            EteraThreadPool::globalInstance()->start(unpublish);
        }
    }
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
