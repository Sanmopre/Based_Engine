#pragma once
#include "UI.h"

class Topbar : public UI
{
public:

	Topbar(bool isActive, Application* application);
	virtual ~Topbar();

	void Start();
	void Update(float dt);


public:

	bool showcase;
};