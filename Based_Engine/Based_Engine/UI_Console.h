#include "UI.h"

class Console : public UI
{
public:
	Console(bool isActive, Application* application);
	virtual ~Console();

	void Update(float dt);

public:

	std::string inputlog;
};