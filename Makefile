CLASS_OBJECTS = Alm.o Cmb3D.o Cmb.o Coordinate.o Counter.o Ek.o Cmbdata.o Random.o

SDL_INCLUDE_DIR = -I/usr/include/SDL/
SDL_LIB_DIR = -L/usr/lib
SDL_LIB = -lSDL -lglut -lGLU -lGL
#GSL_INCLUDE_DIR = -I/usr/include
#GSL_LIB_DIR = -L/usr/lib
GSL_LIB = -lgsl -lgslcblas
LDFLAGS = -lm -lpthread -lX11

CC = g++
LIBS    =  -lgsl -lgslcblas

GCC_FLAGS = -pthread -lgsl -lSDL -lpthread -lglut -lGLU -lGL -lm -lXi -lX11 -lXmu -lXext -lgslcblas -O3

all: cmbtopo

clean:
	rm -f $(CLASS_OBJECTS) cmbtopo.o cmbtopo

cmbtopo: $(CLASS_OBJECTS) cmbtopo.o
	g++ -o cmbtopo cmbtopo.o $(CLASS_OBJECTS) $(SDL_LIB_DIR) $(GSL_LIB_DIR) $(GSL_LIB) $(SDL_LIB) $(GCC_FLAGS) $(GPROF)

cmbtopo.o: cmbtopo.cpp cmbtopo.h cmbtopo_i.h Cmb3D.h Cmb.h
	g++ -c cmbtopo.cpp $(SDL_INCLUDE_DIR) $(GSL_INCLUDE_DIR) $(GCC_FLAGS) $(GPROF)

Alm.o: Alm.cpp Alm.h Alm_i.h
	g++ -c Alm.cpp $(GCC_FLAGS) $(GPROF)

Cmb3D.o: Cmb3D.cpp Cmb3D.h Cmb3D_i.h
	g++ -c Cmb3D.cpp $(SDL_INCLUDE_DIR) $(GSL_INCLUDE_DIR) $(GCC_FLAGS) $(GPROF)

Cmb.o: Cmb.cpp Cmb.h Cmb_i.h
	g++ -c Cmb.cpp $(GSL_INCLUDE_DIR) $(GCC_FLAGS) $(GPROF)

Coordinate.o: Coordinate.cpp Coordinate.h Coordinate.h
	g++ -c Coordinate.cpp $(GCC_FLAGS) $(GPROF)

Counter.o: Counter.cpp Counter.h Counter_i.h
	g++ -c Counter.cpp $(GCC_FLAGS) $(GPROF)

Ek.o: Ek.cpp Ek.h Ek.h NegVector.h
	g++ -c Ek.cpp $(GCC_FLAGS) $(GPROF)

Cmbdata.o: Cmbdata.cpp Cmbdata.h Cmbdata_i.h
	g++ -c Cmbdata.cpp $(GCC_FLAGS) $(GPROF)

Random.o: Random.cpp Random.h
	g++ -c Random.cpp $(GSL_INCLUDE_DIR) $(GCC_FLAGS) $(GPROF)
