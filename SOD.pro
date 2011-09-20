#-------------------------------------------------
#
# Project created by QtCreator 2011-09-13T11:39:49
#
#-------------------------------------------------

QT       += core gui

TARGET = SOD
TEMPLATE = app


SOURCES += main.cpp\
    dants.cpp \
    depot.cpp \
    localsearch.cpp \
    inputcanonicalvrp.cpp \
    opt2.cpp \
    order.cpp \
    route.cpp \
    pheromoneinf.cpp \
    sbas.cpp \
    sweepalgorithm.cpp \
    sod.cpp \
    gui/showroutes.cpp \
    gui/mainwindow.cpp \
    joinroutes.cpp \
    swap.cpp

HEADERS  += \
    dants.h \
    depot.h \
    localsearch.h \
    inputcanonicalvrp.h \
    opt2.h \
    order.h \
    route.h \
    pheromoneinf.h \
    sod.h \
    sweepalgorithm.h \
    sbas.h \
    gui/mainwindow.h \
    gui/showroutes.h \
    joinroutes.h \
    swap.h \
    joinRoutes/simplemove.h \
    joinRoutes/route.h \
    joinRoutes/moves.h \
    joinRoutes/joinroutes.h

FORMS    += mainwindow.ui

































