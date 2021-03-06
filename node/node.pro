TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
DEFINES += NOT_ARDUINO
INCLUDEPATH += ./eiLib ./eiModule
win32{
LIBS += ws2_32.lib
DEFINES += _WIN
}
!win32{
macx
{
DEFINES += __MAC_OSX__
}

}

SOURCES += main.cpp \
    eiLib/comIO.cpp \
    eiLib/comio_os.cpp \
    eiLib/comIOcp.cpp \
    eiLib/eiCfg.cpp \
    eiLib/eiCom.cpp \
    eiLib/eiControl.cpp \
    eiLib/eiDisplay.cpp \
    eiLib/eiLib.cpp \
    eiLib/eimsg.cpp \
    eiLib/eiqueue.cpp \
    eiLib/rs232.cpp \
    eimodule/commodule.cpp \
    eimodule/displaymodule.cpp \
    eimodule/iomodule.cpp \
    eimodule/logicmodule.cpp \
    eimodule/module.cpp \
    eimodule/modulemanager.cpp \
    eimodule/modulemsg.cpp \
    eiLib/msgdef.cpp \
    eiLib/objectfactory.cpp \
    eiLib/createableobject.cpp \
    test/test1.cpp \
    test/util/FileCfg.cpp \
    test/util/sharedmemory.cpp \
    test/util/thread.cpp \
    test/util/mapio.cpp \
    test/util/io.cpp \
    test/util/guid.cpp \
    eiLib/collection.cpp

HEADERS += \
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
    eiLib/eiCom.cpp.autosave \
    eiLib/eiCom.cpp.autosave.J11824 \
    eiLib/osheader.h \
    eimodule/commodule.h \
    eimodule/displaymodule.h \
    eimodule/iomodule.h \
    eimodule/logicmodule.h \
    eimodule/module.h \
    eimodule/modulemanager.h \
    eimodule/modulemsg.h \
    eiLib/msgdef.h \
    eiLib/objectfactory.h \
    eiLib/createableobject.h \
    test/test1.h \
    test/util/FileCfg.h \
    test/util/sharedmemory.h \
    test/util/thread.h \
    test/util/mapio.h \
    test/util/io.h \
    test/util/guid.h \
    test/util/log.h \
    test/util/easylogging++.h \
    eiLib/collection.h
