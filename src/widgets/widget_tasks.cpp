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
    foreach(TasksItem* titem, m_tasks)
        delete titem;
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

void WidgetTasks::addSimpleTask(quint64 id, const QString& text, const QVariantMap& args)
{
    TasksItem* titem = m_tasks.value(id, NULL);

    if (titem == NULL) {
        titem = new TasksItem();

        titem->Args = args;

        titem->Item = new WidgetTasksItem(this);
        titem->Item->setText(0, text);

        titem->Bar  = NULL;
        titem->Wait = 0;

        m_tasks[id] = titem;

        emit onChangeCount(m_tasks.count());
    } else {
        titem->Args = args;
        titem->Item->setText(0, text);
    }
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::removeSimpleTask(quint64 id)
{
    TasksItem* titem = m_tasks.value(id, NULL);

    if (titem == NULL)
        return;

    delete titem->Item;
    delete titem;
    m_tasks.remove(id);

    emit onChangeCount(m_tasks.count());
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::addWaitTask(quint64 parent)
{
    TasksItem* pitem = m_tasks.value(parent, NULL);

    if (pitem == NULL)
        return;

    pitem->Wait++;
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::checkWaitTask(quint64 id)
{
    TasksItem* titem = m_tasks.value(id, NULL);

    if (titem != NULL && titem->Wait == 0 && titem->Item->childCount() == 0)
        removeChildTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::addChildTask(quint64 parent, quint64 id, const QString& text, const QVariantMap& args)
{
    TasksItem* pitem = m_tasks.value(parent, NULL);
    TasksItem* titem = m_tasks.value(id, NULL);

    if (titem == NULL) {
        titem = new TasksItem();

        titem->Args = args;

        if (pitem != NULL) {
            titem->Item = new WidgetTasksItem(pitem->Item);
            if (pitem->Item->isExpanded() == false)
                pitem->Item->setExpanded(true);

            pitem->Wait--;
        } else
            titem->Item = new WidgetTasksItem(this);

        titem->Item->setText(0, text);

        titem->Bar  = NULL;
        titem->Wait = 0;

        m_tasks[id] = titem;

        emit onChangeCount(m_tasks.count());
    } else {
        titem->Args = args;
        titem->Item->setText(0, text);
    }
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::removeChildTask(quint64 id)
{
    TasksItem* titem = m_tasks.value(id, NULL);

    if (titem == NULL)
        return;

    quint64 parent = titem->Args.value("parent", 0).toULongLong();

    delete titem->Item;
    delete titem;

    m_tasks.remove(id);

    if (parent != 0) {
        TasksItem* pitem = m_tasks.value(parent, NULL);
        if (pitem != NULL && pitem->Wait == 0 && pitem->Item->childCount() == 0)
            removeChildTask(parent);
    }

    emit onChangeCount(m_tasks.count());
}
//----------------------------------------------------------------------------------------------

quint64 WidgetTasks::rootID(quint64 id)
{
    quint64 parent = id;

    while (parent != 0) {
        TasksItem* titem = m_tasks.value(parent, NULL);
        if (titem == NULL)
            return 0;

        id = parent;
        parent = titem->Args.value("parent", 0).toULongLong();
    }

    return id;
}
//----------------------------------------------------------------------------------------------

QVariantMap WidgetTasks::args(quint64 id)
{
    TasksItem* titem = m_tasks.value(id, NULL);
    if (titem == NULL)
        return QVariantMap();

    return titem->Args;
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::setArgs(quint64 id, const QVariantMap& args)
{
    TasksItem* titem = m_tasks.value(id, NULL);
    if (titem == NULL)
        return;

    titem->Args = args;
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::setProgress(quint64 id, qint64 done, qint64 total)
{
    TasksItem* titem = m_tasks.value(id, NULL);
    if (titem == NULL)
        return;

    if (titem->Bar == NULL) {
        titem->Bar = new QProgressBar();
        titem->Bar->setMinimum(0);

        setItemWidget(titem->Item, 1, titem->Bar);

        QString source = titem->Args.value("source", "").toString();

        int idx = source.lastIndexOf("/");
#ifdef ETERA_WS_WIN
        if (idx == -1 && titem->Args.contains("overwrite") == true /* put */)
            idx = source.lastIndexOf("\\");
#endif

        if (idx != -1)
            titem->Item->setText(0, source.right(source.length() - idx - 1));
    }

    titem->Bar->setMaximum(total);
    titem->Bar->setValue(done);
}
//----------------------------------------------------------------------------------------------
