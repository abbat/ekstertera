#include "task_rm.h"
//----------------------------------------------------------------------------------------------

EteraTaskRM::EteraTaskRM(const QString& path, bool permanently) : EteraTaskAsync()
{
    m_args["path"]        = path;
    m_args["permanently"] = permanently;
}
//----------------------------------------------------------------------------------------------

EteraTaskRM::~EteraTaskRM()
{
}
//----------------------------------------------------------------------------------------------

void EteraTaskRM::run()
{
    QString path        = m_args["path"].toString();
    bool    permanently = m_args["permanently"].toBool();

    emit onStart(m_id, trUtf8("Удаление %1").arg(path), m_args);

    init();

    if (m_api->rm(path, permanently) == false)
        emit onError(m_id, m_api->lastErrorCode(), m_api->lastErrorMessage(), m_api->isAsyncError(), m_args);
    else {
        successLock();
        emit onSuccess(m_id, m_args);
        successUnlock();
    }

    cleanup();
}
//----------------------------------------------------------------------------------------------
