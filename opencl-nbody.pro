######################################################################
# Automatically generated by qmake (2.01a) Wed Jan 18 10:21:42 2012
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . src
INCLUDEPATH += . /usr/local/include/ ./src/ /usr/include/
CONFIG -= qt
CONFIG += debug
CFLAGS += -Wall -Wextra -pg
CXXFLAGS += -Wall -Wextra -pg
unix:LIBS += -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -L/usr/lib/x86_64-linux-gnu -lGLU -lGL -L/usr/local/lib -lOpenCL

OBJECTS_DIR = obj

# Input
HEADERS += src/*.h
SOURCES += src/*.cpp
