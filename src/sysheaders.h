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
#include <QPainter>
#include <QMenuBar>
#include <QToolBar>
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
#include <QSystemTrayIcon> // TODO: Проверить работу на Qt 5.x https://bugreports.qt.io/browse/QTBUG-31762
#include <QDesktopServices>
#include <QSslConfiguration>
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
    #include <windows.h>
    #include <shellapi.h>
    #include <commctrl.h>
    #include <commoncontrols.h>

    // IID_IImageList не определен в CommonControls.h для MinGW
    #define ETERA_IID_IImageList (QUuid(0x46eb5926, 0x582e, 0x4017, 0x9f, 0xdf, 0xe8, 0x99, 0x8d, 0xaa, 0x9, 0x50))
#endif

//
// сторонние библиотеки
//

// https://github.com/gaudecker/qt-json
#include "3dparty/json.h"

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

// версия (0.0.8)
#define ETERA_VERSION_MAJOR 0
#define ETERA_VERSION_MINOR 0
#define ETERA_VERSION_PATCH 8
#define ETERA_VERSION_NUM   ((ETERA_VERSION_MAJOR << 16) | (ETERA_VERSION_MINOR << 8) | (ETERA_VERSION_PATCH))
#define ETERA_VERSION       ETERA_STR(ETERA_VERSION_MAJOR) "." ETERA_STR(ETERA_VERSION_MINOR) "." ETERA_STR(ETERA_VERSION_PATCH)

#endif   // _ekstertera_sysheaders_h_
