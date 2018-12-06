#include "pch.h"
#include "Core.h"

using namespace std;

/*
	Last thing I did was redo graphics and input lib and made a tiny demonstration of those libraries


	To do:
		game
			minigame2
			minigame3
			minigame4
		nicer 

	Finished:
		input library
		graphics library
		FPS counter
		improve stability for graphics library (added mutex for multithreading)
		added minigame 1
*/


int main() {

	Core core;
	//core.Test();
	core.Run();
	
}