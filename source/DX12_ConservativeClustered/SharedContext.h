#pragma once
class KGraphicsDevice;
class Logging;

struct SharedContext 
{
	KGraphicsDevice* gfx_device;
	Logging* log;
};

extern SharedContext shared_context;