@ECHO OFF

REM ***************
REM * имя проекта *
REM ***************
SET PROJECT_NAME=ekstertera
SET PROJECT_FILE=%PROJECT_NAME%.pro

REM ************************
REM * исполняемые файлы qt *
REM ************************
SET QT_BIN=c:\projects\qt\bin
SET QT_QMAKE=%QT_BIN%\qmake.exe
SET QT_LRELEASE=%QT_BIN%\lrelease.exe

REM *****************
REM * Microsoft SDK *
REM *****************

SET MS_SDK=%ProgramFiles%\Microsoft SDKs\Windows\v7.1
SET MS_SDK_INC=%MS_SDK%\Include
SET MS_SDK_LIB=%MS_SDK%\Lib

REM *******************************
REM * создание проекта и makefile *
REM *******************************
%QT_QMAKE% -project -recursive -Wall -nopwd -o %PROJECT_FILE% ^
    "CODEC = UTF-8" ^
    "CODECFORTR = UTF-8" ^
    "CONFIG += release" ^
    "QT += network" ^
    "INCLUDEPATH += src" ^
    "LIBS += -L$$quote(%MS_SDK_LIB%) -luuid" ^
    "TRANSLATIONS += src/translations/ekstertera_en.ts" ^
    "RC_FILE += src/resource.rc" ^
    src 3dparty/json

%QT_LRELEASE% -compress -removeidentical %PROJECT_FILE%

%QT_QMAKE% %PROJECT_FILE%

REM ******************************************
REM * сборка (консоль должна быть запущена в *
REM * Qt Command Prompt)                     *
REM ******************************************
mingw32-make.exe
