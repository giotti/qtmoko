# configurazioni
QT += network \
    sql \
    core \
    gui
CONFIG += warn_on \
    qt \
    release

# sorgenti
HEADERS += src/configura.h \
    src/statistiche.h \
    src/thread.h \
    src/random.h \
    src/finestra.h \
    src/giocatore.h \
    src/avvio.h \
    src/mazzo.h \
    src/server.h \
    src/client.h \
    src/fine.h \
    src/carta.h
FORMS += ui/configura.ui \
    ui/statistiche.ui \
    ui/finestra.ui \
    ui/avvio.ui \
    ui/fine.ui
SOURCES += src/configura.cpp \
    src/statistiche.cpp \
    src/thread.cpp \
    src/random.cpp \
    src/finestra.cpp \
    src/giocatore.cpp \
    src/avvio.cpp \
    src/mazzo.cpp \
    src/carta.cpp \
    src/server.cpp \
    src/client.cpp \
    src/fine.cpp \
    src/main.cpp

# build
macx:OBJECTS_DIR += build/o/mac
unix:OBJECTS_DIR += build/o/unix
win32:OBJECTS_DIR += build/o/win32
MOC_DIR += build/moc
RCC_DIR += build/rcc
UI_DIR += build/ui

# binario
DESTDIR += bin

# percorsi
DEPENDPATH += . \
    src \
    ui
INCLUDEPATH += . \
    src

# altro
RC_FILE += QBriscola.rc
RESOURCES += resources/QBriscola.qrc
TRANSLATIONS += resources/translations/trans_Italian.ts


