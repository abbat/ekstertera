#include "widget_tasks.h"
//----------------------------------------------------------------------------------------------

WidgetTasks::WidgetTasks(QWidget* parent) : QTreeWidget(parent)
{
    setHeaderHidden(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setColumnCount(2);
    setUniformRowHeights(true);
}
//----------------------------------------------------------------------------------------------

WidgetTasks::~WidgetTasks()
{
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::resizeEvent(QResizeEvent* event)
{
    QSize size = event->size();

    setColumnWidth(0, size.width() * 70 / 100);
    setColumnWidth(1, size.width() * 30 / 100);

    QTreeWidget::resizeEvent(event);
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::addTask(quint64 id, const QString& text, const QString& tooltip)
{
    Q_ASSERT(id != 0);
    addChildTask(0, id, text, tooltip);
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::addChildTask(quint64 parent, quint64 id, const QString& text, const QString& tooltip)
{
    Q_ASSERT(id != 0);

    WidgetTasksItem* pitem = m_tasks.value(parent, NULL);
    WidgetTasksItem* titem = m_tasks.value(id, NULL);

    Q_ASSERT(parent == 0 || pitem != NULL);

    if (titem == NULL) {
        if (pitem != NULL) {
            titem = new WidgetTasksItem(id, pitem);
            if (pitem->isExpanded() == false)
                pitem->setExpanded(true);
        } else
            titem = new WidgetTasksItem(id, this);

        titem->setText(0, text);
        titem->setToolTip(0, tooltip);

        m_tasks[id] = titem;

        emit onChangeCount(m_tasks.count());
    } else {
        titem->setText(0, text);
        titem->setToolTip(0, tooltip);
    }
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::checkTask(quint64 id)
{
    // если у задачи есть дочерние элементы,
    // то она не будет удалена
    removeTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::removeTask(quint64 id)
{
    Q_ASSERT(id != 0);

    WidgetTasksItem* item = m_tasks.value(id, NULL);
    if (item == NULL)
        return;

    int oldval = m_tasks.count();

    removeTask(item);

    int newval = m_tasks.count();

    if (oldval != newval)
        emit onChangeCount(newval);
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::removeTask(WidgetTasksItem* item)
{
    // например, для задачи получения списка файлов
    // могут быть активны дочерние задачи, по этому
    // реальное удаление должно быть отложено до завершения
    // дочерних задач
    if (item->childCount() != 0)
        return;

    WidgetTasksItem* pitem = static_cast<WidgetTasksItem*>(item->parent());

    m_tasks.remove(item->id());
    m_reply.remove(item->id());

    delete item;

    if (pitem != NULL)
        removeTask(pitem);
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::childIDs(quint64 id, QList<quint64>& ids)
{
    Q_ASSERT(id != 0);

    WidgetTasksItem* item = m_tasks.value(id, NULL);

    Q_ASSERT(item != NULL);

    childIDs(item, ids);
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::childIDs(WidgetTasksItem* element, QList<quint64>& ids)
{
    for (int i = 0; i < element->childCount(); i++) {
        WidgetTasksItem* item = static_cast<WidgetTasksItem*>(element->child(i));
        childIDs(item, ids);
    }

    ids.append(element->id());
}
//----------------------------------------------------------------------------------------------

quint64 WidgetTasks::rootID(quint64 id)
{
    Q_ASSERT(id != 0);

    quint64 root = 0;

    WidgetTasksItem* item = m_tasks.value(id, NULL);

    Q_ASSERT(item != NULL);

    while (item != NULL) {
        root = item->id();
        item = static_cast<WidgetTasksItem*>(item->parent());
    }

    return root;
}
//----------------------------------------------------------------------------------------------

QMessageBox::StandardButton WidgetTasks::reply(quint64 id)
{
    Q_ASSERT(id != 0);
    return m_reply.value(id, QMessageBox::NoButton);
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::setReply(quint64 id, QMessageBox::StandardButton reply)
{
    Q_ASSERT(id != 0);
    m_reply[id] = reply;
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::setProgress(quint64 id, qint64 done, qint64 total)
{
    Q_ASSERT(id != 0);

    WidgetTasksItem* item = m_tasks.value(id, NULL);
    if (item == NULL)
        return;

    if (item->bar() == NULL) {
        QProgressBar* bar = new QProgressBar();
        bar->setMinimum(0);

        item->setBar(bar);

        setItemWidget(item, 1, bar);
    }

    QProgressBar* bar = item->bar();

    bar->setMaximum(total);
    bar->setValue(done);
}
//----------------------------------------------------------------------------------------------
