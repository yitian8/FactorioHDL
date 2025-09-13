CXX = g++
CXXFLAGS = -std=c++20 -I include -Wall -Wextra
SRC = src/run_scanner.cpp src/scanner.cpp src/token.cpp
OUT = scanner

all: $(OUT)

$(OUT): $(SRC) $(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

clean: del $(OUT).exe 2>nul || rm -f $(OUT)