#include "Lab1.hpp"
#include "scripts/include/camera.hpp"
#include "scripts/include/rotation.hpp"
#include <numeric> // For std::iota
#include <glm/gtc/matrix_transform.hpp>
#include "Grid.hpp"

Lab1::Lab1(GLFWWindowImpl& win) : Layer(win)
{
	m_mainScene.reset(new Scene);
	ShaderDescription phongShaderDesc;
	phongShaderDesc.type = ShaderType::rasterization;
	phongShaderDesc.vertexSrcPath = "./assets/Shaders/PhongVert.glsl";
	phongShaderDesc.fragmentSrcPath = "./assets/Shaders/PhongFrag.glsl";

	std::shared_ptr<Shader> phongShader;
	phongShader = std::make_shared<Shader>(phongShaderDesc);
	VBOLayout cubeLayout = {
		{GL_FLOAT, 3},  // position
		{GL_FLOAT, 3},  // normal
		{GL_FLOAT, 2}   // UV
	};
	//"H:\module2\assets\models\Vampire\vampire.obj"

	Model Model("./assets/models/Vampire/vampire.obj");
	std::shared_ptr<VAO> modelVAO;
	modelVAO = std::make_shared<VAO>(Model.m_meshes[0].indices);
	modelVAO->addVertexBuffer(Model.m_meshes[0].vertices, cubeLayout);


	//"H:\module2\assets\models\Vampire\textures\diffuse.png"
	std::shared_ptr<Texture> modelAlbedo;
	modelAlbedo = std::make_shared<Texture>("./assets/models/Vampire/textures/diffuse.png");

	//"H:\module2\assets\models\Vampire\textures\specular.png"
	std::shared_ptr<Texture> modelSpecular;
	modelSpecular = std::make_shared<Texture>("./assets/models/Vampire/textures/specular.png");

	//"H:\module2\assets\models\Vampire\textures\normal.png"
	std::shared_ptr<Texture> modelNormal;
	modelNormal = std::make_shared<Texture>("./assets/models/Vampire/textures/normal.png");

	//std::shared_ptr<Material> modelMaterial;
	//modelMaterial = std::make_shared<Material>(phongShader);
	//modelMaterial->setValue("u_albedoMap", cubeTexture);
	//modelMaterial->setValue("u_albedo", m_colour);

	std::shared_ptr<Material> modelMaterial;
	modelMaterial = std::make_shared<Material>(phongShader);
	modelMaterial->setValue("u_albedoMap", modelAlbedo);
	modelMaterial->setValue("u_specularMap", modelSpecular);
	//modelMaterial->setValue("u_normalMap", modelNormal);

	//Actor cube;
	//cube.geometry = cubeVAO;
	//cube.material = cubeMaterial;
	//cube.translation = glm::vec3(0.f, 1.f, -6.f);
	//cube.recalc();
	//modelIdx = m_mainScene->m_actors.size();
	//m_mainScene->m_actors.push_back(cube);

	Actor model;
	model.geometry = modelVAO;
	model.material = modelMaterial;
	model.translation = glm::vec3(0.f, -5.f, -11.f);
	model.scale = glm::vec3(5.f, 5.0f, 5.f);
	model.recalc();
	modelIdx = m_mainScene->m_actors.size();
	m_mainScene->m_actors.push_back(model);

	DirectionalLight dl;
	dl.direction = glm::normalize(glm::vec3(-0.2, -1.0, -0.5));
	m_mainScene->m_directionalLights.push_back(dl);

	Actor camera;
	cameraIdx = m_mainScene->m_actors.size();
	m_mainScene->m_actors.push_back(camera);





//=====================================================================================================================
// 													FLOOR
//=====================================================================================================================
	std::shared_ptr<Grid> grid = std::make_shared<Grid>();
	std::vector<float> floor_vertices = grid->getVertices();
	std::vector<unsigned int> floor_indices = grid->getIndices();

	VBOLayout floorLayout = {
		{GL_FLOAT, 3},  // position
		{GL_FLOAT, 2}   // UV
	};

	std::shared_ptr<VAO> floorVAO;
	floorVAO = std::make_shared<VAO>(floor_indices);
	floorVAO->addVertexBuffer(floor_vertices, floorLayout);

	std::shared_ptr<Texture> floorTexture;
	floorTexture = std::make_shared<Texture>("./assets/models/whiteCube/letterCube.png");

	ShaderDescription floorShaderdesc;
	floorShaderdesc.type = ShaderType::rasterization;
	floorShaderdesc.vertexSrcPath = "./assets/Shaders/FloorVert.glsl";
	floorShaderdesc.fragmentSrcPath = "./assets/Shaders/FloorFrag.glsl";

	std::shared_ptr<Shader> FloorShader;
	FloorShader = std::make_shared<Shader>(floorShaderdesc);

	std::shared_ptr<Material> floorMaterial;
	floorMaterial = std::make_shared<Material>(FloorShader);
	floorMaterial->setValue("u_albedoMap", floorTexture);
	floorMaterial->setValue("u_albedo", m_colour);


	Actor Floor;
	Floor.geometry = floorVAO;
	Floor.material = floorMaterial;

	Floor.translation = glm::vec3(-50.f, -5.f, -50.f);
	Floor.recalc();
	floorIdx = m_mainScene->m_actors.size();
	m_mainScene->m_actors.push_back(Floor);


//=============================================================================================================
										//Point lights
//=============================================================================================================


	PointLight pointlight;
	uint32_t numPointLights = 10;
	for (int i = 0; i < numPointLights; i++)
	{
		//pointlight.colour = glm::vec3(Randomiser::uniformFloatBetween(0.,1.), Randomiser::uniformFloatBetween(0.,1.), Randomiser::uniformFloatBetween(0.,1.));
		pointlight.colour = pointLightColour;
		pointlight.position = glm::vec3(Randomiser::uniformFloatBetween(-30.0, 30.0), 2.0f, Randomiser::uniformFloatBetween(-30.0, 30.0));
		pointlight.constants = glm::vec3(1.0f, 0.7f, 0.02f);
		m_mainScene->m_pointLights.push_back(pointlight);

	}
//=============================================================================================================
// 	   //skybox
//=============================================================================================================

	std::vector<uint32_t> skyboxIndices(skyboxVertices.size() / 3);
	std::iota(skyboxIndices.begin(), skyboxIndices.end(), 0);

	std::shared_ptr<VAO> skyboxVAO;
	skyboxVAO = std::make_shared<VAO>(skyboxIndices);
	skyboxVAO->addVertexBuffer(skyboxVertices, { {GL_FLOAT, 3} });

	std::array<const char*, 6> cubeMapPaths = {
		"./assets/textures/SkyBox/right.png",
		"./assets/textures/SkyBox/left.png",
		"./assets/textures/SkyBox/top.png",
		"./assets/textures/SkyBox/bottom.png",
		"./assets/textures/SkyBox/back.png",
		"./assets/textures/SkyBox/front.png"

	};

	std::shared_ptr<CubeMap> cubeMap;
	cubeMap = std::make_shared<CubeMap>(cubeMapPaths, false, false);

	ShaderDescription boxShaderdesc;
	boxShaderdesc.type = ShaderType::rasterization;
	boxShaderdesc.vertexSrcPath = "./assets/Shaders/BoxVert.glsl";
	boxShaderdesc.fragmentSrcPath = "./assets/Shaders/BoxFrag.glsl";

	std::shared_ptr<Shader> BoxShader;
	BoxShader = std::make_shared<Shader>(boxShaderdesc);



	std::shared_ptr<Material> boxMaterial;
	boxMaterial = std::make_shared<Material>(BoxShader);
	boxMaterial->setValue("u_skybox", cubeMap);

	Actor skybox;
	skybox.geometry = skyboxVAO;
	skybox.material = boxMaterial;
	boxMaterial->setValue("u_skyboxView", glm::inverse(skybox.transform));
	skybox.recalc();
	boxIdx = m_mainScene->m_actors.size();
	m_mainScene->m_actors.push_back(skybox);

//=============================================================================================================


	/*************************
	*  Main Render PAss
	**************************/
	RenderPass mainPass;
	mainPass.scene = m_mainScene;
	mainPass.parseScene();
	mainPass.target = std::make_shared<FBO>();
	mainPass.camera.projection = glm::perspective(45.f, m_winRef.getWidthf() / m_winRef.getHeightf(), 0.1f, 1000.f);
	mainPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	mainPass.camera.updateView(m_mainScene->m_actors.at(cameraIdx).transform);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_view", mainPass.camera.view);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_projection", mainPass.camera.projection);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_viewPos", m_mainScene->m_actors.at(cameraIdx).translation);
	mainPass.UBOmanager.setCachedValue("b_lights", "dLight.colour", m_mainScene->m_directionalLights.at(0).colour);
	mainPass.UBOmanager.setCachedValue("b_lights", "dLight.direction", m_mainScene->m_directionalLights.at(0).direction);

	for (int i = 0; i < numPointLights; i++)
	{
		mainPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].colour", m_mainScene->m_pointLights.at(i).colour);
		mainPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].position", m_mainScene->m_pointLights.at(i).position);
		mainPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].constants", m_mainScene->m_pointLights.at(i).constants);
		
	}

	m_mainScene->m_actors.at(cameraIdx).attachScript<CameraScript>(mainPass.scene->m_actors.at(cameraIdx), m_winRef, glm::vec3(1.6f, 0.6f, 2.f), 0.5f);
	m_mainScene->m_actors.at(modelIdx).attachScript<RotationScript>(mainPass.scene->m_actors.at(modelIdx), glm::vec3(0.0f, 0.5f, 0.0f), GLFW_KEY_1); // vec 3 rotates the model


	m_mainRenderer.addRenderPass(mainPass);
}
void Lab1::onRender() const
{
	m_mainRenderer.render();
}
void Lab1::onUpdate(float timestep)
{
	// per frame uniforms
	auto floormat = m_mainScene->m_actors.at(floorIdx).material;
	floormat->setValue("u_albedo", m_colour);

	if (m_wireFrame) m_mainRenderer.getRenderPass(0).drawInWireFrame = true;
	else m_mainRenderer.getRenderPass(0).drawInWireFrame = false;

	// Update scripts
	for (auto it = m_mainScene->m_actors.begin(); it != m_mainScene->m_actors.end(); ++it)
	{
		it->onUpdate(timestep);
	}

	// Update camera  and its position in UBO
	auto& camera = m_mainScene->m_actors.at(cameraIdx);
	auto& pass = m_mainRenderer.getRenderPass(0);
	pass.camera.updateView(camera.transform);
	pass.UBOmanager.setCachedValue("b_camera", "u_view", pass.camera.view);
	pass.UBOmanager.setCachedValue("b_camera", "u_viewPos", camera.translation);

	//auto& skybox = m_mainScene->m_actors.at(boxIdx);
	//skybox.material->setValue("u_skyboxView", glm::mat4(glm::mat3(pass.camera.view)));

}


void Lab1::onImGUIRender()
{
	float ms = 1000.0f / ImGui::GetIO().Framerate; ;  //milisecconds
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("GAMR3521");

	ImGui::Text("FPS %.3f ms/frame (%.1f FPS)", ms, ImGui::GetIO().Framerate);  // display FPS and ms
	//ImGui::ColorEdit3("pointlight", (float*)&pointLightColour);

	ImGui::ColorEdit3("Colour", (float*)&m_colour);
	ImGui::Checkbox("Wireframe", &m_wireFrame);

	ImGui::End();
	ImGui::Render();

	

}

void Lab1::onKeyPressed(KeyPressedEvent& e)
{
	for (auto it = m_mainScene->m_actors.begin(); it != m_mainScene->m_actors.end(); ++it)
	{
		it->onKeyPress(e);
	}
	/*if (m_wireFrame == false)
	{
		m_wireFrame = true;

	}
	if (m_wireFrame == true)
	{
		m_wireFrame = false;

	}*/
}


