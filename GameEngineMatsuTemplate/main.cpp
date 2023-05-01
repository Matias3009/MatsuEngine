#include <iostream>
#include "EngineP.h"

//Include a headers files with scenes:

#include "Scene1.h"

//Create a Instance of the scene (class):

SceneOne* scene1 = new SceneOne();

int main() {
	scene1 = new SceneOne();

	//Create the Game Manager:
	GameManager	game(*scene1, 800, 600, "ElectroGame");
	//Set the Style of the window:
	game.setStyleWindow(5);
	//Create and Init a thread:
	thread t(game);
	t.join();
}
