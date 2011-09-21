TARGET = cutepairs
HEADERS += TBlock.h \
    TGameBoard.h \
    TMainForm.h \
    TStatisticsForm.h \
    TChampion.h
SOURCES += TBlock.cpp \
    TGameBoard.cpp \
    main.cpp \
    TMainForm.cpp \
    TStatisticsForm.cpp \
    TChampion.cpp
RESOURCES += qt-pairs.qrc
TRANSLATIONS += translations/qt-pairs_en.ts  \
    translations/qt-pairs_ru.ts \
    translations/qt-pairs_es.ts\
    translations/qt-pairs_ca.ts\
    translations/qt-pairs_fr.ts\
    translations/qt-pairs_de.ts
QT += xml
symbian:{
    LIBS +=-lcone \
            -leikcore \
            -lavkon
    ICON = icon.svg
    TARGET.UID3 = 0x20030CE9

    default_deployment.pkg_prerules = \
        #";Language" \
        #"&EN,SP,CA,FR,GE" \
        " " \
        #"; SIS header: name, uid, version" \
        "$${LITERAL_HASH}{\"CutePairs\"},(0x20030CE9),1,0,0" \#Release
        " " \
        "; Localised Vendor name" \
        "%{\"ssorgatem\"}" \
        " " \
        "; Unique Vendor name" \
        ":\"ssorgatem\"" \
        " " \
        "; Default HW/platform dependencies" \
        "[0x1028315F],0,0,0,{\"S60ProductID\"}" \
        "[0x20022E6D],0,0,0,{\"S60ProductID\"}" \
        " "
}
