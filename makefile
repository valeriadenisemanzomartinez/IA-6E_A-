CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Iinclude -I"C:/msys64/mingw64/include"
LDFLAGS = -mwindows
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = bin/rescue-squad.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
