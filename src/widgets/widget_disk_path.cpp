#include "widget_disk_path.h"
//----------------------------------------------------------------------------------------------

WidgetDiskPath::WidgetDiskPath(QWidget* parent) : QWidget(parent)
{
    setMinimumHeight(24);
    setMaximumHeight(24);
}
//----------------------------------------------------------------------------------------------

WidgetDiskPath::~WidgetDiskPath()
{
}
//----------------------------------------------------------------------------------------------

void WidgetDiskPath::changePath(const QString& path)
{
    for (int i = 0; i < m_buttons.count(); i++) {
        delete m_buttons[i];
    }

    m_buttons.clear();

    QString patrial;
    QStringList parts = path.split("/");

    for (int i = 0; i < parts.count(); i++) {
        if (parts[i].isEmpty() == true)
            continue;

        QToolButton* button = new QToolButton(this);

        if (i == 0)
            button->move(0, 0);
        else
            button->move(m_buttons[i - 1]->geometry().right() + 1, 0);

        button->setText(parts[i]);
        button->setFocusPolicy(Qt::NoFocus);
        button->setVisible(true);

        patrial += parts[i] + "/";
        button->setProperty("path", patrial);

        connect(button, SIGNAL(clicked()), this, SLOT(button_clicked()));

        m_buttons.append(button);
    }
}
//----------------------------------------------------------------------------------------------

void WidgetDiskPath::button_clicked()
{
    QToolButton* button = static_cast<QToolButton*>(sender());
    QString path = button->property("path").toString();
    emit onPathChangeRequest(path);
}
//----------------------------------------------------------------------------------------------
