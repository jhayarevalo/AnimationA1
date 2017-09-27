# This Makefile is written for Linux and Mac OS.

#Add header files here if you have added any h file
HEADERS =\
 defMesh.h\
 glm.h\
 simpleMath.h\
 skeleton.h\
 splitstring.h\


#Add source files here if you have added any cpp file
SOURCES =\
 defMesh.cpp\
 glm.cpp\
 simpleMath.cpp\
 skeleton.cpp\
 splitstring.cpp\
 glMain.cpp\


#Name of binary file
BINARY = base
CC = g++
CFLAGS = -g
LDFLAGS = 
UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S),Linux)
  LDFLAGS += -L/usr/lib/nvidia-331 -lGL -lGLU -lglut
endif
ifeq ($(UNAME_S),Darwin)
  LDFLAGS += -framework OpenGL -lglut
  #Uncomment this if you wish to use glut provided by Apple
  #LDFLAGS += -framework OpenGL -framework GLUT	
endif

$(BINARY):$(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS)
