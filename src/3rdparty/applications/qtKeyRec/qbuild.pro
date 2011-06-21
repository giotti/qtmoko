TEMPLATE=app
TARGET=qtKeyRec

CONFIG+=qtopia
DEFINES+=QTOPIA

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

# Package info
pkg [
    name=qtKeyRec
    desc="Sgarmi emulator"
    license=GPLv2
    version=1.0
    maintainer="giotti"
]

# Input files
FORMS +=\
    qtKeyRec.ui\
    widget.ui

HEADERS=\
    qtKeyRec.h\
    rotate.h\
    widget.h

SOURCES=\
    main.cpp\
    qtKeyRec.cpp\
    rotate.cpp\
    widget.cpp

# Install rules
target [
    hint=sxe
    domain=untrusted
]

desktop [
    hint=desktop
    files=qtKeyRec.desktop
    path=/apps/Games
]

pics [
    hint=pics
    files=pics/*
    path=/pics/qtKeyRec
]

