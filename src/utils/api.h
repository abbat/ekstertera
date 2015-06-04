/*!
 * \file
 * \brief Работа с REST API Яндекс.Диск
 */

#ifndef _ekstertera_api_h_
#define _ekstertera_api_h_

#include "sysheaders.h"

/*!
 * \brief Результат вызова метода EteraAPI::info - данные о Диске пользователя
 */
class EteraInfo
{
    public:

        EteraInfo();

        /*!
         * \brief Парсинг json
         * \param body Данные вызова в формате json
         * \return Флаг успешного парсинга
         */
        bool parse(const QString& body);

        /*!
         * \brief Объем файлов, уже хранящихся на Диске
         * \return Объем в байтах
         */
        quint64 used() const { return m_used; }

        /*!
         * \brief Объем свободного места на Диске
         * \return Объем в байтах
         */
        quint64 free() const { return m_free; }

        /*!
         * \brief Общий объем Диска, доступный пользователю
         * \return Объем в байтах
         */
        quint64 total() const { return m_total; }

        /*!
         * \brief Объем файлов, находящихся в Корзине
         * \return Объем в байтах
         */
        quint64 trash() const { return m_trash; }

        /*!
         * \brief Абсолютный адрес папки applications Диска
         * \return Путь
         */
        QString applications() const { return m_applications; }

        /*!
         * \brief Абсолютный адрес папки downloads Диска
         * \return Путь
         */
        QString downloads() const { return m_downloads; }

    private:

        quint64 m_used;           /*!< \brief Объем файлов, уже хранящихся на Диске, в байтах     */
        quint64 m_free;           /*!< \brief Объем свободного места на Диске в байтах            */
        quint64 m_total;          /*!< \brief Общий объем Диска, доступный пользователю, в байтах */
        quint64 m_trash;          /*!< \brief Объем файлов, находящихся в Корзине, в байтах       */
        QString m_applications;   /*!< \brief Абсолютный адрес папки applications Диска           */
        QString m_downloads;      /*!< \brief Абсолютный адрес папки downloads Диска              */
};

/*!
 * \brief Тип объекта на диске
 */
typedef enum {
    eitUnknown,   /*!< \brief Объект неизвестного типа    */
    eitFile,      /*!< \brief Объект является файлом      */
    eitDir        /*!< \brief Объект является директорией */
} EteraItemType;

/*!
 * \brief Тип медиа для объекта на диске
 */
typedef enum {
    eimtAudio,         /*!< \brief Аудио-файлы                                                                            */
    eimtBackup,        /*!< \brief Файлы резервных и временных копий                                                      */
    eimtBook,          /*!< \brief Электронные книги                                                                      */
    eimtCompressed,    /*!< \brief Сжатые и архивированные файлы                                                          */
    eimtData,          /*!< \brief Файлы с базами данных                                                                  */
    eimtDevelopment,   /*!< \brief Файлы с кодом (C++, Java, XML и т. п.), а также служебные файлы IDE                    */
    eimtDiskimage,     /*!< \brief Образы носителей информации в различных форматах и сопутствующие файлы (например, CUE) */
    eimtDocument,      /*!< \brief Документы офисных форматов (Word, OpenOffice и т. п.)                                  */
    eimtEncoded,       /*!< \brief Зашифрованные файлы                                                                    */
    eimtExecutable,    /*!< \brief Исполняемые файлы                                                                      */
    eimtFlash,         /*!< \brief Файлы с флэш-видео или анимацией                                                       */
    eimtFont,          /*!< \brief Файлы шрифтов                                                                          */
    eimtImage,         /*!< \brief Изображения                                                                            */
    eimtSettings,      /*!< \brief Файлы настроек для различных программ                                                  */
    eimtSpreadsheet,   /*!< \brief Файлы офисных таблиц (Numbers, Lotus)                                                  */
    eimtText,          /*!< \brief Текстовые файлы                                                                        */
    eimtUnknown,       /*!< \brief Неизвестный тип                                                                        */
    eimtVideo,         /*!< \brief Видео-файлы                                                                            */
    eimtWeb            /*!< \brief Различные файлы, используемые браузерами и сайтами (CSS, сертификаты, файлы закладок)  */
} EteraItemMediaType;

/*!
 * \brief Результат вызова EteraAPI::stat или EteraAPI::ls - описатель объекта на диске
 */
class EteraItem
{
    public:

        EteraItem();

        /*!
         * \brief Парсинг json
         * \param body Данные вызова в формате json
         * \return Флаг успешного парсинга
         */
        bool parse(const QString& body);

        /*!
         * \brief Парсинг json
         * \param json Данные вызова в формате json
         * \return Флаг успешного парсинга
         */
        bool parse(const QtJson::JsonObject& json);

        /*!
         * \brief Объект является директорией?
         * \return True если объект директория
         */
        bool isDir() const { return m_type == eitDir; }

        /*!
         * \brief Объект является файлом?
         * \return True если объект файл
         */
        bool isFile() const { return m_type == eitFile; }

        /*!
         * \brief Объект является публичным?
         * \return True если объект публичный
         */
        bool isPublic() const { return m_public_key.isEmpty() == false; }

        /*!
         * \brief Имя объекта
         * \return Имя объекта
         */
        QString name() const { return m_name; }

        /*!
         * \brief Расширение файла
         * \return Расширение файла
         */
        QString extension() const;

        /*!
         * \brief Путь объекта
         * \return Путь объекта
         */
        QString path() const { return m_path; }

        /*!
         * \brief Базовый путь объекта
         * \return Базовый путь объекта
         * Для директории будет имя самой директории со слешем
         * Для файла будет имя директории, содержащей файл со слешем
         */
        QString basePath() const;

        /*!
         * \brief Родительский путь объекта
         * \return Родительский путь объекта
         * Для директории будет имя родительской директории со слешем
         * Для файла будет равно вызову basePath()
         */
        QString parentPath() const;

        /*!
         * \brief Тип объекта
         * \return Тип объекта
         */
        EteraItemType type() const { return m_type; }

        /*!
         * \brief Дата создания объекта
         * \return Дата создания объекта
         */
        QDateTime created() const { return m_created; }

        /*!
         * \brief Дата модификации объекта
         * \return Дата модификации объекта
         */
        QDateTime modified() const { return m_modified; }

        /*!
         * \brief Размер объекта
         * \return Размер объекта
         */
        quint64 size() const { return m_size; }

        /*!
         * \brief MD5 объекта
         * \return MD5 объекта
         */
        QString md5() const { return m_md5; }

        /*!
         * \brief Тип медиа объекта
         * \return Тип медиа объекта
         */
        EteraItemMediaType mediaType() const { return m_media_type; }

        /*!
         * \brief Строковый тип медиа объекта
         * \return Строковый тип медиа объекта
         */
        QString mediaString() const { return m_media_string; }

        /*!
         * \brief Тип MIME объекта
         * \return Тип MIME объекта
         */
        QString mimeType() const { return m_mime_type; }

        /*!
         * \brief Ссылка на превьюшку объекта
         * \return Ссылка на превьюшку объекта
         */
        QString preview() const { return m_preview; }

        /*!
         * \brief Публичный ключ
         * \return Публичный ключ
         */
        QString publicKey() const { return m_public_key; }

        /*!
         * \brief Публичная ссылка
         * \return Публичная ссылка
         */
        QString publicURL() const { return m_public_url; }

    private:

        //
        // общие поля
        //

        QString            m_name;       /*!< \brief Имя              */
        QString            m_path;       /*!< \brief Путь             */
        EteraItemType      m_type;       /*!< \brief Тип              */
        QDateTime          m_created;    /*!< \brief Дата создания    */
        QDateTime          m_modified;   /*!< \brief Дата модификации */

        //
        // поля файлов
        //

        quint64            m_size;           /*!< \brief Размер              */
        QString            m_md5;            /*!< \brief MD5                 */
        QString            m_media_string;   /*!< \brief Тип медиа           */
        EteraItemMediaType m_media_type;     /*!< \brief Тип медиа           */
        QString            m_mime_type;      /*!< \brief MIME                */
        QString            m_preview;        /*!< \brief Ссылка на превьюшку */

        //
        // поля общих ресурсов
        //

        QString m_public_key;   /*!< \brief Публичный ключ   */
        QString m_public_url;   /*!< \brief Публичная ссылка */
};

/*!
 * \brief Список объектов Диска
 */
typedef QList<EteraItem> EteraItemList;

/*!
 * \brief Работа с API Диска
 */
class EteraAPI : public QObject
{
    Q_OBJECT

    public:

        /*!
         * \brief Конструктор
         * \param noprogress Не посылать сигналы прогресса операции
         */
        EteraAPI(QObject* parent = NULL, bool noprogress = false);
        ~EteraAPI();

        /*!
         * \brief Инициализация API
         */
        static void init();

        /*!
         * \brief Освобождение ресурсов API
         */
        static void cleanup();

        /*!
         * \brief Локализация текстов ошибок
         */
        void retranslateUi();

        /*!
         * \brief Получение кода последней ошибки
         * \return Код ошибки
         * 0 - ок
         * 1 - общая ошибка
         * 2xx .. 5xx - код HTTP ошибки
         */
        int lastErrorCode() const { return m_error_code; }

        /*!
         * \brief Получение текста последней ошибки
         * \return Текст ошибки
         */
        QString lastErrorMessage() const { return m_error_message; }

        /*!
         * \brief Флаг возникновения ошибки во время ожидания результата асинхронной операции
         * Позволяет сделать предположение относительно результата самой операции
         * \return true если ошибка произошла во время ожидания
         */
        bool isAsyncError() const { return m_async_error; }

        /*!
         * \brief Человекочитаемый размер
         * \param val Размер в байтах
         * \return Человекочитаемый размер
         */
        static QString humanBytes(quint64 val);

        /*!
         * \brief Удаление концевых нулей
         * \param val Строковое представление дробного числа
         * \return Человекочитаемый размер без концевых нулей
         */
        static QString humanZeros(const QString& val);

        /*!
         * \brief Получение OAuth токена
         * \param auth_code Код для получения токена или пустое значения для открытия браузера и запроса кода
         * \return OAuth токен
         */
        QString getToken(const QString& auth_code = "");

        /*!
         * \brief Установка OAuth токена
         * \param token OAuth токен
         */
        void setToken(const QString& token) { m_token = token; }

        /*!
         * \brief Получение информации о диске
         * Сигнал onINFO
         */
        void info();

        /*!
         * \brief Получение информации об объекте на диске
         * \param path Путь к объекту
         * \param result Описатель объекта
         * \param preview Размер превью (см. https://tech.yandex.ru/disk/api/reference/meta-docpage/)
         * \param crop Параметр для обрезания превью (см. https://tech.yandex.ru/disk/api/reference/meta-docpage/)
         * \return Флаг успеха
         */
        bool stat(const QString& path, EteraItem& result, const QString& preview = "", bool crop = false);

        /*!
         * \brief Получение списка объектов на диске
         * DEPRECATED
         * \param path Путь к объекту или родительской директории
         * \param result Список описателей объектов
         * \param preview Размер превью (см. https://tech.yandex.ru/disk/api/reference/meta-docpage/)
         * \param crop Параметр для обрезания превью (см. https://tech.yandex.ru/disk/api/reference/meta-docpage/)
         * \return Флаг успеха
         */
        bool ls(const QString& path, EteraItemList& result, const QString& preview = "", bool crop = false);

        /*!
         * \brief Получение списка объектов на диске
         * Сигнал onLS
         * \param path Путь к объекту или родительской директории
         * \param preview Размер превью
         * \param crop Параметр для обрезания превью
         * \param offset Смещение
         * \param limit Количество (для значения 0 используется параметр по умолчанию)
         */
        void ls(const QString& path, const QString& preview = "", bool crop = false, quint64 offset = 0, quint64 limit = 0);

        /*!
         * \brief Создание директории на диске
         * \param path Путь к директории
         * \return Флаг успеха
         */
        bool mkdir(const QString& path);

        /*!
         * \brief Удаление объекта на диске
         * \param path Путь к объекту
         * \param permanently Флаг полного удаления (false - удаление в корзину)
         * \return Флаг успеха
         */
        bool rm(const QString& path, bool permanently);

        /*!
         * \brief Копирование объекта на диске
         * \param source Исходный путь
         * \param target Путь назначения
         * \param overwrite Флаг перезаписи (false - перезапись запрещена)
         * \return Флаг успеха
         */
        bool cp(const QString& source, const QString& target, bool overwrite);

        /*!
         * \brief Перемещение объекта
         * \param source Исходный путь
         * \param target Путь назначения
         * \param overwrite Флаг перезаписи (false - перезапись запрещена)
         * \return Флаг успеха
         */
        bool mv(const QString& source, const QString& target, bool overwrite);

        /*!
         * \brief Загрузка локального файла на диск
         * \param source Имя локального файла
         * \param target Путь назначения
         * \param overwrite Признак перезаписи (false - перезапись запрещена)
         * \return Флаг успеха
         */
        bool put(const QString& source, const QString& target, bool overwrite);

        /*!
         * \brief Получение файла с диска
         * \param source Имя файла на диске
         * \param target Имя локального файла
         * \return Флаг успеха
         */
        bool get(const QString& source, const QString& target);

        /*!
         * \brief Получение файла с диска
         * \param url Ссылка
         * \param target Буфер для записи
         * \return Флаг успеха
         */
        bool get(const QUrl& url, QIODevice* target);

        /*!
         * \brief Открыть доступ к объекту
         * \param path Имя файла или директории на диске
         * \return Флаг успеха
         */
        bool publish(const QString& path);

        /*!
         * \brief Закрыть доступ к объекту
         * \param path Имя файла или директории на диске
         * \return Флаг успеха
         */
        bool unpublish(const QString& path);

    private:

        QString m_user_agent;   /*!< \brief User-Agent в заголовках                              */
        QString m_base_url;     /*!< \brief Базовый URL для вызовов API                          */
        QString m_app_id;       /*!< \brief ID приложения                                        */
        QString m_app_secret;   /*!< \brief Секрет приложения                                    */
        QString m_token;        /*!< \brief OAuth токен                                          */
        quint64 m_limit;        /*!< \brief Количество элементов получаемых за раз в операции ls */

        int     m_error_code;      /*!< \brief Код последней ошибки    */
        QString m_error_message;   /*!< \brief Текст последней ошибки  */
        bool    m_async_error;     /*!< \brief Флаг асинхронной ошибки */

        bool m_noprogress;   /*!< \brief Флаг запрета посылать сигналы прогресса операции */

        QNetworkAccessManager m_http;   /*!< \brief Транспорт HTTPS */

        QNetworkReply* m_reply;   /*!< \brief Переменная для временного хранения текущего ответа */
        QIODevice*     m_io;      /*!< \brief Переменная для временного хранения текущей цели    */

        /*!
         * \brief Сообщение OK
         */
        QString OK_MESSAGE;

        /*!
         * \brief Сообщение об ошибке разбора JSON ответа или преобразования значений
         */
        QString JSON_PARSE_ERROR;

        /*!
         * \brief Сообщение о некорректном URL в объекте Link
         */
        QString MALFORMED_LINK_URL;

        /*!
         * \brief Сообщение о неподдерживаемом методе в объекте Link
         */
        QString UNSUPPORTED_LINK_METHOD;

        /*!
         * \brief Ошибка открытия файла
         */
        QString FILE_OPEN_ERROR;

        /*!
         * \brief Установка последней ошибки
         * \param code Код ошибки
         * \param message Текст ошибки
         * \return true для code = 0 иначе false
         */
        bool setLastError(int code, const QString& message = "");

        /*!
         * \brief Установка стандартных заголовков для запроса
         * \param request Объект запроса
         * \param length Длина тела для POST/PUT запроса
         * \param auth Устанавливать заголовок Authorization
         * \param any Устанавливать заголовок Accept в * иначе application/json
         */
        void setDefaultHeaders(QNetworkRequest& request, quint64 length = 0, bool auth = true, bool any = false);

        /*!
         * \brief Список строковых key/value для параметров запроса
         */
        typedef QMap<QString, QString> EteraArgs;

        /*!
         * \brief Подготовка запроса, установка стандартных заголовков
         * \param request Объект запроса
         * \param relurl Относительный URL запроса
         * \param args Аргументы запроса
         * \param length Длина тела для POST/PUT запроса
         */
        void prepareRequest(QNetworkRequest& request, const QString& relurl, const EteraArgs& args = EteraArgs(), quint64 length = 0);

        /*!
         * \brief Тип запроса к api
         */
        typedef enum {
            ermGET,     /*!< \brief GET    */
            ermPOST,    /*!< \brief POST   */
            ermPUT,     /*!< \brief PUT    */
            ermDELETE   /*!< \brief DELETE */
        } EteraRequestMethod;

        /*!
         * \brief Выполнение запроса
         * DEPRECATED
         * \param request Запрос
         * \param code HTTP-код ответа
         * \param body Тело ответа
         * \param method Метод
         * \param data Тело для POST/PUT запроса (исключает io)
         * \param io Тело для POST/PUT запроса (исключает data)
         * \return false при возникновении сетевой ошибки, иначе true
         */
        bool makeRequest(const QNetworkRequest& request, int& code, QString& body, EteraRequestMethod method = ermGET, const QString& data = "", QIODevice* io = NULL);

        /*!
         * \brief Старт выполнения запроса
         * \param request Запрос
         * \param method Метод
         * \param data Тело для POST/PUT запроса (исключает io)
         * \param io Тело для POST/PUT запроса (исключает data)
         * \return false при возникновении ошибки, иначе true
         */
        bool startRequest(const QNetworkRequest& request, EteraRequestMethod method = ermGET, const QString& data = "", QIODevice* io = NULL);

        /*!
         * \brief Разбор результата выполнения запроса
         * \param code HTTP-код ответа
         * \param body Тело ответа
         * \return false при возникновении сетевой ошибки, иначе true
         */
        bool parseReply(int& code, QString& body);

        /*!
         * \brief Выполнение простого запроса
         * DEPRECATED
         * \param code Код ответа
         * \param body Тело ответа
         * \param url Относительный URL
         * \param args Аргументы запроса
         * \return Флаг успеха
         */
        bool makeSimpleRequest(int& code, QString& body, const QString& url, const EteraArgs& args = EteraArgs(), EteraRequestMethod method = ermGET);

        /*!
         * \brief Старт выполнения простого запроса
         * \param url Относительный URL
         * \param args Аргументы запроса
         * \return Флаг успеха
         */
        bool startSimpleRequest(const QString& url, const EteraArgs& args = EteraArgs(), EteraRequestMethod method = ermGET);

        /*!
         * \brief Проверка, что URL принадлежит домену yandex
         * \param url Проверяемый URL
         * \return Результат проверки
         */
        bool checkYandexDomain(const QUrl& url);

        /*!
         * \brief Парсинг объекта Link
         * \param link Тело объекта Link
         * \param url Полученный URL
         * \param method Полученный метод
         * \return Флаг успеха
         */
        bool parseLink(const QString& link, QUrl& url, EteraRequestMethod& method);

        /*!
         * \brief Ожидание асинхронной операции
         * \param link Тело объекта Link
         * \return Флаг успеха
         */
        bool wait(const QString& link);

    private slots:

        //
        // события HTTPS обработчика
        //

        void on_download_progress(qint64 done, qint64 total);   /*!< \brief Прогресс чтения данных     */
        void on_upload_progress(qint64 done, qint64 total);     /*!< \brief Прогресс отправки данных   */
        void on_ssl_errors(const QList<QSslError> &errors);     /*!< \brief Ошибка SSL                 */
        void on_ready_read();                                   /*!< \brief Готовность к приему данных */

        /*!
         * \brief Обработка сигнала выхода из приложения
         */
        void on_about_to_quit();

        void on_info_finished();   /*!< \brief Завершение вызова info() */
        void on_ls_finished();     /*!< \brief Завершение вызова ls()   */

    signals:

        /*!
         * \brief Сигнал прогресса операции
         * \param done Выполнено
         * \param total Всего
         */
        void onProgress(qint64 done, qint64 total);

        /*!
         * \brief Сигнал ошибки ssl
         * DEPRECATED
         * \param error Ошибки
         */
        void onError(const QString& error);

        /*!
         * \brief Сигнал ошибки
         * \param api API
         * \param code Код ошибки
         * \param error Текст ошибки
         */
        void onError(EteraAPI* api);

        /*!
         * \brief Сигнал получения информации о диске
         * \param api API
         * \param info Результат
         */
        void onINFO(EteraAPI* api, const EteraInfo& info);

        /*!
         * \brief Сигнал получения списка файлов и директорий
         * \param api API
         * \param list Список описателей объектов
         * \param path Путь
         * \param preview Размер превью
         * \param crop Параметр для обрезания превью
         * \param offset Смещение
         * \param limit Количество
         * При limit < list.count() дальнейшие запросы можно прекращать
         */
        void onLS(EteraAPI* api, const EteraItemList& list, const QString& path, const QString& preview, bool crop, quint64 offset, quint64 limit);
};

#endif   // _ekstertera_api_h_
