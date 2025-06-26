#include "../inc/Scene.h"
#include "../inc/Log.h"

Scene::Scene(std::string name)
{
	mName = name;
	mCurrentTime = 0.0f;
	mLastTime = 0.0f;
	mDeltaTime = 0.0f;
	mLastMousePoint[0] = 0.0f;
	mLastMousePoint[1] = 0.0f;
	mTimeKnot = 0.0f;
	mRenderTimesPerSecond = 0;
}

bool Scene::Initialize()
{
	WindowsInit();
	CreateShaders();
	CreateModels();
	CreateRenderItems();
	CreateLights();
	CreateCamera();
	return true;
}

void Scene::BeginRender()
{
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(mWindow, double(mWidth) / 2, double(mHeight) / 2);
	while (!glfwWindowShouldClose(mWindow))
	{
		glUseProgram(mProgramID);
		double xpos, ypos;
		glfwGetCursorPos(mWindow, &xpos, &ypos);
		glfwGetWindowSize(mWindow, &mWidth, &mHeight);
		OnResize(mWidth, mHeight);
		OnMouseMove(xpos, ypos);

		OnKeyDown();
		UpdateScene();
		glClearColor(135.0f / 256.0f, 206.0f / 256.0f, 235.0f / 256.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		for (const auto &item : mRenderItemList)
		{
			item.second->Draw(mProgramID);
		}
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
}

Scene::~Scene()
{
	glfwTerminate();
}

bool Scene::LinkShader() const
{
	for (const auto &it : mShaderList)
	{
		it.second.AttachShader(mProgramID);
	}
	glLinkProgram(mProgramID);
	int success;
	char infoLog[512];
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(mProgramID, 512, NULL, infoLog);
		LOG_ERROR("Fail to link shaders: " + std::string(infoLog));
		return false;
	}
	LOG_INFO("Succeed to link shaders");
	return true;
}

void Scene::UpdateScene()
{
	mCamera.Update(mProgramID);
	for (const auto &light : mLightList)
	{
		light->AddLight(mProgramID);
	}
	mLastTime = mCurrentTime;
	mCurrentTime = glfwGetTime();
	mDeltaTime = mCurrentTime - mLastTime;
	++mRenderTimesPerSecond;
	if (mCurrentTime - mTimeKnot >= 1.0f)
	{
		std::string title = mName + " FPS: " + std::to_string(mRenderTimesPerSecond / (mCurrentTime - mTimeKnot));
		glfwSetWindowTitle(mWindow, title.substr(0, title.find('.') + 3).c_str());
		mRenderTimesPerSecond = 0;
		mTimeKnot = mCurrentTime;
	}
	float speed = 0.7f;
	for (const auto &item : mRenderItemList)
	{
		// item.second->UpdateRenderItem(glm::rotate(item.second->GetNowModelMatrix(), mDeltaTime * speed, glm::vec3(-1.0, 1.0, 0.0)));
	}
}

bool Scene::WindowsInit()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow =
		glfwCreateWindow(mWidth, mHeight, (mName + " FPS: 0.00").c_str(), nullptr, nullptr);
	glfwGetWindowSize(mWindow, &mWidth, &mHeight);
	if (mWindow == nullptr)
	{
		LOG_ERROR("Fail to create GLFW window");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(mWindow);
	// glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewInit();
	// glfwSetCursorPos(mWindow, mWidth / 2, mHeight / 2);
	mLastMousePoint[0] = double(mWidth) / 2;
	mLastMousePoint[1] = double(mHeight) / 2;
	LOG_INFO("Succeed to create GLFW window: " + mName);
	return true;
}

void Scene::CreateShaders()
{
	mProgramID = glCreateProgram();
	mShaderList["vertexshader"] = Shader(ShaderType::VERTEX_SHADER, "Shaders/testvertexshader.vs");
	mShaderList["fragment"] = Shader(ShaderType::FRAGMENT_SHADER, "Shaders/20250408fragmentshader.fs");
	LinkShader();
}

void Scene::CreateModels()
{
	std::shared_ptr<Model> test_cube_model = std::make_shared<Model>();
	test_cube_model->LoadModel("Resources/SashaDragon/SashaDragon.fbx");
	mModelList["Dragon"] = test_cube_model;
	LOG_INFO("Create model: Dragon");
}

void Scene::CreateRenderItems()
{
	std::string name_of_model;
	for (auto &model : mModelList)
	{
		name_of_model = model.first;
		mRenderItemList[name_of_model] = std::make_unique<RenderItem>(model.second);
		LOG_INFO("Create render item: " + name_of_model);
	}
	// mRenderItemList["cube1"] = std::make_unique<RenderItem>(cube_model, glm::mat4x4(1.0f));
	// mRenderItemList["cube2"] = std::make_unique<RenderItem>(cube_model, glm::translate(glm::mat4x4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
}

void Scene::CreateLights()
{
	// mLightList.push_back(std::make_unique<PointLight>("pLight0", glm::vec3(0.2f, 1.0f, 2.0f)));
	mLightList.push_back(std::make_unique<ParallelLight>("pLight0", glm::vec3(-1.0f, -1.0f, -1.0f)));
	LOG_INFO("Create a light");
	mLightList.back()->DebugInfo();
}

void Scene::CreateCamera()
{
	glm::vec3 pos = glm::vec3(5, 0, 0);
	constexpr float l = pos.length();
	float phi = asin(pos.y / l) * 180.0f / 3.14159265358979f;
	float theta = 0;
	if (std::abs(pos.x) > 1e-6)
	{
		theta = atan(pos.z / pos.x) * 180.0f / 3.14159265358979f;
		if (pos.x < 0)
		{
			theta += 180.0f;
		}
	}
	else if (pos.x * pos.z >= 0)
	{
		theta = 90.0f;
	}
	else
	{
		theta = -90.0f;
	}
	mCamera = Camera(pos, theta, phi);
}

void Scene::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
	mCamera.SetRaito((float)width / (float)height);
}

void Scene::OnMouseMove(double xpos, double ypos)
{
	if (glfwGetKey(mWindow, GLFW_KEY_1) == GLFW_PRESS)
	{
		mLastMousePoint[0] = xpos;
		mLastMousePoint[1] = ypos;
		return;
	}
	mCamera.TurnLeftRight((xpos - mLastMousePoint[0]) / mWidth * 360.0 * 0.5);
	mCamera.TurnUpDown((ypos - mLastMousePoint[1]) / mHeight * 180.0 * 0.5);
	// mLastMousePoint[0] = xpos;
	// mLastMousePoint[1] = ypos;
	glfwSetCursorPos(mWindow, double(mWidth) / 2, double(mHeight) / 2);
}

void Scene::OnMouseScroll(double xoffset, double yoffset)
{
	mCamera.Scale(-yoffset * 5.0);
}

void Scene::OnKeyDown()
{
	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(mWindow, true);
	}
	if (glfwGetKey(mWindow, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mCamera.Scale(-0.5f);
	}
	if (glfwGetKey(mWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mCamera.Scale(0.5f);
	}
	glm::vec3 X, Y, Z;
	mCamera.GetXYZDirection(X, Y, Z);
	float speed = 2.0f;
	if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		mCamera.Move(-Z * speed * mDeltaTime);
	}
	if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		mCamera.Move(Z * speed * mDeltaTime);
	}
	if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		mCamera.Move(-X * speed * mDeltaTime);
	}
	if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		mCamera.Move(X * speed * mDeltaTime);
	}
	if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		mCamera.Move(Y * speed * mDeltaTime);
	}
	if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		mCamera.Move(-Y * speed * mDeltaTime);
	}
}
