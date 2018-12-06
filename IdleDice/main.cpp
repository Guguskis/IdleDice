#include "pch.h"
#include "Core.h"
#include "GraphicsLib.h"

using namespace std;

/*
	Last thing I did was redo graphics and input lib and made a tiny demonstration of those libraries


	To do:
		improve stability for graphics library
		game

	Finished:
		input library
		graphics library
		FPS counter
*/




string dices[6][7] = {
	{	"###########",
		"#         #",
		"#         #",
		"#    O    #",
		"#         #",
		"#         #",
		"###########"},
	{	"###########",
		"#O        #",
		"#         #",
		"#         #",
		"#         #",
		"#        O#",
		"###########"},
	{	"###########",
		"#O        #",
		"#         #",
		"#    O    #",
		"#         #",
		"#        O#",
		"###########"},
	{	"###########",
		"#O       O#",
		"#         #",
		"#         #",
		"#         #",
		"#O       O#",
		"###########"},
	{	"###########",
		"#O       O#",
		"#         #",
		"#         #",
		"#         #",
		"#O       O#",
		"###########"},
	{	"###########",
		"#O       O#",
		"#         #",
		"#O       O#",
		"#         #",
		"#O       O#",
		"###########"},
};
int money = 0;

void DrawDices(int dicesNum[]) {

	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 6; j++) {
			cout << dices[dicesNum[j]][i] << "	";
		}
		cout << endl;
	}

}

void RollDices() {
	while (money < 100) {
		system("cls");
		for (int i = 0; i < 10; i++) {
			int dicesNum[6] = { rand() % 6, rand() % 6, rand() % 6, rand() % 6, rand() % 6, rand() % 6 };
			DrawDices(dicesNum);
			cout << "\n\nMoney: " << money;
			Sleep(50 - i);
			system("cls");
		}

		int dicesNum[6] = { rand() % 6, rand() % 6, rand() % 6, rand() % 6, rand() % 6, rand() % 6 };
		DrawDices(dicesNum);
		cout << string(5, ' ') << dicesNum[0] + 1;
		for (int i = 1; i < 6; i++) {
			cout << string(15, ' ') << dicesNum[i] + 1;
		}
		cout << "\n\n";
		cout << "Money: " << money;
		Sleep(750);
	}
}

void  GetInput() {
	while (money<100) {
		int firstValue = _getch();
		int secondValue = _getch();
		if (firstValue == 224 && (secondValue == 75 || secondValue==80)) money--;
		else if (firstValue == 224 && (secondValue == 77 || secondValue==72)) money++;
	}
}
void TestCodeCount(int n) {
	while (1) {
		cout << n ;
		Sleep(500);
	}

}
int main() {
/*
	thread graphics(RollDices);
	thread input(GetInput);
	
	graphics.join();
	input.join();*/
	
	Core core;
	core.Test();

}