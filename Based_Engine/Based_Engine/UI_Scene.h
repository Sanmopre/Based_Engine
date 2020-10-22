#pragma once
#include "UI.h"

#include "imgui.h"

class Scene : public UI
{
public:
	Scene(bool isActive, Application* application);
	virtual ~Scene();

	void Update(float dt);
	void Cleanup();

	void OnResize(ImVec2 newWindowSize);

private:
	ImVec2 windowSize;
	ImVec2 imageSize;

};