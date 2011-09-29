#-------------------------------------------------
#
# Project created by QtCreator 2011-09-13T11:39:49
#
#-------------------------------------------------

QT       += core gui

TARGET = SOD
TEMPLATE = app


SOURCES += main.cpp\
    heuristic/D-Ants/dants.cpp \
    struct/depot.cpp \
    heuristic/localsearch.cpp \
    inputcanonicalvrp.cpp \
    heuristic/opt2.cpp \
    struct/order.cpp \
    struct/route.cpp \
    heuristic/D-Ants/pheromoneinf.cpp \
    heuristic/D-Ants/sbas.cpp \
    heuristic/sweepalgorithm.cpp \
    struct/sod.cpp \
    gui/showroutes.cpp \
    gui/mainwindow.cpp \
    heuristic/newJoinRoutes/joinroutes.cpp \
    heuristic/swap.cpp \
    heuristic/savingsalgorithm.cpp

HEADERS  += \
    heuristic/D-Ants/dants.h \
    struct/depot.h \
    heuristic/localsearch.h \
    inputcanonicalvrp.h \
    heuristic/opt2.h \
    struct/order.h \
    struct/route.h \
    heuristic/D-Ants/pheromoneinf.h \
    struct/sod.h \
    heuristic/sweepalgorithm.h \
    heuristic/D-Ants/sbas.h \
    gui/mainwindow.h \
    gui/showroutes.h \
    heuristic/newJoinRoutes/joinroutes.h \
    heuristic/swap.h \
    joinRoutes/simplemove.h \
    joinRoutes/route.h \
    joinRoutes/moves.h \
    joinRoutes/joinroutes.h \
    heuristic/savingsalgorithm.h

FORMS    += mainwindow.ui



































