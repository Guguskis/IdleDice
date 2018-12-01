#include "pch.h"
#include "Core.h"


void Core::HandleGraphics(){
	//this method handles graphics.
	//TODO: should also execute calculations
	GraphicsLib graphics;
	while (mGameIsRunning) {
		graphics.DrawScreen();
	}
}

void Core::Test() {
	//This method contains children class Test methods to make it easier to debug
	GraphicsLib graphicController;
	graphicController.Test();

}

void Core::GameLoop(){
	//This method is used to run the game. It runs graphics and input on seperate threads
	thread graphicsWorker(&Core::HandleGraphics, this);
}


Core::Core(){
	GraphicsLib::SetData(mHeight, mWidth, mLayers, &mCurrLayer);
}