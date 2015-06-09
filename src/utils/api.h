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
        EteraAPI(QObject* parent = NULL);
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
         * \brief Генерация следующего id вызова api
         * \return Новый ID вызова api
         */
        static quint64 nextID();

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
         * \brief Остановка активного запроса
         */
        void abort() { if (m_reply != NULL) m_reply->abort(); }

        /*!
         * \brief Получение OAuth токена
         * Сигнал onTOKEN
         * \param auth_code Код для получения токена или пустое значения для открытия браузера и запроса кода
         */
        void getToken(const QString& auth_code = "");

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
         * Сигнал OnSTAT
         * \param path Путь к объекту
         * \param preview Размер превью
         * \param crop Параметр для обрезания превью
         */
        void stat(const QString& path, const QString& preview = "", bool crop = false);

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
         * Сигнал onMKDIR
         * \param path Путь к директории
         */
        void mkdir(const QString& path);

        /*!
         * \brief Удаление объекта на диске
         * Сигнал onRM
         * \param path Путь к объекту
         * \param permanently Флаг полного удаления (false - удаление в корзину)
         */
        void rm(const QString& path, bool permanently);

        /*!
         * \brief Копирование объекта на диске
         * Сигнал onCP
         * \param source Исходный путь
         * \param target Путь назначения
         * \param overwrite Флаг перезаписи (false - перезапись запрещена)
         */
        void cp(const QString& source, const QString& target, bool overwrite);

        /*!
         * \brief Перемещение объекта
         * Сигнал onMV
         * \param source Исходный путь
         * \param target Путь назначения
         * \param overwrite Флаг перезаписи (false - перезапись запрещена)
         */
        void mv(const QString& source, const QString& target, bool overwrite);

        /*!
         * \brief Загрузка локального файла на диск
         * Сигнал onPUT
         * \param source Имя локального файла
         * \param target Путь назначения
         * \param overwrite Признак перезаписи (false - перезапись запрещена)
         */
        void put(const QString& source, const QString& target, bool overwrite);

        /*!
         * \brief Загрузка данных по url на диск
         * Сигнал onPUT
         * \param url Ссылка
         * \param device Устройство чтения
         */
        void put(const QString& url, QIODevice* device);

        /*!
         * \brief Получение файла с диска
         * Сигнал onGET
         * \param source Имя файла на диске
         * \param target Имя локального файла
         */
        void get(const QString& source, const QString& target);

        /*!
         * \brief Загрузка url с сервиса
         * Сигнал onGET
         * \param url Ссылка
         * \param device Устройство записи
         * При device == NULL будет создан QBuffer
         */
        void get(const QString& url, QIODevice* device = NULL);

        /*!
         * \brief Открыть доступ к объекту
         * Сигнал onPUBLISH
         * \param path Имя файла или директории на диске
         */
        void publish(const QString& path);

        /*!
         * \brief Закрыть доступ к объекту
         * Сигнал onUNPUBLISH
         * \param path Имя файла или директории на диске
         */
        void unpublish(const QString& path);

        //
        // аргументы вызовов api
        //

        quint64    id()          const { return m_id;          }
        quint64    offset()      const { return m_offset;      }
        quint64    limit()       const { return m_limit;       }
        QString    path()        const { return m_path;        }
        QString    preview()     const { return m_preview;     }
        QString    source()      const { return m_source;      }
        QString    target()      const { return m_target;      }
        QString    url()         const { return m_url;         }
        bool       crop()        const { return m_crop;        }
        bool       permanently() const { return m_permanently; }
        bool       overwrite()   const { return m_overwrite;   }
        QIODevice* device()      const { return m_device;      }

        void setSource(const QString& source) { m_source    = source;    }
        void setTarget(const QString& target) { m_target    = target;    }
        void setOverwrite(bool overwrite)     { m_overwrite = overwrite; }

        //
        // мета-свойства
        //

        quint64       parentId() const { return m_parent_id; }
        EteraItemType ensure()   const { return m_ensure;    }

        void setParentId(quint64 parent_id)  { m_parent_id = parent_id; }
        void setEnsure(EteraItemType ensure) { m_ensure    = ensure;    }

    private:

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

    private:

        /*!
         * \brief OAuth токен
         */
        QString m_token;

        /*!
         * \brief Код последней ошибки
         */
        int m_error_code;

        /*!
         * \brief Текст последней ошибки
         */
        QString m_error_message;

        /*!
         * \brief Транспорт HTTPS
         */
        QNetworkAccessManager m_http;

        QNetworkReply* m_reply;    /*!< \brief Переменная для временного хранения текущего ответа */
        QIODevice*     m_io;       /*!< \brief Переменная для временного хранения текущей цели    */

        //
        // аргументы вызовов api
        //

        quint64    m_id;
        quint64    m_offset;
        quint64    m_limit;
        QString    m_path;
        QString    m_preview;
        QString    m_source;
        QString    m_target;
        QString    m_url;
        bool       m_crop;
        bool       m_permanently;
        bool       m_overwrite;
        QIODevice* m_device;

        //
        // мета-свойства для различных "костылей"
        //

        quint64       m_parent_id;
        EteraItemType m_ensure;

        //
        // аргументы внутренних вызовов
        //

        QString m_link;

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
        bool checkYandexDomain(const QString& url);

        /*!
         * \brief Парсинг объекта Link
         * \param link Тело объекта Link
         * \param url Полученный URL
         * \param method Полученный метод
         * \return Флаг успеха
         */
        bool parseLink(const QString& link, QString& url, EteraRequestMethod& method);

        /*!
         * \brief Старт запроса на ожидание асинхронной операции
         * \param link Тело объекта Link
         * \return Флаг успеха
         */
        bool startWait(const QString& link);

        /*!
         * \brief Проверка результата запроса на ожидание асинхронной операции
         * \param wait Флаг необходимости повторного запроса startWait
         * \return true в случае, если операция завершена, false в случае ошибки
         */
        bool parseWait(bool& wait);

    private slots:

        //
        // события HTTPS обработчика
        //

        void on_progress(qint64 done, qint64 total);          /*!< \brief Прогресс обработки данных  */
        void on_ssl_errors(const QList<QSslError> &errors);   /*!< \brief Ошибка SSL                 */
        void on_ready_read();                                 /*!< \brief Готовность к приему данных */

        //
        // события завершения HTTPS запроса
        //

        void on_token_finished();       /*!< \brief Завершение вызова getToken()       */
        void on_info_finished();        /*!< \brief Завершение вызова info()           */
        void on_stat_finished();        /*!< \brief Завершение вызова stat()           */
        void on_ls_finished();          /*!< \brief Завершение вызова ls()             */
        void on_mkdir_finished();       /*!< \brief Завершение вызова mkdir()          */
        void on_rm_finished();          /*!< \brief Завершение вызова rm()             */
        void on_rm_wait_finished();     /*!< \brief Завершение ожидания вызова rm()    */
        void on_cp_finished();          /*!< \brief Завершение вызова cp()             */
        void on_cp_wait_finished();     /*!< \brief Завершение ожидания вызова cp()    */
        void on_mv_finished();          /*!< \brief Завершение вызова mv()             */
        void on_mv_wait_finished();     /*!< \brief Завершение ожидания вызова mv()    */
        void on_put_file_finished();    /*!< \brief Завершение вызова put() для файла  */
        void on_put_url_finished();     /*!< \brief Завершение вызова put() для ссылки */
        void on_get_file_finished();    /*!< \brief Завершение вызова get() для файла  */
        void on_get_url_finished();     /*!< \brief Завершение вызова get() для ссылки */
        void on_publish_finished();     /*!< \brief Завершение вызова publish()        */
        void on_unpublish_finished();   /*!< \brief Завершение вызова unpublish()      */

    signals:

        /*!
         * \brief Сигнал ошибки
         * \param api API
         */
        void onError(EteraAPI* api);

        /*!
         * \brief Сигнал прогресса операции
         * \param api API
         * \param done Выполнено
         * \param total Всего
         */
        void onProgress(EteraAPI* api, qint64 done, qint64 total);

        /*!
         * \brief Сигнал получения информации о диске
         * \param api API
         * \param info Результат
         */
        void onTOKEN(EteraAPI* api, const QString& token);

        /*!
         * \brief Сигнал получения информации о диске
         * \param api API
         * \param info Результат
         */
        void onINFO(EteraAPI* api, const EteraInfo& info);

        /*!
         * \brief Сигнал получения информации об объекте
         * \param api API
         * \param item Результат
         */
        void onSTAT(EteraAPI* api, const EteraItem& item);

        /*!
         * \brief Сигнал получения списка файлов и директорий
         * \param api API
         * \param list Список описателей объектов
         * \param limit Количество
         * При limit < list.count() дальнейшие запросы можно прекращать
         */
        void onLS(EteraAPI* api, const EteraItemList& list, quint64 limit);

        /*!
         * \brief Сигнал создания директории
         * \param api API
         */
        void onMKDIR(EteraAPI* api);

        /*!
         * \brief Сигнал удаления объекта
         * \param api API
         */
        void onRM(EteraAPI* api);

        /*!
         * \brief Сигнал копирования объекта
         * \param api API
         */
        void onCP(EteraAPI* api);

        /*!
         * \brief Сигнал перемещения объекта
         * \param api API
         */
        void onMV(EteraAPI* api);

        /*!
         * \brief Сигнал загрузки url на сервис
         * \param api API
         */
        void onPUT(EteraAPI* api);

        /*!
         * \brief Сигнал загрузки url с сервиса
         * \param api API
         */
        void onGET(EteraAPI* api);

        /*!
         * \brief Сигнал открытия доступа к объекту
         * \param api API
         */
        void onPUBLISH(EteraAPI* api);

        /*!
         * \brief Сигнал закрытия доступа к объекту
         * \param api API
         */
        void onUNPUBLISH(EteraAPI* api);
};

//
// вспомогательные хэлперы работы с сигналами и слотами
//

#define ETERA_API_CONNECT_SIMPLE(api, signal, slot) connect(api, SIGNAL(signal(EteraAPI*)), SLOT(slot(EteraAPI*)))
#define ETERA_API_DISCONNECT_SIMPLE(api, signal, slot) disconnect(api, SIGNAL(signal(EteraAPI*)), this, SLOT(slot(EteraAPI*)))

#define ETERA_API_CONNECT_ERROR(api, slot)        ETERA_API_CONNECT_SIMPLE(api, onError,     slot)
#define ETERA_API_CONNECT_MKDIR(api, slot)        ETERA_API_CONNECT_SIMPLE(api, onMKDIR,     slot)
#define ETERA_API_CONNECT_RM(api, slot)           ETERA_API_CONNECT_SIMPLE(api, onRM,        slot)
#define ETERA_API_CONNECT_CP(api, slot)           ETERA_API_CONNECT_SIMPLE(api, onCP,        slot)
#define ETERA_API_CONNECT_MV(api, slot)           ETERA_API_CONNECT_SIMPLE(api, onMV,        slot)
#define ETERA_API_CONNECT_PUT(api, slot)          ETERA_API_CONNECT_SIMPLE(api, onPUT,       slot)
#define ETERA_API_CONNECT_GET(api, slot)          ETERA_API_CONNECT_SIMPLE(api, onGET,       slot)
#define ETERA_API_CONNECT_PUBLISH(api, slot)      ETERA_API_CONNECT_SIMPLE(api, onPUBLISH,   slot)
#define ETERA_API_CONNECT_UNPUBLISH(api, slot)    ETERA_API_CONNECT_SIMPLE(api, onUNPUBLISH, slot)
#define ETERA_API_CONNECT_PROGRESS(api, slot)     connect(api, SIGNAL(onProgress(EteraAPI*, qint64, qint64)), SLOT(slot(EteraAPI*, qint64, qint64)))
#define ETERA_API_CONNECT_TOKEN(api, slot)        connect(api, SIGNAL(onTOKEN(EteraAPI*, const QString&)), SLOT(slot(EteraAPI*, const QString&)))
#define ETERA_API_CONNECT_INFO(api, slot)         connect(api, SIGNAL(onINFO(EteraAPI*, const EteraInfo&)), SLOT(slot(EteraAPI*, const EteraInfo&)))
#define ETERA_API_CONNECT_STAT(api, slot)         connect(api, SIGNAL(onSTAT(EteraAPI*, const EteraItem&)), SLOT(slot(EteraAPI*, const EteraItem&)))
#define ETERA_API_CONNECT_LS(api, slot)           connect(api, SIGNAL(onLS(EteraAPI*, const EteraItemList&, quint64)), SLOT(slot(EteraAPI*, const EteraItemList&, quint64)))

#define ETERA_API_DISCONNECT_ERROR(api, slot)     ETERA_API_DISCONNECT_SIMPLE(api, onError,     slot)
#define ETERA_API_DISCONNECT_MKDIR(api, slot)     ETERA_API_DISCONNECT_SIMPLE(api, onMKDIR,     slot)
#define ETERA_API_DISCONNECT_RM(api, slot)        ETERA_API_DISCONNECT_SIMPLE(api, onRM,        slot)
#define ETERA_API_DISCONNECT_CP(api, slot)        ETERA_API_DISCONNECT_SIMPLE(api, onCP,        slot)
#define ETERA_API_DISCONNECT_MV(api, slot)        ETERA_API_DISCONNECT_SIMPLE(api, onMV,        slot)
#define ETERA_API_DISCONNECT_PUT(api, slot)       ETERA_API_DISCONNECT_SIMPLE(api, onPUT,       slot)
#define ETERA_API_DISCONNECT_GET(api, slot)       ETERA_API_DISCONNECT_SIMPLE(api, onGET,       slot)
#define ETERA_API_DISCONNECT_PUBLISH(api, slot)   ETERA_API_DISCONNECT_SIMPLE(api, onPUBLISH,   slot)
#define ETERA_API_DISCONNECT_UNPUBLISH(api, slot) ETERA_API_DISCONNECT_SIMPLE(api, onUNPUBLISH, slot)
#define ETERA_API_DISCONNECT_PROGRESS(api, slot)  disconnect(api, SIGNAL(onProgress(EteraAPI*, qint64, qint64)), SLOT(slot(EteraAPI*, qint64, qint64)))
#define ETERA_API_DISCONNECT_TOKEN(api, slot)     disconnect(api, SIGNAL(onTOKEN(EteraAPI*, const QString&)), SLOT(slot(EteraAPI*, const QString&)))
#define ETERA_API_DISCONNECT_INFO(api, slot)      disconnect(api, SIGNAL(onINFO(EteraAPI*, const EteraInfo&)), SLOT(slot(EteraAPI*, const EteraInfo&)))
#define ETERA_API_DISCONNECT_STAT(api, slot)      disconnect(api, SIGNAL(onSTAT(EteraAPI*, const EteraItem&)), SLOT(slot(EteraAPI*, const EteraItem&)))
#define ETERA_API_DISCONNECT_LS(api, slot)        disconnect(api, SIGNAL(onLS(EteraAPI*, const EteraItemList&, quint64)), SLOT(slot(EteraAPI*, const EteraItemList&, quint64)))

#define ETERA_API_TASK_SIMPLE(api, method, success, error) \
    ETERA_API_CONNECT_ERROR(api, error); \
    ETERA_API_CONNECT_##method(api, success)

#define ETERA_API_TASK_PROGRESS(api, method, success, error, progress) \
    ETERA_API_TASK_SIMPLE(api, method, success, error); \
    ETERA_API_CONNECT_PROGRESS(api, progress)

#define ETERA_API_TASK_MKDIR(api, success, error)         ETERA_API_TASK_SIMPLE(api,   MKDIR,     success, error)
#define ETERA_API_TASK_RM(api, success, error)            ETERA_API_TASK_SIMPLE(api,   RM,        success, error)
#define ETERA_API_TASK_CP(api, success, error)            ETERA_API_TASK_SIMPLE(api,   CP,        success, error)
#define ETERA_API_TASK_MV(api, success, error)            ETERA_API_TASK_SIMPLE(api,   MV,        success, error)
#define ETERA_API_TASK_PUBLISH(api, success, error)       ETERA_API_TASK_SIMPLE(api,   PUBLISH,   success, error)
#define ETERA_API_TASK_UNPUBLISH(api, success, error)     ETERA_API_TASK_SIMPLE(api,   UNPUBLISH, success, error)
#define ETERA_API_TASK_TOKEN(api, success, error)         ETERA_API_TASK_SIMPLE(api,   TOKEN,     success, error)
#define ETERA_API_TASK_INFO(api, success, error)          ETERA_API_TASK_SIMPLE(api,   INFO,      success, error)
#define ETERA_API_TASK_STAT(api, success, error)          ETERA_API_TASK_SIMPLE(api,   STAT,      success, error)
#define ETERA_API_TASK_LS(api, success, error)            ETERA_API_TASK_SIMPLE(api,   LS,        success, error)
#define ETERA_API_TASK_PUT(api, success, error, progress) ETERA_API_TASK_PROGRESS(api, PUT,       success, error, progress)
#define ETERA_API_TASK_GET(api, success, error, progress) ETERA_API_TASK_PROGRESS(api, GET,       success, error, progress)

#define ETERA_API_CONTINUE_TASK_SIMPLE(api, method, success, error, previous) \
    ETERA_API_DISCONNECT_ERROR(api, previous); \
    ETERA_API_TASK_SIMPLE(api, method, success, error)

#define ETERA_API_CONTINUE_TASK_PROGRESS(api, method, success, error, progress, previous) \
    ETERA_API_DISCONNECT_ERROR(api, previous); \
    ETERA_API_TASK_PROGRESS(api, method, success, error, progress)

#define ETERA_API_CONTINUE_TASK_MKDIR(api, success, error, previous)         ETERA_API_CONTINUE_TASK_SIMPLE(api,   MKDIR,     success, error, previous)
#define ETERA_API_CONTINUE_TASK_RM(api, success, error, previous)            ETERA_API_CONTINUE_TASK_SIMPLE(api,   RM,        success, error, previous)
#define ETERA_API_CONTINUE_TASK_CP(api, success, error, previous)            ETERA_API_CONTINUE_TASK_SIMPLE(api,   CP,        success, error, previous)
#define ETERA_API_CONTINUE_TASK_MV(api, success, error, previous)            ETERA_API_CONTINUE_TASK_SIMPLE(api,   MV,        success, error, previous)
#define ETERA_API_CONTINUE_TASK_PUBLISH(api, success, error, previous)       ETERA_API_CONTINUE_TASK_SIMPLE(api,   PUBLISH,   success, error, previous)
#define ETERA_API_CONTINUE_TASK_UNPUBLISH(api, success, error, previous)     ETERA_API_CONTINUE_TASK_SIMPLE(api,   UNPUBLISH, success, error, previous)
#define ETERA_API_CONTINUE_TASK_TOKEN(api, success, error, previous)         ETERA_API_CONTINUE_TASK_SIMPLE(api,   TOKEN,     success, error, previous)
#define ETERA_API_CONTINUE_TASK_INFO(api, success, error, previous)          ETERA_API_CONTINUE_TASK_SIMPLE(api,   INFO,      success, error, previous)
#define ETERA_API_CONTINUE_TASK_STAT(api, success, error, previous)          ETERA_API_CONTINUE_TASK_SIMPLE(api,   STAT,      success, error, previous)
#define ETERA_API_CONTINUE_TASK_LS(api, success, error, previous)            ETERA_API_CONTINUE_TASK_SIMPLE(api,   LS,        success, error, previous)
#define ETERA_API_CONTINUE_TASK_PUT(api, success, error, progress, previous) ETERA_API_CONTINUE_TASK_PROGRESS(api, PUT,       success, error, progress, previous)
#define ETERA_API_CONTINUE_TASK_GET(api, success, error, progress, previous) ETERA_API_CONTINUE_TASK_PROGRESS(api, GET,       success, error, progress, previous)

#endif   // _ekstertera_api_h_
