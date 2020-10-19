#include "imgui.h"
#include "GUIclass.h"
#include "Module.h"

class Scene : public GUI
{
public:
	Scene(bool isActive, Application* application);
	virtual ~Scene();

	void Update(float dt);
	void Cleanup();


private:
	ImVec2 windowSize;
	ImVec2 imageSize;

};