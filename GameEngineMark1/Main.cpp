#include <iostream>
#include <d3d12.h>
#include "Engine/System.h"

int main() 
{
	std::cout << "hello world" << "  oh snap!!!!" << "Are you sure!?" << " YES I AM" << std::endl;

	System sys;
	if(sys.Initialize()) 
	{
		sys.Run();
	}

	sys.Shutdown();

	return 0;
}


/*
#include "Game.h";

void renderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 0.0, 0.0, 1.0);//clear red

	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	Game game = Game(argc, argv);
	
	// register callbacks
	//glutDisplayFunc(renderScene);
	game.Draw(renderScene);

	glutMainLoop();

	return 0;
}
*/