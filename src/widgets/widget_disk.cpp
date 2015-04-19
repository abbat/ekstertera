#include "widget_disk.h"
//----------------------------------------------------------------------------------------------
#include "widget_disk_item.h"
//----------------------------------------------------------------------------------------------
#include "settings.h"
#include "form_info.h"
//----------------------------------------------------------------------------------------------

WidgetDisk::WidgetDisk(QWidget* parent) :
    QWidget(parent)
{
    m_clipboard = EteraClipboard::instance();

    m_list = new QListWidget(this);

    m_list->setWrapping(true);
    m_list->setResizeMode(QListView::Adjust);
    m_list->setContextMenuPolicy(Qt::CustomContextMenu);
    m_list->setIconSize(QSize(48, 48));
    m_list->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_list->setSortingEnabled(true);
    //m_list->setDragDropMode(QAbstractItemView::DragOnly);

    setAcceptDrops(true);

    // контекстное меню
    m_menu        = new QMenu(m_list);
    m_menu_open   = m_menu->addAction(QIcon::fromTheme("folder-open"), "");
    m_menu->addSeparator();
    m_menu_new    = m_menu->addAction(QIcon::fromTheme("folder-new"),  "");
    m_menu->addSeparator();
    m_menu_cut    = m_menu->addAction(QIcon::fromTheme("edit-cut"),    "");
    m_menu_copy   = m_menu->addAction(QIcon::fromTheme("edit-copy"),   "");
    m_menu_paste  = m_menu->addAction(QIcon::fromTheme("edit-paste"),  "");
    m_menu->addSeparator();
    m_menu_delete = m_menu->addAction(QIcon::fromTheme("edit-delete"), "");
    m_menu->addSeparator();
    m_menu_rename = m_menu->addAction("");
    m_menu->addSeparator();
    m_menu_share = m_menu->addAction(QIcon::fromTheme("emblem-shared"), "");
    m_menu_revoke = m_menu->addAction(QIcon::fromTheme("document-revert"), "");
    m_menu->addSeparator();
    m_menu_info   = m_menu->addAction(QIcon::fromTheme("document-properties", QIcon(":/icons/info16.png")), "");

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

    // события виджета
    connect(m_list, SIGNAL(itemSelectionChanged()), this, SLOT(on_item_selection_changed()));
    connect(m_list, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(item_activated(QListWidgetItem*)));
    connect(m_list, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(custom_context_menu_requested(const QPoint&)));
    connect(m_list->itemDelegate(), SIGNAL(closeEditor(QWidget*, QAbstractItemDelegate::EndEditHint)), this, SLOT(item_end_edit(QWidget*, QAbstractItemDelegate::EndEditHint)));

    // события api
    connect(&m_api, SIGNAL(onProgress(qint64, qint64)), this, SLOT(api_on_progress(qint64, qint64)));
    connect(&m_api, SIGNAL(onError(const QString&)), this, SLOT(api_on_error(const QString&)));

    // локализация
    retranslateUi();
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

    m_api.retranslateUi();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::setToken(const QString& token)
{
    m_api.setToken(token);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::api_on_progress(qint64 done, qint64 total)
{
    emit onApiProgress(done, total);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::api_on_error(const QString& error)
{
    emit onApiError(1, error);
}
//----------------------------------------------------------------------------------------------

bool WidgetDisk::fatalApiError(bool clear)
{
    // очищаем список объектов, чтобы не вводить в заблуждение
    if (clear == true) {
        m_list->clear();

        // очистка буфера обмена для предотвращения неопределенного поведения
        m_clipboard->clear();
    }

    // курсор мог быть в статусе Qt::BusyCursor
    m_list->setCursor(Qt::ArrowCursor);

    // вызов сигнала
    emit onApiError(m_api.lastErrorCode(), m_api.lastErrorMessage());

    // всегда false
    return false;
}
//----------------------------------------------------------------------------------------------

bool WidgetDisk::changePath(const QString& path)
{
    emit onApiProgress(trUtf8("Смена пути на %1").arg(path));

    m_list->setCursor(Qt::BusyCursor);

    m_list->clear();

    EteraItemList result;
    if (m_api.ls(path, result) == false)
        return fatalApiError();

    for (int i = 0; i < result.count(); i++)
        new WidgetDiskItem(m_list, result[i]);

    m_list->setCursor(Qt::ArrowCursor);

    emit onPathChanged(path);

    m_path = path;

    emit onApiSuccess();

    return true;
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::updateBufferList(bool copy_mode)
{
    QList<QListWidgetItem*> selected = m_list->selectedItems();

    if (selected.isEmpty() == true)
        return;

    m_clipboard->clear();

    if (copy_mode == true)
        m_clipboard->setCopyMode();
    else
        m_clipboard->setCutMode();

    for (int i = 0; i < selected.count(); i++)
        m_clipboard->append(*((static_cast<WidgetDiskItem*>(selected[i]))->item()));
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::item_activated(QListWidgetItem* item)
{
    WidgetDiskItem*  witem = static_cast<WidgetDiskItem*>(item);
    const EteraItem* eitem = witem->item();

    if (eitem->isDir() == false)
        return;

    QString path = eitem->path() + "/";

    changePath(path);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::wheelEvent(QWheelEvent* event)
{
    m_list->horizontalScrollBar()->event(event);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::resizeEvent(QResizeEvent* event)
{
    m_list->resize(width() - 2, height() - 2);
    QWidget::resizeEvent(event);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::custom_context_menu_requested(const QPoint& pos)
{
    QList<QListWidgetItem*> selected = m_list->selectedItems();

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

    m_menu_paste->setEnabled(m_clipboard->isEmpty() == false);

    m_menu->exec(m_list->viewport()->mapToGlobal(pos));
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_open_triggered()
{
    WidgetDiskItem* witem = static_cast<WidgetDiskItem*>(m_list->currentItem());
    if (witem == NULL)
        return;

    const EteraItem* eitem = witem->item();

    if (eitem->isDir() == true) {
        QString path = eitem->path() + "/";
        changePath(path);
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_new_triggered()
{
    bool ok;
    QString value = QInputDialog::getText(this, trUtf8("Создать директорию"), trUtf8("Введите имя новой директории"), QLineEdit::Normal, trUtf8("Новая папка"), &ok);
    if (ok == false || value.isEmpty() == true)
        return;

    QString path = m_path + value;

    emit onApiProgress(trUtf8("Создание %1").arg(path));

    m_list->setCursor(Qt::BusyCursor);

    if (m_api.mkdir(path) == false) {
        m_list->setCursor(Qt::ArrowCursor);

        // CONFLICT, директория уже существует
        if (m_api.lastErrorCode() == 409) {
            QMessageBox::warning(this, trUtf8("Внимание!"), trUtf8("%1 уже существует").arg(path));
            emit onApiSuccess();
            return;
        }

        // мягкая ошибка - директория скорее всего не создана
        fatalApiError(false);
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка создания %1:\n%2").arg(path).arg(m_api.lastErrorMessage()));
        return;
    }

    emit onApiProgress(trUtf8("Получение информации о %1").arg(path));

    EteraItem item;
    if (m_api.stat(path, item) == false) {
        fatalApiError();
        return;
    }

    m_list->setCurrentItem(new WidgetDiskItem(m_list, item), QItemSelectionModel::ClearAndSelect);

    m_list->setCursor(Qt::ArrowCursor);

    emit onApiSuccess();
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
    if (m_clipboard->isEmpty() == true)
        return;

    m_list->setCursor(Qt::BusyCursor);

    while (m_clipboard->isEmpty() == false) {
        EteraItem src = m_clipboard->first();
        QString   dst = m_path + src.name();

        // нет смысла перемещать само в себя
        if (src.path() == dst) {
            m_list->setCursor(Qt::ArrowCursor);
            QMessageBox::warning(this, trUtf8("Внимание!"), trUtf8("Источник и приемник совпадают"));
            return;
        }

        bool result;
        if (m_clipboard->copyMode() == true) {
            emit onApiProgress(trUtf8("Копирование %1").arg(src.path()));
            result = m_api.cp(src.path(), dst, false);
        } else {
            emit onApiProgress(trUtf8("Перемещение %1").arg(src.path()));
            result = m_api.mv(src.path(), dst, false);
        }

        if (result == false) {
            // если ошибка на вызове копирования/перемещения, то предполагаем мягкую ошибку
            // в противном случае состояние является неопределенным и разумнее фаталить
            fatalApiError(m_api.isAsyncError());

            if (m_clipboard->copyMode() == true)
                QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка копирования %1:\n%2").arg(src.path()).arg(m_api.lastErrorMessage()));
            else
                QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка перемещения %1:\n%2").arg(src.path()).arg(m_api.lastErrorMessage()));

            return;
        }

        emit onApiProgress(trUtf8("Получение информации о %1").arg(dst));

        EteraItem item;
        if (m_api.stat(dst, item) == false) {
            fatalApiError();
            return;
        }

        m_list->setCurrentItem(new WidgetDiskItem(m_list, item), QItemSelectionModel::ClearAndSelect);

        m_clipboard->removeFirst();
    }

    m_list->setCursor(Qt::ArrowCursor);

    emit onApiSuccess();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_delete_triggered()
{
    QList<QListWidgetItem*> selected = m_list->selectedItems();

    int count = selected.count();

    if (count == 0)
        return;

    m_list->setCursor(Qt::BusyCursor);

    for (int i = 0; i < count; i++) {
        WidgetDiskItem*  witem = static_cast<WidgetDiskItem*>(selected[i]);
        const EteraItem* eitem = witem->item();

        emit onApiProgress(trUtf8("Удаление %1").arg(eitem->path()));

        if (m_api.rm(eitem->path()) == false) {
            // если ошибка во время ожидания операции, то скорее всего объект будет удален
            // если объект был удален ранее, то его нужно удалить из виджета
            // в противном случае предполагаем ошибку при вызове метода удаления и объект не был удален
            if (m_api.isAsyncError() == true || m_api.lastErrorCode() == 404) {
                m_clipboard->removeByPath(eitem->path());

                delete witem;
            }

            fatalApiError(false);

            QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка удаления %1:\n%2").arg(eitem->path()).arg(m_api.lastErrorMessage()));

            return;
        }

        m_clipboard->removeByPath(eitem->path());

        delete witem;
    }

    m_list->setCursor(Qt::ArrowCursor);

    emit onApiSuccess();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::menu_rename_triggered()
{
    QList<QListWidgetItem*> selected = m_list->selectedItems();

    if (selected.count() == 1) {
        WidgetDiskItem* witem = static_cast<WidgetDiskItem*>(m_list->currentItem());
        witem->setFlags(witem->flags() | Qt::ItemIsEditable);
        m_list->editItem(witem);
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::item_end_edit(QWidget* editor, QAbstractItemDelegate::EndEditHint /*hint*/)
{
    WidgetDiskItem* witem = static_cast<WidgetDiskItem*>(m_list->currentItem());
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

    emit onApiProgress(trUtf8("Переименование %1").arg(eitem->path()));

    m_list->setCursor(Qt::BusyCursor);

    if (m_api.mv(eitem->path(), path, false) == false) {
        // если ошибка на вызове переименования, то предполагаем мягкую ошибку
        // в противном случае состояние является неопределенным и разумнее фаталить
        witem->revertText();
        fatalApiError(m_api.isAsyncError());
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка переименования %1:\n%2").arg(eitem->path()).arg(m_api.lastErrorMessage()));

        return;
    }

    emit onApiProgress(trUtf8("Получение информации о %1").arg(path));

    EteraItem item;
    if (m_api.stat(path, item) == false) {
        fatalApiError();
        return;
    }

    m_clipboard->removeByPath(eitem->path());

    witem->replaceItem(item);

    m_list->setCursor(Qt::ArrowCursor);

    emit onApiSuccess();
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
    WidgetDiskItem* witem = static_cast<WidgetDiskItem*>(m_list->currentItem());
    if (witem == NULL)
        return;

    const EteraItem* eitem = witem->item();

    FormInfo info(*eitem, this);
    info.exec();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/uri-list") == true)
        event->acceptProposedAction();
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::dropEvent(QDropEvent* event)
{
    m_answer = QMessageBox::NoButton;

    if (event->mimeData()->hasFormat("text/uri-list") == true) {
        QList<QUrl> urls = event->mimeData()->urls();
        for (int i = 0; i < urls.count(); i++) {
            if (urls[i].scheme() == "file")
                if (putLocalObject(urls[i].toLocalFile()) == false)
                    return;
        }

        event->acceptProposedAction();
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::putLocalObjects(const QStringList& paths)
{
    m_answer = QMessageBox::NoButton;

    for (int i = 0; i < paths.count(); i++)
        putLocalObject(paths[i]);
}
//----------------------------------------------------------------------------------------------

bool WidgetDisk::putLocalObject(const QString& path)
{
    QFileInfo info(path);

    // пропускаем симлинки и bundle
    if (info.isDir() == false && info.isFile() == false)
        return true;

    QString target = m_path + info.fileName();

    m_list->setCursor(Qt::BusyCursor);

    if (info.isDir() == true && putLocalDir(path, target) == false)
        return false;
    else if (info.isFile() == true && putLocalFile(path, target) == false)
        return false;

    emit onApiProgress(trUtf8("Получение информации о %1").arg(target));

    EteraItem item;
    if (m_api.stat(target, item) == false)
        return fatalApiError();

    // если элемент не существовал ранее, создаем, иначе обновляем
    QList<QListWidgetItem*> existing = m_list->findItems(info.fileName(), Qt::MatchExactly);
    if (existing.isEmpty() == true)
        m_list->setCurrentItem(new WidgetDiskItem(m_list, item), QItemSelectionModel::ClearAndSelect);
    else {
        WidgetDiskItem* witem = static_cast<WidgetDiskItem*>(existing[0]);
        witem->replaceItem(item);
        m_list->setCurrentItem(witem, QItemSelectionModel::ClearAndSelect);
    }

    m_list->setCursor(Qt::ArrowCursor);

    emit onApiSuccess();

    return true;
}
//----------------------------------------------------------------------------------------------

bool WidgetDisk::putLocalFile(const QString& source, const QString& target)
{
    emit onApiProgress(trUtf8("Загрузка %1").arg(source));

    bool overwrite = (m_answer == QMessageBox::YesToAll ? true : false);

    if (m_api.put(source, target, overwrite) == true)
        return true;

    if (m_api.lastErrorCode() == 409) {
        // ранее было указано не перезаписывать файлы
        if (m_answer == QMessageBox::NoToAll)
            return true;

        // что делать с конфликтами?
        m_answer = QMessageBox::question(this, trUtf8("Файл уже существует!"), trUtf8("Файл %1 уже существует, перезаписать?").arg(target), QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll);

        // оставить копию диска
        if (m_answer == QMessageBox::No || m_answer == QMessageBox::NoToAll)
            return true;

        // перезаписать локальной копией
        if (m_api.put(source, target, true) == true)
            return true;
    }

    fatalApiError(false);

    QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка загрузки %1:\n%2").arg(source).arg(m_api.lastErrorMessage()));

    return false;
}
//----------------------------------------------------------------------------------------------

bool WidgetDisk::putLocalDir(const QString& source, const QString& target)
{
    emit onApiProgress(trUtf8("Создание %1").arg(target));

    if (m_api.mkdir(target) == false) {
        // если директория существует, можно продолжать работу
        if (m_api.lastErrorCode() != 409) {
            fatalApiError(false);
            QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка создания %1:\n%2").arg(target).arg(m_api.lastErrorMessage()));
            return false;
        }
    }

    QFileInfoList list = QDir(source).entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);

    for (int i = 0; i < list.count(); i++) {
        QFileInfo info = list[i];

        if (info.isDir() == true && putLocalDir(info.absoluteFilePath(), target + "/" + info.fileName()) == false)
            return false;
        else if (info.isFile() == true && putLocalFile(info.absoluteFilePath(), target + "/" + info.fileName()) == false)
            return false;
    }

    return true;
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::on_item_selection_changed()
{
    if (m_list->selectedItems().count() == 0)
        emit onChangePossibleActions(false);
    else
        emit onChangePossibleActions(true);
}
//----------------------------------------------------------------------------------------------

void WidgetDisk::getRemoteObjects(const QString& path)
{
    m_answer = QMessageBox::NoButton;

    QList<QListWidgetItem*> selected = m_list->selectedItems();

    int count = selected.count();

    if (count == 0)
        return;

    m_list->setCursor(Qt::BusyCursor);

    for (int i = 0; i < count; i++) {
        WidgetDiskItem*  witem = static_cast<WidgetDiskItem*>(selected[i]);
        const EteraItem* eitem = witem->item();

        if (eitem->isDir() == true && getRemoteDir(eitem->path(), path + "/" + eitem->name()) == false)
            return;
        else if (eitem->isFile() == true && getRemoteFile(eitem->path(), path + "/" + eitem->name()) == false)
            return;
    }

    m_list->setCursor(Qt::ArrowCursor);

    emit onApiSuccess();
}
//----------------------------------------------------------------------------------------------

bool WidgetDisk::getRemoteFile(const QString& source, const QString& target)
{
    emit onApiProgress(trUtf8("Загрузка %1").arg(source));

    QFileInfo info(target);

    if (info.exists() == true) {
        // ранее было указано не перезаписывать файлы
        if (m_answer == QMessageBox::NoToAll)
            return true;

        // что делать с конфликтами?
        if (m_answer != QMessageBox::YesToAll) {
            m_answer = QMessageBox::question(this, trUtf8("Файл уже существует!"), trUtf8("Файл %1 уже существует, перезаписать?").arg(target), QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll);

            // оставить локальную копию
            if (m_answer == QMessageBox::No || m_answer == QMessageBox::NoToAll)
                return true;
        }

        if (info.isDir() == true) {
            if (removeDir(target) == false)
                return false;
        } else if (info.isFile() == true || info.isSymLink() == true) {
            if (info.dir().remove(info.absoluteFilePath()) == false)
                return false;
        } else
            return false;
    }

    if (m_api.get(source, target) == true)
        return true;

    fatalApiError(false);

    QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка загрузки %1:\n%2").arg(source).arg(m_api.lastErrorMessage()));

    return false;

}
//----------------------------------------------------------------------------------------------

bool WidgetDisk::getRemoteDir(const QString& source, const QString& target)
{
    QFileInfo info(target);
    if (info.exists() == false && info.dir().mkdir(info.absoluteFilePath()) == false) {
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка создания %1").arg(info.absoluteFilePath()));
        return false;
    }

    emit onApiProgress(trUtf8("Чтение %1").arg(target));

    EteraItemList list;
    if (m_api.ls(source, list) == false) {
        fatalApiError(false);
        QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка чтения %1:\n%2").arg(target).arg(m_api.lastErrorMessage()));
        return false;
    }

    for (int i = 0; i < list.count(); i++) {
        EteraItem item = list[i];
        if (item.isDir() == true && getRemoteDir(item.path(), target + "/" + item.name()) == false)
            return false;
        else if (item.isFile() == true && getRemoteFile(item.path(), target + "/" + item.name()) == false)
            return false;
    }

    return true;
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

void WidgetDisk::shareObjects(bool share)
{
    QList<QListWidgetItem*> selected = m_list->selectedItems();

    int count = selected.count();

    if (count == 0)
        return;

    m_list->setCursor(Qt::BusyCursor);

    for (int i = 0; i < count; i++) {
        WidgetDiskItem*  witem = static_cast<WidgetDiskItem*>(selected[i]);
        const EteraItem* eitem = witem->item();

        if (share == true) {
            emit onApiProgress(trUtf8("Открытие доступа для %1").arg(eitem->path()));

            if (m_api.publish(eitem->path()) == false) {
                fatalApiError(false);
                QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка открытия доступа для %1:\n%2").arg(eitem->path()).arg(m_api.lastErrorMessage()));
                return;
            }
        } else {
            emit onApiProgress(trUtf8("Закрытие доступа для %1").arg(eitem->path()));

            if (m_api.unpublish(eitem->path()) == false) {
                fatalApiError(false);
                QMessageBox::critical(this, trUtf8("Ошибка!"), trUtf8("Ошибка закрытия доступа %1:\n%2").arg(eitem->path()).arg(m_api.lastErrorMessage()));
                return;
            }
        }

        emit onApiProgress(trUtf8("Получение информации о %1").arg(eitem->path()));

        EteraItem item;
        if (m_api.stat(eitem->path(), item) == false) {
            fatalApiError();
            return;
        }

        witem->replaceItem(item);
    }

    m_list->setCursor(Qt::ArrowCursor);

    emit onApiSuccess();
}
//----------------------------------------------------------------------------------------------
