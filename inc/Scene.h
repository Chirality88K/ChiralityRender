#ifndef SCENE_H
#define SCENE_H
#include "RenderItem.h"
#include "Camera.h"
#include "Light.h"
#include <iostream>
#include <GLFW/glfw3.h>

#define MAXLIGHTNUM 16

class Scene
{
public:
	Scene(std::string name);
	bool Initialize();
	void BeginRender();
	virtual ~Scene();
protected:
	bool LinkShader()const;
	void UpdateScene();
private:
	bool WindowsInit();
	void CreateShaders();
	void CreateModels();
	void CreateRenderItems();
	void CreateLights();
	void CreateCamera();
	void OnResize(int width, int height);
	void OnMouseMove(double xpos, double ypos);
	void OnMouseScroll(double xoffset, double yoffset);
	void OnKeyDown();
private:
	std::string mName;
	std::unordered_map<std::string, std::shared_ptr<Model>> mModelList;
	std::unordered_map<std::string, std::unique_ptr<RenderItem>> mRenderItemList;
	Camera mCamera = Camera(glm::vec3(0, 0, 3), 90.0f, 0.0f);
	std::vector <std::unique_ptr<Light>> mLightList;
	std::unordered_map<std::string, std::shared_ptr<ADS_Material>> mMaterialList;
	std::unordered_map<std::string, Shader> mShaderList;
	unsigned int mProgramID;

	float mCurrentTime;
	float mLastTime;
	float mDeltaTime;
	double mLastMousePoint[2];
	float mTimeKnot;
	unsigned short mRenderTimesPerSecond;

	GLFWwindow* mWindow = nullptr;
	int mWidth = 1000;
	int mHeight = 800;
};

#endif
