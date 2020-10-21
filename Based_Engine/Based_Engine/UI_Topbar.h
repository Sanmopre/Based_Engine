#include "imgui.h"
#include "UI.h"
#include "Module.h"

class Topbar : public UI
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