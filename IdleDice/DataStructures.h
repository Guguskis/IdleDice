#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "pch.h"
using namespace std;

struct Pixel {
	/*
		symb - displayed symbol
		bgCol, fgCol - background and symbol colors respectively
	*/
	char symb=' ';
	int bgCol = 0, fgCol = 7;
	bool update = true;
};

enum {
	col_black=0,
	col_darkBlue=1,
	col_darkGreen=2,
	col_darkCyan=3,
	col_darkRed=4,
	col_darkPink=5,
	col_darkYellow=6,
	col_darkWhite=7,
	col_grey=8,
	col_blue=9,
	col_green=10,
	col_cyan=11,
	col_red=12,
	col_pink=13,
	col_yellow=14,
	col_white=15,
};

struct Command {
	string type;
	int y, x, bgCol, fgCol, height, width;
	string text;
	vector<vector<Pixel>> *arr;
};
#endif