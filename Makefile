###########################################################
# Project 1 Makefile

CC = g++
CFLAGS = -Wall -ggdb
INCLUDE = -I/lusr/X11/include -I/lusr/include
LIBDIR = -L/lusr/X11/lib -L/lusr/lib
# Libraries that use native graphics hardware --
# appropriate for Linux machines in Taylor basement
ifdef SystemRoot
	LIBS = -lglut32 -lGLU32 -lopenGL32 -lpthread -lm
else
	ifeq ($(shell uname), Linux)
		LIBS = -lglut -lGLU -lGL -lpthread -lm
	endif
endif

###########################################################
# Options if compiling on Mac
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
CC = g++
CFLAGS = -Wall -g -D__MAC__
INCLUDE = 
LIBDIR = -L/lusr/X11/lib
LIBS = -framework OpenGL -framework GLUT
endif

###########################################################
# Uncomment the following line if you are using Mesa
#LIBS = -lglut -lMesaGLU -lMesaGL -lm

raytrace: raytrace.cpp geometry.cpp light.cpp lowlevel.cpp raytrace.h lowlevel.h
	${CC} ${CFLAGS} ${INCLUDE} -o raytrace ${LIBDIR} raytrace.cpp geometry.cpp light.cpp lowlevel.cpp ${LIBS} 

clean:
	rm -f raytrace *.o core
