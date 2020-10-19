SOURCES = $(wildcard src/*.cc) $(wildcard src/*/*.cc)
OBJECTS = $(SOURCES:.c=.o)
CFLAGS = -g -std=c++17 -O2 -Wall -Wextra -Werror -pedantic

LDFLAGS := -lpthread -lserved -lboost_system
LDFLAGS += -lprometheus-cpp-core -lprometheus-cpp-pull

plant_server.out: $(OBJECTS)
	g++ $(CFLAGS) -o $@ $^ $(LDFLAGS)
clean::
	rm plant_server.out
