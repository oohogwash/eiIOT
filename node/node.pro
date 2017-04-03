TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

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
    eiLib/rs232.cpp

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
    eiLib/eiCom.cpp.autosave.J11824
