CXX = g++
CXXFLAGS = -Wall -std=c++17

SRC_DIR = src
BUILD_DIR = build
TARGET = program.exe

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rmdir /s /q $(BUILD_DIR)
	del /q $(TARGET)

run: $(TARGET)
	$(TARGET)
