#include "task_sync.h"
//----------------------------------------------------------------------------------------------

EteraTaskSync::EteraTaskSync() : EteraTask()
{
}
//----------------------------------------------------------------------------------------------

EteraTaskSync::~EteraTaskSync()
{
}
//----------------------------------------------------------------------------------------------

void EteraTaskSync::init()
{
    m_api = new EteraAPI(NULL, true);

    m_api->setToken(m_token);

    connect(m_api, SIGNAL(onError(const QString&)), this, SLOT(api_on_error(const QString&)));
}
//----------------------------------------------------------------------------------------------

void EteraTaskSync::cleanup()
{
    delete m_api;
    m_api = NULL;
}
//----------------------------------------------------------------------------------------------

void EteraTaskSync::api_on_error(const QString& error)
{
    emit onError(m_id, 1, error, m_args);
}
//----------------------------------------------------------------------------------------------
