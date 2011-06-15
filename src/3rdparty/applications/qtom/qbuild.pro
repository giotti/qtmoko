TEMPLATE=app
TARGET=qtom

CONFIG+=qtopia
DEFINES+=QTOPIA

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

# Package info
pkg [
    name=qtom
    desc="OM GUI"
    license=GPLv2
    version=1.0
    maintainer="giotti"
]

# Input files
FORMS +=\
    qtom.ui\
    widget.ui

HEADERS=\
    qtom.h\
    widget.h

SOURCES=\
    main.cpp\
    qtom.cpp\
    widget.cpp

# Install rules
target [
    hint=sxe
    domain=untrusted
]

desktop [
    hint=desktop
    files=qtom.desktop
    path=/apps/Applications
]

pics [
    hint=pics
    files=pics/*
    path=/pics/qtom
]

