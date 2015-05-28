/*!
 * \file
 * \brief Точка входа
 */
//----------------------------------------------------------------------------------------------
#include "utils/icon.h"
#include "utils/pool.h"
#include "utils/settings.h"
#include "utils/clipboard.h"
#include "utils/translator.h"
#include "forms/form_main.h"
//----------------------------------------------------------------------------------------------
/*!
 * (no comments)
 * \param argc (no comments)
 * \param argv (no comments)
 * \return (no comments)
 */
int main (int argc, char* argv[])
{
    // инициализация ресурсов
    Q_INIT_RESOURCE(resource);

    // инициализация приложения
    QApplication app(argc, argv);

#ifndef Q_WS_MAC
    // отображать иконки в меню для всех, кроме MacOS
    app.setAttribute(Qt::AA_DontShowIconsInMenus, false);
#endif

    // установка разделов для хранения конфигурации приложения
    QCoreApplication::setOrganizationName("ekstertera");
    QCoreApplication::setApplicationName("ekstertera");

    // инициализация настроек
    EteraSettings::init();
    // инициализация локализации
    EteraTranslator::init();
    // инициализация буфера обмена
    EteraClipboard::init();
    // инициализация пула асинхронных задач и api
    EteraThreadPool::init();
    // инициализация провайдера иконок
    EteraIconProvider::init();

    // главная форма
    FormMain* form = new FormMain();
    form->show();

    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

    int retval = app.exec();

    delete form;

    // очистка ресурсов провайдера иконок
    EteraIconProvider::cleanup();
    // очистка ресурсов пула асинхронных задач и api
    EteraThreadPool::cleanup();
    // очистка ресурсов буфера обмена
    EteraClipboard::cleanup();
    // очистка ресурсов локализации
    EteraTranslator::cleanup();
    // очистка ресурсов настроек
    EteraSettings::cleanup();

    return retval;
}
//----------------------------------------------------------------------------------------------
