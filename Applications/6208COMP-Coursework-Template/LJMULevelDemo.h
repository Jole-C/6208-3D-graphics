#pragma once

#include "Application.h"

#include "Win32RenderWindow.h"
#include "RendererDX11.h"

#include "ViewPerspective.h"

//Hieroglyph Includes
#include "Camera.h"
#include "Scene.h"
#include "GeometryActor.h"
#include "PointLight.h"

//STL Includes
#include <vector>

//LJMU Framework Includes
#include "LJMUTextOverlay.h"

#include "Spline.h"

using namespace Glyph3;

	//////////////////////////////////////
	//LJMULevelDemo.H
	//Class Application for a DirectX 11
	//Driven Application using the DirectX Toolkit
	//Hieroglyph 3 Rendering Engine and LUA.
	//////////////////////////////////////

class LJMULevelDemo : public Application //Inherit from the Hieroglyph Base Class
{

public:
	//------------CONSTRUCTORS------------------------------------------------
	LJMULevelDemo();	//Standard Empty Constructor which builds the object

public:
	//------------INHERITED METHODS-------------------------------------------
	virtual void Initialize();					//Initialise the DirectX11 Scene
	virtual void Update();						//Update the DirectX Scene
	virtual void Shutdown();					//Shutdown the DirectX11 Scene

	virtual bool ConfigureEngineComponents();	//Initialise Hieroglyph and DirectX TK Modules
	virtual void ShutdownEngineComponents();	//Destroy Hieroglyph and DirectX TK Modules

	virtual void TakeScreenShot();				//Allow a screenshot to be generated

	virtual bool HandleEvent(EventPtr pEvent);	//Handle an I/O Event
	virtual std::wstring GetName();				//Get the Name of this App Instance

	//------------CUSTOM METHODS-----------------------------------------------
	void inputAssemblyStage();					//Stage to setup our VB and IB Info
		
	std::wstring outputFPSInfo();				//Convert the timer's Frames Per Second to a formatted string
	
	RendererDX11* GetRenderer() { return m_pRenderer11; }; //Return a reference to the renderer
	
	void AddObject(class Object* newObject);
	void UpdateObjects();
	class LightController* GetLightController() { return lightController; }

	bool disableLightUpdating = false;
	bool toggleLightUpdating = false;

	class EarthSphere* earth;
	Camera* m_pCamera;			//Camera Object


protected:
	//-------------CLASS MEMBERS-----------------------------------------------
	RendererDX11*			m_pRenderer11;		//Pointer to our DirectX 11 Device
	Win32RenderWindow*		m_pWindow;			//Pointer to our Windows-Based Window

	int						m_iSwapChain;		//Index of our Swap Chain 
	ResourcePtr				m_RenderTarget;		//Pointer to the GPU Render Target for Colour
	ResourcePtr				m_DepthTarget;		//Pointer to the GPU Render Target for Depth
	
	std::vector<Object*> m_pObjects;
	Spline spline;
	float splineTime;
	class Spaceship* newSpaceship;
	class LightObject* skyLight;
	class TerrainMeshProcedural* terrainProcedural;
	class TerrainMeshHeightmap* terrainHeightmap;
	class SkySphere* skySphere;
	LightController* lightController;
	bool useProceduralTerrain = true;
	bool pressingTerrainKey = false;
	bool useCameraSpline = false;
	bool pressingCameraKey = false;
	int terrainSwapCooldown = 200;
	int toggleLightCooldown = 200;
	int useSplineCooldown = 200;

	//--------------HIEROGLYPH OBJECTS-----------------------------------------
	ViewPerspective*		m_pRenderView;		//3D Output View - DirectX 11 Accelerated
	LJMUTextOverlay*        m_pRender_text;		//2D Output View - DirectX 11 Accelerated

	const float				DEG_TO_RAD = GLYPH_PI / 180.0f;

	int					m_iscreenWidth = 1920;
	int					m_iscreenHeight = 1080;
};
