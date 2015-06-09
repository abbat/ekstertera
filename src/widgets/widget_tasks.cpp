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

void WidgetTasks::addSimpleTask(quint64 id, const QString& text)
{
    TasksItem* titem = m_tasks.value(id, NULL);

    if (titem == NULL) {
        titem = new TasksItem();

        titem->Item = new WidgetTasksItem(this);
        titem->Item->setText(0, text);

        titem->Bar    = NULL;
        titem->Parent = 0;
        titem->Answer = QMessageBox::NoButton;

        m_tasks[id] = titem;

        emit onChangeCount(m_tasks.count());
    } else
        titem->Item->setText(0, text);
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::removeSimpleTask(quint64 id)
{
    TasksItem* titem = m_tasks.value(id, NULL);

    if (titem == NULL)
        return;

    delete titem->Bar;
    delete titem->Item;
    delete titem;

    m_tasks.remove(id);

    emit onChangeCount(m_tasks.count());
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::addChildTask(quint64 parent, quint64 id, const QString& text)
{
    TasksItem* pitem = m_tasks.value(parent, NULL);
    TasksItem* titem = m_tasks.value(id, NULL);

    if (titem == NULL) {
        titem = new TasksItem();

        if (pitem != NULL) {
            titem->Item = new WidgetTasksItem(pitem->Item);
            if (pitem->Item->isExpanded() == false)
                pitem->Item->setExpanded(true);
        } else
            titem->Item = new WidgetTasksItem(this);

        titem->Item->setText(0, text);

        titem->Bar    = NULL;
        titem->Parent = parent;
        titem->Answer = QMessageBox::NoButton;

        m_tasks[id] = titem;

        emit onChangeCount(m_tasks.count());
    } else
        titem->Item->setText(0, text);
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::removeChildTask(quint64 id)
{
    TasksItem* titem = m_tasks.value(id, NULL);

    if (titem == NULL)
        return;

    quint64 parent = titem->Parent;

    delete titem->Item;
    delete titem;

    m_tasks.remove(id);

    if (parent != 0) {
        TasksItem* pitem = m_tasks.value(parent, NULL);
        if (pitem != NULL && pitem->Item->childCount() == 0)
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
        parent = titem->Parent;
    }

    return id;
}
//----------------------------------------------------------------------------------------------

QMessageBox::StandardButton WidgetTasks::answer(quint64 id)
{
    TasksItem* titem = m_tasks.value(id, NULL);
    if (titem == NULL)
        return QMessageBox::NoButton;

    return titem->Answer;
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::setAnswer(quint64 id, QMessageBox::StandardButton answer)
{
    TasksItem* titem = m_tasks.value(id, NULL);
    if (titem == NULL)
        return;

    titem->Answer = answer;
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
    }

    titem->Bar->setMaximum(total);
    titem->Bar->setValue(done);
}
//----------------------------------------------------------------------------------------------
