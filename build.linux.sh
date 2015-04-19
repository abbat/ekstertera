#!/usr/bin/env bash

set -e

# версия qt
export QT_SELECT=4

# имя проекта
PROJECT="ekstertera"

# .pro файл
PROJECT_FILE="${PROJECT}.pro"

# очистка
if [ -f "Makefile" ]; then
    make clean
fi

# опции (модули) qt
QT_OPTS="network"
if [ "${QT_SELECT}" -eq "4" ]; then
    QT_OPTS="${QT_OPTS}"
else
    QT_OPTS="${QT_OPTS} core widgets"
fi

# создание pro-файла
qmake -project -recursive -Wall -nopwd -o "${PROJECT_FILE}" \
    "CODEC = UTF-8" \
    "CODECFORTR = UTF-8" \
    "CONFIG += debug_and_release" \
    "QT += ${QT_OPTS}" \
    "TRANSLATIONS += \
    src/translations/ekstertera_en.ts" \
    src

# qttools5-dev-tools для qt5
lupdate -no-obsolete "${PROJECT_FILE}"
lrelease -compress -removeidentical "${PROJECT_FILE}"

# создание make-файлов
qmake "${PROJECT_FILE}"

# создание бинарника
make $1

# удаление символов
strip -s "${PROJECT}"

# запуск
if [ -z "${TRAVIS}" ]; then
    "./${PROJECT}"
fi
