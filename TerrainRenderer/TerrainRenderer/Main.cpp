#include "System.h"
#include <noise.h>
#include <random>
#include "noiseutils.h"
#include "HeightMap.h"
#include "EasyBMP.h"

using namespace std;
using namespace TerrainRenderer;

//!This terrain renderer will procedurally texture and render a large-scale terrain.
/*!*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	//Rendering the terrain mesh using the height map
	System* system;
	bool result;

	system = new System;
	if (!system)
	{
		return 0;
	}

	result = system->Initialize();
	if (result)
	{
		system->Run();
	}

	system->Shutdown();
	delete system;
	system = nullptr;

	return 0;
}