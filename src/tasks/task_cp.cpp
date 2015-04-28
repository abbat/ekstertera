#include "task_cp.h"
//----------------------------------------------------------------------------------------------

EteraTaskCP::EteraTaskCP(const QString& source, const QString& target, bool overwrite) : EteraTaskAsync()
{
    m_args["source"]    = source;
    m_args["target"]    = target;
    m_args["overwrite"] = overwrite;
}
//----------------------------------------------------------------------------------------------

EteraTaskCP::~EteraTaskCP()
{
}
//----------------------------------------------------------------------------------------------

void EteraTaskCP::run()
{
    QString source    = m_args["source"].toString();
    QString target    = m_args["target"].toString();
    bool    overwrite = m_args["overwrite"].toBool();

    emit onStart(m_id, trUtf8("Копирование %1 в %2").arg(source).arg(target), m_args);

    init();

    if (m_api->cp(source, target, overwrite) == false)
        emit onError(m_id, m_api->lastErrorCode(), m_api->lastErrorMessage(), m_api->isAsyncError(), m_args);
    else {
        emit onStart(m_id, trUtf8("Чтение информации о %1").arg(target), m_args);

        EteraItem item;
        if (m_api->stat(target, item) == false)
            emit onError(m_id, m_api->lastErrorCode(), m_api->lastErrorMessage(), true, m_args);
        else {
            successLock();
            emit onSuccess(m_id, item, m_args);
            successUnlock();
        }
    }

    cleanup();
}
//----------------------------------------------------------------------------------------------
