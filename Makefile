CXX = g++
CXXFLAGS = -I /mnt/c/Users/idaco/Documents/prgs/rapidjson/include
LDFLAGS = -lcurl

TARGET = graph-crawler
SRC = graph-crawler.cpp
OBJ = graph-crawler.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $(TARGET) $(OBJ) $(LDFLAGS)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c -o $(OBJ) $(SRC)

clean:
	rm -f $(OBJ) $(TARGET)
