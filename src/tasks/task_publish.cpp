#include "task_publish.h"
//----------------------------------------------------------------------------------------------

EteraTaskPUBLISH::EteraTaskPUBLISH(const QString& path) : EteraTaskSync()
{
    m_args["path"] = path;
}
//----------------------------------------------------------------------------------------------

EteraTaskPUBLISH::~EteraTaskPUBLISH()
{
}
//----------------------------------------------------------------------------------------------

void EteraTaskPUBLISH::run()
{
    QString path = m_args["path"].toString();

    emit onStart(m_id, trUtf8("Открытие доступа к %1").arg(path), m_args);

    init();

    if (m_api->publish(path) == false)
        emit onError(m_id, m_api->lastErrorCode(), m_api->lastErrorMessage(), m_args);
    else {
        emit onStart(m_id, trUtf8("Чтение информации о %1").arg(path), m_args);

        EteraItem item;
        if (m_api->stat(path, item) == false)
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
