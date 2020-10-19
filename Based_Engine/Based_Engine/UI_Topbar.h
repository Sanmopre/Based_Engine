#include "imgui.h"
#include "GUIclass.h"
#include "Module.h"

class Topbar : public GUI
{
public:
	Topbar(bool isActive, Application* application);
	virtual ~Topbar();

	void Start();
	void Update(float dt);


public:
	std::string inputlog;

	bool showcase;
};