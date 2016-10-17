CC    = g++
#CC   = clang++-3.6 -L /usr/lib/gcc/x86_64-linux-gnu/4.8 -I /usr/include/x86_64-linux-gnu/c++/4.8 -I /usr/include/c++/4.8 

FLAGS = -std=c++11 -g -Wall -pedantic


game.out: Character.h Player.h Dragon.h Area.h FelSpawn.h Item.h ContainerItem.h Keepable.h
	$(CC) $(FLAGS) -o game.out main.cpp Character.cpp Player.cpp Dragon.cpp Area.cpp FelSpawn.cpp Item.cpp ContainerItem.cpp Keepable.cpp



clean:
	rm -f *.o *.out

