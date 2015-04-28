#include "task_put.h"
//----------------------------------------------------------------------------------------------

EteraTaskPUT::EteraTaskPUT(const QString& source, const QString& target, bool overwrite) : EteraTaskProgress()
{
    m_args["source"]    = source;
    m_args["target"]    = target;
    m_args["overwrite"] = overwrite;
}
//----------------------------------------------------------------------------------------------

EteraTaskPUT::~EteraTaskPUT()
{
}
//----------------------------------------------------------------------------------------------

void EteraTaskPUT::run()
{
    QString source    = m_args["source"].toString();
    QString target    = m_args["target"].toString();
    bool    overwrite = m_args["overwrite"].toBool();

    emit onStart(m_id, trUtf8("Загрузка %1 в %2").arg(source).arg(target), m_args);

    init();

    if (m_api->put(source, target, overwrite) == false)
        emit onError(m_id, m_api->lastErrorCode(), m_api->lastErrorMessage(), m_args);
    else {
        emit onStart(m_id, trUtf8("Чтение информации о %1").arg(target), m_args);

        EteraItem item;
        if (m_api->stat(target, item) == false)
            emit onError(m_id, m_api->lastErrorCode(), m_api->lastErrorMessage(), m_args);
        else {
            successLock();
            emit onSuccess(m_id, item, m_args);
            successUnlock();
        }
    }

    cleanup();
}
//----------------------------------------------------------------------------------------------
