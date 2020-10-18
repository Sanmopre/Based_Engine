#pragma once
#include "Globals.h"

#include <vector>

class GUI
{
public:
	GUI(char* ui_name, bool is_enabled);
	virtual ~GUI();

	void Start();

	virtual void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();
	char* GetName();
	virtual void CleanUp();

public:	

	bool docked;
	bool enabled;

private:
	char* name;
};