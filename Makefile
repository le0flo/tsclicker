CC = g++
SOURCE_DIR = src
BUILD_DIR = build
APP_NAME = tsclicker.dll

FLAGS = -std=c++17 -c -O2 -Wall -fPIC $(TS_INCLUDES)

TS_INCLUDES = -Iinclude

all: tsclicker

tsclicker: objects
	$(CC) -o $(BUILD_DIR)/$(APP_NAME) -shared $(BUILD_DIR)/plugin.o $(BUILD_DIR)/utils.o $(BUILD_DIR)/clicker.o

objects: clean
	mkdir -p $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/plugin.o $(SOURCE_DIR)/plugin.cpp $(FLAGS)
	$(CC) -o $(BUILD_DIR)/utils.o -Iinclude $(SOURCE_DIR)/utils.cpp $(FLAGS)
	$(CC) -o $(BUILD_DIR)/clicker.o -Iinclude $(SOURCE_DIR)/clicker.cpp $(FLAGS)

clean:
	rm -rf $(BUILD_DIR)/*
