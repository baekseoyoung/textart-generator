# In bash:
export LD_LIBRARY_PATH=$(PWD)

all: asciiart.o asciicalculate.o asciicalculate.hpp sdl_boxes.hpp win_filemanage.hpp sdl_events.hpp
	g++ -Wall -o a asciiart.o asciicalculate.o \
	 `pkg-config --cflags --libs opencv4` \
	 `pkg-config --cflags --libs sdl2` \
	 `pkg-config --cflags --libs sdl2_ttf` \
	 `pkg-config --cflags --libs sdl2_image` \

asciiart.o: asciiart.cpp asciicalculate.o sdl_boxes.hpp
	g++ -Wall -c asciiart.cpp -o asciiart.o\
	 `pkg-config --cflags --libs opencv4` \
	 `pkg-config --cflags --libs sdl2` \
	 `pkg-config --cflags --libs sdl2_ttf` \
	 `pkg-config --cflags --libs sdl2_image` \

asciicalculate.o: asciicalculate.cpp asciicalculate.hpp
	g++ -Wall -c asciicalculate.cpp -o asciicalculate.o

clean:
	rm *.o