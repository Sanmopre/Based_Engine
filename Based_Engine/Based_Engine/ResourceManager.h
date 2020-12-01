#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "Module.h"

class ResourceManager : public Module
{
public:

	ResourceManager(Application* app, bool active = true);
	~ResourceManager();

	bool Start();
	update_status Update();
	bool CleanUp();

private:

};

#endif //__RESOURCES_H__ 
