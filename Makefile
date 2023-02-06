all:
	gcc -Iinclude -Llib -o main.exe main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lstdc++