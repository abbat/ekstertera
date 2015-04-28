#include "task_get.h"
//----------------------------------------------------------------------------------------------

EteraTaskGET::EteraTaskGET(const QString& source, const QString& target) : EteraTaskProgress()
{
    m_args["source"] = source;
    m_args["target"] = target;
}
//----------------------------------------------------------------------------------------------

EteraTaskGET::~EteraTaskGET()
{
}
//----------------------------------------------------------------------------------------------

void EteraTaskGET::run()
{
    QString source = m_args["source"].toString();
    QString target = m_args["target"].toString();

    emit onStart(m_id, trUtf8("Сохранение %1 в %2").arg(source).arg(target), m_args);

    init();

    if (m_api->get(source, target) == false)
        emit onError(m_id, m_api->lastErrorCode(), m_api->lastErrorMessage(), m_args);
    else {
        successLock();
        emit onSuccess(m_id, m_args);
        successUnlock();
    }

    cleanup();
}
//----------------------------------------------------------------------------------------------
