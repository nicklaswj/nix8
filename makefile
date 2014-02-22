CC=g++

CFLAGS=-c -Wall `pkg-config gtkmm-3.0 --cflags`

nix8: CPU.o screen.o memory.o main.o mainWindow.o
	$(CC) -o nix8 `pkg-config gtkmm-3.0 --libs` main.o screen.o mainWindow.o CPU.o memory.o

mainWindow.o: mainWindow.cpp
	$(CC) $(CFLAGS) mainWindow.cpp

screen.o: screen.cpp
	$(CC) $(CFLAGS) screen.cpp

CPU.o: CPU.cpp
	$(CC) $(CFLAGS) CPU.cpp

memory.o: memory.cpp
	$(CC) $(CFLAGS) memory.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

testMain.o: testMain.cpp
	$(CC) $(CFLAGS) testMain.cpp

memoryTest.o: memoryTest.cpp
	$(CC) $(CFLAGS) memoryTest.cpp

CPUTest.o: CPUTest.cpp
	$(CC) $(CFLAGS) CPUTest.cpp

test: memoryTest.o memory.o CPU.o CPUTest.o testMain.o
	$(CC) -o unitTest memoryTest.o testMain.o memory.o CPU.o CPUTest.o

clean:
	rm -rf *.o nix8 unitTest
