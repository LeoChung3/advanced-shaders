#include "Lab7.hpp"
#include "scripts/include/camera.hpp"
#include "scripts/include/rotation.hpp"
#include <numeric> // For std::iota
#include <glm/gtc/matrix_transform.hpp>
#include "Grid.hpp"


// not finished lab 7 - task 5
Lab7::Lab7(GLFWWindowImpl& win) : Layer(win)
{
	m_mainScene.reset(new Scene);


	ShaderDescription phongShaderDesc;
	phongShaderDesc.type = ShaderType::rasterization;
	phongShaderDesc.vertexSrcPath = "./assets/Shaders/PhongVert.glsl";
	phongShaderDesc.fragmentSrcPath = "./assets/Shaders/Lab6/GpassFrag.glsl";
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

	std::shared_ptr<Material> modelMaterial;
	modelMaterial = std::make_shared<Material>(phongShader);
	modelMaterial->setValue("u_albedoMap", modelAlbedo);
	modelMaterial->setValue("u_specularMap", modelSpecular);
	//================================================================
	VBOLayout depthLayout = { {GL_FLOAT, 3} };

	ShaderDescription depthShaderDesc;
	depthShaderDesc.type = ShaderType::rasterization;
	depthShaderDesc.vertexSrcPath = "./assets/Shaders/Lab3/DepthVert.glsl";
	depthShaderDesc.fragmentSrcPath = "./assets/Shaders/Lab3/DepthFrag.glsl";

	std::shared_ptr<Shader> depthShader = std::make_shared<Shader>(depthShaderDesc);

	std::shared_ptr<VAO> modeldepthVAO;
	modeldepthVAO = std::make_shared<VAO>(Model.m_meshes[0].indices);
	modeldepthVAO->addVertexBuffer(Model.m_meshes[0].positions, depthLayout);


	std::shared_ptr<Material> depthShaderMaterial;
	depthShaderMaterial = std::make_shared<Material>(depthShader);

	//================================================================
	uint32_t actorNum = 30;
	Actor model;
	for (int i = 0; i < actorNum; i++)
	{
		model.geometry = modelVAO;
		model.depthGeometry = modeldepthVAO;
		model.material = modelMaterial;
		model.depthMaterial = depthShaderMaterial;
		model.translation = glm::vec3(Randomiser::uniformFloatBetween(-30.0, 30.0), -5.f, Randomiser::uniformFloatBetween(-30.0, 30.0));
		model.scale = glm::vec3(5.f, 5.0f, 5.f);
		model.recalc();
		modelIdx = m_mainScene->m_actors.size();
		m_mainScene->m_actors.push_back(model);
	}

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

	//std::shared_ptr<Texture> floorTexture;
	//floorTexture = std::make_shared<Texture>("./assets/models/whiteCube/letterCube.png");

	std::shared_ptr<Texture> heightmapping;
	heightmapping = std::make_shared<Texture>("./assets/textures/snowdon.png");

	std::shared_ptr<Texture> stonetexture;
	stonetexture = std::make_shared<Texture>("./assets/textures/stone.png");
	
	//std::shared_ptr<Texture> stoneTexture;
	//stoneTexture = std::make_shared<Texture>("./assets/textures/stone.png");

	ShaderDescription floorShaderdesc;
	floorShaderdesc.type = ShaderType::tessellationAndGeometry;

	floorShaderdesc.vertexSrcPath = "./assets/Shaders/Lab7/FloorVert.glsl";
	floorShaderdesc.controlSrcPath = "./assets/Shaders/Lab7/FloorTCS.glsl";
	floorShaderdesc.evaluationSrcPath = "./assets/Shaders/Lab7/FloorTES.glsl";
	floorShaderdesc.geometrySrcPath = "./assets/Shaders/Lab7/Floorgeo.glsl";
	floorShaderdesc.fragmentSrcPath = "./assets/Shaders/Lab7/FloorFrag.glsl";

	std::shared_ptr<Shader> FloorShader;
	FloorShader = std::make_shared<Shader>(floorShaderdesc);

	std::shared_ptr<Material> floorMaterial;
	floorMaterial = std::make_shared<Material>(FloorShader);
	floorMaterial->setValue("texture2", stonetexture);
	floorMaterial->setValue("u_heightMap", heightmapping);
	floorMaterial->setValue("scale", mapscale);
	floorMaterial->setPrimitive(GL_PATCHES);

	std::shared_ptr<VAO> floordepthVAO;
	floordepthVAO = std::make_shared<VAO>(grid->getIndices());
	std::vector<float> floorpositions = grid->getVertexPositions();
	floordepthVAO->addVertexBuffer(floorpositions, depthLayout);

	Actor Floor;
	Floor.geometry = floorVAO;
	Floor.depthGeometry = floordepthVAO;
	Floor.material = floorMaterial;
	Floor.depthMaterial = depthShaderMaterial;
	Floor.translation = glm::vec3(-50.f, -5.f, -50.f);
	Floor.recalc();
	floorIdx = m_mainScene->m_actors.size();
	m_mainScene->m_actors.push_back(Floor);

	//=====================================================================================================================
	// 													billboards
	//=====================================================================================================================


	std::vector<float> billbPositions;
	int numBillBoards = 6;
	std::vector<uint32_t> billboardIndices = {0, 1, 2, 3};
	for (int i = 0; i < numBillBoards; i++)
	{
		billbPositions.push_back(Randomiser::uniformFloatBetween(-50.0, 50.0));
		billbPositions.push_back(Randomiser::uniformFloatBetween(0.0, 8.0));
		billbPositions.push_back(Randomiser::uniformFloatBetween(-30.0, -35.0));

	}
	std::shared_ptr<VAO> treeVAO;

	treeVAO = std::make_shared<VAO>(billboardIndices);
	treeVAO->addVertexBuffer(billbPositions, { { GL_FLOAT, 3 } });

	std::shared_ptr<Texture> treetexture;
	treetexture = std::make_shared<Texture>("./assets/textures/tree.png");

	ShaderDescription treeShaderdesc;
	treeShaderdesc.type = ShaderType::geometry;
	treeShaderdesc.vertexSrcPath = "./assets/Shaders/Lab7/BillBoardVert.glsl";
	treeShaderdesc.geometrySrcPath = "./assets/Shaders/Lab7/BillBoardGeo.glsl";
	treeShaderdesc.fragmentSrcPath = "./assets/Shaders/Lab7/BillBoardFrag.glsl";
	std::shared_ptr<Shader> treeShader;
	treeShader = std::make_shared<Shader>(treeShaderdesc);

	std::shared_ptr<Material> treeMaterial;
	treeMaterial = std::make_shared<Material>(treeShader);
	treeMaterial->setPrimitive(GL_POINTS);
	treeMaterial->setValue("tree", treetexture);

	Actor billboard;
	billboard.geometry = treeVAO;
	billboard.material = treeMaterial;
	billboardIdx = m_mainScene->m_actors.size();
	m_mainScene->m_actors.push_back(billboard);

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


	//=============================================================================================================//
	//==============================			shadow pass				   ========================================//
	//=============================================================================================================//

	ShadowMapVars m_ShadowMapVars;
	DepthPass shadowPass;
	shadowPass.scene = m_mainScene;
	shadowPass.parseScene();
	shadowPass.target = std::make_shared<FBO>(glm::ivec2(4096,4096), prePassLayout);
	shadowPass.viewPort = { 0, 0, 4096, 4096 };

	dl.direction = glm::vec3(0.0f, -1.f, -0.001f);

	glm::vec3 lightPosition = (m_ShadowMapVars.centre - dl.direction) * m_ShadowMapVars.distanceAlongLightVector;
	glm::mat4 shadowView = glm::lookAt(lightPosition, m_ShadowMapVars.centre, m_ShadowMapVars.UP);
	shadowPass.camera.view = shadowView;

	shadowPass.camera.projection = glm::ortho(
		-m_ShadowMapVars.orthoSize,
		m_ShadowMapVars.orthoSize,
		-m_ShadowMapVars.orthoSize,
		m_ShadowMapVars.orthoSize,
		-m_ShadowMapVars.orthoSize/2,
		m_ShadowMapVars.orthoSize*2);

	shadowPass.UBOmanager.setCachedValue("b_LightCamera", "u_view", shadowPass.camera.view);
	shadowPass.UBOmanager.setCachedValue("b_LightCamera", "u_projection", shadowPass.camera.projection);


	modelMaterial->setValue("u_lightSpaceTransform", shadowPass.camera.projection * shadowPass.camera.view);
	//floorMaterial->setValue("u_lightSpaceTransform", shadowPass.camera.projection * shadowPass.camera.view);
	modelMaterial->setValue("u_shadowMap", shadowPass.target->getTarget(0));
	//floorMaterial->setValue("u_shadowMap", shadowPass.target->getTarget(0));

	m_mainRenderer.addDepthPass(shadowPass);


	//=============================================================================================================//
	//==============================			geometry pass			   ========================================//
	//=============================================================================================================//
	RenderPass mainPass;
	mainPass.scene = m_mainScene;
	mainPass.parseScene();
	//mainPass.target = std::make_shared<FBO>();
	mainPass.target = std::make_shared<FBO>(m_winRef.getSize(), GBuffer);
	mainPass.camera.projection = glm::perspective(45.f, m_winRef.getWidthf() / m_winRef.getHeightf(), 0.1f, 200.f);
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

	m_mainScene->m_actors.at(cameraIdx).attachScript<CameraScript>(mainPass.scene->m_actors.at(cameraIdx), m_winRef, glm::vec3(4.8f, 1.8f, 6.f), 2.f);
	m_mainScene->m_actors.at(modelIdx).attachScript<RotationScript>(mainPass.scene->m_actors.at(modelIdx), glm::vec3(0.0f, 0.5f, 0.0f), GLFW_KEY_1); // vec 3 rotates the model


	m_mainRenderer.addRenderPass(mainPass);


	//=============================================================================================================//
	//==============================			post processing				   ====================================//
	//=============================================================================================================//

	float width = m_winRef.getWidthf();
	float height = m_winRef.getHeightf();

	const std::vector<float> screenVertices = {
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		width, 0.0f, 0.0f, 1.0f, 1.0f,
		width, height, 0.0f, 1.0f, 0.0f,
		0.0f, height, 0.0f, 0.0f, 0.0f
	};

	const std::vector<uint32_t> screenIndices = { 0,1,2,2,3,0 };


	VBOLayout screenQuadLayout = {
		{GL_FLOAT, 3},  // vertiex position
		{GL_FLOAT, 2}   // UV cordinates
	};

	std::shared_ptr<VAO> screenQuadVao;

	screenQuadVao = std::make_shared<VAO>(screenIndices);
	screenQuadVao->addVertexBuffer(screenVertices, screenQuadLayout);


	//=============================================================================================================//
	//==============================			sky box pass			   ========================================//
	//=============================================================================================================//
	skyboxScene.reset(new Scene);
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
	boxIdx = skyboxScene->m_actors.size();
	skyboxScene->m_actors.push_back(skybox);

	RenderPass skypass;
	skypass.scene = skyboxScene;
	skypass.parseScene();
	skypass.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	skypass.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };
	skypass.camera.projection = glm::ortho(0.f, width, height, 0.f);


	skypass.UBOmanager.setCachedValue("b_camera", "u_view", mainPass.camera.view);
	skypass.UBOmanager.setCachedValue("b_camera", "u_projection", mainPass.camera.projection);
	skypass.UBOmanager.setCachedValue("b_camera", "u_viewPos", m_mainScene->m_actors.at(cameraIdx).translation);


	m_mainRenderer.addRenderPass(skypass);

	//=============================================================================================================//
	//==============================			light pass				   ========================================//
	//=============================================================================================================//


	m_screenScene.reset(new Scene);
	ShaderDescription lightpassdesc;
	lightpassdesc.type = ShaderType::rasterization;
	lightpassdesc.vertexSrcPath = "./assets/Shaders/lab5/lightVert.glsl";
	lightpassdesc.fragmentSrcPath = "./assets/Shaders/lab5/lightFrag.glsl";

	std::shared_ptr<Shader> lightpassShader;
	lightpassShader = std::make_shared<Shader>(lightpassdesc);

	std::shared_ptr<Material> lightpassMaterial;
	lightpassMaterial = std::make_shared<Material>(lightpassShader);
	lightpassMaterial->setValue("gBufferPosWs", mainPass.target->getTarget(0));
	lightpassMaterial->setValue("gBufferNormals", mainPass.target->getTarget(1));
	lightpassMaterial->setValue("gBufferDiffSpec", mainPass.target->getTarget(2));

	lightpassMaterial->setValue("skyboxstuff", skypass.target->getTarget(0));
	lightpassMaterial->setValue("depthstuff", mainPass.target->getTarget(3));

	lightpassMaterial->setValue("u_lightSpaceTransform", shadowPass.camera.projection* shadowPass.camera.view);
	lightpassMaterial->setValue("u_shadowMap", shadowPass.target->getTarget(0));

	Actor lightpassactor;
	lightpassactor.geometry = screenQuadVao;
	lightpassactor.material = lightpassMaterial;
	screenIdx = m_screenScene->m_actors.size();
	m_screenScene->m_actors.push_back(lightpassactor);

	RenderPass lightpass;
	lightpass.scene = m_screenScene;
	lightpass.parseScene();
	lightpass.target = std::make_shared<FBO>();
	lightpass.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };
	lightpass.camera.projection = glm::ortho(0.f, width, height, 0.f);

	lightpass.UBOmanager.setCachedValue("b_camera2D", "u_view2D", lightpass.camera.view);
	lightpass.UBOmanager.setCachedValue("b_camera2D", "u_projection2D", lightpass.camera.projection);
	lightpass.UBOmanager.setCachedValue("b_camera", "u_view", mainPass.camera.view);
	lightpass.UBOmanager.setCachedValue("b_camera", "u_projection", mainPass.camera.projection);
	lightpass.UBOmanager.setCachedValue("b_camera", "u_viewPos", m_mainScene->m_actors.at(cameraIdx).translation);
	lightpass.UBOmanager.setCachedValue("b_lights", "dLight.colour", m_mainScene->m_directionalLights.at(0).colour);
	lightpass.UBOmanager.setCachedValue("b_lights", "dLight.direction", m_mainScene->m_directionalLights.at(0).direction);

	m_mainRenderer.addRenderPass(lightpass);

}
void Lab7::onRender() const
{
	m_mainRenderer.render();
}
void Lab7::onUpdate(float timestep)
{
	auto& GPass = m_mainRenderer.getRenderPass(1);
	auto& skypass = m_mainRenderer.getRenderPass(2);
	auto& lightPass = m_mainRenderer.getRenderPass(3);

	ShadowMapVars m_ShadowMapVars;

	glm::vec3 lightPosition = (m_ShadowMapVars.centre - lightDirection) * m_ShadowMapVars.distanceAlongLightVector;
	glm::mat4 shadowView = glm::lookAt(lightPosition, m_ShadowMapVars.centre, m_ShadowMapVars.UP);

	auto& shadowPass = m_mainRenderer.getDepthPass(0);
	shadowPass.camera.view = shadowView;
	shadowPass.UBOmanager.setCachedValue("b_LightCamera", "u_view", shadowPass.camera.view);

	auto floormat = m_mainScene->m_actors.at(floorIdx).material;
	auto modelmat = m_mainScene->m_actors.at(modelIdx).material;

	
	floormat->setValue("tesslevel", TessLevel);
	floormat->setValue("scale", mapscale);
	floormat->setValue("scale2", mapscale);


	auto skyboxmat = skyboxScene->m_actors.at(0).material;
	skyboxmat->setValue("u_skyboxView", glm::mat4(glm::mat3(GPass.camera.view)));

	auto lightmat = m_screenScene->m_actors.at(screenIdx).material;
	lightmat->setValue("u_lightSpaceTransform", shadowPass.camera.projection * shadowPass.camera.view);

	if (m_wireFrame) m_mainRenderer.getRenderPass(0).drawInWireFrame = true;
	else m_mainRenderer.getRenderPass(0).drawInWireFrame = false;

	// Update scripts
	for (auto it = m_mainScene->m_actors.begin(); it != m_mainScene->m_actors.end(); ++it)
	{
		it->onUpdate(timestep);
	}




	// Update camera  and its position in UBO
	auto& camera = m_mainScene->m_actors.at(cameraIdx);

	GPass.camera.updateView(camera.transform);

	GPass.UBOmanager.setCachedValue("b_camera", "u_view", GPass.camera.view);
	GPass.UBOmanager.setCachedValue("b_camera", "u_viewPos", camera.translation);
	skypass.UBOmanager.setCachedValue("b_camera", "u_view", GPass.camera.view);
	skypass.UBOmanager.setCachedValue("b_camera", "u_viewPos", camera.translation);
	lightPass.UBOmanager.setCachedValue("b_camera", "u_view", GPass.camera.view);
	lightPass.UBOmanager.setCachedValue("b_camera", "u_viewPos", camera.translation);

}


void Lab7::onImGUIRender()
{
	float ms = 1000.0f / ImGui::GetIO().Framerate; ;  //milisecconds
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("GAMR3521");

	ImGui::Text("FPS %.3f ms/frame (%.1f FPS)", ms, ImGui::GetIO().Framerate);  // display FPS and ms
	ImGui::ColorEdit3("Colour", (float*)&m_colour); // changes colour of floor
	ImGui::Checkbox("Wireframe", &m_wireFrame); // changes if wireframe or not
	ImGui::SliderFloat("Gamma", &Gamma, 1.0f, 2.2f); // changes gamma correction
	ImGui::SliderFloat("tesselelation level", &TessLevel, 1.0f, 60.0f); // changes gamma correction
	ImGui::SliderFloat("map scale", &mapscale, 1.0f, 20.0f); // changes gamma correction

	GLuint textureID;
	ImVec2 imageSize = ImVec2(256, 256);
	ImVec2 uv0 = ImVec2(0.0f, 1.0f);
	ImVec2 uv1 = ImVec2(1.0f, 0.0f);

	if (ImGui::CollapsingHeader("mainpass", true))
	{
		textureID = m_mainRenderer.getRenderPass(0).target->getTarget(0)->getID();
		ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);
		ImGui::SameLine();
		textureID = m_mainRenderer.getRenderPass(0).target->getTarget(1)->getID();
		ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);

		textureID = m_mainRenderer.getRenderPass(0).target->getTarget(2)->getID();
		ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);
		ImGui::SameLine();

		textureID = m_mainRenderer.getRenderPass(2).target->getTarget(0)->getID();
		ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);
	}
	ImGui::SliderFloat3("light direction", (float*)&lightDirection, -0.5f, 0.0f);
	textureID = m_mainRenderer.getDepthPass(0).target->getTarget(0)->getID();
	ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);

	ImGui::End();
	ImGui::Render();
}

void Lab7::onKeyPressed(KeyPressedEvent& e)
{
	for (auto it = m_mainScene->m_actors.begin(); it != m_mainScene->m_actors.end(); ++it)
	{
		it->onKeyPress(e);
	}
}