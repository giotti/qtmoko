# qbriscola CMakeLists.txt

PROJECT(QBriscola)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.0)
SET(QBR "qbriscola" CACHE STRING "Name of the executeable")
SET(OPTIMIZATION_FLAGS "-O2")
ADD_DEFINITIONS(${OPTIMIZATION_FLAGS})

SET(qbriscola_UIS
	ui/avvio.ui
	ui/configura.ui
	ui/finestra.ui
	ui/fine.ui
	ui/statistiche.ui
)

SET(qbriscola_SRCS
	src/avvio.cpp
	src/fine.cpp
	src/mazzo.cpp
	src/thread.cpp
	src/carta.cpp
	src/finestra.cpp
	src/random.cpp
	src/client.cpp
	src/giocatore.cpp
	src/server.cpp
	src/configura.cpp
	src/main.cpp
	src/statistiche.cpp
)

SET(qbriscola_MOC_HDRS
	src/avvio.h
	src/configura.h
	src/server.h
	src/fine.h
	src/statistiche.h
	src/client.h
	src/finestra.h
	src/thread.h
)

SET(qbriscola_RCS
	QBriscola.rc
)

SET(qbriscola_RESOURCES
	resources/QBriscola.qrc
)

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

QT4_WRAP_UI (qbriscola_UIS_H ${qbriscola_UIS})
QT4_WRAP_CPP (qbriscola_MOC_SRCS ${qbriscola_MOC_HDRS})
QT4_ADD_RESOURCES (qbriscola_RESOURCES ${qbriscola_RESOURCES})
QT4_AUTOMOC (${qbriscola_SRCS})

ADD_DEFINITIONS(
	-Wall
	-DQT_NO_DEBUG
	${QT_DEFINITIONS}
)

INCLUDE_DIRECTORIES(
	${QT_INCLUDES}
	${QT_INCLUDE_DIR}/QtUiTools
	${CMAKE_BINARY_DIR}
	.
	src
	src/ui
)

SET(qbriscola_SRCS ${qbriscola_SRCS} QBriscola.rc)
SET(QT_UITOOLSLIB ${QT_QTUITOOLS_LIBRARY} QtDesigner)

ADD_EXECUTABLE(${QBR}
	${qbriscola_SRCS}
	${qbriscola_UIS_H}
	${qbriscola_MOC_SRCS}
	${qbriscola_UIS}
	${qbriscola_RESOURCES}
)

TARGET_LINK_LIBRARIES(${QBR}
	${QT_UITOOLSLIB}
	${QT_QTSQL_LIBRARY}
	${QT_LIBRARIES}
	${QT_QTXML_LIBRARY}
	${QT_QTNETWORK_LIBRARY}
)

SET(DESK qbriscola.desktop)
SET(ICO resources/icons/qbriscola.png)
SET(BIN ${CMAKE_BINARY_DIR}/${QBR})

INSTALL(FILES ${DESK} DESTINATION "share/applications")
INSTALL(FILES ${ICO} DESTINATION "share/icons")
INSTALL(PROGRAMS ${BIN} DESTINATION "bin")
