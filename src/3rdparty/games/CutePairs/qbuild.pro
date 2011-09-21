TEMPLATE=app
TARGET=cutepairs

CONFIG+=qtopia
DEFINES+=QTOPIA

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

# Package info
pkg [
    name=cutepairs
    desc="Find pairs game"
    license=GPLv2
    version=1.0
    maintainer="giotti"
]

# Input files
#FORMS +=\

HEADERS=\
    TBlock.h\
    TChampion.h\
    TGameBoard.h\
    TMainForm.h\
    TStatisticsForm.h

SOURCES=\
    main.cpp\
    TBlock.cpp\
    TChampion.cpp\
    TGameBoard.cpp\
    TMainForm.cpp\
    TStatisticsForm.cpp

RESOURCES += qt-pairs.qrc
TRANSLATIONS += translations/qt-pairs_en.ts  \
    translations/qt-pairs_ru.ts \
    translations/qt-pairs_es.ts\
    translations/qt-pairs_ca.ts\
    translations/qt-pairs_fr.ts\
    translations/qt-pairs_de.ts
QT += xml

# Install rules
target [
    hint=sxe
    domain=untrusted
]

desktop [
    hint=desktop
    files=CutePairs.desktop
    path=/apps/Games
]

pics [
    hint=pics
    files=pics/*
    path=/pics/cutepairs
]

