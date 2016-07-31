/*!
 * \file
 * \brief Файл для включения всех системных и библиотечных заголовочных файлов
 */

#ifndef _ekstertera_sysheaders_h_
#define _ekstertera_sysheaders_h_

//
// стандартные заголовочные файлы Qt
//

#include <QSet>
#include <QDir>
#include <QMenu>
#include <QTimer>
#include <QLabel>
#include <QDebug>
#include <QQueue>
#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QBuffer>
#include <QThread>
#include <QPainter>
#include <QMenuBar>
#include <QToolBar>
#include <QDateTime>
#include <QMultiMap>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QSettings>
#include <QMimeData>
#include <QFileInfo>
#include <QTreeView>
#include <QStatusBar>
#include <QScrollBar>
#include <QSslCipher>
#include <QTextCodec>
#include <QFormLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QListWidget>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QMainWindow>
#include <QCloseEvent>
#include <QTranslator>
#include <QStringList>
#include <QWheelEvent>
#include <QToolButton>
#include <QFileDialog>
#include <QInputDialog>
#include <QProgressBar>
#include <QApplication>
#include <QLibraryInfo>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QListWidgetItem>
#include <QSystemTrayIcon>
#include <QDesktopServices>
#include <QSslConfiguration>
#include <QStyledItemDelegate>
#include <QProcessEnvironment>
#include <QNetworkAccessManager>

#if QT_VERSION >= 0x050000
    // в Qt 5.x часть методов QUrl типа addQueryItem признаны obsolete
    #include <QUrlQuery>

    // Qt 5.x specific
    #include <QMimeType>
    #include <QMimeDatabase>
#endif

//
// windows
//

#ifdef Q_WS_WIN
    // стандартные определения для Windows 7
    #ifndef _WIN32_WINNT_WIN7
        #define _WIN32_WINNT_WIN7 0x0601
    #endif
    #ifndef WINVER
        #define WINVER _WIN32_WINNT_WIN7
    #endif
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT WINVER
    #endif
    #ifndef NTDDI_WIN7
        #define NTDDI_WIN7 0x06010000
    #endif
    #ifndef NTDDI_VERSION
        #define NTDDI_VERSION NTDDI_WIN7
    #endif
    #ifndef _WIN32_IE_IE80
        #define _WIN32_IE_IE80 0x0800
    #endif
    #ifndef _WIN32_IE
        #define _WIN32_IE _WIN32_IE_IE80
    #endif

    #include <windows.h>
    #include <shellapi.h>
    #include <commctrl.h>
    #include <commoncontrols.h>
#endif

//
// сторонние библиотеки
//

// https://github.com/gaudecker/qt-json
#include "3dparty/json/json.h"

// бэкпорты из Qt 5.x
#if QT_VERSION < 0x050000
    // https://qt.gitorious.org/qtplayground/mimetypes
    #include "3dparty/qt5/mimetypes/qmimetype.h"
    #include "3dparty/qt5/mimetypes/qmimedatabase.h"
#endif

//
// общие определения для проекта
//

// В Qt 5.x более нет макроса Q_WS_X11, который использовался для определения *nix подобных систем, определяем свои
#ifdef Q_WS_WIN
    #define ETERA_WS_WIN
#endif

#ifdef Q_WS_MAC
    #define ETERA_WS_MAC
#endif

#if defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
    #define ETERA_WS_X11
#endif

#if defined(ETERA_WS_X11) || defined(ETERA_WS_WIN)
    #define ETERA_WS_X11_OR_WIN
#endif

// в Qt 5.x сломана иконка трея, необходима собственная реализация
// так же необходима реализация отдельного поведения для Ubuntu Unity
#ifndef ETERA_CUSTOM_TRAY_ICON
    #if defined(ETERA_WS_X11) && (defined(ETERA_CUSTOM_TRAY_ICON_GTK) || defined(ETERA_CUSTOM_TRAY_ICON_UNITY))
        #define ETERA_CUSTOM_TRAY_ICON
    #endif
#else
    #error "Use ETERA_CUSTOM_TRAY_ICON_GTK or ETERA_CUSTOM_TRAY_ICON_UNITY directive instead ETERA_CUSTOM_TRAY_ICON"
#endif

// конкатенация строк для макроса версии
#define ETERA_STR_EXPAND(token) #token
#define ETERA_STR(token)        ETERA_STR_EXPAND(token)

// версия (0.1.11)
#define ETERA_APP_NAME      "ekstertera"
#define ETERA_VERSION_MAJOR 0
#define ETERA_VERSION_MINOR 1
#define ETERA_VERSION_PATCH 11
#define ETERA_VERSION_NUM   ((ETERA_VERSION_MAJOR << 16) | (ETERA_VERSION_MINOR << 8) | (ETERA_VERSION_PATCH))
#define ETERA_VERSION       ETERA_STR(ETERA_VERSION_MAJOR) "." ETERA_STR(ETERA_VERSION_MINOR) "." ETERA_STR(ETERA_VERSION_PATCH)

// константы api
#define ETERA_API_USER_AGENT ETERA_APP_NAME "/" ETERA_VERSION
#define ETERA_API_BASE_URL   "https://cloud-api.yandex.net/v1/disk"
#define ETERA_API_APP_ID     "51cba73d70c343fd96f0765e1eeb0435"
#define ETERA_API_APP_SECRET "73cf4dacd4f74e7a97b77d036f90eb91"

#endif   // _ekstertera_sysheaders_h_
