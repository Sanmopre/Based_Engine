#include "imgui.h"
#include "UI.h"
#include "Module.h"

class Console : public UI
{
public:
	Console(bool isActive, Application* application);
	virtual ~Console();

	void Update(float dt);

public:

	std::string inputlog;
};