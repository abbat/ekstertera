/*!
 * \file
 * \brief Виджет списка асинхронных задач
 */

#ifndef _ekstertera_widgets_widget_tasks_h_
#define _ekstertera_widgets_widget_tasks_h_

#include "widget_tasks_item.h"

/*!
 * \brief Виджет списка асинхронных задач
 */
class WidgetTasks : public QTreeWidget
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param parent Родительский виджет
         */
        WidgetTasks(QWidget* parent);
        ~WidgetTasks();

        /*!
         * \brief Добавление простой задачи
         * \param id ID задачи
         * \param text Текст задачи
         * \param args Аргументы задачи
         */
        void addSimpleTask(quint64 id, const QString& text, const QVariantMap& args = QVariantMap());

        /*!
         * \brief Удаление простой задачи
         * \param id ID задачи
         */
        void removeSimpleTask(quint64 id);

        /*!
         * \brief Добавление задачи на ожидание addChildTask
         * Фактически, блокирование удаления родительской задачи у которых нет дочерних
         * \param parent ID родительской задачи
         */
        void addWaitTask(quint64 parent);

        /*!
         * \brief Проверка задачи на возможность удаления
         * Проверяется количество ожидающих / дочерних задач
         * \param id ID задачи
         */
        void checkWaitTask(quint64 id);

        /*!
         * \brief Добавление подчиненной задачи
         * \param parent ID родительской задачи
         * \param id ID задачи
         * \param text Текст задачи
         * \param args Аргументы задачи
         */
        void addChildTask(quint64 parent, quint64 id, const QString& text, const QVariantMap& args = QVariantMap());

        /*!
         * \brief Удаление дочерней задачи
         * \param id ID задачи
         */
        void removeChildTask(quint64 id);

        /*!
         * \brief Получение ID корневой задачи
         * \param id ID дочерней задачи
         * \return ID корневой задачи или 0
         */
        quint64 rootID(quint64 id);

        /*!
         * \brief Получение аргументов задачи
         * \param id ID задачи
         */
        QVariantMap args(quint64 id);

        /*!
         * \brief Установка аргументов задачи
         * \param id ID задачи
         * \param args Аргументы задачи
         */
        void setArgs(quint64 id, const QVariantMap& args);

        /*!
         * \brief Обновление прогресса задачи
         * \param id ID задачи
         * \param done Завершено
         * \param total Всего
         */
        void setProgress(quint64 id, qint64 done, qint64 total);

    protected:

        /*!
         * \brief QWidget::resizeEvent
         * Масштабирование колонок
         */
        void resizeEvent(QResizeEvent* event);

    private:

        /*!
         * \brief Элемент карты задач
         */
        typedef struct {
            WidgetTasksItem* Item;    /*!< \brief Графический элемент         */
            QProgressBar*    Bar;     /*!< \brief Виджет прогресса            */
            QVariantMap      Args;    /*!< \brief Аргументы задачи            */
            quint64          Wait;    /*!< \brief Количество задач в ожидании */
        } TasksItem;

        /*!
         * \brief Карта задач по id задачи
         */
        QMap<quint64, TasksItem*> m_tasks;

    signals:

        /*!
         * \brief Сигнал смены количества задач
         * \param count Количество задач
         */
        void onChangeCount(int count);
};

#endif   // _ekstertera_widgets_widget_tasks_h_
