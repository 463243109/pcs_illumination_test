QT += core
QT -= gui

TARGET = pcs_illumination_test
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11
TEMPLATE = app

SOURCES += main.cpp \
    density_cluster.cpp \
    corner_point.cpp

INCLUDEPATH += D:/opencv/include \

LIBS+= \
        D:/opencv/lib/libopencv_* \

HEADERS += \
    density_cluster.h \
    corner_point.h
