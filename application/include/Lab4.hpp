#pragma once
#include <DemonRenderer.hpp>
class Lab4 : public Layer
{
public:
	Lab4(GLFWWindowImpl& win);
private:
	void onRender() const override;
	void onUpdate(float timestep) override;
	void onKeyPressed(KeyPressedEvent& e) override;
	void onImGUIRender() override;
private:
	std::shared_ptr<Scene> m_mainScene;
	std::shared_ptr<Scene> m_screenScene;

	Renderer m_mainRenderer;
	// Actor positions for ease of use and to avoid additonal magic numbers
	size_t floorIdx;
	size_t modelIdx;
	size_t cameraIdx;
	size_t pointlightIdx;
	size_t boxIdx;
	size_t screenIdx;
	size_t edgeIdx;
	size_t buff_Idx;
	size_t blurIdx;
	//Gui

	glm::vec3 lightDirection = glm::vec3(0.0f, -1.f, -0.001f);
	glm::vec3 pointLightColour{ 0.0f, 1.0f, 0.0f };

	glm::vec3 m_colour{ 1.0f, 0.0f, 0.0f };
	bool m_wireFrame = false;
	bool haveEdge = false;
	float DoEdge = 1.0f;
	float Gamma = 2.2f;
	std::vector<float> skyboxVertices = {
		// positions          
		-100.f,  100.f, -100.f,
		-100.f, -100.f, -100.f,
		 100.f, -100.f, -100.f,
		 100.f, -100.f, -100.f,
		 100.f,  100.f, -100.f,
		-100.f,  100.f, -100.f,

		-100.f, -100.f,  100.f,
		-100.f, -100.f, -100.f,
		-100.f,  100.f, -100.f,
		-100.f,  100.f, -100.f,
		-100.f,  100.f,  100.f,
		-100.f, -100.f,  100.f,

		 100.f, -100.f, -100.f,
		 100.f, -100.f,  100.f,
		 100.f,  100.f,  100.f,
		 100.f,  100.f,  100.f,
		 100.f,  100.f, -100.f,
		 100.f, -100.f, -100.f,

		-100.f, -100.f,  100.f,
		-100.f,  100.f,  100.f,
		 100.f,  100.f,  100.f,
		 100.f,  100.f,  100.f,
		 100.f, -100.f,  100.f,
		-100.f, -100.f,  100.f,

		-100.f,  100.f, -100.f,
		 100.f,  100.f, -100.f,
		 100.f,  100.f,  100.f,
		 100.f,  100.f,  100.f,
		-100.f,  100.f,  100.f,
		-100.f,  100.f, -100.f,

		-100.f, -100.f, -100.f,
		-100.f, -100.f,  100.f,
		 100.f, -100.f, -100.f,
		 100.f, -100.f, -100.f,
		-100.f, -100.f,  100.f,
		 100.f, -100.f,  100.f
	};
	FBOLayout typicalLayout =
	{
		{AttachmentType::ColourHDR, true},
		{AttachmentType::Depth, false}
	};
	FBOLayout colAndDepthLayout =
	{
		{AttachmentType::ColourHDR, true},
		{AttachmentType::Depth, true}
	};
	FBOLayout prePassLayout =
	{
		{AttachmentType::Depth, true}
	};


	struct ShadowMapVars
	{
		glm::vec3 centre;
		float distanceAlongLightVector;
		float orthoSize;
		const glm::vec3 UP = glm::vec3(0.0, 1.0f, 0.0f);

		ShadowMapVars(glm::vec3 c = glm::vec3(0.0), float dist = 60.0f) : centre(c), distanceAlongLightVector(dist)
		{
			orthoSize = distanceAlongLightVector * 0.75;
		};
	};
};