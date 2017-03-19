QT += core
QT -= gui

CONFIG += c++11

TARGET = tst1
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    eiLib/eiCfg.cpp \
    eiLib/eiCom.cpp \
    eiLib/eiLib.cpp \
    eiLib/eiControl.cpp \
    eiDash/display.cpp \
    eiDash/displaywidget.cpp \
    eiDash/iopoint.cpp \
    eiDash/connection.cpp \
    eiLib/eiDisplay.cpp \
    eimodule/iomodule.cpp \
    eimodule/displaymodule.cpp \
    eimodule/logicmodule.cpp \
    eimodule/commodule.cpp \
    eikernel/thread.cpp \
    eiLib/comIO.cpp \
    eimodule/modulemanager.cpp \
    eimodule/module.cpp \
    usr/egt.cpp \
    usr/rangecheckmodule.cpp \
    usr/analogueiomodule.cpp \
    eikernel/exec.cpp \
    eikernel/execstate.cpp \
    test/testmodule.cpp \
    eiLib/rs232.cpp \
    test/testrs232.cpp \
    eiLib/comIOcp.cpp \
    eiLib/eiqueue.cpp




DISTFILES += \
    eiLib alias

HEADERS += \
    eiLib/eiCfg.h \
    eiLib/eiCom.h \
    eiLib/eiLib.h \
    eiDash/display.h \
    eiDash/displaywidget.h \
    eiDash/iopoint.h \  
    eiLib/eiControl.h \
    eiLib/eiDisplay.h \
    eiDash/display.h \
    eiDash/displaywidget.h \
    eiDash/iopoint.h \
    eimodule/iomodule.h \
    eimodule/displaymodule.h \
    eimodule/logicmodule.h \
    eimodule/commodule.h \
    eimodule/modulemanager.h \
    eimodule/module.h \
    usr/egt.h \
    usr/rangecheckmodule.h \
    usr/analogueiomodule.h \
    eikernel/exec.h \
    eikernel/execstate.h \
    eikernel/wqueue.h \
    eiDash/connection.h \
    test/testmodule.h \
    test/testros.h \
    eiLib/rs232.h \
    test/testrs232.h \
    eikernel/thread.h \
    eiLib/comIO.h \
    eiLib/comIOCP.h \
    eiLib/eiqueue.h \
    eiLib/msgrecord.h \
    eiLib/eimsg.h



