#include "Lab4.hpp"
#include "scripts/include/camera.hpp"
#include "scripts/include/rotation.hpp"
#include <numeric> // For std::iota
#include <glm/gtc/matrix_transform.hpp>
#include "Grid.hpp"

Lab4::Lab4(GLFWWindowImpl& win) : Layer(win)
{
	m_mainScene.reset(new Scene);


	ShaderDescription phongShaderDesc;
	phongShaderDesc.type = ShaderType::rasterization;
	phongShaderDesc.vertexSrcPath = "./assets/Shaders/PhongVert.glsl";
	phongShaderDesc.fragmentSrcPath = "./assets/Shaders/Lab4/PhongFrag.glsl";

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
	modelMaterial->setValue("u_normalMap", modelNormal);

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

	std::shared_ptr<Texture> floorTexture;
	floorTexture = std::make_shared<Texture>("./assets/models/whiteCube/letterCube.png");

	ShaderDescription floorShaderdesc;
	floorShaderdesc.type = ShaderType::rasterization;
	floorShaderdesc.vertexSrcPath = "./assets/Shaders/FloorVert.glsl";
	floorShaderdesc.fragmentSrcPath = "./assets/Shaders/Lab4/FloorFrag.glsl";

	std::shared_ptr<Shader> FloorShader;
	FloorShader = std::make_shared<Shader>(floorShaderdesc);

	std::shared_ptr<Material> floorMaterial;
	floorMaterial = std::make_shared<Material>(FloorShader);
	floorMaterial->setValue("u_albedoMap", floorTexture);
	floorMaterial->setValue("u_albedo", m_colour);

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
	//============================================================================================================

	//=============================================================================================================
	//=============================================================================================================

	//================================================================
	//================================================================

	/*DepthPass prePass;
	prePass.scene = m_mainScene;
	prePass.parseScene();
	prePass.target = std::make_shared<FBO>(m_winRef.getSizef(), prePassLayout);
	prePass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	m_mainRenderer.addDepthPass(prePass);*/

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

	m_mainRenderer.addDepthPass(shadowPass);




	DepthPass ShadowMainPass;
	ShadowMainPass.scene = m_mainScene;
	ShadowMainPass.parseScene();
	ShadowMainPass.target = std::make_shared<FBO>(glm::ivec2(4096, 4096), prePassLayout);
	ShadowMainPass.viewPort = { 0, 0, 4096, 4096 };

	modelMaterial->setValue("u_lightSpaceTransform", shadowPass.camera.projection * shadowPass.camera.view);
	floorMaterial->setValue("u_lightSpaceTransform", shadowPass.camera.projection * shadowPass.camera.view);
	modelMaterial->setValue("u_shadowMap", shadowPass.target->getTarget(0));

	floorMaterial->setValue("u_shadowMap", shadowPass.target->getTarget(0));

	dl.direction = glm::vec3(0.0f, -1.f, -0.001f);


	ShadowMainPass.camera.view = shadowView;

	ShadowMainPass.camera.projection = glm::ortho(-m_ShadowMapVars.orthoSize,
		m_ShadowMapVars.orthoSize,
		-m_ShadowMapVars.orthoSize,
		m_ShadowMapVars.orthoSize,
		-m_ShadowMapVars.orthoSize / 2,
		m_ShadowMapVars.orthoSize * 2);

	ShadowMainPass.camera.projection = glm::perspective(45.f, m_winRef.getWidthf() / m_winRef.getHeightf(), 0.1f, 200.f);
	ShadowMainPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	ShadowMainPass.camera.updateView(m_mainScene->m_actors.at(cameraIdx).transform);

	ShadowMainPass.UBOmanager.setCachedValue("b_LightCamera", "u_view", shadowPass.camera.view);
	ShadowMainPass.UBOmanager.setCachedValue("b_LightCamera", "u_projection", shadowPass.camera.projection);

	//ShadowMainPass.UBOmanager.setCachedValue("b_camera", "u_view", ShadowMainPass.camera.view);
	//ShadowMainPass.UBOmanager.setCachedValue("b_camera", "u_projection", ShadowMainPass.camera.projection);
	//ShadowMainPass.UBOmanager.setCachedValue("b_camera", "u_viewPos", m_mainScene->m_actors.at(cameraIdx).translation);

	m_mainRenderer.addDepthPass(ShadowMainPass);

	/*************************
	*  Main Render PAss
	**************************/
	RenderPass mainPass;
	mainPass.scene = m_mainScene;
	mainPass.parseScene();
	//mainPass.target = std::make_shared<FBO>();
	mainPass.target = std::make_shared<FBO>(m_winRef.getSize(), colAndDepthLayout);
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

	//=============================================================================================================
	// render pass	
	//=============================================================================================================

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

	//lab2

	/*//=============================================================================================================
	// inverse render pass
	//=============================================================================================================
	m_screenScene.reset(new Scene);
	ShaderDescription invertdesc;
	invertdesc.type = ShaderType::rasterization;
	invertdesc.vertexSrcPath = "./assets/Shaders/InvertVert.glsl";
	invertdesc.fragmentSrcPath = "./assets/Shaders/InvertFrag.glsl";

	std::shared_ptr<Shader> invertShader;
	invertShader = std::make_shared<Shader>(invertdesc);

	std::shared_ptr<Material> invertMaterial;
	invertMaterial = std::make_shared<Material>(invertShader);
	invertMaterial->setValue("u_originalTexture", mainPass.target->getTarget(0));

	Actor invert;
	invert.geometry = screenQuadVao;
	invert.material = invertMaterial;
	invert.recalc();
	screenIdx = m_screenScene->m_actors.size();
	m_screenScene->m_actors.push_back(invert);


	RenderPass postProcessing;
	postProcessing.scene = m_screenScene;
	postProcessing.parseScene();
	postProcessing.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	postProcessing.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };

	postProcessing.camera.projection = glm::ortho(0.f, width, height, 0.f);
	postProcessing.UBOmanager.setCachedValue("b_camera2D", "u_view", postProcessing.camera.view);
	postProcessing.UBOmanager.setCachedValue("b_camera2D", "u_projection", postProcessing.camera.projection);


	//=========================================================================================================
	// edge shader
	//=========================================================================================================

	m_screenScene.reset(new Scene);
	ShaderDescription edgedesc;
	edgedesc.type = ShaderType::rasterization;
	edgedesc.vertexSrcPath = "./assets/Shaders/EdgeVert.glsl";
	edgedesc.fragmentSrcPath = "./assets/Shaders/EdgeFrag.glsl";

	std::shared_ptr<Shader> edgeShader;
	edgeShader = std::make_shared<Shader>(edgedesc);

	std::shared_ptr<Material> edgeMaterial;
	edgeMaterial = std::make_shared<Material>(edgeShader);
	edgeMaterial->setValue("u_originalTexture", mainPass.target->getTarget(0));
	edgeMaterial->setValue("a", DoEdge);

	Actor edge;
	edge.geometry = screenQuadVao;
	edge.material = edgeMaterial;
	edgeIdx = m_screenScene->m_actors.size();
	m_screenScene->m_actors.push_back(edge);


	RenderPass edgeProcessing;
	edgeProcessing.scene = m_screenScene;
	edgeProcessing.parseScene();
	edgeProcessing.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	edgeProcessing.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };

	edgeProcessing.camera.projection = glm::ortho(0.f, width, height, 0.f);
	edgeProcessing.UBOmanager.setCachedValue("b_camera2D", "u_view", edgeProcessing.camera.view);
	edgeProcessing.UBOmanager.setCachedValue("b_camera2D", "u_projection", edgeProcessing.camera.projection);*/


	//lab3
	//===================================================================================================================================
	 //BLUR
	//===================================================================================================================================
	m_screenScene.reset(new Scene);
	ShaderDescription blurdesc;
	blurdesc.type = ShaderType::rasterization;
	blurdesc.vertexSrcPath = "./assets/Shaders/Lab2/BlurVert.glsl";
	blurdesc.fragmentSrcPath = "./assets/Shaders/Lab2/BlurFrag.glsl";

	std::shared_ptr<Shader> blurShader;
	blurShader = std::make_shared<Shader>(blurdesc);

	std::shared_ptr<Material> blurMaterial;
	blurMaterial = std::make_shared<Material>(blurShader);
	blurMaterial->setValue("u_originalTexture", mainPass.target->getTarget(0));

	Actor blur;
	blur.geometry = screenQuadVao;
	blur.material = blurMaterial;
	blurIdx = m_screenScene->m_actors.size();

	m_screenScene->m_actors.push_back(blur);


	RenderPass blurProcessing;
	blurProcessing.scene = m_screenScene;
	blurProcessing.parseScene();
	blurProcessing.target = std::make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	blurProcessing.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };

	blurProcessing.camera.projection = glm::ortho(0.f, width, height, 0.f);
	blurProcessing.UBOmanager.setCachedValue("b_camera2D", "u_view", blurProcessing.camera.view);
	blurProcessing.UBOmanager.setCachedValue("b_camera2D", "u_projection", blurProcessing.camera.projection);
	//=============================================================================================================
	//depth buffer
		//=============================================================================================================
	m_screenScene.reset(new Scene);

	ShaderDescription buffdesc;
	buffdesc.type = ShaderType::rasterization;
	buffdesc.vertexSrcPath = "./assets/Shaders/Lab3/BuffVert.glsl";
	buffdesc.fragmentSrcPath = "./assets/Shaders/Lab3/BuffFrag.glsl";

	std::shared_ptr<Shader> buffshader;
	buffshader = std::make_shared<Shader>(buffdesc);

	std::shared_ptr<Material> buffQuadMaterial;
	buffQuadMaterial = std::make_shared<Material>(buffshader);
	buffQuadMaterial->setValue("u_originalTexture", mainPass.target->getTarget(1));



	Actor buff;
	buff.geometry = screenQuadVao;
	buff.material = buffQuadMaterial;
	buff_Idx = m_screenScene->m_actors.size();
	m_screenScene->m_actors.push_back(buff);


	RenderPass buffPass;
	buffPass.scene = m_screenScene;
	buffPass.parseScene();
	buffPass.target = std::make_shared<FBO>(m_winRef.getSize(), colAndDepthLayout);
	buffPass.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };
	buffPass.camera.projection = glm::perspective(45.f, m_winRef.getWidthf() / m_winRef.getHeightf(), 0.1f, 1000.f);
	buffPass.UBOmanager.setCachedValue("b_camera2D", "u_view", buffPass.camera.view);
	buffPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", buffPass.camera.projection);

	//=============================================================================================================
	//  Fog
		//=============================================================================================================
	m_screenScene.reset(new Scene);

	ShaderDescription fogDesc;
	fogDesc.type = ShaderType::rasterization;
	fogDesc.vertexSrcPath = "./assets/Shaders/Lab3/FogVert.glsl";
	fogDesc.fragmentSrcPath = "./assets/Shaders/Lab3/FogFrag.glsl";

	std::shared_ptr<Shader> fogshader;
	fogshader = std::make_shared<Shader>(fogDesc);

	std::shared_ptr<Material> FogQuadMaterial;
	FogQuadMaterial = std::make_shared<Material>(fogshader);
	FogQuadMaterial->setValue("u_originalTexture", mainPass.target->getTarget(0));
	FogQuadMaterial->setValue("u_originalTexture2", mainPass.target->getTarget(1));

	Actor Fog;
	Fog.geometry = screenQuadVao;
	Fog.material = FogQuadMaterial;
	//buff_Idx = m_screenScene->m_actors.size();
	m_screenScene->m_actors.push_back(Fog);


	RenderPass FogPass;
	FogPass.scene = m_screenScene;
	FogPass.parseScene();
	FogPass.target = std::make_shared<FBO>(m_winRef.getSize(), colAndDepthLayout);
	FogPass.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };
	FogPass.camera.projection = glm::perspective(45.f, m_winRef.getWidthf() / m_winRef.getHeightf(), 0.1f, 1000.f);
	FogPass.UBOmanager.setCachedValue("b_camera2D", "u_view", FogPass.camera.view);
	FogPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", FogPass.camera.projection);
	//=============================================================================================================
	// depth of view
	//=============================================================================================================
	m_screenScene.reset(new Scene);

	ShaderDescription depthViewDesc;
	depthViewDesc.type = ShaderType::rasterization;
	depthViewDesc.vertexSrcPath = "./assets/Shaders/Lab3/DepthViewVert.glsl";
	depthViewDesc.fragmentSrcPath = "./assets/Shaders/Lab3/DepthViewFrag.glsl";

	std::shared_ptr<Shader> depthViewshader;
	depthViewshader = std::make_shared<Shader>(depthViewDesc);

	std::shared_ptr<Material> depthViewQuadMaterial;
	depthViewQuadMaterial = std::make_shared<Material>(depthViewshader);
	depthViewQuadMaterial->setValue("u_originalTexture", mainPass.target->getTarget(0));
	depthViewQuadMaterial->setValue("blurTexture", blurProcessing.target->getTarget(0));
	depthViewQuadMaterial->setValue("DepthTexture", mainPass.target->getTarget(1));


	Actor DepthView;
	DepthView.geometry = screenQuadVao;
	DepthView.material = depthViewQuadMaterial;
	//buff_Idx = m_screenScene->m_actors.size();
	m_screenScene->m_actors.push_back(DepthView);


	RenderPass DepthViewPass;
	DepthViewPass.scene = m_screenScene;
	DepthViewPass.parseScene();
	DepthViewPass.target = std::make_shared<FBO>(m_winRef.getSize(), colAndDepthLayout);
	DepthViewPass.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };
	DepthViewPass.camera.projection = glm::perspective(45.f, m_winRef.getWidthf() / m_winRef.getHeightf(), 0.1f, 1000.f);
	DepthViewPass.UBOmanager.setCachedValue("b_camera2D", "u_view", DepthViewPass.camera.view);
	DepthViewPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", DepthViewPass.camera.projection);

	//=============================================================================================================

	//=============================================================================================================
	//screen render pass
	//=============================================================================================================
	m_screenScene.reset(new Scene);
	ShaderDescription Screendesc;
	Screendesc.type = ShaderType::rasterization;
	Screendesc.vertexSrcPath = "./assets/Shaders/Screenvert.glsl";
	Screendesc.fragmentSrcPath = "./assets/Shaders/ScreenFrag.glsl";

	std::shared_ptr<Shader> ScreenShader;
	ScreenShader = std::make_shared<Shader>(Screendesc);

	std::shared_ptr<Material> screenQuadMaterial;
	screenQuadMaterial = std::make_shared<Material>(ScreenShader);
	screenQuadMaterial->setValue("u_originalTexture", mainPass.target->getTarget(0));
	screenQuadMaterial->setValue("Gamma", Gamma);


	Actor Screen;
	Screen.geometry = screenQuadVao;
	Screen.material = screenQuadMaterial;
	screenIdx = m_screenScene->m_actors.size();
	m_screenScene->m_actors.push_back(Screen);

	RenderPass screenPass;
	screenPass.scene = m_screenScene;
	screenPass.parseScene();
	screenPass.target = std::make_shared<FBO>();
	screenPass.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };

	screenPass.camera.projection = glm::ortho(0.f, width, height, 0.f);
	screenPass.UBOmanager.setCachedValue("b_camera2D", "u_view", screenPass.camera.view);
	screenPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", screenPass.camera.projection);


	//=============================================================================================================
	//=============================================================================================================
	//=============================================================================================================



	// //lab2
	//m_mainRenderer.addRenderPass(postProcessing);
	//m_mainRenderer.addRenderPass(edgeProcessing);
	// //lab3


	m_mainRenderer.addRenderPass(blurProcessing);
	m_mainRenderer.addRenderPass(buffPass);
	m_mainRenderer.addRenderPass(FogPass);
	m_mainRenderer.addRenderPass(DepthViewPass);

	m_mainRenderer.addRenderPass(screenPass);
	//=============================================================================================================
}
void Lab4::onRender() const
{
	m_mainRenderer.render();
}
void Lab4::onUpdate(float timestep)
{
	ShadowMapVars m_ShadowMapVars;

	glm::vec3 lightPosition = (m_ShadowMapVars.centre - lightDirection) * m_ShadowMapVars.distanceAlongLightVector;
	glm::mat4 shadowView = glm::lookAt(lightPosition, m_ShadowMapVars.centre, m_ShadowMapVars.UP);

	auto& shadowPass = m_mainRenderer.getDepthPass(0);
	shadowPass.camera.view = shadowView;
	shadowPass.UBOmanager.setCachedValue("b_LightCamera", "u_view", shadowPass.camera.view);
	

	auto& shaowmainpass = m_mainRenderer.getDepthPass(1);
	shaowmainpass.camera.view = shadowView;
	shaowmainpass.UBOmanager.setCachedValue("b_LightCamera", "u_view", shaowmainpass.camera.view);	// per frame uniforms

	auto floormat = m_mainScene->m_actors.at(floorIdx).material;
	auto modelmat = m_mainScene->m_actors.at(modelIdx).material;

	floormat->setValue("u_albedo", m_colour);
	modelmat->setValue("u_lightSpaceTransform", shadowPass.camera.projection * shadowPass.camera.view);
	floormat->setValue("u_lightSpaceTransform", shadowPass.camera.projection * shadowPass.camera.view);

	//auto& edgemat = m_mainRenderer.getRenderPass(2).scene->m_actors.at(edgeIdx).material;
	//edgemat->setValue("a", DoEdge);

	auto& screenmat = m_mainRenderer.getRenderPass(m_mainRenderer.getRenderPassCount() - 1).scene->m_actors.at(screenIdx).material;
	//screenmat->setValue("Gamma", Gamma);

	if (haveEdge) screenmat->setValue("u_originalTexture", m_mainRenderer.getRenderPass(2).target->getTarget(0));
	else screenmat->setValue("u_originalTexture", m_mainRenderer.getRenderPass(0).target->getTarget(0));

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


void Lab4::onImGUIRender()
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

	GLuint textureID;
	ImVec2 imageSize = ImVec2(256, 256);
	ImVec2 uv0 = ImVec2(0.0f, 1.0f);
	ImVec2 uv1 = ImVec2(1.0f, 0.0f);
	if (ImGui::CollapsingHeader("mainpass", true))
	{
		textureID = m_mainRenderer.getRenderPass(1).target->getTarget(0)->getID();
		ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);
	}
	//if (ImGui::CollapsingHeader("invert renderpass", true))
	//{

	//	textureID = m_mainRenderer.getRenderPass(1).target->getTarget(0)->getID();
	//	ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);
	//}

	//if(ImGui::CollapsingHeader("edge renderpass", true))
	//{
	//ImGui::Checkbox("edge", &haveEdge); // changes if wireframe or not

	//ImGui::SliderFloat("edgeShader", &DoEdge, 0, 1); // turns edge on and off
	////ImGui::InputFloat("1 = edges / 2 = no edge", &DoEdge);
	//textureID = m_mainRenderer.getRenderPass(2).target->getTarget(0)->getID();
	//ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);
	//}
	//lab 3
	if (ImGui::CollapsingHeader("Lab3", true))
	{
		//textureID = m_mainRenderer.getDepthPass(0).target->getTarget(0)->getID();
		//ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);

		textureID = m_mainRenderer.getRenderPass(2).target->getTarget(0)->getID();
		ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);
		ImGui::SameLine();
		textureID = m_mainRenderer.getRenderPass(3).target->getTarget(0)->getID();
		ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);
		ImGui::SameLine();
		textureID = m_mainRenderer.getRenderPass(4).target->getTarget(0)->getID();
		ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);

		textureID = m_mainRenderer.getRenderPass(5).target->getTarget(0)->getID();
		ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);
	}
	//lab 4
	ImGui::SliderFloat3("light direction", (float*)&lightDirection, -1.0f, 0.0f);
	//ImGui::DragFloat3("light direction", (float*)&lightDirection, -1.0f, 1.0f);
	textureID = m_mainRenderer.getDepthPass(0).target->getTarget(0)->getID();
	ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);
	ImGui::End();
	ImGui::Render();



}

void Lab4::onKeyPressed(KeyPressedEvent& e)
{
	for (auto it = m_mainScene->m_actors.begin(); it != m_mainScene->m_actors.end(); ++it)
	{
		it->onKeyPress(e);
	}
}