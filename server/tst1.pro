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
    eiLib/eiqueue.cpp \
    eiLib/eibody.cpp \
    eiLib/eilogon.cpp \
    eiArduino/Thread.cpp \
    eiArduino/ThreadController.cpp \
    eiLib/comIO.cpp \
    eiLib/comio_os.cpp \
    eiLib/comIOcp.cpp \
    eiLib/eiCfg.cpp \
    eiLib/eiCom.cpp \
    eiLib/eiControl.cpp \
    eiLib/eiDisplay.cpp \
    eiLib/eiLib.cpp \
    eiLib/eiqueue.cpp \
    eiLib/rs232.cpp \
    test/testros.cpp \
    test/testthread.cpp \
    serial.cpp \
    eiLib/eimsg.cpp \
    msgBody.cpp




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
    eiLib/eimsg.h \
    eiLib/eibody.h \
    eiLib/eilogon.h \
    eiArduino/Thread.h \
    eiArduino/ThreadController.h \
    eiLib/comIO.h \
    eiLib/comio_os.h \
    eiLib/comIOCP.h \
    eiLib/eiCfg.h \
    eiLib/eiCom.h \
    eiLib/eiControl.h \
    eiLib/eiDisplay.h \
    eiLib/eiLib.h \
    eiLib/eimsg.h \
    eiLib/eiqueue.h \
    eiLib/msgrecord.h \
    eiLib/rs232.h \
    test/testthread.h \
    serial.h \
    unix.h \
    msBody.h



