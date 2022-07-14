CC = gcc
CFLAGS = -std=c99 -Wall -ggdb -finput-charset=utf-8 -fexec-charset=utf-8
TARGET = top_call
OBJS = *.c
FILENAME = ./toReadFile/*.txt
PNG = *.png
DOT = *.dot

all:
	mkdir ./build/
	mkdir ./dotFile/
	mkdir ./pngFile/
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
	./$(TARGET) $(FILENAME)
	cp $(TARGET) ./build/
	cp ./toReadFile/$(PNG) ./pngFile/
	rm ./toReadFile/$(PNG) -rf
	cp ./toReadFile/$(DOT) ./dotFile/
	rm ./toReadFile/$(DOT) -rf
#	sxiv ./pngFile/*

clean:
	rm -rf $(TARGET) ./build/
	rm -rf ./pngFile
	rm -rf ./dotFile

install:
	sudo cp $(TARGET) /usr/bin

uninstall:
	sudo rm -rf /usr/bin/$(TARGET)
