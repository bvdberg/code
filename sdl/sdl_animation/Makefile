# Makefile for sdltutorials.com SDL Entities tutorial
# make by gaten: gaten DOT net AT gmail DOT com

# Compiler
CC=g++

# Object files
OBJECTS=CAnimation.o CApp.o CApp_OnCleanup.o CApp_OnEvent.o CApp_OnInit.o CApp_OnLoop.o CApp_OnRender.o CEvent.o CSurface.o

# Flags
CPPFLAGS=-Wall -g `sdl-config --cflags`

# Linking flags
LDFLAGS=`sdl-config --libs` -lSDL -lSDL_image

main: CApp.cpp
	$(CC) $(CPPFLAGS) -c CAnimation.cpp CApp.cpp CApp_OnCleanup.cpp CApp_OnEvent.cpp CApp_OnInit.cpp CApp_OnLoop.cpp CApp_OnRender.cpp CEvent.cpp CSurface.cpp
	$(CC) $(OBJECTS) $(CPPFLAGS) $(LDFLAGS) -o sdl_animation

# Cleanup
clean:
	rm -rf $(OBJECTS)

