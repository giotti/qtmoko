TEMPLATE=app
TARGET=qtbriscola

CONFIG+=qtopia
DEFINES+=QTOPIA

QT += network \
    sql \
    core \
    gui

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

# Package info
pkg [
    name=qtbriscola
    desc="Briscola for Qt"
    license=GPL
    version=1.1.1
    maintainer="giotti"
]

# Input files
FORMS +=\
   ui/avvio.ui\
   ui/configura.ui\
   ui/finestra.ui\
   ui/fine.ui\
   ui/statistiche.ui

HEADERS=\
   src/avvio.h\
   src/carta.h\
   src/client.h\
   src/configura.h\
   src/fine.h\
   src/finestra.h\
   src/giocatore.h\
   src/mazzo.h\
   src/random.h\
   src/server.h\
   src/statistiche.h\
   src/thread.h

SOURCES=\
   src/avvio.cpp\
   src/carta.cpp\
   src/client.cpp\
   src/configura.cpp\
   src/fine.cpp\
   src/finestra.cpp\
   src/giocatore.cpp\
   src/main.cpp\
   src/mazzo.cpp\
   src/random.cpp\
   src/server.cpp\
   src/statistiche.cpp\
   src/thread.cpp

RC_FILE += QBriscola.rc
RESOURCES += resources/QBriscola.qrc
TRANSLATIONS += resources/translations/trans_Italian.ts

# Install rules
target [
    hint=sxe
    domain=untrusted
]

desktop [
    hint=desktop
    files=qbriscola.desktop
    path=/apps/Games
]

pics [
    hint=pics
    files=pics/*
    path=/pics/qbriscola
]

