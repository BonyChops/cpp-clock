TARGET = j18314.exe
SRCS = main.cpp WindowManager.cpp
OBJS = ${SRCS:.cpp=.o}
HEADERS=
ICONRC = myicon.rc
ICONOBJ = myicon.o
CC = i686-pc-cygwin-g++
CCFLAGS = -Wall -I/usr/include/opengl
LD = i686-pc-cygwin-g++
LDFLAGS =
LIBS = -lglpng -lglut32 -lglu32 -lopengl32 -lm

$(TARGET) : $(OBJS)
	i686-pc-cygwin-windres -i $(ICONRC) -o $(ICONOBJ)
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJS) $(ICONOBJ) $(LIBS)

.cpp.o:
	$(CC) $(CCFLAGS) -c $<

$(OBJS) : $(HEADERS) Makefile

clean:
	rm -f $(TARGET) $(OBJS) core