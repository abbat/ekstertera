#include "task_info.h"
//----------------------------------------------------------------------------------------------

EteraTaskINFO::EteraTaskINFO() : EteraTaskSync()
{
}
//----------------------------------------------------------------------------------------------

EteraTaskINFO::~EteraTaskINFO()
{
}
//----------------------------------------------------------------------------------------------

void EteraTaskINFO::run()
{
    emit onStart(m_id, trUtf8("Получение информации о диске"), m_args);

    init();

    EteraInfo info;
    if (m_api->info(info) == false)
        emit onError(m_id, m_api->lastErrorCode(), m_api->lastErrorMessage(), m_args);
    else {
        successLock();
        emit onSuccess(m_id, info, m_args);
        successUnlock();
    }

    cleanup();
}
//----------------------------------------------------------------------------------------------
