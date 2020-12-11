#include "UI.h"

class Assets : public UI
{
public:
	Assets(bool isActive, Application* application);
	virtual ~Assets();

	void Update(float dt);

public:

	std::string inputlog;
};