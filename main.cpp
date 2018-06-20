// ImGui - standalone example application for DirectX 9
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.



#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
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

/////////////////////////////////////////////////////////
//
//	Boost file system conflict with std library
// 	Should of seen that comming :P
//
//	Header contains file open/save ^_^'
//	This means rewriting load/save code.
//
//	#include <boost/filesystem.hpp>
//	using namespace boost::filesystem;
//
///////////////////////////////////////////////////////
//
//	Boost::Asio
//
//	#include <boost/asio/ip/tcp.hpp>
//
//	Basic networking component, it handles tcp/ip.
//	also supoort for other socket protocols 
//	(such as Bluetooth or IRCOMM sockets)
//
//	This will get me HTTP 1.0 support (Text)
//	Below is the full http_client source code.
//
//	Slowly reworking this into my Minimal browser.
//	
//	UPDATE 1
//
//	I have the entire boost library built.
//	Said library works. I have got sockets =)!!
//	
// Grabbed Cef binaries and turned the .libs
//	For the DLL shared linkers to .A format using reimp
//	This will give me HTML 5!! suppprt.
//
//
/////////////////////////////////////////////////////////////
//
//	Threading resources & calls
//
//
void cmd() 
{
system("cmd");
}
void lua() 
{
system("lua");
}
void ipconfigure_bat() 
{
system("ipconfigure.bat");
}
void ipconfigure_netstat_on_bat() 
{
system("ipconfigure.netstat.on.bat");
}
void ipconfigure_netstat_a_bat() 
{
system("ipconfigure.netstat.a.bat");
}
void ipconfigure_netstat_o_bat() 
{
system("ipconfigure.netstat.o.bat");
}
void ipconfigure_netstat_t_bat() 
{
system("ipconfigure.netstat.t.bat");
}
void ipconfigure_netstat_f_bat() 
{
system("ipconfigure.netstat.f.bat");
}
void ipconfigure_wmic_bat() 
{
system("ipconfigure.wmic.bat");
}
void ipconfigure_wmic_help_bat() 
{
system("ipconfigure.wmic.help.bat");
}
/////////////////////////////////////////////////////////////
//using boost::asio::ip::tcp;
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
//	Window bools.
//
////////////////////////////////////////////////////////////
bool show_editor_window 		= true;			// Editor options selector
bool show_welcome				= true;			// Welcome screen/notes/changes loads out of "while"
bool show_about_editor			= false;		// Blurb
bool show_new_wizard			= false;		// Uses prefabs via loading
bool show_compiler				= false;		// Standard cmd call
bool show_run					= false;		// Runs Lua
bool show_save					= false;		// Fully func
bool show_open					= false;		// Fully func
bool show_help_support			= false;		// implement in program web browser?
bool show_options				= false;		// Uses inbuilt imgui call
bool show_IO					= false;		// Fully func
bool show_batch					= false;		// Fully func
bool show_prefab_list			= false;		// Ease of grabbing test scripts
bool show_save_complete			= false;		// Clickable
bool load_snippets				= true;			// Saveable / Loading called out of "while"
bool show_right_click_menu		= false;		// WIP Feature
bool show_guiopen				= false;		// WIP Open file explorer.
bool show_guisave				= false;		// WIP Save file explorer.
bool show_inprogram_dbg			= false;		// Debugger for web browser.
////////////////////////////////////////////////////////////
//
// Web browser bool & functionality.
//
//
bool show_web_browser			= false;		// WIP beast locksup my IDE! / ASIO seems to work.
bool show_web_settings			= false;		// Homepage/Downloads location/ssl/ftp/others
bool save_ftp_login				= false;		// Saved as binary fileformat. STILL WIP
bool mobile_html				= false;		// Mobile phone HTML only. look into me more.
bool status_empty				= true;			// Connection status indicators.
bool status_red					= false;		// "
bool status_yellow				= false;		// "
bool status_green				= false;		// "
bool save_cloud_login			= false;		// Saved as binary fileformat. STILL WIP
bool show_download_manager		= false;		// Used for upstream/downstream progress.
bool show_bookmarks_saved		= false;		// Displays user defined bookmarks

static char bookmarks_kept[1024*16];			// Bookmarks for browser.


static ImGuiOnceUponAFrame Browser_settings;	// Bookmarks & browser settings.
				if (Browser_settings)			// Will have to use my regex load here to parse data.
					{
            			ifstream file("Bookmarks.setting", ios::in|ios::ate);
            			if (file.is_open())
            				{						
							streampos size;			
					  		size = file.tellg();
							file.seekg (0, ios::beg);
							file.read (bookmarks_kept, size);
							file.close();
							}
					}

////////////////////////////////////////////////////////////
//
// Windows and arrays.
//
bool Work_space_one				= false;
bool Work_space_two				= false;
bool Work_space_three			= false;

static char Workspace1[1024*16];
static char Workspace2[1024*16];
static char Workspace3[1024*16];
////////////////////////////////////////////////////////////
//
// Welcome details.
//
static char Welcome[1024*16];
static ImGuiOnceUponAFrame once;
				if (once)
					{
            			ifstream file("Details.txt", ios::in|ios::ate);
            			if (file.is_open())
            				{						
							streampos size;			
					  		size = file.tellg();
							file.seekg (0, ios::beg);
							file.read (Welcome, size);
							file.close();
							}
					}
////////////////////////////////////////////////////////////
//
//	Copy.bat.
//
static char luaDIR[1024] = "copy *.lua ";
static char outDIR[1024] = "copy *.out ";
/////////////////////////////////////////////////////////////
//
// load selection.
//
bool load_selected_one			= false;
bool load_selected_two			= false;
bool load_selected_three		= false;
/////////////////////////////////////////////////////////////
//
// Save selection.
//
bool save_selected_one			= false;
bool save_selected_two			= false;
bool save_selected_three		= false;

/////////////////////////////////////////////////////////////
//
//	RGBA colour for screen.
//
ImVec4 clear_color = ImVec4(0.073f, 0.115f, 0.177f, 1.000f);
//////////////////////////////////////////////////////////////
//
//	Code snippets.
//
static char Codesnippets[1024*16];

static ImGuiOnceUponAFrame OnceAgain;
				if (OnceAgain)
					{
            			ifstream file("Snippets.txt", ios::in|ios::ate);
            			if (file.is_open())
            				{						
							streampos size;			
					  		size = file.tellg();
							file.seekg (0, ios::beg);
							file.read (Codesnippets, size);
							file.close();
							}
					}
////////////////////////////////////////////////////////////////////////////////
//
//	ICONS.
//
///////////////////////////////////////////////////////////////
static LPDIRECT3DTEXTURE9       filenew 		= NULL;		// image idents here. here =)
static LPDIRECT3DTEXTURE9       fileopen 		= NULL;
static LPDIRECT3DTEXTURE9       filesave 		= NULL;
static LPDIRECT3DTEXTURE9       settings 		= NULL;
static LPDIRECT3DTEXTURE9       IO 				= NULL;
static LPDIRECT3DTEXTURE9       compile 		= NULL;
static LPDIRECT3DTEXTURE9       run 			= NULL;
static LPDIRECT3DTEXTURE9       source 			= NULL;
static LPDIRECT3DTEXTURE9       help 			= NULL;
static LPDIRECT3DTEXTURE9       Editor 			= NULL;
static LPDIRECT3DTEXTURE9       batch 			= NULL;
static LPDIRECT3DTEXTURE9       up 				= NULL;
static LPDIRECT3DTEXTURE9       web 			= NULL;
static LPDIRECT3DTEXTURE9       home 			= NULL;
static LPDIRECT3DTEXTURE9       back 			= NULL;
static LPDIRECT3DTEXTURE9       forward 		= NULL;
static LPDIRECT3DTEXTURE9       stop	 		= NULL;
static LPDIRECT3DTEXTURE9       downloads		= NULL;
static LPDIRECT3DTEXTURE9       bookmarks		= NULL;
static LPDIRECT3DTEXTURE9       statusempty		= NULL;
static LPDIRECT3DTEXTURE9       statusred		= NULL;
static LPDIRECT3DTEXTURE9       statusyellow	= NULL;
static LPDIRECT3DTEXTURE9       statusgreen		= NULL;
static LPDIRECT3DTEXTURE9       pause			= NULL;
static LPDIRECT3DTEXTURE9       resume			= NULL;


D3DXCreateTextureFromFile( g_pd3dDevice, "filenew.png" ,	 &filenew );			// image file grabs here :)
D3DXCreateTextureFromFile( g_pd3dDevice, "fileopen.png" ,	 &fileopen );
D3DXCreateTextureFromFile( g_pd3dDevice, "filesave.png" ,	 &filesave );
D3DXCreateTextureFromFile( g_pd3dDevice, "mechanical.png" ,	 &settings );
D3DXCreateTextureFromFile( g_pd3dDevice, "folder.png" ,		 &IO );
D3DXCreateTextureFromFile( g_pd3dDevice, "compile.png" ,	 &compile );
D3DXCreateTextureFromFile( g_pd3dDevice, "source.png" ,		 &source );
D3DXCreateTextureFromFile( g_pd3dDevice, "compilerun.png" ,	 &run );
D3DXCreateTextureFromFile( g_pd3dDevice, "runbatch.png" ,	 &batch );
D3DXCreateTextureFromFile( g_pd3dDevice, "addition.png" ,	 &help );
D3DXCreateTextureFromFile( g_pd3dDevice, "LUA.Editor.png" ,	 &Editor );
D3DXCreateTextureFromFile( g_pd3dDevice, "up.png" ,	 		 &up );
D3DXCreateTextureFromFile( g_pd3dDevice, "web.png" ,	 	 &web );
D3DXCreateTextureFromFile( g_pd3dDevice, "home.png" ,	 	 &home );
D3DXCreateTextureFromFile( g_pd3dDevice, "back.png" ,	 	 &back );
D3DXCreateTextureFromFile( g_pd3dDevice, "forward.png" ,	 &forward );
D3DXCreateTextureFromFile( g_pd3dDevice, "stop.png" ,		 &stop );
D3DXCreateTextureFromFile( g_pd3dDevice, "downloads.png" ,	 &downloads );
D3DXCreateTextureFromFile( g_pd3dDevice, "bookmarks.png" ,	 &bookmarks );
D3DXCreateTextureFromFile( g_pd3dDevice, "statusempty.png" , &statusempty );
D3DXCreateTextureFromFile( g_pd3dDevice, "statusred.png" , 	 &statusred );
D3DXCreateTextureFromFile( g_pd3dDevice, "statusyellow.png" ,&statusyellow );
D3DXCreateTextureFromFile( g_pd3dDevice, "statusgreen.png" , &statusgreen );
D3DXCreateTextureFromFile( g_pd3dDevice, "pause.png" , 		 &pause );
D3DXCreateTextureFromFile( g_pd3dDevice, "run.png" , 		 &resume );
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
//	Mouse WIP COPY/PASTE.
//

static char right_click_selection[40];
static int right_menu_selection = -1;

		if(ImGui::IsMouseClicked( 1,true))
		{
				static ImGuiOnceUponAFrame fourth;
				if (fourth)
				{
					show_right_click_menu = true;
				}
		}
		if(ImGui::IsMouseClicked(0,true))
		{
				static ImGuiOnceUponAFrame fourth;
				if (fourth)
				{
					show_right_click_menu = false;
				}
		}
		if(show_right_click_menu)
		{
			ImVec2 base_pos = ImGui::GetMousePos();
			ImGui::SetNextWindowPos(ImVec2(base_pos.x,base_pos.y),ImGuiCond_Appearing);
			ImGui::SetNextWindowSize(ImVec2(100,105));
			ImGui::Begin("###rightmenu", &show_right_click_menu,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
			ImGui::BeginChild("####rightmenu", ImVec2(-1,-1), true,ImGuiWindowFlags_NoScrollbar);
            sprintf(right_click_selection, "  Copy", 1);
            if (ImGui::Selectable(right_click_selection, right_menu_selection == 1 ))
////////////////////////////////////////////////////////////////////
//
//	Still WIP instead of reinventing the wheel. Forceing CTRL+C hot.
//	Imgui`s built in copy function should pick the key combination up. 
//	Same for paste operation. CTRL+V hot.
//
//	However proveing maddeningly difficult!
//	Will have to post a issue note on github.
//
//	Can also achieve the same with Redo/Undo.
//
//	Keys CTRL+Y: redo & CTRL+Z: undo.
//
//	Failed attemps below BUT keeping for github question to Omar Cornut.

			{
			right_menu_selection = 1; //show_right_click_menu = false;
			//ImGui::GetIO().KeyCtrl == true and ImGuiKey_C == true;
			//ImGui::IsKeyDown(ImGuiKey_C) == true;
			//IsKeyPressedMap(ImGuiKey_C) = true; 
			//ImGuiKey_C == true;
			//ImGuiIO.KeyCtrl = true;ImGuiIO.KeysDown = ImGuiKey_C = true;
			}
			ImGui::Separator();
			sprintf(right_click_selection, "  Paste", 2);
            if (ImGui::Selectable(right_click_selection, right_menu_selection == 2 )){right_menu_selection = 2;} 
   			ImGui::Separator();
			sprintf(right_click_selection, "  Redo", 3);
            if (ImGui::Selectable(right_click_selection, right_menu_selection == 3 )){right_menu_selection = 3;} 
   			ImGui::Separator();
			sprintf(right_click_selection, "  Undo", 4);
            if (ImGui::Selectable(right_click_selection, right_menu_selection == 4 )){right_menu_selection = 4;} 
   			
			ImGui::EndChild();
			ImGui::End();
		}										
//////////////////////////////////////////////////////////////
//
//	New Wizard check boxes & selectables.
//
static char new_wizard_selection_workspace[40];
static int selectedworkspace = -1;

/////////////////////////////////////////////////////////////
//
//	Workspace window create. WIP Changing layout, adding menu bars.
//	
//	Workspace #1


if (Work_space_one)
{
ImGui::Begin("Workspace #1", &Work_space_one, ImGuiWindowFlags_MenuBar);
ImGui::InputTextMultiline("##Workspace1", Workspace1, IM_ARRAYSIZE(Workspace1), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput );
	if (ImGui::BeginMenuBar())
	{
	if (ImGui::BeginMenu("Options"))
		{
    		ImGui::MenuItem("  New", NULL);
    		if (ImGui::MenuItem("  Save", NULL)){show_save = true;}
    		if (ImGui::MenuItem("  Open", NULL)){show_open = true;}
    		ImGui::MenuItem("  Open recent", NULL);
    		if(ImGui::BeginMenu("  Insert"))
    		{
    			ImGui::MenuItem("Box comment", NULL);
    			ImGui::MenuItem("Time & Date", NULL);
    			
    			ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("  Edit"))
    		{
    			ImGui::MenuItem("Indent", NULL);
    			ImGui::MenuItem("Unindent", NULL);
    			
    			ImGui::EndMenu();
			}
    		ImGui::EndMenu();
		}
	ImGui::EndMenuBar();
	}
ImGui::End();
}

//	Workspace #2


if (Work_space_two)
{
ImGui::Begin("Workspace #2", &Work_space_two,ImGuiWindowFlags_MenuBar);
ImGui::InputTextMultiline("##Workspace2", Workspace2, IM_ARRAYSIZE(Workspace2), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput);
	if (ImGui::BeginMenuBar())
	{
	if (ImGui::BeginMenu("Options"))
		{
    		ImGui::MenuItem("  New", NULL);
    		if (ImGui::MenuItem("  Save", NULL)){show_save = true;}
    		if (ImGui::MenuItem("  Open", NULL)){show_open = true;}
    		ImGui::MenuItem("  Open recent", NULL);
    		if(ImGui::BeginMenu("  Insert"))
    		{
    			ImGui::MenuItem("Box comment", NULL);
    			ImGui::MenuItem("Time & Date", NULL);
    			
    			ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("  Edit"))
    		{
    			ImGui::MenuItem("Indent", NULL);
    			ImGui::MenuItem("Unindent", NULL);
    			
    			ImGui::EndMenu();
			}
    		ImGui::EndMenu();
		}
	ImGui::EndMenuBar();
	}
ImGui::End();
}

//	Workspace #3


if (Work_space_three)
{
ImGui::Begin("Workspace #3", &Work_space_three,ImGuiWindowFlags_MenuBar);
ImGui::InputTextMultiline("##Workspace3", Workspace3, IM_ARRAYSIZE(Workspace3), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput);
	if (ImGui::BeginMenuBar())
	{
	if (ImGui::BeginMenu("Options"))
		{
    		ImGui::MenuItem("  New", NULL);
    		if (ImGui::MenuItem("  Save", NULL)){show_save = true;}
    		if (ImGui::MenuItem("  Open", NULL)){show_open = true;}
    		ImGui::MenuItem("  Open recent", NULL);
    		if(ImGui::BeginMenu("  Insert"))
    		{
    			ImGui::MenuItem("Box comment", NULL);
    			ImGui::MenuItem("Time & Date", NULL);
    			
    			ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("  Edit"))
    		{
    			ImGui::MenuItem("Indent", NULL);
    			ImGui::MenuItem("Unindent", NULL);
    			
    			ImGui::EndMenu();
			}
    		ImGui::EndMenu();
		}
	ImGui::EndMenuBar();
	}
ImGui::End();
}

/////////////////////////////////////////////////////////////
//
//	Editor and other windows.
//
		
		show_editor_window 	= true; // Not close-able.
		
		if (show_welcome)
		{
			ImGui::Begin("Welcome screen", &show_welcome);
			ImGui::Text("");
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth()/2 -95);
			ImGui::Image((void *)Editor, ImVec2(200, 200), ImVec2(0,0),ImVec2(1,1),ImVec4(255,255,255,255),ImVec4(0,0,0,0));
			ImGui::Separator();
			ImGui::InputTextMultiline("##Welcome", Welcome, IM_ARRAYSIZE(Welcome), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly);
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
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Output options");ImGui::EndTooltip();}
			
			ImGui::SameLine();
			if (ImGui::ImageButton((void *)batch, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){show_batch = true;}
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Run Batch -\nMoves saved .lua/.out output\nto desired folders");ImGui::EndTooltip();}
			if(show_batch == true){static ImGuiOnceUponAFrame once;if (once){system ("Copy.bat");show_batch = false;}}
			
			
			ImGui::SameLine();
			if (ImGui::ImageButton((void *)compile, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){static ImGuiOnceUponAFrame once;if (once){boost::thread_group threads;threads.create_thread(cmd);}}
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Compile");ImGui::EndTooltip();}
			
			
			ImGui::SameLine();
			if (ImGui::ImageButton((void *)run, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){static ImGuiOnceUponAFrame once;if (once){boost::thread_group threads;threads.create_thread(lua);}}
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Run");ImGui::EndTooltip();}
			
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
			
			ImGui::SameLine();
			if(ImGui::ImageButton((void *)web, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){show_web_browser = true;}
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Web browser");ImGui::EndTooltip();}
			
			ImGui::EndChild();
            ImGui::PopStyleVar();
            ImGui::Separator();
            ImGui::Text("Code Snippets");
            ImGui::Separator();
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::BeginChild("####codesnippets", ImVec2(-1,-1), true,ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            ImGui::InputTextMultiline("##CodeCopyArea", Codesnippets, IM_ARRAYSIZE(Codesnippets), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput);
			ImGui::Text("");
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth()/2 -40);
			if(ImGui::Button("Save Snippets"))
			{
				ofstream save_data("Snippets.txt");
				if (save_data.is_open())
				{save_data << Codesnippets;save_data.close();show_save_complete = true;}
			}
			ImGui::EndChild();
            ImGui::PopStyleVar();
            ImGui::End();
       	}
		if (show_new_wizard)
		{
			ImGui::Begin("New wizard", &show_new_wizard);
			ImGui::Text("Choose Work space.");
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
            ImGui::Text("To select a premade script,\n"
						"Select a workspace use 'Open'\n"
						"from editor tools.");
            ImGui::Separator();
            if (ImGui::Button("Cancel",  ImVec2(50,0))) {  show_new_wizard = false; }
            ImGui::SameLine();
			if(ImGui::Button("Prefabs"))
			{
				show_open = true;
				show_prefab_list = true;
			}
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth()-40,-1);
            if (ImGui::Button("Ok", ImVec2(50,0))) 
			{
				static ImGuiOnceUponAFrame once;
				if (once)
				{
				if (selectedworkspace == 1){Work_space_one = true;}
				if (selectedworkspace == 2){Work_space_two = true;}
				if (selectedworkspace == 3){Work_space_three = true;}
				}
			}
			
            ImGui::End();
		}
		if (show_open)
		{
			ImGui::Begin("Open", &show_open);
			ImGui::Text("To open a file\n\n"
			"place script in the same location as LUA.Editor.exe\n\n"
			"Or navigate to the file using the UI button.\n"
			"Choose a Workspace to load the script into.\n\n"
			"Do not forget to add file extension.  .txt/.lua");
			ImGui::Separator();
			static char load_file[100] = ".lua";    // setup text filtering, could be in latest update.
            string load_name = load_file;
			ImGui::Checkbox("Workspace #1", &load_selected_one);
			ImGui::Checkbox("Workspace #2", &load_selected_two);
			ImGui::Checkbox("Workspace #3", &load_selected_three);					
			ImGui::Separator();
			ImGui::InputText("#####open", load_file, IM_ARRAYSIZE(load_file), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
            ImGui::Separator();
			if (ImGui::Button("Open"))
			{
				if (load_selected_one == true && Work_space_one	== true)
				{ifstream file(load_name.c_str(), ios::in|ios::ate);if (file.is_open()){streampos size;size = file.tellg();file.seekg (0, ios::beg);file.read (Workspace1, size);file.close();}}
			
				if (load_selected_two == true && Work_space_two	== true)
				{ifstream file(load_name.c_str(), ios::in|ios::ate);if (file.is_open()){streampos size;size = file.tellg();file.seekg (0, ios::beg);file.read (Workspace2, size);file.close();}}
				
				if (load_selected_three == true && Work_space_three == true)
				{ifstream file(load_name.c_str(), ios::in|ios::ate);if (file.is_open()){streampos size;size = file.tellg();file.seekg (0, ios::beg);file.read (Workspace3, size);file.close();}}
			}
			ImGui::SameLine();
			if(ImGui::Button("UI Open"))
			{show_guiopen = true;}
			ImGui::SameLine();
			if(ImGui::Button("Prefab"))
			{show_prefab_list = true;}
			ImGui::End();
		}
		
		if (show_save)
		{
			ImGui::Begin("Save as", &show_save);
			ImGui::Text("Type a save filename\n"
			"Saved to the same location as LUA.Editor.exe\n\n"
			"Or use the navigate UI to directly save to a location.\n"
			"Do not forget to add extension .txt/.lua\n\n"
			"Choose a Workspace to save.\n");
			ImGui::Separator();
			ImGui::Checkbox("Workspace #1", &save_selected_one);
			ImGui::Checkbox("Workspace #2", &save_selected_two);
			ImGui::Checkbox("Workspace #3", &save_selected_three);
			if (save_selected_one == true){save_selected_two = false;save_selected_three = false;}
			if (save_selected_two == true){save_selected_one = false;save_selected_three = false;}
			if (save_selected_three == true){save_selected_two = false;save_selected_one = false;}
			static char save_file[100] = "New_save.lua";    // setup text filtering, could be in latest update.
            string save_name = save_file;							
			ImGui::Separator();
			ImGui::InputText("####save", save_file, IM_ARRAYSIZE(save_file), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
            ImGui::Separator();
			if(ImGui::Button("Save"))
			{
				if(save_selected_one == true){ofstream save_data(save_name.c_str());if (save_data.is_open()){save_data << Workspace1;save_data.close(); show_save_complete = true;}}
				if(save_selected_two == true){ofstream save_data(save_name.c_str());if (save_data.is_open()){save_data << Workspace2;save_data.close();show_save_complete = true;}}
				if(save_selected_three == true){ofstream save_data(save_name.c_str());if (save_data.is_open()){save_data << Workspace3;save_data.close();show_save_complete = true;}}
			}
			ImGui::SameLine();
			if(ImGui::Button("UI Open"))
			{show_guisave = true;}
			ImGui::End();
		}
		if (show_save_complete)
		{
			ImGui::Begin("###saved", &show_save_complete);
			if(ImGui::Button("SAVED",ImVec2(-1,-1))) {show_save_complete = false;}
			ImGui::End();
		}
		if (show_prefab_list)
		{
			ImGui::Begin("Prefabs", &show_prefab_list);
			ImGui::Text("Type in the selected with extension");
			ImGui::Separator();
			ImGui::Text("These are simple tests for Lua.  Some of them contain useful code.\n"
						"They are meant to be run to make sure Lua is built correctly and also\n"
						"to be read, to see how Lua programs look.\n"
						"Here is a one-line summary of each program:\n\n");
						
			ImGui::Text("bisect.lua");ImGui::SameLine(200);ImGui::Text("bisection method for solving non-linear equations");
   			ImGui::Text("cf.lua");ImGui::SameLine(200);ImGui::Text("temperature conversion table (celsius to farenheit)");
   			ImGui::Text("echo.lua");ImGui::SameLine(200);ImGui::Text("echo command line arguments");
   			ImGui::Text("env.lua");ImGui::SameLine(200);ImGui::Text("environment variables as automatic global variables");
   			ImGui::Text("factorial.lua");ImGui::SameLine(200);ImGui::Text("factorial without recursion");
   			ImGui::Text("fib.lua");ImGui::SameLine(200);ImGui::Text("fibonacci function with cache");
   			ImGui::Text("fibfor.lua");ImGui::SameLine(200);ImGui::Text("fibonacci numbers with coroutines and generators");
   			ImGui::Text("globals.lua");ImGui::SameLine(200);ImGui::Text("report global variable usage");
   			ImGui::Text("hello.lua");ImGui::SameLine(200);ImGui::Text("the first program in every language");
   			ImGui::Text("life.lua");ImGui::SameLine(200);ImGui::Text("Conways Game of Life");
   			ImGui::Text("luac.lua");ImGui::SameLine(200);ImGui::Text("bare-bones luac");
   			ImGui::Text("printf.lua");ImGui::SameLine(200);ImGui::Text("an implementation of printf");
   			ImGui::Text("readonly.lua");ImGui::SameLine(200);ImGui::Text("make global variables readonly");
   			ImGui::Text("sieve.lua");ImGui::SameLine(200);ImGui::Text("the sieve of of Eratosthenes programmed with coroutines");
   			ImGui::Text("sort.lua");ImGui::SameLine(200);ImGui::Text("two implementations of a sort function");
   			ImGui::Text("table.lua");ImGui::SameLine(200);ImGui::Text("make table, grouping all data for the same item");
   			ImGui::Text("trace-calls.lua");ImGui::SameLine(200);ImGui::Text("trace calls");
   			ImGui::Text("trace-globals.lua");ImGui::SameLine(200);ImGui::Text("trace assigments to global variables");
   			ImGui::Text("undefined.lua");ImGui::SameLine(200);ImGui::Text("catch \"undefined\" global variables");
   			ImGui::Text("xd.lua");ImGui::SameLine(200);ImGui::Text("hex dump");
			ImGui::End();
		}
		if (show_IO)
		{
			ImGui::Begin("Output options", &show_IO);
			ImGui::Text("Type in the selected fields.\n"
						"To alter the output options then update batch.\n\n"
						"Once the batch file is made, you then only change the directories\n"
						"When you change working folders. Otherwise options are kept in batch.\n\n"
						"Default directory is the same location of LUA.Editor\n\n"
						"No blank spaces & special characters.\n"
						"Run 'Batch' from Editor tools.\n\n"
						"Batch uses a wildcard and will move all .lua/.out");
			ImGui::Separator();
			static char save_Output_saved[1024] = "C:\\%DIR%";    // setup text filtering, could be in latest update.
            string saved_lua_Output = save_Output_saved;							
			ImGui::Text("Output script save folder - Where you wish the .lua scripts to be moved to.");
			ImGui::InputText("###save_Output_saved", save_Output_saved, IM_ARRAYSIZE(save_Output_saved), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
            ImGui::SameLine();
            ImGui::Button("Save");
			ImGui::Separator();
			static char save_Output_complation[1024] = "C:\\%DIR%";    // setup text filtering, could be in latest update.
            string save_Output_compiler = save_Output_complation;							
			ImGui::Text("Compiler output folder - Where you wish the .out scripts to be moved to.");
			ImGui::InputText("###save_Output_complation", save_Output_complation, IM_ARRAYSIZE(save_Output_complation), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
            ImGui::SameLine();
            ImGui::Button("Save");
			ImGui::Separator();
			if(ImGui::Button("Update batch"))
			{
				ofstream save_data("Copy.bat");
				if (save_data.is_open())
				{
				save_data << luaDIR;
				save_data << saved_lua_Output;
				save_data << '\n';
				save_data << outDIR;
				save_data << save_Output_compiler;
				save_data.close();
				show_save_complete = true;}
			}
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
            ImGui::Text("Built for the purpose of making & testing scripts much easier.\n"
						"Was using Notepad ++, while good still lacked functionality.\n"
						"So this program was born.\n\n");
            ImGui::End();
        }
        if(show_help_support)
		{
			ImGui::Begin("Help & Support", &show_help_support,ImGuiWindowFlags_AlwaysHorizontalScrollbar);
			ImGui::Text("LUA.Editor\n\n"
			"Usage :- \n"
			"Simply save a workspace to compile or run.\n\n");
			ImGui::Separator();
			ImGui::Text("LUA interpreter\n\n"
						"Type in compiler console 'lua filename.lua' to test a script.\n\n");
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
///////////////////////////////////////////////////////////////////////////
//
//	WIP ui open/save file explorer using boost file system.
//	Should be able to add a small folder icon to represent directories.
//	Same also for .txt/.lua/.out files.
//
//
//	Nowi have a generic save/load ui to copy & paste into
//	other projects.
//
		if (show_guiopen)
		{
			ImGui::Begin("Generic open file explorer", &show_guiopen);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild("####leftpanel", ImVec2(50,300), true);
			ImGui::Text("");
			ImGui::Separator();
			ImGui::Separator();
			ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild("####openviewer", ImVec2(-1,300), true);
			if(ImGui::ImageButton((void *)up, ImVec2(15, 15), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){}
            if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Up");ImGui::EndTooltip();}
			ImGui::SameLine();
			ImGui::Text("C:\\home");
			ImGui::Separator();
			ImGui::Separator();
			ImGui::EndChild();
			ImGui::BeginChild("####bottompanel", ImVec2(300,58), true);
			ImGui::Text("");
			ImGui::PushItemWidth(-1);
			ImGui::SameLine();
			static char open_file[40] = "";
			ImGui::InputText("###open_file", open_file, IM_ARRAYSIZE(open_file), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
			ImGui::Text("");
			ImGui::SameLine();
////////////////////////////////////////////////////////////////////
//
//	Filters output display.
//
			static int filetype = -1;
			ImGui::Combo("####dummyme",&filetype," \0.txt\0.lua\0", 3);
			ImGui::PopItemWidth();
			ImGui::EndChild();
///////////////////////////////////////////////////////////////////
			ImGui::SameLine();
			ImGui::BeginChild("####bottompanelopen", ImVec2(-1,58), true);
			ImGui::Button("Open",ImVec2(80,40));
			ImGui::SameLine();
			ImGui::Button("Info",ImVec2(80,40));
			ImGui::EndChild();
			ImGui::PopStyleVar();
  			ImGui::End();
		}
		
		if (show_guisave)
		{
			ImGui::Begin("Generic save file explorer", &show_guisave);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild("####leftpanel", ImVec2(50,300), true);
			ImGui::Text("");
			ImGui::Separator();
			ImGui::Separator();
			ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild("####openviewer", ImVec2(-1,300), true);
			if(ImGui::ImageButton((void *)up, ImVec2(15, 15), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){}
            if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Up");ImGui::EndTooltip();}
			ImGui::SameLine();
			ImGui::Text("C:\\home");
			ImGui::Separator();
			ImGui::Separator();
			ImGui::EndChild();
			ImGui::BeginChild("####bottompanel", ImVec2(300,58), true);
			ImGui::Text("");
			ImGui::PushItemWidth(-1);
			ImGui::SameLine();
			static char open_file[40] = "";
			ImGui::InputText("###save_file", open_file, IM_ARRAYSIZE(open_file), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
			ImGui::Text("");
			ImGui::SameLine();
//////////////////////////////////////////////////////////////////////
//
//	Save type.
//
			static int filetype = -1;
			ImGui::Combo("####dummyme",&filetype," \0.txt\0.lua\0", 3);
			ImGui::PopItemWidth();
			ImGui::EndChild();
//////////////////////////////////////////////////////////////////////
			ImGui::SameLine();
			ImGui::BeginChild("####bottompanelopen", ImVec2(-1,58), true);
			ImGui::Button("Save",ImVec2(80,40));
			ImGui::SameLine();
			ImGui::Button("Info",ImVec2(80,40));
			ImGui::EndChild();
			ImGui::PopStyleVar();
  			ImGui::End();
		}
////////////////////////////////////////////////////////////////////////
//
//	Web browser - WIP -Skeleton-
//
//
//	Oh boy the loverly code, I need to save this.
//	Itching so badly to write a server app =)
//
if (show_web_browser)
		{
			show_inprogram_dbg	= true;
			ImGui::Begin("WIP *Skeleton* Minimal web browser", &show_web_browser);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::BeginChild("####webmenu", ImVec2(-1,35), true,ImGuiWindowFlags_NoScrollbar);
            
			if(ImGui::ImageButton((void *)home, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){}
            if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Home");ImGui::EndTooltip();}
			
			ImGui::SameLine();
			if(ImGui::ImageButton((void *)back, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){}
            if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Back");ImGui::EndTooltip();}
			
			ImGui::SameLine();
			if(ImGui::ImageButton((void *)stop, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){}
            if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Stop");ImGui::EndTooltip();}
			
			ImGui::SameLine();
			if(ImGui::ImageButton((void *)forward, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){}
            if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Forward");ImGui::EndTooltip();}
			
			ImGui::SameLine();
			ImGui::PushItemWidth(300);
			static char web_url[1024] = "www.google.com";
			ImGui::InputText("###url_address", web_url, IM_ARRAYSIZE(web_url), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::PopItemWidth();
			
			ImGui::SameLine();
			if(ImGui::ImageButton((void *)web, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255)))
			
			{
				
			}
            
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Go/Refresh");ImGui::EndTooltip();}
			
			
			ImGui::SameLine();
			if(ImGui::ImageButton((void *)bookmarks, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){show_bookmarks_saved = true;}
            if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Bookmarks");ImGui::EndTooltip();}
			
			ImGui::SameLine();
			if(ImGui::ImageButton((void *)downloads, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){show_download_manager = true;}
            if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Downloads & Uploads");ImGui::EndTooltip();}
			
			ImGui::SameLine();
			if(ImGui::ImageButton((void *)settings, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){show_web_settings = true;}
            if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Browser settings");ImGui::EndTooltip();}
			
			
			ImGui::EndChild();
			
            ImGui::BeginChild("####web_page_display", ImVec2(-1,400), true,ImGuiWindowFlags_NoScrollbar);
            
			ImGui::EndChild();
			ImGui::Separator();
			char buf[128];
    		sprintf(buf, "Status : %c", "|/-\\"[(int)(ImGui::GetTime()/0.25f)&3], ImGui::GetFrameCount());
    		ImGui::Text(buf);
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth()/2 +350);
			if(status_empty){if(ImGui::ImageButton((void *)statusempty, ImVec2(16, 16), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){}}
			if(status_red){if(ImGui::ImageButton((void *)statusred, ImVec2(16, 16), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){}}
			if(status_yellow){if(ImGui::ImageButton((void *)statusyellow, ImVec2(16, 16), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){}}
			if(status_green){if(ImGui::ImageButton((void *)statusgreen, ImVec2(16, 16), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){}}
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Status - Click for details.");ImGui::EndTooltip();}
			
//////////////////////////////////////////////////////////
//
//	Used for tcp/ip checking connectivity.
//	Will be broken down and removed.
//	Useful for copy/paste.
//
//			
			if(status_empty){status_yellow = false;status_green = false;status_red = false;}
			if(status_red){status_yellow = false;status_green = false;status_empty = false;}
			if(status_yellow){status_red = false;status_green = false;status_empty = false;}
			if(status_green){status_yellow = false;status_red = false;status_empty = false;}

/////////////////////////////////////////////////////////
			
			ImGui::PopStyleVar();
			ImGui::End();
		}
		

if(show_web_settings)
		{
			ImGui::Begin("Browser settings", &show_web_settings, ImGuiWindowFlags_MenuBar);
				if (ImGui::BeginMenuBar())
				{
				if (ImGui::BeginMenu("Options"))
				{
		    		
		    		if (ImGui::MenuItem("  Save", NULL)){}
		    		if (ImGui::MenuItem("  Import", NULL)){}
		    		ImGui::MenuItem("  Export", NULL);
		    		ImGui::MenuItem("  Default", NULL);
					
		    		ImGui::EndMenu();
					}
				ImGui::EndMenuBar();
				}
			ImGui::Text("Settings for browser.\nClick on the headers for details.\nClick menu options to save.");
			ImGui::Text("");
//////////////////////////////////////////////////////////////
//
//	Helpful batch file calls.
//
			if (ImGui::CollapsingHeader("TCP/IP")) 
            {
			
			if(ImGui::Button("TCP::IP -I"))
			{
				static ImGuiOnceUponAFrame Ipstatus;	
				if (Ipstatus)
					{
						boost::thread_group threads;
						threads.create_thread(ipconfigure_bat);
					}
			}
			ImGui::SameLine();
			if(ImGui::Button("Netstat On"))
			{
				static ImGuiOnceUponAFrame Ipstatus;	
				if (Ipstatus)
					{
						boost::thread_group threads;
						threads.create_thread(ipconfigure_netstat_on_bat);
					}
			}
			ImGui::SameLine();
			if(ImGui::Button("Netstat -A"))
			{
				static ImGuiOnceUponAFrame Ipstatus;	
				if (Ipstatus)
					{
						boost::thread_group threads;
						threads.create_thread(ipconfigure_netstat_a_bat);
					}
			}
			
			if(ImGui::Button("Netstat -O"))
			{
				static ImGuiOnceUponAFrame Ipstatus;	
				if (Ipstatus)
					{
						boost::thread_group threads;
						threads.create_thread(ipconfigure_netstat_o_bat);

					}
			}
			ImGui::SameLine();
			if(ImGui::Button("Netstat -T"))
			{
				static ImGuiOnceUponAFrame Ipstatus;	
				if (Ipstatus)
					{
						boost::thread_group threads;
						threads.create_thread(ipconfigure_netstat_t_bat);

					}
			}
			ImGui::SameLine();
			if(ImGui::Button("Netstat -F"))
			{
				static ImGuiOnceUponAFrame Ipstatus;	
				if (Ipstatus)
					{
						boost::thread_group threads;
						threads.create_thread(ipconfigure_netstat_f_bat);

					}
			}
			}
			ImGui::Separator();
			if (ImGui::CollapsingHeader("WMIC")) 
			{
				
			if(ImGui::Button("WMIC Open"))
			{
				static ImGuiOnceUponAFrame Ipstatus1;	
				if (Ipstatus1)
					{
						boost::thread_group threads;
						threads.create_thread(ipconfigure_wmic_bat);
					}
			}
			ImGui::SameLine();
			if(ImGui::Button("WMIC - Support & Information"))
			{
				static ImGuiOnceUponAFrame Ipstatus;	
				if (Ipstatus)
					{
						boost::thread_group threads;
						threads.create_thread(ipconfigure_wmic_help_bat);

					}
			}
			}
////////////////////////////////////////////////////////////////
			ImGui::Separator();
			if (ImGui::CollapsingHeader("Browser options")) 
			{
			ImGui::Text("");	
			ImGui::Text("Mobile phone or Desktop browsing:");
			ImGui::Checkbox("Mobile only", &mobile_html);
			ImGui::Separator();
			ImGui::Text("Bookmarks:");
			ImGui::BeginChild("####bookmarks", ImVec2(-1,150), true,ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::InputTextMultiline("##bookmarks_kept", bookmarks_kept, IM_ARRAYSIZE(bookmarks_kept), ImVec2(-1.0f, -1.0 * 16), ImGuiInputTextFlags_AllowTabInput);
			if(ImGui::SmallButton("Update bookmarks")){ofstream save_data("Bookmarks.setting");if (save_data.is_open()){save_data << bookmarks_kept;save_data.close(); show_save_complete = true;}}
			if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Update to save bookmarks.");ImGui::EndTooltip();}
			ImGui::EndChild();
			ImGui::PushItemWidth(-1);
			ImGui::Separator();
			ImGui::Text("Homepage:");
			static char homepage[256] = "www.google.com";
			ImGui::InputText("###default_homepage", homepage, IM_ARRAYSIZE(homepage), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
			ImGui::Separator();
			ImGui::Text("Downloads folder:");
			static char download_folder[256] = "C:\\";
			ImGui::InputText("###default_download", download_folder, IM_ARRAYSIZE(download_folder), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
			ImGui::PopItemWidth();
			}
			
			ImGui::Separator();
			if (ImGui::CollapsingHeader("FTP")) 
            {
            ImGui::PushItemWidth(-1);
			ImGui::Text("");	
			ImGui::Text("FTP Settings:");
			ImGui::Text("");
			ImGui::Text("IP:PORT");
			static char ftpconnect[256] = "192.168.0.1:1000";
			ImGui::InputText("###default_ftpip", ftpconnect, IM_ARRAYSIZE(ftpconnect), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
			ImGui::Text("");
			ImGui::Text("Login details:");
			static char ftp_connect_name[256] = "Username";
			ImGui::InputText("###default_ftp_name", ftp_connect_name, IM_ARRAYSIZE(ftp_connect_name), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
			static char ftp_connect_password[256] = "Password";
			ImGui::InputText("###default_ftp_password", ftp_connect_password, IM_ARRAYSIZE(ftp_connect_password), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_Password);
			ImGui::Checkbox("Save FTP Login credentials", &save_ftp_login);
			ImGui::Text("");
			ImGui::Text("FTP Connection test. ");
			ImGui::SameLine();
			ImGui::SmallButton("Test");
			ImGui::PopItemWidth();
			}
			ImGui::Separator();
			if (ImGui::CollapsingHeader("Cloud Settings")) 
			{
			ImGui::PushItemWidth(-1);	
			ImGui::Text("");	
			ImGui::Text("Cloud Settings:");
			ImGui::Text("");
			ImGui::Text("Web address:");
			static char cloudconnect[256] = "www.google.com/cloud";
			ImGui::InputText("###default_cloudaddress", cloudconnect, IM_ARRAYSIZE(cloudconnect), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
			ImGui::Text("");
			ImGui::Text("Login details:");
			static char cloud_connect_name[256] = "Username";
			ImGui::InputText("###default_cloud_name", cloud_connect_name, IM_ARRAYSIZE(cloud_connect_name), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
			static char cloud_connect_password[256] = "Password";
			ImGui::InputText("###default_cloud_password", cloud_connect_password, IM_ARRAYSIZE(cloud_connect_password), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_Password);
			ImGui::Checkbox("Save Cloud Login credentials", &save_cloud_login);
			ImGui::Text("");
			ImGui::Text("Cloud Connection test. ");
			ImGui::SameLine();
			ImGui::SmallButton("Test");
			ImGui::PopItemWidth();
			}
			ImGui::Separator();
			ImGui::End();
		}
/////////////////////////////////////////////////////////////////
//
//	Downloads manager.
//
//	Handles upstream/downstream.
//
if (show_download_manager)
	{
		ImGui::Begin("Downloads & Uploads Manager", &show_download_manager);
		ImGui::Text("Download");
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("####Downloading", ImVec2(-1,135), true);
		ImGui::Text("Filename: ");
		ImGui::Separator();
		ImGui::Text("Host:");
		ImGui::Separator();
		static float Download_rate = 0.0f;
		ImGui::Text("Transfer:  %fKb/s",Download_rate);
		ImGui::Separator();
		static float Download = 0.0f;
		ImGui::ProgressBar(Download, ImVec2(-1.0f,0.0f));
		ImGui::Separator();
		if(ImGui::ImageButton((void *)resume, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){}
        if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Continue");ImGui::EndTooltip();}
			
		ImGui::SameLine();
		if(ImGui::ImageButton((void *)pause, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){}
        if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Pause");ImGui::EndTooltip();}
		
		ImGui::SameLine();
		if(ImGui::ImageButton((void *)stop, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){}
        if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Cancel");ImGui::EndTooltip();}
		
		ImGui::SameLine();
		ImGui::Button("FTP Download");	
		
		ImGui::EndChild();
		ImGui::PopStyleVar();
		ImGui::Text("Upload");
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("####Uploading", ImVec2(-1,135), true);
		ImGui::Text("Filename: ");
		ImGui::Separator();
		ImGui::Text("Upload location:");
		ImGui::Separator();
		static float Upload_rate = 0.0f;
		ImGui::Text("Transfer:  %fKb/s",Upload_rate);
		ImGui::Separator();
		static float Upload = 0.0f;
		ImGui::ProgressBar(Upload, ImVec2(-1.0f,0.0f));
		ImGui::Separator();
		if(ImGui::ImageButton((void *)resume, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){}
        if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Continue");ImGui::EndTooltip();}
			
		ImGui::SameLine();
		if(ImGui::ImageButton((void *)pause, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){}
        if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Pause");ImGui::EndTooltip();}
		
		ImGui::SameLine();
		if(ImGui::ImageButton((void *)stop, ImVec2(20, 20), ImVec2(0,0),ImVec2(1,1),0,ImVec4(0,0,0,0),ImVec4(255,255,255,255))){}
        if (ImGui::IsItemHovered()){ImGui::BeginTooltip();ImGui::Text("Cancel");ImGui::EndTooltip();}
		
		ImGui::SameLine();
		ImGui::Button("FTP Upload");	
		
		ImGui::EndChild();
		ImGui::PopStyleVar();
		ImGui::End();
	}
//////////////////////////////////////////////////////////////
//
//	Bookmarks select.
//
//	Will used selected to seek.get(0,"url list location");
//	E.G :-
//
//	if seleced is left clicked
//	seek.get(0,i); https://github.com/ (1 from 0 - 2 - currently)
//
//	Load into buffer, pass buffer to resolver.
//	Bookmark page loaded, url in textinput changed
//	to reflect bookmark selected.
//
//	If selected is blank do nothing?.
//
if(show_bookmarks_saved)
	{
		
		ImGui::Begin("Bookmarks", &show_bookmarks_saved);
		char Bookmark_display[1024*16];
		ifstream file("Bookmarks.setting", ios::in|ios::ate);
			if (file.is_open())
				{						
				streampos size;			
		  		size = file.tellg();
				file.seekg (0, ios::beg);
				for (int w = 0; w < size; w++)
				{
				file.getline(Bookmark_display, size);	
				int i = -1;
				bool selected;
				char buf[32];
				sprintf(buf, "%s", Bookmark_display);
			    if (ImGui::Selectable(buf, selected == i)) 
				selected = i;
				}
				file.close();
				}
												
		ImGui::End();
	}
/////////////////////////////////////////////////////////////////////
//
//	Debugger. 
//
//	Temp, while code under construction.
//
if(show_inprogram_dbg)
		{
			ImGui::Begin("Debugger", &show_inprogram_dbg);
			ImGui::Checkbox("red",&status_red);
			ImGui::Checkbox("yellow",&status_yellow);
			ImGui::Checkbox("green",&status_green);
			ImGui::Checkbox("empty",&status_empty);
			
			ImGui::End();
		}
/////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//
// Rendering.
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
