#-------------------------------------------------
#
# Project created by QtCreator 2018-01-04T22:13:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = poisson_image_editting
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    picture.cpp \
    cloning.cpp

HEADERS  += mainwindow.h \
    picture.h \
    cloning.h 
FORMS    += mainwindow.ui \
    picture.ui

INCLUDEPATH += eigen/Eigen

DISTFILES += \
    README.md
