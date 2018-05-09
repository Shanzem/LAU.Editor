// ImGui - standalone example application for DirectX 9
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include <d3dx9.h>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>





using namespace std;
// Data
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            ImGui_ImplDX9_InvalidateDeviceObjects();
            g_d3dpp.BackBufferWidth  = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
            if (hr == D3DERR_INVALIDCALL)
                IM_ASSERT(0);
            ImGui_ImplDX9_CreateDeviceObjects();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int main(int, char**)
{
    // Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL, _T("ImGui Example"), NULL };
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindow(_T("ImGui Example"), _T("LUA.Editor"), WS_OVERLAPPEDWINDOW, 0, 0, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    LPDIRECT3D9 pD3D;
    if ((pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
    {
        UnregisterClass(_T("ImGui Example"), wc.hInstance);
        return 0;
    }
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // Present with vsync

    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // Present without vsync, maximum unthrottled framerate

    // Create the D3DDevice
    if (pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
    {
        pD3D->Release();
        UnregisterClass(_T("ImGui Example"), wc.hInstance);
        return 0;
    }

    // Setup ImGui binding
    ImGui_ImplDX9_Init(hwnd, g_pd3dDevice);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
/////////////////////////////////////////////////////////////
//
//	Window bools
//
////////////////////////////////////////////////////////////
bool show_editor_window 		= true;		// Editor options selector
bool show_editor_window0		= true;		// Welcome screen/notes/changes
bool show_editor_window1	 	= false;	// Working window blank
bool show_editor_window2		= false;	// Working window blank
bool show_editor_window3 		= false;	// Working window blank
bool show_editor_window4 		= false;	// Working window blank
bool show_editor_window5 		= false;	// Working window blank
bool show_editor_window6 		= false;	// Working window blank
bool show_LUA_commands 			= false;
bool show_about_editor			= false;
bool show_new_wizard			= false;
bool show_new_help				= false;
bool show_compiler				= false;
bool show_run					= false;
bool show_save					= false;
bool show_open					= false;
bool show_help_support			= false;
bool show_options				= false;
bool show_IO					= false;
bool show_batch					= false;
////////////////////////////////////////////////////////////
//
//
bool accept_source0_workspace1	= false;
bool accept_source1_workspace1	= false;
bool accept_source2_workspace1	= false;
/////////////////////////////////////////////////////////////
//
//
bool accept_source0_workspace2	= false;
bool accept_source1_workspace2	= false;
bool accept_source2_workspace2	= false;
/////////////////////////////////////////////////////////////
//
//
bool accept_source0_workspace3	= false;
bool accept_source1_workspace3	= false;
bool accept_source2_workspace3	= false;
/////////////////////////////////////////////////////////////
//
//
bool accept_source0_workspace4	= false;
bool accept_source1_workspace4	= false;
bool accept_source2_workspace4	= false;
/////////////////////////////////////////////////////////////
//
//
bool accept_source0_workspace5	= false;
bool accept_source1_workspace5	= false;
bool accept_source2_workspace5	= false;
/////////////////////////////////////////////////////////////
//RGBA colour for screen.
ImVec4 clear_color = ImVec4(0.073f, 0.115f, 0.177f, 1.000f);

////////////////////////////////////////////////////////////////////////////////
//
//	ICONS
//
///////////////////////////////////////////////////////////////
static LPDIRECT3DTEXTURE9       filenew = NULL;		// image idents here. here =)
static LPDIRECT3DTEXTURE9       fileopen = NULL;
static LPDIRECT3DTEXTURE9       filesave = NULL;
static LPDIRECT3DTEXTURE9       settings = NULL;
static LPDIRECT3DTEXTURE9       IO = NULL;
static LPDIRECT3DTEXTURE9       compile = NULL;
static LPDIRECT3DTEXTURE9       run = NULL;
static LPDIRECT3DTEXTURE9       source = NULL;
static LPDIRECT3DTEXTURE9       help = NULL;
static LPDIRECT3DTEXTURE9       Editor = NULL;
static LPDIRECT3DTEXTURE9       batch = NULL;
D3DXCreateTextureFromFile( g_pd3dDevice, "filenew.png" , &filenew );			// image file grabs here :)
D3DXCreateTextureFromFile( g_pd3dDevice, "fileopen.png" , &fileopen );
D3DXCreateTextureFromFile( g_pd3dDevice, "filesave.png" , &filesave );
D3DXCreateTextureFromFile( g_pd3dDevice, "mechanical.png" , &settings );
D3DXCreateTextureFromFile( g_pd3dDevice, "folder.png" , &IO );
D3DXCreateTextureFromFile( g_pd3dDevice, "compile.png" , &compile );
D3DXCreateTextureFromFile( g_pd3dDevice, "source.png" , &source );
D3DXCreateTextureFromFile( g_pd3dDevice, "compilerun.png" , &run );
D3DXCreateTextureFromFile( g_pd3dDevice, "runbatch.png" , &batch );
D3DXCreateTextureFromFile( g_pd3dDevice, "addition.png" , &help );
D3DXCreateTextureFromFile( g_pd3dDevice, "LUA.Editor.png" , &Editor );
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);
    while (msg.message != WM_QUIT)
    {
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }									   ///////////////////////////////////////////
        ImGui_ImplDX9_NewFrame();             // IT ALL STARTS HERE
//////////////////////////////////////////////////////////////
//
//	Working Variables
//
//////////////////////////////////////////////////////////////		
static char text1[1024*16] = 	"Notes/Revision/Changes\n\n"
								"Welcome to the LUA editor Click NEW to begin!\n\n"
								"TODO Save & Open! - Thats it!.\n"
								"Grabbing latest LUA AND LUAC.";
								
								
/////////////////////////////////////////////////////////////
//
//	Templates
static char Template0[1024*16];
//////////////////////////////////////////////////////////////
static char Template1[1024*16] = 	"-- the first program in every language\n\n"
									"io.write(\"Hello world, from \",_VERSION,\"!\")";
//////////////////////////////////////////////////////////////////
static char Template2[1024*16] = "-- life.lua\n"
"-- original by Dave Bollinger <DBollinger@compuserve.com> posted to lua-l\n"
"-- modified to use ANSI terminal escape sequences\n"
"-- modified to use for instead of while\n\n"
"local write=io.write\n"
"ALIVE=\" A \"	DEAD=\" D \"\n"
"ALIVE=\" AO \"	DEAD=\" D- \"\n\n"
"function delay() -- NOTE: SYSTEM-DEPENDENT, adjust as necessary\n"
"	for i=1,10000 do end\n"
"-- local i=os.clock()+1 while(os.clock()<i) do end\n"
"end\n\n"
"function ARRAY2D(w,h)\n"
"	local t = {w=w,h=h}\n"
"	for y=1,h do\n"
"		t[y] = {}\n"
"		for x=1,w do\n"
"			t[y][x]=0\n"
"   end\n"
"  end\n"
" return t\n"
"end\n\n"
"_CELLS = {}\n\n"
"-- give birth to a \"shape\" within the cell array\n"
"function _CELLS:spawn(shape,left,top)\n"
"for y=0,shape.h-1 do\n"
"    for x=0,shape.w-1 do\n"
"      self[top+y][left+x] = shape[y*shape.w+x+1]\n"
"    end\n"
"  end\n"
"end\n\n"
"-- run the CA and produce the next generation\n"
"function _CELLS:evolve(next)\n"
"  local ym1,y,yp1,yi=self.h-1,self.h,1,self.h\n"
"  while yi > 0 do\n"
"    local xm1,x,xp1,xi=self.w-1,self.w,1,self.w\n"
"    while xi > 0 do\n"
"      local sum = self[ym1][xm1] + self[ym1][x] + self[ym1][xp1] +\n"
"                  self[y][xm1] + self[y][xp1] +\n"
"                  self[yp1][xm1] + self[yp1][x] + self[yp1][xp1]\n"
"      next[y][x] = ((sum==2) and self[y][x]) or ((sum==3) and 1) or 0\n"
"      xm1,x,xp1,xi = x,xp1,xp1+1,xi-1\n"
"    end\n"
"    ym1,y,yp1,yi = y,yp1,yp1+1,yi-1\n"
"  end\n"
"end\n\n"
"-- output the array to screen\n"
"function _CELLS:draw()\n"
"  local out=\"\" -- accumulate to reduce flicker\n"
"  for y=1,self.h do\n"
"   for x=1,self.w do\n"
"      out=out..(((self[y][x]>0) and ALIVE) or DEAD)\n"
"    end\n"
"    out=out..\" \\ n \"\n"
"  end\n"
"  write(out)\n"
"end\n\n"
"-- constructor\n"
"function CELLS(w,h)\n"
"  local c = ARRAY2D(w,h)\n"
"  c.spawn = _CELLS.spawn\n"
"  c.evolve = _CELLS.evolve\n"
"  c.draw = _CELLS.draw\n"
"  return c\n"
"end\n\n"
"--\n"
"-- shapes suitable for use with spawn() above\n"
"--\n"
"HEART = { 1,0,1,1,0,1,1,1,1; w=3,h=3 }\n"
"GLIDER = { 0,0,1,1,0,1,0,1,1; w=3,h=3 }\n"
"EXPLODE = { 0,1,0,1,1,1,1,0,1,0,1,0; w=3,h=4 }\n"
"FISH = { 0,1,1,1,1,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0; w=5,h=4 }\n"
"BUTTERFLY = { 1,0,0,0,1,0,1,1,1,0,1,0,0,0,1,1,0,1,0,1,1,0,0,0,1; w=5,h=5 }\n\n"
"-- the main routine\n"
"function LIFE(w,h)\n"
"  -- create two arrays\n"
"  local thisgen = CELLS(w,h)\n"
"  local nextgen = CELLS(w,h)\n\n"
"-- create some life\n"
"  -- about 1000 generations of fun, then a glider steady-state\n"
"  thisgen:spawn(GLIDER,5,4)\n"
"  thisgen:spawn(EXPLODE,25,10)\n"
"  thisgen:spawn(FISH,4,12)\n\n"
"-- run until break\n"
"  local gen=1\n"
"  write(\"\027[2J\")	-- ANSI clear screen\n"
"while 1 do\n"
"    thisgen:evolve(nextgen)\n"
"    thisgen,nextgen = nextgen,thisgen\n"
"    write(\"\027[H\")	-- ANSI home cursor\n"
"thisgen:draw()\n"
"    write(\"Life - generation \",gen,\"\ n\")\n"
"    gen=gen+1\n"
"    if gen>2000 then break end\n"
"    --delay()		-- no delay\n"
"  end\n"
"end\n\n"
"LIFE(40,20)";
//////////////////////////////////////////////////////////////
//
//	Code snippets
static char Codesnippets[1024*16] = 	"Copy & paste us\n\n"
										"-----	<-Comment\n\n"
										"io.write(\" \")\n\n";
										
//////////////////////////////////////////////////////////////
static char Name1[100];
static char Name2[100];
static char Name3[100];
static char Name4[100];
static char Name5[100];
static int selectedsource = -1;
static int selectedworkspace = -1;
static int selectedworkspaceloader = -1;
static int selectedworkspacesaver = -1;
static char new_wizard_selection[32];
static char new_wizard_selection_workspace[32];

//////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
//
//	Workspace window create with or without source.
//
if (accept_source0_workspace1)
{
ImGui::Begin("##Template0Workspace1Window", &show_editor_window1);
ImGui::InputTextMultiline("##Template0Workspace1", Template0, IM_ARRAYSIZE(Template0), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput);
ImGui::Text("");ImGui::SameLine(ImGui::GetWindowContentRegionWidth()/2);if(ImGui::Button("Close")){accept_source0_workspace1 = false;}
ImGui::End();
accept_source1_workspace1 = false;
accept_source2_workspace1 = false;
}
if (accept_source1_workspace1)
{
ImGui::Begin("##Template1Workspace1Window", &show_editor_window1);
ImGui::InputTextMultiline("##Template1Workspace1", Template1, IM_ARRAYSIZE(Template1), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput);
ImGui::Text("");ImGui::SameLine(ImGui::GetWindowContentRegionWidth()/2);if(ImGui::Button("Close")){accept_source1_workspace1 = false;}
ImGui::End();
accept_source0_workspace1 = false;
accept_source2_workspace1 = false;
}
if (accept_source2_workspace1)
{
ImGui::Begin("##Template2Workspace1Window", &show_editor_window1);
ImGui::InputTextMultiline("##Template2Workspace1", Template2, IM_ARRAYSIZE(Template2), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput);
ImGui::Text("");ImGui::SameLine(ImGui::GetWindowContentRegionWidth()/2);if(ImGui::Button("Close")){accept_source2_workspace1 = false;}
ImGui::End();
accept_source1_workspace1 = false;
accept_source0_workspace1 = false;
}
if (accept_source0_workspace2)
{
ImGui::Begin("##Template0Workspace2Window", &show_editor_window1);
ImGui::InputTextMultiline("##Template0Workspace2", Template0, IM_ARRAYSIZE(Template0), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput);
ImGui::Text("");ImGui::SameLine(ImGui::GetWindowContentRegionWidth()/2);if(ImGui::Button("Close")){accept_source0_workspace2 = false;}
ImGui::End();
accept_source1_workspace2 = false;
accept_source2_workspace2 = false;
}
if (accept_source1_workspace2)
{
ImGui::Begin("##Template1Workspace2Window", &show_editor_window1);
ImGui::InputTextMultiline("##Template1Workspace2", Template1, IM_ARRAYSIZE(Template1), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput);
ImGui::Text("");ImGui::SameLine(ImGui::GetWindowContentRegionWidth()/2);if(ImGui::Button("Close")){accept_source1_workspace2 = false;}
ImGui::End();
accept_source0_workspace2 = false;
accept_source2_workspace2 = false;
}
if (accept_source2_workspace2)
{
ImGui::Begin("##Template2Workspace2Window", &show_editor_window1);
ImGui::InputTextMultiline("##Template2Workspace2", Template2, IM_ARRAYSIZE(Template2), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput);
ImGui::Text("");ImGui::SameLine(ImGui::GetWindowContentRegionWidth()/2);if(ImGui::Button("Close")){accept_source2_workspace2 = false;}
ImGui::End();
accept_source1_workspace2 = false;
accept_source0_workspace2 = false;
}
if (accept_source0_workspace3)
{
ImGui::Begin("##Template0Workspace3Window", &show_editor_window1);
ImGui::InputTextMultiline("##Template0Workspace3", Template0, IM_ARRAYSIZE(Template0), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput);
ImGui::Text("");ImGui::SameLine(ImGui::GetWindowContentRegionWidth()/2);if(ImGui::Button("Close")){accept_source0_workspace3 = false;}
ImGui::End();
accept_source1_workspace3 = false;
accept_source2_workspace3 = false;
}
if (accept_source1_workspace3)
{
ImGui::Begin("##Template1Workspace3Window", &show_editor_window1);
ImGui::InputTextMultiline("##Template1Workspace3", Template1, IM_ARRAYSIZE(Template1), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput);
ImGui::Text("");ImGui::SameLine(ImGui::GetWindowContentRegionWidth()/2);if(ImGui::Button("Close")){accept_source1_workspace3 = false;}
ImGui::End();
accept_source0_workspace3 = false;
accept_source2_workspace3 = false;
}
if (accept_source2_workspace3)
{
ImGui::Begin("##Template2Workspace3Window", &show_editor_window1);
ImGui::InputTextMultiline("##Template2Workspace3", Template2, IM_ARRAYSIZE(Template2), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput);
ImGui::Text("");ImGui::SameLine(ImGui::GetWindowContentRegionWidth()/2);if(ImGui::Button("Close")){accept_source2_workspace3 = false;}
ImGui::End();
accept_source1_workspace3 = false;
accept_source0_workspace3 = false;
}
/////////////////////////////////////////////////////////////
//
//	Editor and other windows
//
////////////////////////////////////////////////////////////
			
		
		show_editor_window 	= true;
		if (show_editor_window0)
		{
			ImGui::Begin("Welcome screen", &show_editor_window0);
			ImGui::Text("");
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth()/2 -90);
			ImGui::Image((void *)Editor, ImVec2(200, 200), ImVec2(0,0),ImVec2(1,1),ImVec4(255,255,255,255),ImVec4(0,0,0,0));
			ImGui::Separator();
			ImGui::InputTextMultiline("##source1", text1, IM_ARRAYSIZE(text1), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly);
            ImGui::End();
		}
		if (show_editor_window)
		{
			ImGui::Begin("Editor Tools", &show_editor_window);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::BeginChild("####menu", ImVec2(-1,35), true,ImGuiWindowFlags_NoScrollbar);
            
			if(ImGui::ImageButton((void *)filenew, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){show_new_wizard = true;}
            if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("New");ImGui::EndTooltip();}
			
			ImGui::SameLine();
			if(ImGui::ImageButton((void *)fileopen, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){show_open = true;}
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Open");ImGui::EndTooltip();}
			
			ImGui::SameLine();
			if(ImGui::ImageButton((void *)filesave, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){show_save = true;}
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Save");ImGui::EndTooltip();}
			
			ImGui::SameLine();
			if(ImGui::ImageButton((void *)IO, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){show_IO = true;}
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Input/Output options");ImGui::EndTooltip();}
			
			ImGui::SameLine();
			if (ImGui::ImageButton((void *)batch, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){show_batch = true;}
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Run Batch -\n Moves last saved and compiler output\n to desired folders");ImGui::EndTooltip();}
			if(show_batch == true){static ImGuiOnceUponAFrame once;if (once){system ("cmd");show_batch = false;}}
			
			
			ImGui::SameLine();
			if (ImGui::ImageButton((void *)compile, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){show_compiler = true;}
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Compile");ImGui::EndTooltip();}
			if(show_compiler == true){static ImGuiOnceUponAFrame once;if (once){system ("cmd");show_compiler = false;}}
			
			
			ImGui::SameLine();
			if (ImGui::ImageButton((void *)run, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){show_run = true;}
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Run");ImGui::EndTooltip();}
			if(show_run == true){static ImGuiOnceUponAFrame once;if (once){system ("lua");show_run = false;}}
			
			ImGui::SameLine();
			if(ImGui::ImageButton((void *)settings, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){show_options = true;}
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Settings");ImGui::EndTooltip();}
			
			ImGui::SameLine();
			if (ImGui::ImageButton((void *)source, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255)))
			{show_about_editor = true;}
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("About");ImGui::EndTooltip();}
			
			ImGui::SameLine();
			if(ImGui::ImageButton((void *)help, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){show_help_support = true;}
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Help & Support");ImGui::EndTooltip();}
			
			ImGui::EndChild();
            ImGui::PopStyleVar();
            ImGui::Separator();
            ImGui::Text("Code Snippets");
            ImGui::Separator();
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::BeginChild("####codesnippets", ImVec2(-1,-1), true,ImGuiWindowFlags_NoScrollbar);
            ImGui::InputTextMultiline("##CodeCopyArea", Codesnippets, IM_ARRAYSIZE(Codesnippets), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput);
			
			ImGui::EndChild();
            ImGui::PopStyleVar();
            ImGui::End();
		}
		if (show_new_wizard)
		{
			ImGui::Begin("New source wizard", &show_new_wizard);
			ImGui::Text("Choose a Prefab & Work space.");
			ImGui::Separator();
            ImGui::Text("Prefabs:");
            ImGui::Separator();
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::BeginChild("####newwizard", ImVec2(-1,100), true,ImGuiWindowFlags_NoScrollbar);
            sprintf(new_wizard_selection, "Blank", 0);
            if (ImGui::Selectable(new_wizard_selection, selectedsource == 0 )){selectedsource = 0;} 
            sprintf(new_wizard_selection, "Hello world", 1);
            if (ImGui::Selectable(new_wizard_selection, selectedsource == 1 )){selectedsource = 1;} 
            sprintf(new_wizard_selection, "Life -by Dave Bollinger", 2);
            if (ImGui::Selectable(new_wizard_selection, selectedsource == 2 )){selectedsource = 2;} 
            
			ImGui::EndChild();
            ImGui::PopStyleVar();
            ImGui::Separator();
            ImGui::Text("Work Space: Use for multiple.");
            ImGui::Separator();
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::BeginChild("####newwizardworkspace", ImVec2(-1,100), true,ImGuiWindowFlags_NoScrollbar);
            sprintf(new_wizard_selection_workspace, "Work space #1", 1);
            if (ImGui::Selectable(new_wizard_selection_workspace, selectedworkspace == 1 )){selectedworkspace = 1;} 
            sprintf(new_wizard_selection_workspace, "Work space #2", 2);
            if (ImGui::Selectable(new_wizard_selection_workspace, selectedworkspace == 2 )){selectedworkspace = 2;} 
            sprintf(new_wizard_selection_workspace, "Work space #3", 3);
            if (ImGui::Selectable(new_wizard_selection_workspace, selectedworkspace == 3 )){selectedworkspace = 3;} 
   			ImGui::EndChild();
            ImGui::PopStyleVar();
            ImGui::Separator();
            if (ImGui::Button("Cancel",  ImVec2(50,0))) {  show_new_wizard = false; }
            ImGui::SameLine(ImGui::GetWindowContentRegionWidth()-40,-1);
            if (ImGui::Button("Ok", ImVec2(50,0))) 
			{
				static ImGuiOnceUponAFrame once;
				if (once)
				{
				if (selectedworkspace == 1 && selectedsource == 0){accept_source0_workspace1	= true;accept_source1_workspace1	= false;accept_source2_workspace1	= false;}
				if (selectedworkspace == 1 && selectedsource == 1){accept_source1_workspace1	= true;accept_source0_workspace1	= false;accept_source2_workspace1	= false;}
				if (selectedworkspace == 1 && selectedsource == 2){accept_source2_workspace1	= true;accept_source0_workspace1	= false;accept_source1_workspace1	= false;}
				if (selectedworkspace == 2 && selectedsource == 0){accept_source0_workspace2	= true;accept_source1_workspace2	= false;accept_source2_workspace2	= false;}
				if (selectedworkspace == 2 && selectedsource == 1){accept_source1_workspace2	= true;accept_source0_workspace2	= false;accept_source2_workspace2	= false;}
				if (selectedworkspace == 2 && selectedsource == 2){accept_source2_workspace2	= true;accept_source0_workspace2	= false;accept_source1_workspace2	= false;}
				if (selectedworkspace == 3 && selectedsource == 0){accept_source0_workspace3	= true;accept_source1_workspace3	= false;accept_source2_workspace3	= false;}
				if (selectedworkspace == 3 && selectedsource == 1){accept_source1_workspace3	= true;accept_source0_workspace3	= false;accept_source2_workspace3	= false;}
				if (selectedworkspace == 3 && selectedsource == 2){accept_source2_workspace3	= true;accept_source0_workspace3	= false;accept_source1_workspace3	= false;}
				
				}
			}
            ImGui::End();
		}
		if (show_open)
		{
			ImGui::Begin("Open", &show_open);
			ImGui::Text("To open a file\n"
			"place script in the same location as LUA.Editor.exe\n\n"
			"Choose a Work space to load the script into.");
			ImGui::Separator();
			static char load_file[40] = "Type filename here...";    // setup text filtering, could be in latest update.
            string load_name = load_file;							
			ImGui::RadioButton("Workspace #1", &selectedworkspaceloader, 0);ImGui::SameLine();ImGui::RadioButton("Workspace #2", &selectedworkspaceloader, 1);ImGui::SameLine();ImGui::RadioButton("Workspace #3", &selectedworkspaceloader, 2);
			ImGui::Separator();
			ImGui::InputText("#####open", load_file, IM_ARRAYSIZE(load_file), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
            ImGui::Separator();
			ImGui::Button("Open");
			ImGui::End();
		}
		
		if (show_save)
		{
			ImGui::Begin("Save as", &show_save);
			ImGui::Text("Type a save filename\n"
			"Saved to the same location as LUA.Editor.exe\n\n"
			"Choose a Workspace to save.\n");
			ImGui::Separator();
			static char save_file[40] = "Type filename here...";    // setup text filtering, could be in latest update.
            string save_name = save_file;							
			ImGui::RadioButton("Workspace #1", &selectedworkspacesaver, 0);ImGui::SameLine();ImGui::RadioButton("Workspace #2", &selectedworkspacesaver, 1);ImGui::SameLine();ImGui::RadioButton("Workspace #3", &selectedworkspacesaver, 2);
			ImGui::Separator();
			ImGui::InputText("####save", save_file, IM_ARRAYSIZE(save_file), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
            ImGui::Separator();
			ImGui::Button("Save");
			ImGui::End();
		}
		if (show_IO)
		{
			ImGui::Begin("Input/Output options", &show_IO);
			ImGui::Text("Type in the selected fields\n"
						"To alter the IO options\n\n"
						"!NO BLANK SPACES IN DIR!\n"
						"!Run batch file when saved or complation complete!");
			ImGui::Separator();
			static char save_Input[40] = "C:\\%DIR%";    // setup text filtering, could be in latest update.
            string save_IO_input = save_Input;
			ImGui::Text("Input script folder");
			ImGui::SameLine(200);							
			ImGui::InputText("###Input", save_Input, IM_ARRAYSIZE(save_Input), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
            ImGui::SameLine();
            ImGui::Button("Save");
			ImGui::Separator();
            static char save_Output_saved[40] = "C:\\%DIR%";    // setup text filtering, could be in latest update.
            string saved_IO_Output = save_Output_saved;							
			ImGui::Text("Output script save folder");
			ImGui::SameLine(200);							
			ImGui::InputText("###save_Output_saved", save_Output_saved, IM_ARRAYSIZE(save_Output_saved), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
            ImGui::SameLine();
            ImGui::Button("Save");
			ImGui::Separator();
			static char save_Output_complation[40] = "C:\\%DIR%";    // setup text filtering, could be in latest update.
            string save_Output_compile_IO = save_Output_complation;							
			ImGui::Text("Compiler output folder");
			ImGui::SameLine(200);							
			ImGui::InputText("###save_Output_complation", save_Output_complation, IM_ARRAYSIZE(save_Output_complation), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
            ImGui::SameLine();
            ImGui::Button("Save");
			ImGui::Separator();
			ImGui::End();
		}
		if (show_options)
		{
			ImGui::Begin("Settings", &show_options);
			ImGui::ShowStyleEditor();
			ImGui::ShowMetricsWindow();
			ImGui::End();
		}
		if (show_about_editor)
		{	
			ImGui::Begin("About", &show_about_editor);
            ImGui::Text("");
            ImGui::Text("Dear ImGui, %s", ImGui::GetVersion());
            ImGui::Text("By Omar Cornut and all dear imgui contributors.");
        	ImGui::Text("Dear ImGui is licensed under the MIT License, see LICENSE for more information.");
        	ImGui::Text("");
			ImGui::Text("LUA Editor by Luke Hays");
            ImGui::Text("");
			ImGui::Text("Written in C++, using Dev ++ with the TDM Mingw64 compiler.");
            ImGui::Text("Useing IMGUI API.");
            ImGui::Text("");
            ImGui::End();
        }
        if(show_help_support)
		{
			ImGui::Begin("Help & Support", &show_help_support,ImGuiWindowFlags_AlwaysHorizontalScrollbar);
			ImGui::Text("LUA.Editor\n\n"
			"Simply save a workspace to compile or run.");
			ImGui::Separator();
			ImGui::Text("LUA interpreter\n\n");
			ImGui::Text("lua is the stand-alone Lua interpreter. It loads and executes Lua programs, either in textual source form or in precompiled binary form.\n" 
			"(Precompiled binaries are output by luac, the Lua compiler.) lua can be used as a batch interpreter and also interactively.\n\n"
			"The given options (see below) are executed and then the Lua program in file script is loaded and executed. The given args are available to script as strings in a global table named arg.\n" 
			"If these arguments contain spaces or other characters special to the shell, then they should be quoted (but note that the quotes will be removed by the shell).\n" 
			"The arguments in arg start at 0, which contains the string `script'. The index of the last argument is stored in \" arg.n \". The arguments given in the command line before script, including the name of the interpreter, are available in negative indices in arg.\n\n"
			"At the very start, before even handling the command line, lua executes the contents of the environment variable LUA_INIT, if it is defined. If the value of LUA_INIT is of the form `@filename', then filename is executed. Otherwise, the string is assumed to be a Lua statement and is executed.\n\n"
			"Options start with - and are described below. You can use \"--\" to signal the end of options.\n\n"
			"If no arguments are given, then \"-v -i\" is assumed when the standard input is a terminal; otherwise, \"-\" is assumed.\n\n"
			"In interactive mode, lua prompts the user, reads lines from the standard input, and executes them as they are read. If a line does not contain a complete statement, then a secondary prompt is displayed and lines are read until a complete statement is formed or a syntax error is found.\n" 
			"So, one way to interrupt the reading of an incomplete statement is to force a syntax error: adding a `;' in the middle of a statement is a sure way of forcing a syntax error (except inside multiline strings and comments; these must be closed explicitly).\n" 
			"If a line starts with `=', then lua displays the values of all the expressions in the remainder of the line. The expressions must be separated by commas. The primary prompt is the value of the global variable _PROMPT, if this value is a string; otherwise, the default prompt is used.\n" 
			"Similarly, the secondary prompt is the value of the global variable _PROMPT2. So, to change the prompts, set the corresponding variable to a string of your choice. You can do that after calling the interpreter or on the command line with \"_PROMPT\" \"=\'lua: \'\", for example.\n" 
			"(Note the need for quotes, because the string contains a space.) The default prompts are ``> '' and ``>> ''.\" \n\n"
			"OPTIONS\n\n"
			" - load and execute the standard input as a file, that is, not interactively, even when the standard input is a terminal.\n\n"
			"-e \"stat\" execute statement stat. You need to quote stat if it contains spaces, quotes, or other characters special to the shell.\n\n"
			"-i enter interactive mode after script is executed.\n\n"
			"-l \"file\" call require( file) before executing script. Typically used to load libraries (hence the letter l).\n\n"
			"-v show version information.\n\n");
			ImGui::Text("SYNOPSIS\n\n"
						"lua [ options ] [ script [ args ] ]");
			ImGui::Separator();
			ImGui::Text("To use simply type 'luac' in the compiler console.\n\n");
			ImGui::Text("LUAC Compiler\n\n");
			ImGui::Text("luac is the Lua compiler. It translates programs written in the Lua programming language into binary files that can be latter loaded and executed.\n\n"
						"The main advantages of precompiling chunks are: faster loading, protecting source code from user changes, and off-line syntax checking.\n\n"
						"Pre-compiling does not imply faster execution because in Lua chunks are always compiled into bytecodes before being executed. luac simply allows those bytecodes to be saved in a file for later execution.\n\n"
						"luac produces a single output file containing the bytecodes for all source files given. By default, the output file is named luac.out, but you can change this with the -o option.\n\n"
						"The binary files created by luac are portable to all architectures with the same word size. This means that binary files created on a 32-bit platform (such as Intel) can be read without change in another 32-bit platform (such as Sparc),\n" 
						"even if the byte order (``endianness'') is different. On the other hand, binary files created on a 16-bit platform cannot be read in a 32-bit platform, nor vice-versa.\n\n"
						"In the command line, you can mix text files containing Lua source and binary files containing precompiled chunks. This is useful to combine several precompiled chunks, even from different (but compatible) platforms, into a single precompiled chunk.\n\n"
						"You can use \"-\" to indicate the standard input as a source file and \"--\" to signal the end of options (that is, all remaining arguments will be treated as files even if they start with \"-\").\n\n"
						"The internal format of the binary files produced by luac is likely to change when a new version of Lua is released. So, save the source files of all Lua programs that you precompile.\n\n"
						"OPTIONS\n\n"
						"Options must be separate.\n\n"
						"-l produce a listing of the compiled bytecode for Lua's virtual machine. Listing bytecodes is useful to learn about Lua's virtual machine. If no files are given, then luac loads luac.out and lists its contents.\n\n"
						"-o \"file\" output to file, instead of the default luac.out. The output file may be a source file because all files are loaded before the output file is written. Be careful not to overwrite precious files.\n\n"
						"-p load files but do not generate any output file. Used mainly for syntax checking and for testing precompiled chunks: corrupted files will probably generate errors when loaded.\n" 
						"Lua always performs a thorough integrity test on precompiled chunks. Bytecode that passes this test is completely safe, in the sense that it will not break the interpreter. However, there is no guarantee that such code does anything sensible.\n" 
						"(None can be given, because the halting problem is unsolvable.) If no files are given, then luac loads luac.out and tests its contents. No messages are displayed if the file passes the integrity test.\n\n"
						"-s strip debug information before writing the output file. This saves some space in very large chunks, but if errors occur when running these chunks, then the error messages may not contain the full information they usually do (line numbers and names of locals are lost).\n\n"
						"-v show version information.\n\n");
			ImGui::Text("SYNOPSIS\n\n"
					"luac [ options ] [ filenames ]");
			ImGui::End();
		}
////////////////////////////////////////////////////////////
//
// Rendering
//
///////////////////////////////////////////////////////////
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*255.0f), (int)(clear_color.y*255.0f), (int)(clear_color.z*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
        {
            ImGui_ImplDX9_InvalidateDeviceObjects();
            g_pd3dDevice->Reset(&g_d3dpp);
            ImGui_ImplDX9_CreateDeviceObjects();
        }
    }
	Shutdown:
    ImGui_ImplDX9_Shutdown();
    if (g_pd3dDevice) g_pd3dDevice->Release();
    if (pD3D) pD3D->Release();
    UnregisterClass(_T("ImGui Example"), wc.hInstance);

    return 0;
}
