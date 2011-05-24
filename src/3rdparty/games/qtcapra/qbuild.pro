TEMPLATE=app
TARGET=qtcapra

CONFIG+=qtopia
DEFINES+=QTOPIA

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

# Package info
pkg [
    name=qtcapra
    desc="Sgarbi emulator"
    license=BSD
    version=0.1
    maintainer="Giotti <xxxxxxx@xxxxxx.xx>"
]

# Input files
HEADERS=\
    qtcapra.h

SOURCES=\
    main.cpp\
    qtcapra.cpp

# Install rules
target [
    hint=sxe
    domain=untrusted
]

desktop [
    hint=desktop
    files=qtcapra.desktop
    path=/apps/Applications
]

pics [
    hint=pics
    files=pics/*
    path=/pics/qtcapra
]

