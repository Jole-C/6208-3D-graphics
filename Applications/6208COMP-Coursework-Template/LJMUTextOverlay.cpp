#include "LJMUTextOverlay.h"
#include "Entity3D.h"
#include "Node3D.h"
#include "Texture2dConfigDX11.h"
#include "Log.h"
#include "IParameterManager.h"
#include "PipelineManagerDX11.h"
#include "Texture2dDX11.h"

using namespace Glyph3;

///////////////////////////
// Constructor for our Text
// Overlay Class. 
///////////////////////////
LJMUTextOverlay::LJMUTextOverlay(RendererDX11& prenderer, ResourcePtr prtt, std::wstring pfont, int psize)
{
	this->_obj_rendertarget = prtt;
	ResourceDX11* tptr_res = prenderer.GetResourceByIndex(this->_obj_rendertarget->m_iResource);

	if (tptr_res->GetType() == RT_TEXTURE2D)
	{
		Texture2dDX11* ttex = (Texture2dDX11*)tptr_res;
		D3D11_TEXTURE2D_DESC tdesc = ttex->GetActualDescription();

		D3D11_VIEWPORT tviewport;
		tviewport.Width = static_cast<float>(tdesc.Width);
		tviewport.Height = static_cast<float>(tdesc.Height);
		tviewport.MinDepth = 0.0f;
		tviewport.MaxDepth = 1.0f;
		tviewport.TopLeftX = 0;
		tviewport.TopLeftY = 0;
		this->_obj_viewport = prenderer.CreateViewPort(tviewport);
	}

	//Create the Sprite Font and the Sprite Renderer (SpriteBatch) for our Text Elements
	this->_obj_ptr_font = SpriteFontLoaderDX11::LoadFont(pfont,psize, 0,false);	
	this->_obj_spritebatch = new SpriteRendererDX11();
	this->_obj_spritebatch->Initialize();
}

///////////////////////////
// Destructor For our Text
// Overlay Class
///////////////////////////
LJMUTextOverlay::~LJMUTextOverlay()
{
	SAFE_DELETE(this->_obj_spritebatch);
}

///////////////////////////
// Queue the Render Tasks
///////////////////////////
void LJMUTextOverlay::QueuePreTasks(RendererDX11* prenderer)
{
	prenderer->QueueTask(this);
}

///////////////////////////
// Execute our Text Rendering
// Task
///////////////////////////
void LJMUTextOverlay::ExecuteTask(PipelineManagerDX11* ppipelinemanager, IParameterManager* pparamManager)
{
	//If we have any text elements to draw
	if (this->_list_text.size() > 0)
	{
		// Set the parameters for rendering this view
		ppipelinemanager->ClearRenderTargets();
		ppipelinemanager->OutputMergerStage.DesiredState.RenderTargetViews.SetState(0,this->_obj_rendertarget->m_iResourceRTV);
		ppipelinemanager->ApplyRenderTargets();

		ppipelinemanager->RasterizerStage.DesiredState.ViewportCount.SetState(1);
		ppipelinemanager->RasterizerStage.DesiredState.Viewports.SetState(0,this->_obj_viewport);
		ppipelinemanager->RasterizerStage.DesiredState.RasterizerState.SetState(0);

		// Set default states for these stages
		ppipelinemanager->OutputMergerStage.DesiredState.DepthStencilState.SetState(0);
		ppipelinemanager->OutputMergerStage.DesiredState.BlendState.SetState(0);

		//Loop over each text element and use the spritebatch to draw them
		for (auto tentry : this->_list_text)
		{
			//Use the RenderText function of our Sprite Batch to draw the text
			this->_obj_spritebatch->RenderText(ppipelinemanager, pparamManager,
		           this->_obj_ptr_font, tentry.text.c_str(), tentry.xform, tentry.colour);
		}

		//Clear our list of text entries each frame
		this->_list_text.clear();
	}
}

///////////////////////////
// Add a Text Entry with
// the given parameters
///////////////////////////
void LJMUTextOverlay::writeText(std::wstring& ptext, Matrix4f& pxform, Vector4f& pcolour)
{
	this->_list_text.push_back(LJMUTextEntry(ptext, pxform, pcolour));
}

///////////////////////////
// Get the Name of this Task
///////////////////////////
std::wstring LJMUTextOverlay::GetName()
{
	return(L"LJMUTextOverlay");
}

///////////////////////////
// Resize the Viewport
///////////////////////////
void LJMUTextOverlay::Resize(UINT pwidth, UINT pheight)
{
	RendererDX11::Get()->ResizeViewport(this->_obj_viewport,pwidth,pheight);
}


//------------INTERFACE METHODS---------------------------------------------------

///////////////////////////
//
///////////////////////////
void LJMUTextOverlay::SetRenderParams(IParameterManager* pParamManager)
{
	//NO RENDER PARAMS NEEDED
}

///////////////////////////
//
///////////////////////////
void LJMUTextOverlay::SetUsageParams(IParameterManager* pParamManager)
{
	//NO USAGE PARAMS NEEDED
}

///////////////////////////
//
///////////////////////////
void LJMUTextOverlay::Update(float ptime)
{
	//NOTHING TO UPDAATE
}