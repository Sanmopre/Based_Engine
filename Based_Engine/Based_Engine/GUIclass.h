#pragma once
#include "Globals.h"

#include <vector>

class GUI
{
public:
	GUI(char* ui_name, bool is_enabled);
	virtual ~GUI();

	void Start();

	void PreUpdate();
	void Update();
	void PostUpdate();
	char* GetName();
	void CleanUp();

public:	

	bool docked;
	bool enabled;

private:
	char* name;
};