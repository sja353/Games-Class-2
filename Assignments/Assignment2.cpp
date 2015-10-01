#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include "GameApp.h"
#include <stdlib.h>
//#include <iostream.h>

int main(int argc, char* argv[]){
	GameApp app;
	while (!app.UpdateAndRender()){}
	return 0;
}