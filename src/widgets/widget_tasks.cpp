#include "widget_tasks.h"
//----------------------------------------------------------------------------------------------
#include "utils/settings.h"
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

EteraAPI* WidgetTasks::createAPI(quint64 id)
{
    EteraAPI* api = new EteraAPI(this, id);
    api->setToken(EteraSettings::instance()->token());
    setAPI(id, api);
    return api;
}
//----------------------------------------------------------------------------------------------

EteraAPI* WidgetTasks::resetAPI(EteraAPI* api, quint64 id)
{
    if (api == NULL)
        api = createAPI(id);
    else {
        removeTask(api->id());
        api->disconnect(this);
        api->setID(id);
    }

    return api;
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::releaseAPI(EteraAPI* api)
{
    if (api != NULL) {
        removeTask(api->id());
        api->deleteLater();
    }
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::addTask(quint64 id, const QString& text, const QString& tooltip, EteraAPI* api)
{
    addChildTask(0, id, text, tooltip, api);
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::addChildTask(quint64 parent, quint64 id, const QString& text, const QString& tooltip, EteraAPI* api)
{
    WidgetTasksItem* pitem = m_tasks.value(parent, NULL);
    WidgetTasksItem* titem = m_tasks.value(id, NULL);

    if (titem == NULL) {
        if (pitem != NULL) {
            titem = new WidgetTasksItem(id, pitem);
            if (pitem->isExpanded() == false)
                pitem->setExpanded(true);
        } else
            titem = new WidgetTasksItem(id, this);

        titem->setText(0, text);
        titem->setToolTip(0, tooltip);

        titem->setAPI(api);
        titem->setParentID(parent);

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
    WidgetTasksItem* titem = m_tasks.value(id, NULL);
    if (titem == NULL)
        return;

    if (titem->childCount() > 0)
        return;

    removeTask(id);
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::removeTask(quint64 id)
{
    WidgetTasksItem* titem = m_tasks.value(id, NULL);
    if (titem == NULL)
        return;

    if (titem->childCount() > 0)
        return;

    quint64 parent = titem->parentID();

    delete titem;

    m_tasks.remove(id);

    if (parent != 0) {
        WidgetTasksItem* pitem = m_tasks.value(parent, NULL);
        if (pitem != NULL && pitem->childCount() == 0)
            removeTask(parent);
    }

    emit onChangeCount(m_tasks.count());
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::abortTask(quint64 id, QList<quint64>& aborted)
{
    WidgetTasksItem* titem = m_tasks.value(id, NULL);
    if (titem == NULL)
        return;

    abortTask(titem, aborted);
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::abortTask(WidgetTasksItem* element, QList<quint64>& aborted)
{
    for (int i = 0; i < element->childCount(); i++) {
        WidgetTasksItem* item = static_cast<WidgetTasksItem*>(element->child(i));
        abortTask(item, aborted);
    }

    aborted.append(element->id());

    EteraAPI* api = element->api();
    if (api != NULL)
        api->abort();
}
//----------------------------------------------------------------------------------------------

quint64 WidgetTasks::rootID(quint64 id)
{
    quint64 parent = id;

    while (parent != 0) {
        WidgetTasksItem* titem = m_tasks.value(parent, NULL);
        if (titem == NULL)
            return 0;

        id = parent;
        parent = titem->parentID();
    }

    return id;
}
//----------------------------------------------------------------------------------------------

QMessageBox::StandardButton WidgetTasks::reply(quint64 id)
{
    WidgetTasksItem* titem = m_tasks.value(id, NULL);
    if (titem == NULL)
        return QMessageBox::NoButton;

    return titem->reply();
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::setReply(quint64 id, QMessageBox::StandardButton reply)
{
    WidgetTasksItem* titem = m_tasks.value(id, NULL);
    if (titem == NULL)
        return;

    titem->setReply(reply);
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::setAPI(quint64 id, EteraAPI* api)
{
    WidgetTasksItem* titem = m_tasks.value(id, NULL);
    if (titem == NULL)
        return;

    titem->setAPI(api);
}
//----------------------------------------------------------------------------------------------

void WidgetTasks::setProgress(quint64 id, qint64 done, qint64 total)
{
    WidgetTasksItem* titem = m_tasks.value(id, NULL);
    if (titem == NULL)
        return;

    if (titem->bar() == NULL) {
        QProgressBar* bar = new QProgressBar();
        bar->setMinimum(0);

        titem->setBar(bar);

        setItemWidget(titem, 1, bar);
    }

    QProgressBar* bar = titem->bar();

    bar->setMaximum(total);
    bar->setValue(done);
}
//----------------------------------------------------------------------------------------------
