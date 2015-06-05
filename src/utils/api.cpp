#include "api.h"
//----------------------------------------------------------------------------------------------
#include <unistd.h>
//----------------------------------------------------------------------------------------------
/*!
 * \brief Флаг разрешения работы api
 */
static bool g_api_enabled = false;
//----------------------------------------------------------------------------------------------

EteraInfo::EteraInfo()
{
    m_used  = 0;
    m_free  = 0;
    m_total = 0;
    m_trash = 0;
}
//----------------------------------------------------------------------------------------------

bool EteraInfo::parse(const QString& body)
{
    bool ok;
    QtJson::JsonObject json = QtJson::parse(body, ok).toMap();
    if (ok == false)
        return false;

    m_used = json["used_space"].toULongLong(&ok);
    if (ok == false)
        return false;

    m_total = json["total_space"].toULongLong(&ok);
    if (ok == false)
        return false;

    m_trash = json["trash_size"].toULongLong(&ok);
    if (ok == false)
        return false;

    if (m_total < m_used)
        m_free = 0;
    else
        m_free = m_total - m_used;

    QtJson::JsonObject system_folders = json["system_folders"].toMap();

    m_applications = system_folders["applications"].toString();
    m_downloads    = system_folders["downloads"].toString();

    return true;
}
//----------------------------------------------------------------------------------------------

EteraItem::EteraItem()
{
    m_type       = eitUnknown;
    m_size       = 0;
    m_media_type = eimtUnknown;
}
//----------------------------------------------------------------------------------------------

bool EteraItem::parse(const QString& body)
{
    bool ok;
    QtJson::JsonObject json = QtJson::parse(body, ok).toMap();
    if (ok == false)
        return false;

    return parse(json);
}
//----------------------------------------------------------------------------------------------

bool EteraItem::parse(const QtJson::JsonObject& json)
{
    bool ok;

    m_name = json["name"].toString();
    m_path = json["path"].toString();

    m_public_key = json["public_key"].toString();
    m_public_url = json["public_url"].toString();

    m_created = json["created"].toDateTime();
    if (m_created.isValid() == false)
        return false;

    // тайм-зона не парсится, предполагаем UTC
    m_created.setTimeSpec(Qt::UTC);

    m_modified = json["modified"].toDateTime();
    if (m_modified.isValid() == false)
        return false;

    // тайм-зона не парсится, предполагаем UTC
    m_modified.setTimeSpec(Qt::UTC);

    QString type = json["type"].toString();
    if (type == "file")
        m_type = eitFile;
    else if (type == "dir")
        m_type = eitDir;
    else
        return false;

    if (m_type == eitDir)
        return true;

    m_md5       = json["md5"].toString();
    m_mime_type = json["mime_type"].toString();
    m_preview   = json["preview"].toString();

    m_size = json["size"].toULongLong(&ok);
    if (ok == false)
        return false;

    m_media_string = json["media_type"].toString();
    if (m_media_string == "audio")
        m_media_type = eimtAudio;
    else if (m_media_string == "backup")
        m_media_type = eimtBackup;
    else if (m_media_string == "book")
        m_media_type = eimtBook;
    else if (m_media_string == "compressed")
        m_media_type = eimtCompressed;
    else if (m_media_string == "data")
        m_media_type = eimtData;
    else if (m_media_string == "development")
        m_media_type = eimtDevelopment;
    else if (m_media_string == "diskimage")
        m_media_type = eimtDiskimage;
    else if (m_media_string == "document")
        m_media_type = eimtDocument;
    else if (m_media_string == "encoded")
        m_media_type = eimtEncoded;
    else if (m_media_string == "executable")
        m_media_type = eimtExecutable;
    else if (m_media_string == "flash")
        m_media_type = eimtFlash;
    else if (m_media_string == "font")
        m_media_type = eimtFont;
    else if (m_media_string == "image")
        m_media_type = eimtImage;
    else if (m_media_string == "settings")
        m_media_type = eimtSettings;
    else if (m_media_string == "spreadsheet")
        m_media_type = eimtSpreadsheet;
    else if (m_media_string == "text")
        m_media_type = eimtText;
    else if (m_media_string == "unknown")
        m_media_type = eimtUnknown;
    else if (m_media_string == "video")
        m_media_type = eimtVideo;
    else if (m_media_string == "web")
        m_media_type = eimtWeb;
    else
        return false;

    return true;
}
//----------------------------------------------------------------------------------------------

QString EteraItem::extension() const
{
    if (isFile() == true) {
        int idx = m_name.lastIndexOf(".");
        if (idx != -1)
            return m_name.right(m_name.length() - idx - 1).toLower();
    }

    return "";
}
//----------------------------------------------------------------------------------------------

QString EteraItem::basePath() const
{
    if (isDir() == true)
        return m_path + "/";
    else if (isFile() == true)
        return m_path.left(m_path.length() - m_name.length());

    return "";
}
//----------------------------------------------------------------------------------------------

QString EteraItem::parentPath() const
{
    if (isDir() == true)
        return m_path.left(m_path.length() - m_name.length());
    else if (isFile() == true)
        return basePath();

    return "";
}
//----------------------------------------------------------------------------------------------

void EteraAPI::init()
{
#if QT_VERSION < 0x050400
    // предпочитаемый список шифров tls1 (см. ниже про версии Qt и поддержку)
    // openssl ciphers -tls1 -s 'HIGH:!TLSv1.2:!aNULL:!MD5:!3DES:!CAMELLIA:!SRP:!PSK:@STRENGTH'
    static const char* ETERA_CIPHERS[] =
    {
        "ECDHE-RSA-AES256-SHA",     // SSLv3 Kx=ECDH        Au=RSA   Enc=AES(256) Mac=SHA1
        "ECDHE-ECDSA-AES256-SHA",   // SSLv3 Kx=ECDH        Au=ECDSA Enc=AES(256) Mac=SHA1
        "DHE-RSA-AES256-SHA",       // SSLv3 Kx=DH          Au=RSA   Enc=AES(256) Mac=SHA1
        "DHE-DSS-AES256-SHA",       // SSLv3 Kx=DH          Au=DSS   Enc=AES(256) Mac=SHA1
        "ECDH-RSA-AES256-SHA",      // SSLv3 Kx=ECDH/RSA    Au=ECDH  Enc=AES(256) Mac=SHA1
        "ECDH-ECDSA-AES256-SHA",    // SSLv3 Kx=ECDH/ECDSA  Au=ECDH  Enc=AES(256) Mac=SHA1
        "AES256-SHA",               // SSLv3 Kx=RSA         Au=RSA   Enc=AES(256) Mac=SHA1
        "ECDHE-RSA-AES128-SHA",     // SSLv3 Kx=ECDH        Au=RSA   Enc=AES(128) Mac=SHA1
        "ECDHE-ECDSA-AES128-SHA",   // SSLv3 Kx=ECDH        Au=ECDSA Enc=AES(128) Mac=SHA1
        "DHE-RSA-AES128-SHA",       // SSLv3 Kx=DH          Au=RSA   Enc=AES(128) Mac=SHA1
        "DHE-DSS-AES128-SHA",       // SSLv3 Kx=DH          Au=DSS   Enc=AES(128) Mac=SHA1
        "ECDH-RSA-AES128-SHA",      // SSLv3 Kx=ECDH/RSA    Au=ECDH  Enc=AES(128) Mac=SHA1
        "ECDH-ECDSA-AES128-SHA",    // SSLv3 Kx=ECDH/ECDSA  Au=ECDH  Enc=AES(128) Mac=SHA1
        "AES128-SHA",               // SSLv3 Kx=RSA         Au=RSA   Enc=AES(128) Mac=SHA1
        NULL
    };
#else
    // предпочитаемый список шифров tls1.2 (см. ниже про версии Qt и поддержку)
    // openssl ciphers -tls1 -s 'HIGH:!aNULL:!MD5:!3DES:!CAMELLIA:!SRP:!PSK:@STRENGTH'
    static const char* ETERA_CIPHERS[] =
    {
        "ECDHE-RSA-AES256-GCM-SHA384",     // TLSv1.2 Kx=ECDH       Au=RSA   Enc=AESGCM(256) Mac=AEAD
        "ECDHE-ECDSA-AES256-GCM-SHA384",   // TLSv1.2 Kx=ECDH       Au=ECDSA Enc=AESGCM(256) Mac=AEAD
        "ECDHE-RSA-AES256-SHA384",         // TLSv1.2 Kx=ECDH       Au=RSA   Enc=AES(256)    Mac=SHA384
        "ECDHE-ECDSA-AES256-SHA384",       // TLSv1.2 Kx=ECDH       Au=ECDSA Enc=AES(256)    Mac=SHA384
        "ECDHE-RSA-AES256-SHA",            // SSLv3   Kx=ECDH       Au=RSA   Enc=AES(256)    Mac=SHA1
        "ECDHE-ECDSA-AES256-SHA",          // SSLv3   Kx=ECDH       Au=ECDSA Enc=AES(256)    Mac=SHA1
        "DHE-DSS-AES256-GCM-SHA384",       // TLSv1.2 Kx=DH         Au=DSS   Enc=AESGCM(256) Mac=AEAD
        "DHE-RSA-AES256-GCM-SHA384",       // TLSv1.2 Kx=DH         Au=RSA   Enc=AESGCM(256) Mac=AEAD
        "DHE-RSA-AES256-SHA256",           // TLSv1.2 Kx=DH         Au=RSA   Enc=AES(256)    Mac=SHA256
        "DHE-DSS-AES256-SHA256",           // TLSv1.2 Kx=DH         Au=DSS   Enc=AES(256)    Mac=SHA256
        "DHE-RSA-AES256-SHA",              // SSLv3   Kx=DH         Au=RSA   Enc=AES(256)    Mac=SHA1
        "DHE-DSS-AES256-SHA",              // SSLv3   Kx=DH         Au=DSS   Enc=AES(256)    Mac=SHA1
        "ECDH-RSA-AES256-GCM-SHA384",      // TLSv1.2 Kx=ECDH/RSA   Au=ECDH  Enc=AESGCM(256) Mac=AEAD
        "ECDH-ECDSA-AES256-GCM-SHA384",    // TLSv1.2 Kx=ECDH/ECDSA Au=ECDH  Enc=AESGCM(256) Mac=AEAD
        "ECDH-RSA-AES256-SHA384",          // TLSv1.2 Kx=ECDH/RSA   Au=ECDH  Enc=AES(256)    Mac=SHA384
        "ECDH-ECDSA-AES256-SHA384",        // TLSv1.2 Kx=ECDH/ECDSA Au=ECDH  Enc=AES(256)    Mac=SHA384
        "ECDH-RSA-AES256-SHA",             // SSLv3   Kx=ECDH/RSA   Au=ECDH  Enc=AES(256)    Mac=SHA1
        "ECDH-ECDSA-AES256-SHA",           // SSLv3   Kx=ECDH/ECDSA Au=ECDH  Enc=AES(256)    Mac=SHA1
        "AES256-GCM-SHA384",               // TLSv1.2 Kx=RSA        Au=RSA   Enc=AESGCM(256) Mac=AEAD
        "AES256-SHA256",                   // TLSv1.2 Kx=RSA        Au=RSA   Enc=AES(256)    Mac=SHA256
        "AES256-SHA",                      // SSLv3   Kx=RSA        Au=RSA   Enc=AES(256)    Mac=SHA1
        "ECDHE-RSA-AES128-GCM-SHA256",     // TLSv1.2 Kx=ECDH       Au=RSA   Enc=AESGCM(128) Mac=AEAD
        "ECDHE-ECDSA-AES128-GCM-SHA256",   // TLSv1.2 Kx=ECDH       Au=ECDSA Enc=AESGCM(128) Mac=AEAD
        "ECDHE-RSA-AES128-SHA256",         // TLSv1.2 Kx=ECDH       Au=RSA   Enc=AES(128)    Mac=SHA256
        "ECDHE-ECDSA-AES128-SHA256",       // TLSv1.2 Kx=ECDH       Au=ECDSA Enc=AES(128)    Mac=SHA256
        "ECDHE-RSA-AES128-SHA",            // SSLv3   Kx=ECDH       Au=RSA   Enc=AES(128)    Mac=SHA1
        "ECDHE-ECDSA-AES128-SHA",          // SSLv3   Kx=ECDH       Au=ECDSA Enc=AES(128)    Mac=SHA1
        "DHE-DSS-AES128-GCM-SHA256",       // TLSv1.2 Kx=DH         Au=DSS   Enc=AESGCM(128) Mac=AEAD
        "DHE-RSA-AES128-GCM-SHA256",       // TLSv1.2 Kx=DH         Au=RSA   Enc=AESGCM(128) Mac=AEAD
        "DHE-RSA-AES128-SHA256",           // TLSv1.2 Kx=DH         Au=RSA   Enc=AES(128)    Mac=SHA256
        "DHE-DSS-AES128-SHA256",           // TLSv1.2 Kx=DH         Au=DSS   Enc=AES(128)    Mac=SHA256
        "DHE-RSA-AES128-SHA",              // SSLv3   Kx=DH         Au=RSA   Enc=AES(128)    Mac=SHA1
        "DHE-DSS-AES128-SHA",              // SSLv3   Kx=DH         Au=DSS   Enc=AES(128)    Mac=SHA1
        "ECDH-RSA-AES128-GCM-SHA256",      // TLSv1.2 Kx=ECDH/RSA   Au=ECDH  Enc=AESGCM(128) Mac=AEAD
        "ECDH-ECDSA-AES128-GCM-SHA256",    // TLSv1.2 Kx=ECDH/ECDSA Au=ECDH  Enc=AESGCM(128) Mac=AEAD
        "ECDH-RSA-AES128-SHA256",          // TLSv1.2 Kx=ECDH/RSA   Au=ECDH  Enc=AES(128)    Mac=SHA256
        "ECDH-ECDSA-AES128-SHA256",        // TLSv1.2 Kx=ECDH/ECDSA Au=ECDH  Enc=AES(128)    Mac=SHA256
        "ECDH-RSA-AES128-SHA",             // SSLv3   Kx=ECDH/RSA   Au=ECDH  Enc=AES(128)    Mac=SHA1
        "ECDH-ECDSA-AES128-SHA",           // SSLv3   Kx=ECDH/ECDSA Au=ECDH  Enc=AES(128)    Mac=SHA1
        "AES128-GCM-SHA256",               // TLSv1.2 Kx=RSA        Au=RSA   Enc=AESGCM(128) Mac=AEAD
        "AES128-SHA256",                   // TLSv1.2 Kx=RSA        Au=RSA   Enc=AES(128)    Mac=SHA256
        "AES128-SHA",                      // SSLv3   Kx=RSA        Au=RSA   Enc=AES(128)    Mac=SHA1
        NULL
    };
#endif

    // поддерживаемый список шифров
    QList<QSslCipher> cipher_list;

    const char** ciphers = ETERA_CIPHERS;
    while ((*ciphers) != NULL) {
        QSslCipher cipher(*ciphers, QSsl::SslV3);
        if (cipher.isNull() == false)
            cipher_list.append(cipher);

        ciphers++;
    }

    // задание конфигурации ssl по умолчанию
    QSslConfiguration ssl_default = QSslConfiguration::defaultConfiguration();

#if QT_VERSION < 0x050000
    // до версии 5.0.x поддерживается только TLSv1
    // т.о. для отключения SSLv3 включаем принудительный TLSv1
    ssl_default.setProtocol(QSsl::TlsV1);
#else
    #if QT_VERSION < 0x050400
        // до версии 5.4.0 для QSsl::SecureProtocols используется
        // SSLv23_client_method и SSL_OP_ALL | SSL_OP_NO_SSLv2
        // т.о. для отключения SSLv3 включаем принудительный TLSv1
        ssl_default.setProtocol(QSsl::TlsV1_0);
    #else
        // в версии 5.4.0 для QSsl::SecureProtocols используется
        // SSLv23_client_method и SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3
        // т.о. можем работать TLSv1+
        ssl_default.setProtocol(QSsl::SecureProtocols);
    #endif
#endif

    ssl_default.setCiphers(cipher_list);

    QSslConfiguration::setDefaultConfiguration(ssl_default);

    qRegisterMetaType<EteraInfo>("EteraInfo");
    qRegisterMetaType<EteraItem>("EteraItem");
    qRegisterMetaType<EteraItemList>("EteraItemList");

    g_api_enabled = true;
}
//----------------------------------------------------------------------------------------------

void EteraAPI::cleanup()
{
    g_api_enabled = false;
}
//----------------------------------------------------------------------------------------------

EteraAPI::EteraAPI(QObject* parent, bool noprogress) : QObject(parent)
{
    m_io    = NULL;
    m_reply = NULL;

    m_user_agent = QString("ekstertera/%1").arg(ETERA_VERSION);
    m_base_url   = "https://cloud-api.yandex.net/v1/disk";
    m_app_id     = "51cba73d70c343fd96f0765e1eeb0435";
    m_app_secret = "73cf4dacd4f74e7a97b77d036f90eb91";
    m_limit      = 1000;

    m_noprogress = noprogress;

    retranslateUi();

    setLastError(0);
}
//----------------------------------------------------------------------------------------------

EteraAPI::~EteraAPI()
{
}
//----------------------------------------------------------------------------------------------

void EteraAPI::retranslateUi()
{
    OK_MESSAGE              = trUtf8("OK");
    JSON_PARSE_ERROR        = trUtf8("Ошибка разбора JSON");
    MALFORMED_LINK_URL      = trUtf8("Неподдерживаемый URL объекта Link");
    UNSUPPORTED_LINK_METHOD = trUtf8("Неподдерживаемый метод объекта Link");
    FILE_OPEN_ERROR         = trUtf8("Ошибка открытия файла");
}
//----------------------------------------------------------------------------------------------

void EteraAPI::on_about_to_quit()
{
    if (m_reply != NULL)
        m_reply->abort();

    // приложение завершает работу - запрет работы api
    g_api_enabled = false;
}
//----------------------------------------------------------------------------------------------

void EteraAPI::on_download_progress(qint64 done, qint64 total)
{
    if (g_api_enabled == true)
        emit onProgress(done, total);
    else
        m_reply->abort();
}
//----------------------------------------------------------------------------------------------

void EteraAPI::on_upload_progress(qint64 done, qint64 total)
{
    if (g_api_enabled == true)
        emit onProgress(done, total);
    else
        m_reply->abort();
}
//----------------------------------------------------------------------------------------------

void EteraAPI::on_ssl_errors(const QList<QSslError>& errors)
{
    QString message;
    for (int i = 0; i < errors.count(); i++)
        message += errors[i].errorString() + "\n";

    emit onError(message);
    emit onError(this);
}
//----------------------------------------------------------------------------------------------

void EteraAPI::on_ready_read()
{
    if (g_api_enabled == true)
        m_io->write(m_reply->readAll());
    else
        m_reply->abort();
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::setLastError(int code, const QString& message)
{
    m_error_code = code;

    if (code != 0) {
        m_error_message = QString("[%1]: %2").arg(code).arg(message);
        emit onError(this);
    } else {
        m_async_error   = false;
        m_error_message = OK_MESSAGE;
    }

    return code == 0;
}
//----------------------------------------------------------------------------------------------

QString EteraAPI::humanZeros(const QString& val)
{
    int i = val.length() - 1;
    while (i >= 0 && (val[i] == '0' || val[i] == '.'))
        i--;

    return val.left(i + 1);
}
//----------------------------------------------------------------------------------------------

QString EteraAPI::humanBytes(quint64 val)
{
    QString prefix;
    QString postfix;

    if (val < (quint64)1024) {
        prefix = QString::number(val);
    } else if (val < (quint64)1024 * 1024) {
        prefix  = QString("%1").arg((double)val / 1024, 0, 'f', 0);
        postfix = trUtf8("КБ");
    } else if (val < (quint64)1024 * 1024 * 1024) {
        prefix  = QString("%1").arg((double)val / 1024 / 1024, 0, 'f', 0);
        postfix = trUtf8("МБ");
    } else if (val < (quint64)1024 * 1024 * 1024 * 1024) {
        prefix  = humanZeros(QString("%1").arg((double)val / 1024 / 1024 / 1024, 0, 'f', 2));
        postfix = trUtf8("ГБ");
    } else {
        prefix  = humanZeros(QString("%1").arg((double)val / 1024 / 1024 / 1024 / 1024, 0, 'f', 2));
        postfix = trUtf8("ТБ");
    }

    return prefix + postfix;
}
//----------------------------------------------------------------------------------------------

void EteraAPI::setDefaultHeaders(QNetworkRequest& request, quint64 length, bool auth, bool any)
{
    request.setRawHeader("User-Agent", m_user_agent.toUtf8());

    if (any == false)
        request.setRawHeader("Accept", "application/json");
    else
        request.setRawHeader("Accept", "*/*");

    if (auth == true)
        request.setRawHeader("Authorization", QString("OAuth %1").arg(m_token).toUtf8());

    if (length != 0)
        request.setHeader(QNetworkRequest::ContentLengthHeader, QString::number(length).toUtf8());
}
//----------------------------------------------------------------------------------------------

void EteraAPI::prepareRequest(QNetworkRequest& request, const QString& relurl, const EteraArgs& args, quint64 length)
{
    QUrl url(m_base_url + relurl);

#if QT_VERSION >= 0x050000
    QUrlQuery query;
#endif

    for (EteraArgs::const_iterator i = args.constBegin(); i != args.constEnd(); i++)
#if QT_VERSION < 0x050000
        url.addQueryItem(i.key(), i.value());
#else
        query.addQueryItem(i.key(), i.value());
#endif

#if QT_VERSION >= 0x050000
    url.setQuery(query);
#endif

    request.setUrl(url);

    setDefaultHeaders(request, length);
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::makeRequest(const QNetworkRequest& request, int& code, QString& body, EteraRequestMethod method, const QString& data, QIODevice* io)
{
    if (g_api_enabled == false)
        return setLastError(1, trUtf8("API не инициализировано"));

    QNetworkReply* reply;

    switch (method) {
        case ermGET:
            reply = m_http.get(request);
            break;

        case ermPOST:
            if (io != NULL)
                reply = m_http.post(request, io);
            else
                reply = m_http.post(request, data.toUtf8());
            break;

        case ermPUT:
            if (io != NULL)
                reply = m_http.put(request, io);
            else
                reply = m_http.put(request, data.toUtf8());
            break;

        case ermDELETE:
            reply = m_http.deleteResource(request);
            break;

        default:
            return setLastError(1, "Not implemented");
    }

    m_reply = reply;

    QObject::connect(reply, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(on_ssl_errors(const QList<QSslError>&)));

    if (m_noprogress == false) {
        QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(on_download_progress(qint64, qint64)));
        QObject::connect(reply, SIGNAL(uploadProgress(qint64, qint64)),   this, SLOT(on_upload_progress(qint64, qint64)));
    }

    if (method == ermGET && io != NULL) {
        m_io = io;
        QObject::connect(reply, SIGNAL(readyRead()), this, SLOT(on_ready_read()));
    }

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(on_about_to_quit()));
    loop.exec();

    m_io    = NULL;
    m_reply = NULL;

    QVariant http_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if (http_code.isValid() == false || http_code.isNull() == true) {
        QNetworkReply::NetworkError error   = reply->error();
        QString                     message = reply->errorString();
        return setLastError(error, message);
    }

    code = http_code.toInt();

    if (code >= 200 && code < 300)
        body = QString::fromUtf8(reply->readAll());
    else if (code == 301 || code == 302)
        body = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl().toString();
    else
        body = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

    delete reply;

    return true;
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::startRequest(const QNetworkRequest& request, EteraRequestMethod method, const QString& data, QIODevice* io)
{
    m_io    = NULL;
    m_reply = NULL;

    if (g_api_enabled == false)
        return setLastError(1, trUtf8("API не инициализировано"));

    QNetworkReply* reply;

    switch (method) {
        case ermGET:
            reply = m_http.get(request);
            break;

        case ermPOST:
            if (io != NULL)
                reply = m_http.post(request, io);
            else
                reply = m_http.post(request, data.toUtf8());
            break;

        case ermPUT:
            if (io != NULL)
                reply = m_http.put(request, io);
            else
                reply = m_http.put(request, data.toUtf8());
            break;

        case ermDELETE:
            reply = m_http.deleteResource(request);
            break;

        default:
            return setLastError(1, "Not implemented");
    }

    m_io    = io;
    m_reply = reply;

    QObject::connect(reply, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(on_ssl_errors(const QList<QSslError>&)));

    if (method == ermGET && io != NULL)
        QObject::connect(reply, SIGNAL(readyRead()), this, SLOT(on_ready_read()));

    return true;
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::parseReply(int& code, QString& body)
{
    QVariant http_code = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if (http_code.isValid() == false || http_code.isNull() == true) {
        QNetworkReply::NetworkError error   = m_reply->error();
        QString                     message = m_reply->errorString();

        m_reply->close();
        m_reply->deleteLater();
        m_reply = NULL;

        return setLastError(error, message);
    }

    code = http_code.toInt();

    if (code >= 200 && code < 300)
        body = QString::fromUtf8(m_reply->readAll());
    else if (code == 301 || code == 302)
        body = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl().toString();
    else
        body = m_reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

    m_reply->close();
    m_reply->deleteLater();
    m_reply = NULL;

    return true;
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::makeSimpleRequest(int& code, QString& body, const QString& url, const EteraArgs& args, EteraRequestMethod method)
{
    setLastError(0);

    QNetworkRequest request;
    prepareRequest(request, url, args);

    if (method == ermPOST) {
        // content-type missing in HTTP POST, defaulting to application/x-www-form-urlencoded.
        request.setHeader(QNetworkRequest::ContentTypeHeader,   "application/json");
        request.setHeader(QNetworkRequest::ContentLengthHeader, "0");
    }

    return makeRequest(request, code, body, method);
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::startSimpleRequest(const QString& url, const EteraArgs& args, EteraRequestMethod method)
{
    setLastError(0);

    QNetworkRequest request;
    prepareRequest(request, url, args);

    if (method == ermPOST) {
        // content-type missing in HTTP POST, defaulting to application/x-www-form-urlencoded.
        request.setHeader(QNetworkRequest::ContentTypeHeader,   "application/json");
        request.setHeader(QNetworkRequest::ContentLengthHeader, "0");
    }

    return startRequest(request, method);
}
//----------------------------------------------------------------------------------------------

QString EteraAPI::getToken(const QString& auth_code)
{
    setLastError(0);

    // базовый домен для авторизации по OAuth, разный для разных языков
    QString oauth_base = trUtf8("oauth.yandex.ru");

    if (auth_code.isEmpty() == true) {
        QString oauth_url = QString("https://%1/authorize?response_type=code&client_id=%2").arg(oauth_base).arg(m_app_id);
        QDesktopServices::openUrl(oauth_url);
        return "";
    }

    QString oauth_url  = QString("https://%1/token").arg(oauth_base);
    QString oauth_data = QString("grant_type=authorization_code&code=%1&client_id=%2&client_secret=%3").arg(auth_code).arg(m_app_id).arg(m_app_secret);

    QNetworkRequest request(oauth_url);
    setDefaultHeaders(request, oauth_data.length(), false);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    bool    ok;
    int     code;
    QString body;

    if (makeRequest(request, code, body, ermPOST, oauth_data) != true)
        return "";

    if (code != 200) {
        setLastError(code, body);
        return "";
    }

    QtJson::JsonObject json = QtJson::parse(body, ok).toMap();
    if (ok == false) {
        setLastError(1, JSON_PARSE_ERROR);
        return "";
    }

    QString result = json["access_token"].toString();

    if (result.isEmpty() == true) {
        setLastError(code, body);
        return "";
    }

    setLastError(0);

    return result;
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::checkYandexDomain(const QUrl& url)
{
    if (url.isValid() == false)
        return false;

    if (url.scheme().compare("https", Qt::CaseInsensitive) != 0)
        return false;

    QRegExp re("^[a-z0-9\\.\\-]+\\.yandex\\.(net|ru|com|com\\.tr)$", Qt::CaseInsensitive);
    if (url.host().indexOf(re) != 0)
        return false;

    return true;
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::parseLink(const QString& link, QUrl& url, EteraRequestMethod& method)
{
    bool ok;
    QtJson::JsonObject json = QtJson::parse(link, ok).toMap();
    if (ok == false)
        return setLastError(1, JSON_PARSE_ERROR);

    QString _url = json["href"].toString();

    url = QUrl::fromEncoded(_url.toUtf8());

    if (checkYandexDomain(url) == false)
        return setLastError(1, MALFORMED_LINK_URL);

    QString s_method = json["method"].toString().toUpper();
    if (s_method == "GET")
        method = ermGET;
    else if (s_method == "PUT")
        method = ermPUT;
    else if (s_method == "POST")
        method = ermPOST;
    else if (s_method == "DELETE")
        method = ermDELETE;
    else
        return setLastError(1, JSON_PARSE_ERROR);

    return true;
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::wait(const QString& link)
{
    // взвод флага ошибки ожидания
    m_async_error = true;

    QUrl               url;
    EteraRequestMethod method;

    if (parseLink(link, url, method) == false)
        return false;

    if (method != ermGET)
        return setLastError(1, UNSUPPORTED_LINK_METHOD);

    QNetworkRequest request(url);
    setDefaultHeaders(request);

    while (true) {
        sleep(1);

        bool    ok;
        int     code;
        QString body;

        if (makeRequest(request, code, body, method) == false)
            return false;

        if (code != 200)
            return setLastError(code, body);

        QtJson::JsonObject json = QtJson::parse(body, ok).toMap();
        if (ok == false)
            return setLastError(1, JSON_PARSE_ERROR);

        QString status = json["status"].toString().toLower();
        if (status == "in-progress")
            continue;
        else if (status == "failure") {
            // операция завершилась неудачно, но не во время ожидания - сброс флага
            m_async_error = false;
            return setLastError(1, trUtf8("Операцию завершить не удалось, попробуйте повторить запрос позже"));
        } else if (status == "success")
            break;
        else
            return setLastError(1, trUtf8("Неизвестный статус операции"));
    }

    return setLastError(0);
}
//----------------------------------------------------------------------------------------------

void EteraAPI::info()
{
    if (startSimpleRequest("") == true)
        connect(m_reply, SIGNAL(finished()), this, SLOT(on_info_finished()));
}
//----------------------------------------------------------------------------------------------

void EteraAPI::on_info_finished()
{
    int     code;
    QString body;

    if (parseReply(code, body) == false)
        return;

    if (code != 200) {
        setLastError(code, body);
        return;
    }

    EteraInfo info;
    if (info.parse(body) == false) {
        setLastError(1, JSON_PARSE_ERROR);
        return;
    }

    setLastError(0);

    emit onINFO(this, info);
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::stat(const QString& path, EteraItem& result, const QString& preview, bool crop)
{
    EteraArgs args;

    args["path"]   = path;
    args["offset"] = "0";
    args["limit"]  = "0";

    if (preview.isEmpty() == false)
        args["preview_size"] = preview;
    if (crop == true)
        args["preview_crop"] = "true";

    int     code;
    QString body;

    if (makeSimpleRequest(code, body, "/resources", args) == false)
        return false;

    if (code != 200)
        return setLastError(code, body);

    if (result.parse(body) == false)
        return setLastError(1, JSON_PARSE_ERROR);

    return setLastError(0);
}
//----------------------------------------------------------------------------------------------

void EteraAPI::stat(const QString& path, const QString& preview, bool crop)
{
    EteraArgs args;

    args["path"]   = path;
    args["offset"] = "0";
    args["limit"]  = "0";

    if (preview.isEmpty() == false)
        args["preview_size"] = preview;
    if (crop == true)
        args["preview_crop"] = "true";

    setProperty("path",    path);
    setProperty("preview", preview);
    setProperty("crop",    crop);

    if (startSimpleRequest("/resources", args) == true)
        connect(m_reply, SIGNAL(finished()), this, SLOT(on_stat_finished()));
}
//----------------------------------------------------------------------------------------------

void EteraAPI::on_stat_finished()
{
    int     code;
    QString body;

    if (parseReply(code, body) == false)
        return;

    if (code != 200) {
        setLastError(code, body);
        return;
    }

    EteraItem item;

    if (item.parse(body) == false) {
        setLastError(1, JSON_PARSE_ERROR);
        return;
    }

    setLastError(0);

    emit onSTAT(this, item);
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::ls(const QString& path, EteraItemList& result, const QString& preview, bool crop)
{
    quint64 offset = 0;

    EteraArgs args;

    args["path"]   = path;
    args["offset"] = QString::number(offset);
    args["limit"]  = QString::number(m_limit);

    if (preview.isEmpty() == false)
        args["preview_size"] = preview;
    if (crop == true)
        args["preview_crop"] = "true";

    while (true) {
        int     code;
        QString body;

        if (makeSimpleRequest(code, body, "/resources", args) == false)
            return false;

        if (code != 200)
            return setLastError(code, body);

        bool ok;
        QtJson::JsonObject json = QtJson::parse(body, ok).toMap();
        if (ok == false)
            return setLastError(1, JSON_PARSE_ERROR);

        if (json.contains("_embedded") == false) {
            EteraItem item;
            if (item.parse(json) == false)
                return setLastError(1, JSON_PARSE_ERROR);

            result.append(item);

            return setLastError(0);
        }

        json = json["_embedded"].toMap();
        QVariantList items = json["items"].toList();

        for (QVariantList::const_iterator i = items.constBegin(); i < items.constEnd(); i++) {
            EteraItem item;
            if (item.parse((*i).toMap()) == false)
                return setLastError(1, JSON_PARSE_ERROR);

            result.append(item);
        }

        if ((quint64)items.count() != m_limit)
            return setLastError(0);

        quint64 limit = json["limit"].toULongLong(&ok);
        if (ok == false)
            return setLastError(1, JSON_PARSE_ERROR);

        offset += limit;
        args["offset"] = QString::number(offset);
    }

    return setLastError(0);
}
//----------------------------------------------------------------------------------------------

void EteraAPI::ls(const QString& path, const QString& preview, bool crop, quint64 offset, quint64 limit)
{
    if (limit == 0)
        limit = 20;

    EteraArgs args;

    args["path"]   = path;
    args["limit"]  = QString::number(limit);
    args["offset"] = QString::number(offset);

    if (preview.isEmpty() == false)
        args["preview_size"] = preview;
    if (crop == true)
        args["preview_crop"] = "true";

    setProperty("path",    path);
    setProperty("preview", preview);
    setProperty("crop",    crop);
    setProperty("offset",  offset);
    setProperty("limit",   limit);

    if (startSimpleRequest("/resources", args) == true)
        connect(m_reply, SIGNAL(finished()), this, SLOT(on_ls_finished()));
}
//----------------------------------------------------------------------------------------------

void EteraAPI::on_ls_finished()
{
    QString path    = property("path").toString();
    QString preview = property("preview").toString();
    bool    crop    = property("crop").toBool();
    quint64 offset  = property("offset").toULongLong();
    quint64 limit   = property("limit").toULongLong();

    int     code;
    QString body;

    if (parseReply(code, body) == false)
        return;

    if (code != 200) {
        setLastError(code, body);
        return;
    }

    bool ok;
    QtJson::JsonObject json = QtJson::parse(body, ok).toMap();
    if (ok == false) {
        setLastError(1, JSON_PARSE_ERROR);
        return;
    }

    EteraItemList list;

    if (json.contains("_embedded") == false) {
        EteraItem item;
        if (item.parse(json) == false) {
            setLastError(1, JSON_PARSE_ERROR);
            return;
        }

        list.append(item);

        setLastError(0);

        emit onLS(this, list, path, preview, crop, offset, limit);

        return;
    }

    json = json["_embedded"].toMap();
    QVariantList items = json["items"].toList();

    for (QVariantList::const_iterator i = items.constBegin(); i < items.constEnd(); i++) {
        EteraItem item;
        if (item.parse((*i).toMap()) == false) {
            setLastError(1, JSON_PARSE_ERROR);
            return;
        }

        list.append(item);
    }

    limit = json["limit"].toULongLong(&ok);
    if (ok == false) {
        setLastError(1, JSON_PARSE_ERROR);
        return;
    }

    setLastError(0);

    emit onLS(this, list, path, preview, crop, offset, limit);
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::mkdir(const QString& path)
{
    EteraArgs args;

    args["path"] = path;

    int     code;
    QString body;

    if (makeSimpleRequest(code, body, "/resources", args, ermPUT) == false)
        return false;

    if (code != 201)
        return setLastError(code, body);

    return setLastError(0);
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::rm(const QString& path, bool permanently)
{
    EteraArgs args;

    args["path"] = path;

    if (permanently == true)
        args["permanently"] = "true";

    int     code;
    QString body;

    if (makeSimpleRequest(code, body, "/resources", args, ermDELETE) == false)
        return false;

    if (code == 204)
        // 204 No content (ресурс успешно удален)
        return setLastError(0);
    else if (code == 202)
        // 202 Accepted (удаление папки начато)
        return wait(body);

    return setLastError(code, body);
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::cp(const QString& source, const QString& target, bool overwrite)
{
    EteraArgs args;

    args["from"] = source;
    args["path"] = target;

    if (overwrite == true)
        args["overwrite"] = "true";

    int     code;
    QString body;

    if (makeSimpleRequest(code, body, "/resources/copy", args, ermPOST) == false)
        return false;

    if (code == 201)
        // 201 Created (ресурс успешно скопирован)
        return setLastError(0);
    else if (code == 202)
        // 202 Accepted (копирование папки начато)
        return wait(body);

    return setLastError(code, body);
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::mv(const QString& source, const QString& target, bool overwrite)
{
    EteraArgs args;

    args["from"] = source;
    args["path"] = target;

    if (overwrite == true)
        args["overwrite"] = "true";

    int     code;
    QString body;

    if (makeSimpleRequest(code, body, "/resources/move", args, ermPOST) == false)
        return false;

    if (code == 201)
        // 201 Created (ресурс успешно перемещен)
        return setLastError(0);
    else if (code == 202) {
        // 202 Accepted (перемещение папки начато)
        return wait(body);
    }

    return setLastError(code, body);
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::put(const QString& source, const QString& target, bool overwrite)
{
    EteraArgs args;

    args["path"] = target;

    if (overwrite == true)
        args["overwrite"] = "true";

    int     code;
    QString body;

    if (makeSimpleRequest(code, body, "/resources/upload", args, ermGET) == false)
        return false;

    if (code != 200)
        return setLastError(code, body);

    QUrl               url;
    EteraRequestMethod method;

    if (parseLink(body, url, method) == false)
        return false;

    if (method != ermPUT)
        return setLastError(1, UNSUPPORTED_LINK_METHOD);

    QFile file(source);

    if (file.open(QIODevice::ReadOnly) == false)
         return setLastError(file.error(), FILE_OPEN_ERROR);

    QNetworkRequest request(url);
    setDefaultHeaders(request, file.size());
    request.setRawHeader("Content-Type", "application/octet-stream");

    if (makeRequest(request, code, body, method, "", &file) == false)
        return false;

    if (code == 201)
        // 201 Created (ресурс успешно создан)
        return setLastError(0);

    return setLastError(code, body);
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::get(const QString& source, const QString& target)
{
    EteraArgs args;

    args["path"] = source;

    int     code;
    QString body;

    if (makeSimpleRequest(code, body, "/resources/download", args, ermGET) == false)
        return false;

    if (code != 200)
        return setLastError(code, body);

    QUrl               url;
    EteraRequestMethod method;

    if (parseLink(body, url, method) == false)
        return false;

    if (method != ermGET)
        return setLastError(1, UNSUPPORTED_LINK_METHOD);

    QFile file(target);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate) == false)
         return setLastError(file.error(), FILE_OPEN_ERROR);

    if (get(url, &file) == false) {
        file.remove();
        return false;
    }

    return true;
}
//----------------------------------------------------------------------------------------------

bool EteraAPI::get(const QUrl& url, QIODevice* target)
{
    QNetworkRequest request(url);
    setDefaultHeaders(request, 0, true, true);

    int     code;
    QString body;

    while (true) {
        if (makeRequest(request, code, body, ermGET, "", target) == false)
            return false;

        if (code == 200)
            return setLastError(0);

        if (code == 301 || code == 302) {
            QUrl _url = QUrl::fromEncoded(body.toUtf8());
            if (checkYandexDomain(_url) == false)
                return setLastError(1, MALFORMED_LINK_URL);

            request.setUrl(_url);

            continue;
        }

        break;
    }

    return setLastError(code, body);
}
//----------------------------------------------------------------------------------------------

void EteraAPI::publish(const QString& path)
{
    EteraArgs args;

    args["path"] = path;

    setProperty("path", path);

    if (startSimpleRequest("/resources/publish", args, ermPUT) == true)
        connect(m_reply, SIGNAL(finished()), this, SLOT(on_publish_finished()));
}
//----------------------------------------------------------------------------------------------

void EteraAPI::on_publish_finished()
{
    QString path = property("path").toString();

    int     code;
    QString body;

    if (parseReply(code, body) == false)
        return;

    if (code != 200) {
        setLastError(code, body);
        return;
    }

    setLastError(0);

    emit onPUBLISH(this, path);
}
//----------------------------------------------------------------------------------------------

void EteraAPI::unpublish(const QString& path)
{
    EteraArgs args;

    args["path"] = path;

    setProperty("path", path);

    if (startSimpleRequest("/resources/unpublish", args, ermPUT) == true)
        connect(m_reply, SIGNAL(finished()), this, SLOT(on_unpublish_finished()));
}
//----------------------------------------------------------------------------------------------

void EteraAPI::on_unpublish_finished()
{
    QString path = property("path").toString();

    int     code;
    QString body;

    if (parseReply(code, body) == false)
        return;

    if (code != 200) {
        setLastError(code, body);
        return;
    }

    setLastError(0);

    emit onUNPUBLISH(this, path);
}
//----------------------------------------------------------------------------------------------
