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
#include <QHash>
#include <QMenu>
#include <QLabel>
#include <QDebug>
#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QMenuBar>
#include <QToolBar>
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
#include <QPushButton>
#include <QSpacerItem>
#include <QListWidget>
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

#endif   // _ekstertera_sysheaders_h_
