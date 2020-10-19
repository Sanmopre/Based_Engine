#include "imgui.h"
#include "GUIclass.h"
#include "Module.h"

class Console : public GUI
{
public:
	Console(bool isActive, Application* application);
	virtual ~Console();


	void Update(float dt);


public:
	std::string inputlog;
};