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
#include <QCommonStyle>
#include <QApplication>
#include <QLibraryInfo>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QListWidgetItem>
#include <QDesktopServices>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

// в Qt > 5 часть методов QUrl типа addQueryItem признаны obsolete
#if QT_VERSION >= 0x050000
    #include <QUrlQuery>
#endif

//
// сторонние библиотеки
//

// https://github.com/gaudecker/qt-json
#include "3dparty/json.h"

//
// общие определения для проекта
//

#define ETERA_STR_EXPAND(token) #token
#define ETERA_STR(token)        ETERA_STR_EXPAND(token)

#define ETERA_VERSION_MAJOR 0
#define ETERA_VERSION_MINOR 0
#define ETERA_VERSION_PATCH 5
#define ETERA_VERSION_NUM   ((ETERA_VERSION_MAJOR << 16) | (ETERA_VERSION_MINOR << 8) | (ETERA_VERSION_PATCH))
#define ETERA_VERSION       ETERA_STR(ETERA_VERSION_MAJOR) "." ETERA_STR(ETERA_VERSION_MINOR) "." ETERA_STR(ETERA_VERSION_PATCH)

#endif   // _ekstertera_sysheaders_h_
