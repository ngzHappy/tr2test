win32-msvc*{
    QMAKE_CXXFLAGS += /std:c++latest
    QMAKE_CXXFLAGS += /await
    CONFIG+=suppress_vcproj_warnings
}else{
    linux-g++*{
        LIBS += -lstdc++fs
        CONFIG(debug,debug|release){
        }else{
            QMAKE_CXXFLAGS += -Wno-cpp
        }
    }
    QMAKE_CXXFLAGS += -std=c++17
    #set c version
    QMAKE_CFLAGS += -std=c11
}

TARGET = test_tr2

TEMPLATE = app

SOURCES += $$PWD/register_class_information.cpp
HEADERS += $$PWD/register_class_information.hpp

SOURCES += $$PWD/main.cpp

SOURCES += $$PWD/test_sstd_type_traits.cpp
HEADERS += $$PWD/sstd_type_traits.hpp

SOURCES += $$PWD/sstd_virtual_basic.cpp
HEADERS += $$PWD/sstd_virtual_basic.hpp

QT += core
QT += gui
QT += widgets

DISTFILES += \
    readme.txt



