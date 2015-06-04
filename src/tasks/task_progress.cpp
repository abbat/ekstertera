#include "task_progress.h"
//----------------------------------------------------------------------------------------------

EteraTaskProgress::EteraTaskProgress() : EteraTaskSync()
{
}
//----------------------------------------------------------------------------------------------

EteraTaskProgress::~EteraTaskProgress()
{
}
//----------------------------------------------------------------------------------------------

void EteraTaskProgress::init()
{
    m_api = new EteraAPI(NULL, false);

    m_api->setToken(m_token);

    connect(m_api, SIGNAL(onProgress(qint64, qint64)), this, SLOT(api_on_progress(qint64, qint64)));
    connect(m_api, SIGNAL(onError(const QString&)), this, SLOT(api_on_error(const QString&)));
}
//----------------------------------------------------------------------------------------------

void EteraTaskProgress::cleanup()
{
    delete m_api;
    m_api = NULL;
}
//----------------------------------------------------------------------------------------------

void EteraTaskProgress::api_on_progress(qint64 done, qint64 total)
{
    if (m_api != NULL) /* Fix ASSERT: "staticMetaObject.cast(_o)" */
        emit onProgress(m_id, done, total);
}
//----------------------------------------------------------------------------------------------
