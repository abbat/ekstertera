#include "task_stat.h"
//----------------------------------------------------------------------------------------------

EteraTaskSTAT::EteraTaskSTAT(const QString& path, const QString& preview, bool crop) : EteraTaskSync()
{
    m_args["path"]    = path;
    m_args["preview"] = preview;
    m_args["crop"]    = crop;
}
//----------------------------------------------------------------------------------------------

EteraTaskSTAT::~EteraTaskSTAT()
{
}
//----------------------------------------------------------------------------------------------

void EteraTaskSTAT::run()
{
    QString path    = m_args["path"].toString();
    QString preview = m_args["preview"].toString();
    bool    crop    = m_args["crop"].toBool();

    emit onStart(m_id, trUtf8("Чтение информации о %1").arg(path), m_args);

    init();

    EteraItem item;
    if (m_api->stat(path, item, preview, crop) == false)
        emit onError(m_id, m_api->lastErrorCode(), m_api->lastErrorMessage(), m_args);
    else {
        successLock();
        emit onSuccess(m_id, item, m_args);
        successUnlock();
    }

    cleanup();
}
//----------------------------------------------------------------------------------------------
