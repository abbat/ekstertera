#include "task_async.h"
//----------------------------------------------------------------------------------------------

EteraTaskAsync::EteraTaskAsync() : EteraTask()
{
}
//----------------------------------------------------------------------------------------------

EteraTaskAsync::~EteraTaskAsync()
{
}
//----------------------------------------------------------------------------------------------

void EteraTaskAsync::init()
{
    m_api = new EteraAPI(true);

    m_api->setToken(m_token);

    connect(m_api, SIGNAL(onError(const QString&)), this, SLOT(api_on_error(const QString&)));
}
//----------------------------------------------------------------------------------------------

void EteraTaskAsync::cleanup()
{
    delete m_api;
    m_api = NULL;
}
//----------------------------------------------------------------------------------------------

void EteraTaskAsync::api_on_error(const QString& error)
{
    emit onError(m_id, 1, error, m_api->isAsyncError(), m_args);
}
//----------------------------------------------------------------------------------------------
