  # the compiler gcc for C program, define as g++ for C++
  CC = gcc

  # compiler flags
  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
  CFLAGS  = -g -Wall
  LIBS = -lpthread -lm
  # the build target executable
  TARGET = primhilos primproc primsec
  all: primhilos primproc primsec

  primhilos: primhilos.c
	$(CC) $(CFLAGS) -o primhilos primhilos.c $(LIBS)
  primproc: primproc.c
	$(CC) $(CFLAGS) -o primproc primproc.c $(LIBS)	
  primsec: primsec.c
	$(CC) $(CFLAGS) -o primsec primsec.c $(LIBS)