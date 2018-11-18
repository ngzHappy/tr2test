#use filesystem
LIBS += -lstdc++fs
CONFIG(debug,debug|release){
}else{
    QMAKE_CXXFLAGS += -Wno-cpp
}
#set c++ version
QMAKE_CXXFLAGS += -std=c++17
#set c version
QMAKE_CFLAGS += -std=c11

TARGET = test_tr2

TEMPLATE = app

SOURCES += $$PWD/register_class_information.cpp
HEADERS += $$PWD/register_class_information.hpp

SOURCES += $$PWD/main.cpp

QT += core
QT += gui
QT += widgets

DISTFILES += \
    readme.txt



