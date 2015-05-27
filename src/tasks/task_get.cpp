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

    QIODevice* device = NULL;

    if (target.isEmpty() == true) {
        // TODO:
        if (device == NULL)
            return;
    }

    if (device == NULL)
        emit onStart(m_id, trUtf8("Сохранение %1 в %2").arg(source).arg(target), m_args);
    else
        emit onStart(m_id, trUtf8("Получение %1").arg(source), m_args);

    init();

    bool result;
    if (device == NULL)
        result = m_api->get(source, target);
    else
        result = m_api->get(QUrl::fromEncoded(source.toUtf8()), device);

    if (result == false) {
        emit onError(m_id, m_api->lastErrorCode(), m_api->lastErrorMessage(), m_args);
    } else {
        successLock();
        emit onSuccess(m_id, m_args);
        successUnlock();
    }

    cleanup();
}
//----------------------------------------------------------------------------------------------
