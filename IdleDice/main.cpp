#include "pch.h"
#include "Core.h"

using namespace std;

/*
	Last thing I did was redo graphics and input lib and made a tiny demonstration of those libraries


	To do:
		Quick minigame guide
		start menu

	Finished:
		input library
		graphics library
		FPS counter
		improve stability for graphics library (added mutex for multithreading)
		minigame1
		minigame2
		minigame3
		game
*/


int main() {

	Core core;

	core.RunGameCore();
	//core.Test();
}