#pragma once
#include "Module.h"

class TimeManager : public Module
{
public:

	TimeManager(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~TimeManager();
	bool Start();
	update_status PreUpdate();
	update_status Update(float dt);
	update_status PostUpdate();

public:

	bool paused = true;
};
