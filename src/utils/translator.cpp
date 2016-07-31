#include "translator.h"
//----------------------------------------------------------------------------------------------
#include "settings.h"
//----------------------------------------------------------------------------------------------
static EteraTranslator* g_translator = NULL;
//----------------------------------------------------------------------------------------------

void EteraTranslator::init()
{
    g_translator = new EteraTranslator();
}
//----------------------------------------------------------------------------------------------

void EteraTranslator::cleanup()
{
    delete g_translator;
}
//----------------------------------------------------------------------------------------------

EteraTranslator* EteraTranslator::instance()
{
    return g_translator;
}
//----------------------------------------------------------------------------------------------

EteraTranslator::EteraTranslator()
{
    QString language = EteraSettings::instance()->language();

    if (language.isEmpty() == true)
        language = QLocale::system().name().split("_")[0];

    changeTranslator(language);
}
//----------------------------------------------------------------------------------------------

EteraTranslator::~EteraTranslator()
{
}
//----------------------------------------------------------------------------------------------

void EteraTranslator::changeTranslator(const QString& language)
{
    m_language = language;

    // переводчик стандартных сообщений и диалогов
    QCoreApplication::removeTranslator(&m_qt_translator);

    QString tr_file = QLibraryInfo::location(QLibraryInfo::TranslationsPath) + "/qt_" + m_language + ".qm";
    if (QFileInfo(tr_file).exists() == true) {
        m_qt_translator.load(tr_file, "");
        QCoreApplication::installTranslator(&m_qt_translator);
    }

#if QT_VERSION >= 0x050000
    // переводчик стандартных сообщений и диалогов Qt 5.x
    QCoreApplication::removeTranslator(&m_qt5_translator);

    tr_file = QLibraryInfo::location(QLibraryInfo::TranslationsPath) + "/qtbase_" + m_language + ".qm";
    if (QFileInfo(tr_file).exists() == true) {
        m_qt5_translator.load(tr_file, "");
        QCoreApplication::installTranslator(&m_qt5_translator);
    }
#endif

    // переводчик приложения
    QCoreApplication::removeTranslator(&m_app_translator);

    tr_file = ":translations/ekstertera_" + m_language + ".qm";
    if (QFileInfo(tr_file).exists() == true) {
        m_app_translator.load(tr_file, "");
        QCoreApplication::installTranslator(&m_app_translator);
    }
}
//----------------------------------------------------------------------------------------------
