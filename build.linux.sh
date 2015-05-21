#!/usr/bin/env bash

set -e

# имя проекта
PROJECT="ekstertera"

# очистка
if [ -f "Makefile" ]; then
    make clean
fi

# версия qt
QT_VERSION=$(qmake -query 'QT_VERSION' | cut -d '.' -f 1)

# опции (модули) qt
QT_OPTS="network"
SRC_3DPARTY="3dparty/json"

if [ "${QT_VERSION}" -eq "4" ]; then
    SRC_3DPARTY="${SRC_3DPARTY} 3dparty/qt5"
elif [ "${QT_VERSION}" -eq "5" ]; then
    QT_OPTS="${QT_OPTS} core widgets"
else
    echo "Unknown Qt version"
    exit 1
fi

# создание pro-файла
qmake -project -recursive -Wall -nopwd -o "${PROJECT}.pro" \
    "CODEC = UTF-8" \
    "CODECFORTR = UTF-8" \
    "CONFIG += debug_and_release" \
    "QT += ${QT_OPTS}" \
    "INCLUDEPATH += src" \
    "TRANSLATIONS += \
    src/translations/${PROJECT}_en.ts" \
    src ${SRC_3DPARTY}

# qttools5-dev-tools для qt5
lupdate -no-obsolete "${PROJECT}.pro"
lrelease -compress -removeidentical "${PROJECT}.pro"

# создание make-файлов
qmake "${PROJECT}.pro"

# создание бинарника
make $1

# удаление символов
strip -s "${PROJECT}"

# запуск
if [ -z "${TRAVIS}" ]; then
    "./${PROJECT}"
fi
