#include "task_ls.h"
//----------------------------------------------------------------------------------------------

EteraTaskLS::EteraTaskLS(const QString& path, const QString& preview, bool crop) : EteraTaskSync()
{
    m_args["path"]    = path;
    m_args["preview"] = preview;
    m_args["crop"]    = crop;
}
//----------------------------------------------------------------------------------------------

EteraTaskLS::~EteraTaskLS()
{
}
//----------------------------------------------------------------------------------------------

void EteraTaskLS::run()
{
    QString path    = m_args["path"].toString();
    QString preview = m_args["preview"].toString();
    bool    crop    = m_args["crop"].toBool();

    emit onStart(m_id, trUtf8("Чтение директории %1").arg(path), m_args);

    init();

    EteraItemList list;
    if (m_api->ls(path, list, preview, crop) == false)
        emit onError(m_id, m_api->lastErrorCode(), m_api->lastErrorMessage(), m_args);
    else {
        successLock();
        emit onSuccess(m_id, list, m_args);
        successUnlock();
    }

    cleanup();
}
//----------------------------------------------------------------------------------------------
