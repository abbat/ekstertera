/*!
 * \file
 * \brief Файл для включения всех системных и библиотечных заголовочных файлов
 */

#ifndef _ekstertera_sysheaders_h_
#define _ekstertera_sysheaders_h_

//
// стандартные заголовочные файлы Qt
//

#include <QDir>
#include <QUuid>
#include <QMenu>
#include <QLabel>
#include <QDebug>
#include <QMutex>
#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QBuffer>
#include <QPainter>
#include <QMenuBar>
#include <QToolBar>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QSettings>
#include <QRunnable>
#include <QMimeData>
#include <QFileInfo>
#include <QTreeView>
#include <QStatusBar>
#include <QScrollBar>
#include <QSslCipher>
#include <QTextCodec>
#include <QThreadPool>
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

// конкатенация строк для макроса версии
#define ETERA_STR_EXPAND(token) #token
#define ETERA_STR(token)        ETERA_STR_EXPAND(token)

// версия (0.0.12)
#define ETERA_VERSION_MAJOR 0
#define ETERA_VERSION_MINOR 0
#define ETERA_VERSION_PATCH 12
#define ETERA_VERSION_NUM   ((ETERA_VERSION_MAJOR << 16) | (ETERA_VERSION_MINOR << 8) | (ETERA_VERSION_PATCH))
#define ETERA_VERSION       ETERA_STR(ETERA_VERSION_MAJOR) "." ETERA_STR(ETERA_VERSION_MINOR) "." ETERA_STR(ETERA_VERSION_PATCH)

#endif   // _ekstertera_sysheaders_h_
