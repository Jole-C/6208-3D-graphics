//------------Include the Application Header File----------------------------
#include "LJMULevelDemo.h"

//------------DX TK AND STD/STL Includes-------------------------------------
#include <sstream>

//------------Include Hieroglyph Engine Files--------------------------------

//Include the Logging System
#include "Log.h"

//Include the Event System
#include "EventManager.h"
#include "EvtFrameStart.h"
#include "EvtChar.h"
#include "EvtKeyUp.h"
#include "EvtKeyDown.h"
#include "ScriptManager.h"

//Include the DirectX Rendering Components
#include "PipelineManagerDX11.h"
#include "BlendStateConfigDX11.h"
#include "BufferConfigDX11.h"
#include "DepthStencilStateConfigDX11.h"
#include "RasterizerStateConfigDX11.h"
#include "SwapChainConfigDX11.h"
#include "Texture2dConfigDX11.h"
#include "MaterialGeneratorDX11.h"

#include "FirstPersonCamera.h"

#include <FileSystem.h>
#include <SamplerStateConfigDX11.h>

//object includes
#include "XAxisObject.h"
#include "Object.h"
#include "Spaceship.h"
#include "TerrainMeshProcedural.h"
#include "TerrainMeshHeightmap.h"
#include "SplineMesh.h"
#include "MathHelpers.h"
#include "LightController.h"
#include "SkySphere.h"
#include "SpaceStationDomes.h"
#include "SpaceStationRocket.h"
#include "SpaceStationSolarPanels.h"
#include "SpaceStationGun.h"
#include "EarthSphere.h"
#include "SolarSystem.h"
#include "ExplodingPlanet.h"

//Add a Using Directive to avoid typing Glyph3 for basic constructs
using namespace Glyph3;

LJMULevelDemo AppInstance; 

//---------CONSTRUCTORS-------------------------------------------------------

///////////////////////////////////////
//
///////////////////////////////////////
LJMULevelDemo::LJMULevelDemo():
m_pRender_text(nullptr),
m_pRenderView(nullptr),
m_pCamera(nullptr),
m_pRenderer11(nullptr),
m_pWindow(nullptr),
m_iSwapChain(0),
m_DepthTarget(nullptr),
m_RenderTarget(nullptr)
{
	
}

//---------METHODS------------------------------------------------------------

//////////////////////////////////////
// Get the Window Name of this Application
//////////////////////////////////////
std::wstring LJMULevelDemo::GetName()
{
	return(std::wstring(L"6208COMP: Coursework Template"));
}

/////////////////////////////////////
// Assemble our Input Layouts for this
// Stage of the Pipeline.
/////////////////////////////////////
void LJMULevelDemo::inputAssemblyStage()
{			
	//-----SETUP OUR GEOMETRY FOR THIS SCENE-----------------------------------------

}

////////////////////////////////////
// Initialise our DirectX 3D Scene
////////////////////////////////////
void LJMULevelDemo::Initialize()
{
	//Call the Input Assembly Stage to setup the layout of our Engine Objects
	this->inputAssemblyStage();

	this->m_pCamera = new FirstPersonCamera();
	this->m_pCamera->SetEventManager(&EvtManager);

	Vector3f tcamerapos(0, 300, 0);
	this->m_pCamera->Spatial().SetTranslation(tcamerapos);
	this->m_pCamera->Spatial().SetRotation(Vector3f(GLYPH_PI / 8.0f, -GLYPH_PI * 3.0 / 250.0f, 0.0f));

	this->m_pRenderView = new ViewPerspective(*this->m_pRenderer11,
		                                     this->m_RenderTarget, this->m_DepthTarget);
	this->m_pRenderView->SetBackColor(Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
	this->m_pCamera->SetCameraView(this->m_pRenderView);

	this->m_pRender_text = new LJMUTextOverlay(*this->m_pRenderer11, 
		                                      this->m_RenderTarget, 
											  std::wstring(L"Cambria"), 
											  25);	

	this->m_pCamera->SetOverlayView(this->m_pRender_text);
	

	this->m_pCamera->SetProjectionParams(0.1f, 100000.0f, ((float)m_iscreenWidth) / (float)m_iscreenHeight,
		static_cast<float>(GLYPH_PI) / 2.0f);
	
	this->m_pScene->AddCamera(this->m_pCamera);	

	//XAxisObject* newObject = new XAxisObject(this);
	//AddObject(newObject);

	//First, add the light controller and set up light sources
	lightController = new LightController(this);
	AddObject(lightController);

	skyLight = new LightObject(this);
	skyLight->lightInfo.iLightType = Vector4f(static_cast<float>(LIGHT_TYPES::LIGHT_DIRECTIONAL), 0, 0, 0);
	skyLight->lightInfo.LightColour = Vector4f(0.01, 0.01, 0.01, 1);
	skyLight->lightInfo.LightDirection = Vector4f(0, -1, 0, 1);
	lightController->AddLight(skyLight);

	LightObject* point = new LightObject(this);
	point->lightInfo.iLightType = Vector4f(static_cast<float>(LIGHT_TYPES::LIGHT_POINT), 0, 0, 0);
	point->lightInfo.LightColour = Vector4f(1, 1, 1, 1);
	point->lightInfo.LightPosition = Vector4f(0, 100, 350, 1);
	point->lightInfo.LightRange = Vector4f(150, 1, 1, 1);
	lightController->AddLight(point);

	LightObject* point2 = new LightObject(this);
	point2->lightInfo.iLightType = Vector4f(static_cast<float>(LIGHT_TYPES::LIGHT_POINT), 0, 0, 0);
	point2->lightInfo.LightColour = Vector4f(0, 1, 0, 1);
	point2->lightInfo.LightPosition = Vector4f(350, 100, 0, 1);
	point2->lightInfo.LightRange = Vector4f(150, 1, 1, 1);
	lightController->AddLight(point2);

	LightObject* point3 = new LightObject(this);
	point3->lightInfo.iLightType = Vector4f(static_cast<float>(LIGHT_TYPES::LIGHT_POINT), 0, 0, 0);
	point3->lightInfo.LightColour = Vector4f(1, 0, 0, 1);
	point3->lightInfo.LightPosition = Vector4f(0, 100, -350, 1);
	point3->lightInfo.LightRange = Vector4f(150, 1, 1, 1);
	lightController->AddLight(point3);
	
	LightObject* point4 = new LightObject(this);
	point4->lightInfo.iLightType = Vector4f(static_cast<float>(LIGHT_TYPES::LIGHT_POINT), 0, 0, 0);
	point4->lightInfo.LightColour = Vector4f(0, 0, 1, 1);
	point4->lightInfo.LightPosition = Vector4f(-350, 100, 0, 1);
	point4->lightInfo.LightRange = Vector4f(150, 1, 1, 1);
	lightController->AddLight(point4);

	LightObject* spot1 = new LightObject(this);
	spot1->lightInfo.iLightType = Vector4f(static_cast<float>(LIGHT_TYPES::LIGHT_SPOT), 0, 0, 0);
	spot1->lightInfo.LightColour = Vector4f(1, 0, 1, 1);
	spot1->lightInfo.LightPosition = Vector4f(350, 100, 350, 1);
	spot1->lightInfo.LightRange = Vector4f(300, 0, 1, 1);
	spot1->lightInfo.LightFocus = Vector4f(200, 1, 1, 1);
	spot1->lightInfo.LightDirection = Vector4f(1, -1, 0, 1);
	lightController->AddLight(spot1);

	LightObject* spot2 = new LightObject(this);
	spot2->lightInfo.iLightType = Vector4f(static_cast<float>(LIGHT_TYPES::LIGHT_SPOT), 0, 0, 0);
	spot2->lightInfo.LightColour = Vector4f(0, 1, 1, 1);
	spot2->lightInfo.LightPosition = Vector4f(-350, 100, -350, 1);
	spot2->lightInfo.LightRange = Vector4f(300, 0, 1, 1);
	spot2->lightInfo.LightFocus = Vector4f(250, 1, 1, 1);
	spot2->lightInfo.LightDirection = Vector4f(1, -1, 0, 1);
	lightController->AddLight(spot2);

	LightObject* spot3 = new LightObject(this);
	spot3->lightInfo.iLightType = Vector4f(static_cast<float>(LIGHT_TYPES::LIGHT_SPOT), 0, 0, 0);
	spot3->lightInfo.LightColour = Vector4f(1, 1, 0, 1);
	spot3->lightInfo.LightPosition = Vector4f(350, 100, -350, 1);
	spot3->lightInfo.LightRange = Vector4f(300, 0, 1, 1);
	spot3->lightInfo.LightFocus = Vector4f(250, 1, 1, 1);
	spot3->lightInfo.LightDirection = Vector4f(1, -1, 0, 1);
	lightController->AddLight(spot3);

	LightObject* spot4 = new LightObject(this);
	spot4->lightInfo.iLightType = Vector4f(static_cast<float>(LIGHT_TYPES::LIGHT_SPOT), 0, 0, 0);
	spot4->lightInfo.LightColour = Vector4f(1, 1, 1, 1);
	spot4->lightInfo.LightPosition = Vector4f(-350, 100, 350, 1);
	spot4->lightInfo.LightRange = Vector4f(300, 0, 1, 1);
	spot4->lightInfo.LightFocus = Vector4f(250, 1, 1, 1);
	spot4->lightInfo.LightDirection = Vector4f(1, -1, 0, 1);
	lightController->AddLight(spot4);

	//Add spaceship
	newSpaceship = new Spaceship(this);
	AddObject(newSpaceship);

	//Add terrain
	terrainProcedural = new TerrainMeshProcedural(this);
	AddObject(terrainProcedural);

	terrainHeightmap = new TerrainMeshHeightmap(this);
	AddObject(terrainHeightmap);
	terrainHeightmap->paused = true;
	terrainHeightmap->GetNode()->Scale() = Vector3f(0, 0, 0);

	//Add sky sphere
	skySphere = new SkySphere(this);
	AddObject(skySphere);

	//Add planets
	earth = new EarthSphere(this);
	AddObject(earth);

	SolarSystem* solarSystem = new SolarSystem(this);
	AddObject(solarSystem);
	
	ExplodingPlanet* explodingPlanet = new ExplodingPlanet(this);
	AddObject(explodingPlanet);

	//Add props
	SpaceStationDomes* spaceDomes = new SpaceStationDomes(this);
	AddObject(spaceDomes);

	SpaceStationRocket* stationRocket = new SpaceStationRocket(this);
	AddObject(stationRocket);

	SpaceStationSolarPanels* solarPanels = new SpaceStationSolarPanels(this);
	AddObject(solarPanels);

	SpaceStationGun* spaceGun = new SpaceStationGun(this);
	AddObject(spaceGun);

	//Initialise the spline for the camera
	std::vector<Vector3f> splinePoints; splinePoints.push_back(Vector3f(2.726815, 75.184448 + 40, -290.351166));
	splinePoints.push_back(Vector3f(117.009895, 63.716072 + 40, -275.601318));
	splinePoints.push_back(Vector3f(175.285767, 88.172676 + 40, -275.824493));
	splinePoints.push_back(Vector3f(222.112778, 81.388451 + 40, -250.943924));
	splinePoints.push_back(Vector3f(268.563416, 68.087875 + 40, -242.112091));
	splinePoints.push_back(Vector3f(313.077240, 78.642204 + 40, -195.483414));
	splinePoints.push_back(Vector3f(344.041687, 91.079758 + 40, -122.319534));
	splinePoints.push_back(Vector3f(338.596313, 137.755219 + 40, -79.833900));
	splinePoints.push_back(Vector3f(328.372253, 164.697769 + 40, -46.539818));
	splinePoints.push_back(Vector3f(392.992004, 264.355713 + 40, 63.450844));
	splinePoints.push_back(Vector3f(388.737488, 262.776093 + 40, 113.096024));
	splinePoints.push_back(Vector3f(345.218109, 222.651886 + 40, 137.430130));
	splinePoints.push_back(Vector3f(328.345612, 174.736633 + 40, 171.493896));
	splinePoints.push_back(Vector3f(231.944717, 134.046066 + 40, 221.311127));
	splinePoints.push_back(Vector3f(167.963486, 183.759003 + 40, 251.330093));
	splinePoints.push_back(Vector3f(93.154030, 225.771149 + 40, 287.788666));
	splinePoints.push_back(Vector3f(-27.705246, 238.056274 + 40, 262.878448));
	splinePoints.push_back(Vector3f(-110.344437, 285.243988 + 40, 314.229218));
	splinePoints.push_back(Vector3f(-225.263565, 358.849762 + 40, 270.667480));
	splinePoints.push_back(Vector3f(-253.361908, 420.166290 + 40, 250.484177));
	splinePoints.push_back(Vector3f(-310.148865, 393.474335 + 40, 175.987717));
	splinePoints.push_back(Vector3f(-320.696686, 414.087616 + 40, 68.278709));
	splinePoints.push_back(Vector3f(-261.892303, 365.125854 + 40, 21.209131));
	splinePoints.push_back(Vector3f(-182.116699, 349.401855 + 40, 19.536266));
	splinePoints.push_back(Vector3f(-147.902008, 334.766388 + 40, 48.405193));
	splinePoints.push_back(Vector3f(-149.236877, 276.875641 + 40, 108.338531));
	splinePoints.push_back(Vector3f(-204.393616, 264.075043 + 40, 116.476234));
	splinePoints.push_back(Vector3f(-326.761047, 227.011383 + 40, 188.805832));
	splinePoints.push_back(Vector3f(-365.493683, 165.212585 + 40, 125.483879));
	splinePoints.push_back(Vector3f(-372.936829, 99.412048 + 40, 33.436031));
	splinePoints.push_back(Vector3f(-297.870178, 75.402832 + 40, -62.061886));
	splinePoints.push_back(Vector3f(-243.983444, 56.666042 + 40, -146.428879));
	splinePoints.push_back(Vector3f(-190.997375, 38.889526 + 40, -193.429596));
	splinePoints.push_back(Vector3f(-138.011322, 21.113037 + 40, -240.430283));
	splinePoints.push_back(Vector3f(-92.779495, 27.095741 + 40, -262.752350));
	spline.CreateSpline(splinePoints);
}

///////////////////////////////////
// Update the State of our Game and 
// Output the Results to Screen (Render)
/////////////////////////////////// 
void LJMULevelDemo::Update()
{
	this->m_pTimer->Update();
	EvtManager.ProcessEvent(EvtFrameStartPtr( new EvtFrameStart(this->m_pTimer->Elapsed())));

	//----------START RENDERING--------------------------------------------------------------

	this->m_pScene->Update(m_pTimer->Elapsed());

	splineTime += 0.0001;

	if (splineTime >= 1)
	{
		splineTime = 0;
	}

	Vector2f cameraOrigin2D;
	cameraOrigin2D.x = m_pCamera->GetNode()->Position().x;
	cameraOrigin2D.y = m_pCamera->GetNode()->Position().z;

	//Swap terrain
	if (terrainHeightmap != nullptr)
	{
		terrainHeightmap->SetRenderingOrigin(cameraOrigin2D);
	}
	if (terrainProcedural != nullptr)
	{
		terrainProcedural->SetRenderingOrigin(cameraOrigin2D);
	}

	if (pressingTerrainKey == true && terrainSwapCooldown <= 0)
	{
		useProceduralTerrain = !useProceduralTerrain;
		terrainSwapCooldown = 200;
	}

	if (useProceduralTerrain == true)
	{
		terrainProcedural->paused = false;
		terrainProcedural->GetNode()->Scale() = Vector3f(1, 1, 1);
		terrainHeightmap->paused = true;
		terrainHeightmap->GetNode()->Scale() = Vector3f(0, 0, 0);
	}
	else
	{
		terrainHeightmap->paused = false;
		terrainHeightmap->GetNode()->Scale() = Vector3f(1, 1, 1);
		terrainProcedural->paused = true;
		terrainProcedural->GetNode()->Scale() = Vector3f(0, 0, 0);
	}

	terrainSwapCooldown -= 1;

	//Camera spline
	if (pressingCameraKey == true && useSplineCooldown <= 0)
	{
		useSplineCooldown = 200;
		useCameraSpline = !useCameraSpline;
	}

	useSplineCooldown -= 1;

	if (useCameraSpline == true)
	{
		SplinePoint point = spline.FindPointAlongSpline(splineTime);

		Vector3f shipDirection = newSpaceship->GetNode()->Position() - m_pCamera->GetNode()->Position();
		shipDirection.Normalize();
		m_pCamera->Spatial().SetTranslation(point.position);

		float yaw = atan2(shipDirection.x, shipDirection.z);
		float xz = shipDirection.Magnitude();
		float pitch = -atan2(shipDirection.y, xz);
		m_pCamera->Spatial().SetRotation(Vector3f(pitch, yaw, 0));
	}

	//toggle lights
	if (toggleLightUpdating == true && toggleLightCooldown <= 0)
	{
		disableLightUpdating = !disableLightUpdating;
		toggleLightCooldown = 200;
	}

	toggleLightCooldown -= 1;

	//Set the directional light colour
	float skyColour = (skySphere->GetSkyTime() / 2) + 0.01;
	skyLight->lightInfo.LightColour = Vector4f(skyColour, skyColour, skyColour, 1);

	//Update all objects
	UpdateObjects();

	//Render text
	std::wstringstream out;
	out << L"FPS: " << m_pTimer->Framerate();
	out << L"\nPress R to swap terrain";
	out << L"\nPress F to toggle lighting updates";
	out << L"\nPress T to toggle camera spline following";

	float tx = 30.0f;	float ty = 30.0f;
	Matrix4f ttextpos = Matrix4f::Identity();
	ttextpos.SetTranslation(Vector3f(tx, ty, 0.0f));

	static Vector4f twhiteclr(1.0f, 1.0f, 1.0f, 1.0f);
	static Vector4f tyellowclr(1.0f, 1.0f, 0.0f, 1.0f);

	m_pRender_text->writeText(out.str(), ttextpos, twhiteclr);

	//Render the scene
	this->m_pScene->Render(this->m_pRenderer11);

	//--------END RENDERING-------------------------------------------------------------
	this->m_pRenderer11->Present(this->m_pWindow->GetHandle(), this->m_pWindow->GetSwapChain());

	//lightController->ClearConstantBuffers();
}

///////////////////////////////////
// Configure the DirectX 11 Programmable
// Pipeline Stages and Create the Window
// Calls 
///////////////////////////////////
bool LJMULevelDemo::ConfigureEngineComponents()
{

	// Set the render window parameters and initialize the window
	this->m_pWindow = new Win32RenderWindow();
	this->m_pWindow->SetPosition(25, 25);
	this->m_pWindow->SetSize(m_iscreenWidth, m_iscreenHeight);
	this->m_pWindow->SetCaption(this->GetName());
	this->m_pWindow->Initialize(this);


	// Create the renderer and initialize it for the desired device
	// type and feature level.
	this->m_pRenderer11 = new RendererDX11();

	if (!this->m_pRenderer11->Initialize(D3D_DRIVER_TYPE_HARDWARE, D3D_FEATURE_LEVEL_11_0))
	{
		Log::Get().Write(L"Could not create hardware device, trying to create the reference device...");

		if (!this->m_pRenderer11->Initialize(D3D_DRIVER_TYPE_REFERENCE, D3D_FEATURE_LEVEL_10_0))
		{
			ShowWindow(this->m_pWindow->GetHandle(), SW_HIDE);
			MessageBox(this->m_pWindow->GetHandle(), L"Could not create a hardware or software Direct3D 11 device!", L"6107COMP Coursework Template", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
			this->RequestTermination();
			return(false);
		}
		// If using the reference device, utilize a fixed time step for any animations.
		this->m_pTimer->SetFixedTimeStep(1.0f / 10.0f);
	}

	// Create a swap chain for the window that we started out with.  This
	// demonstrates using a configuration object for fast and concise object
	// creation.
	SwapChainConfigDX11 tconfig;
	tconfig.SetWidth(this->m_pWindow->GetWidth());
	tconfig.SetHeight(this->m_pWindow->GetHeight());
	tconfig.SetOutputWindow(this->m_pWindow->GetHandle());
	this->m_iSwapChain = this->m_pRenderer11->CreateSwapChain(&tconfig);
	this->m_pWindow->SetSwapChain(this->m_iSwapChain);
	
	//Create Colour and Depth Buffers
	this->m_RenderTarget = this->m_pRenderer11->GetSwapChainResource(this->m_iSwapChain);

	Texture2dConfigDX11 tdepthconfig;
	tdepthconfig.SetDepthBuffer(m_iscreenWidth, m_iscreenHeight);
	this->m_DepthTarget = this->m_pRenderer11->CreateTexture2D(&tdepthconfig, 0);

	// Bind the swap chain render target and the depth buffer for use in rendering.  
	this->m_pRenderer11->pImmPipeline->ClearRenderTargets();
	this->m_pRenderer11->pImmPipeline->OutputMergerStage.DesiredState.RenderTargetViews.SetState(0, this->m_RenderTarget->m_iResourceRTV);
	this->m_pRenderer11->pImmPipeline->OutputMergerStage.DesiredState.DepthTargetViews.SetState(this->m_DepthTarget->m_iResourceDSV);
	this->m_pRenderer11->pImmPipeline->ApplyRenderTargets();

	D3D11_VIEWPORT tviewport;
	tviewport.Width = static_cast< float >(m_iscreenWidth);
	tviewport.Height = static_cast< float >(m_iscreenHeight);
	tviewport.MinDepth = 0.0f;
	tviewport.MaxDepth = 1.0f;
	tviewport.TopLeftX = 0;
	tviewport.TopLeftY = 0;

	int tvpindex = this->m_pRenderer11->CreateViewPort(tviewport);
	this->m_pRenderer11->pImmPipeline->RasterizerStage.DesiredState.ViewportCount.SetState(1);
	this->m_pRenderer11->pImmPipeline->RasterizerStage.DesiredState.Viewports.SetState(0, tvpindex);
	return(true);
}

//////////////////////////////////
//Handle Input Events in the Application
//////////////////////////////////
bool LJMULevelDemo::HandleEvent(EventPtr pevent)
{
	eEVENT e = pevent->GetEventType();

	if (e == SYSTEM_KEYBOARD_KEYDOWN)
	{
		EvtKeyDownPtr tkey_down = std::static_pointer_cast<EvtKeyDown>(pevent);
		UINT tkeycode = tkey_down->GetCharacterCode();

		if (tkeycode == (UINT)'R')
		{
			pressingTerrainKey = true;
		}

		if (tkeycode == (UINT)'F')
		{
			toggleLightUpdating = true;
		}

		if (tkeycode == (UINT)'T')
		{
			pressingCameraKey = true;
		}
	}
	else if (e == SYSTEM_KEYBOARD_KEYUP)
	{
		EvtKeyUpPtr tkey_up = std::static_pointer_cast<EvtKeyUp>(pevent);
		UINT tkeycode = tkey_up->GetCharacterCode();

		if (tkeycode == (UINT)'R')
		{
			pressingTerrainKey = false;
		}

		if (tkeycode == (UINT)'F')
		{
			toggleLightUpdating = false;
		}

		if (tkeycode == (UINT)'T')
		{
			pressingCameraKey = false;
		}
	}

	return(Application::HandleEvent(pevent));
}

//////////////////////////////////
// Destroy Resources created by the engine
//////////////////////////////////
void LJMULevelDemo::ShutdownEngineComponents()
{
	if (this->m_pRenderer11)
	{
		this->m_pRenderer11->Shutdown();
		delete this->m_pRenderer11;
	}

	if (this->m_pWindow)
	{
		this->m_pWindow->Shutdown();
		delete this->m_pWindow;
	}
}

//////////////////////////////////
// Shutdown the Application
//////////////////////////////////
void LJMULevelDemo::Shutdown()
{
	//NOTHING TO DO HERE
}

//////////////////////////////////
// Take a Screenshot of the Application
//////////////////////////////////
void LJMULevelDemo::TakeScreenShot()
{
	if (this->m_bSaveScreenshot)
	{
		this->m_bSaveScreenshot = false;
		this->m_pRenderer11->pImmPipeline->SaveTextureScreenShot(0, this->GetName());
	}
}

//////////////////////////////////////
// Output our Frame Rate
//////////////////////////////////////
std::wstring LJMULevelDemo::outputFPSInfo()
{
	std::wstringstream out;
	out << L"FPS: " << m_pTimer->Framerate();
	return out.str();
}

void LJMULevelDemo::AddObject(Object* newObject)
{
	m_pObjects.push_back(newObject);
}

void LJMULevelDemo::UpdateObjects()
{
	auto iterator = m_pObjects.begin();

	while (iterator != m_pObjects.end())
	{
		if ((*iterator) != nullptr)
		{
			if ((*iterator)->IsMarkedForDelete() == false)
			{
				if ((*iterator)->paused == false)
				{
					(*iterator)->Update();
				}

				iterator++;
			}
			else
			{
				delete (*iterator);
				iterator = m_pObjects.erase(iterator);
			}
		}
	}
}
