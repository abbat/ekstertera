/*!
 * \file
 * \brief Элемент виджета проводника Яндекс.Диск
 */

#ifndef _ekstertera_widgets_widget_disk_item_h_
#define _ekstertera_widgets_widget_disk_item_h_

#include "utils/api.h"

/*!
 * \brief Элемент виджета проводника Яндекс.Диск
 */
class WidgetDiskItem : public QListWidgetItem
{
    public:

        /*!
         * \brief Конструктор
         * \param parent Родительский виджет
         * \param item Описатель элемента на диске
         * \param preview Флаг предпросмотра
         */
        WidgetDiskItem(QListWidget* parent, const EteraItem& item, bool preview);
        ~WidgetDiskItem();

        /*!
         * \brief Получение описателя элемента
         * \return Описатель элемента
         */
        const EteraItem* item() const { return &m_item; }

        /*!
         * \brief Замена описателя элемента
         * \param item Описатель элемента
         * \param preview Флаг предпросмотра
         */
        void replaceItem(const EteraItem& item, bool preview);

        /*!
         * \brief Откат текста элемента
         */
        void revertText();

        /*!
         * \brief Обновление внешнего вида
         * \param preview Флаг предпросмотра
         */
        void update(bool preview);

        /*!
         * Перегрузка оператора для сортировки
         */
        bool operator<(const QListWidgetItem& other) const;

    private:

        /*!
         * \brief Описатель объекта диска
         */
        EteraItem m_item;
};

#endif   // _ekstertera_widgets_widget_disk_item_h_
