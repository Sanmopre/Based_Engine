#include "UI.h"

class Folder;

class Assets : public UI
{
public:
	Assets(bool isActive, Application* application);
	virtual ~Assets();

	void Update(float dt);

	bool IterateFolder(Folder*);

	std::string selected = "";
};