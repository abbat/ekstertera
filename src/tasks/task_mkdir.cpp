#include "task_mkdir.h"
//----------------------------------------------------------------------------------------------

EteraTaskMKDIR::EteraTaskMKDIR(const QString& path) : EteraTaskSync()
{
    m_args["path"] = path;
}
//----------------------------------------------------------------------------------------------

EteraTaskMKDIR::~EteraTaskMKDIR()
{
}
//----------------------------------------------------------------------------------------------

void EteraTaskMKDIR::run()
{
    QString path = m_args["path"].toString();

    emit onStart(m_id, trUtf8("Создание директории %1").arg(path), m_args);

    init();

    if (m_api->mkdir(path) == false)
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
