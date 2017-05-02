#define CURSORSIZE 0.1
#define MAINMENU 0
#define PLAYING 1
#include <windows.h>		
#include "MainMenu.h"
#include "Level.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

/**********************************************************************************************/

bool mouseClick;
float targetFramerate = 144.0; // 144 chosen as this is the vsync framerate on development PC - sets speed
int ScreenWidth=1500;
int ScreenHeight=1000;
bool keys[256];
float mouse_x;
float mouse_y;
GLFont myfont;
GameObject cursor;
Level currentLevel;
MainMenu menu;
Mix_Music *currentTrack = NULL;
float mouse_xAspect = 1;
float mouse_yAspect = 1;
float framecounter = 0;
double frameTimeOffset;
double frameDuration;
void playGame();
bool startSDLaudio();
void processKeys();
int currentState = MAINMENU;
void init()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Start SDL for audio
	if (!startSDLaudio()) {
		exit(EXIT_FAILURE);
	}
	currentTrack = Mix_LoadMUS("assets/music/menu.ogg");
	Mix_PlayMusic(currentTrack, -1);
	cursor = GameObject(mouse_x, mouse_y, CURSORSIZE, CURSORSIZE);
	menu = MainMenu(&myfont);
	cursor.loadThisTexture("assets/textures/aim.png");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	if (!myfont.Create("assets/fonts/Arial72.glf", -1)) {
		cerr << "Unable to load font. Exiting." << endl;
		exit(EXIT_FAILURE);
	}
}

void update()
{
	if (frameTimeOffset >=10) {
		frameTimeOffset = 10;
	}
	cursor.basicBox.set(mouse_x, mouse_y);
	if (currentState == MAINMENU) {
		if (mouseClick) {
			menu.checkButtons(cursor);
			mouseClick = false;
		}
	}
	if (menu.startPlaying) {
		menu.startPlaying = false;
		Mix_HaltMusic();
		Mix_FreeMusic(currentTrack);
		currentLevel = Level(menu.filePath);
		currentTrack = currentLevel.levelMusic;
		Mix_PlayMusic(currentTrack, -1);
		currentState = PLAYING;
		currentLevel.resetClock();
		currentLevel.resetTicks();
	}
	if (currentState == PLAYING) {
		currentLevel.setSpeedMulitplier(frameTimeOffset);
		currentLevel.advanceClock(frameDuration);
		currentLevel.advanceTicks(frameTimeOffset);
		processKeys();
		currentLevel.update(frameTimeOffset,cursor);

	}
}
void display()
{	
	glClear(GL_COLOR_BUFFER_BIT);
	if (currentState == MAINMENU) {
		menu.drawMenu();
	}
	else if (currentState == PLAYING) {
		currentLevel.draw();
		// HUD
		glColor3f(0, 0, 0);
		glBegin(GL_QUADS);
		glVertex2f(-1 * mouse_xAspect, 0.9);
		glVertex2f(1 * mouse_xAspect, 0.9);
		glVertex2f(1 * mouse_xAspect, 1);
		glVertex2f(-1 * mouse_xAspect, 1);
		glEnd();
		glColor3f(1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		myfont.Begin();
		string time = "Time: ";
		time.append(std::to_string(currentLevel.getClock()));
		time.append("    Score:");
		time.append(std::to_string(currentLevel.player.points));
		myfont.DrawString(time, 0.001, -1 * mouse_xAspect, 1);
		
		// END of HUD
	}
	cursor.drawTextured();
}
void playGame() {
	
}

void reshape(int w, int h)
{
	ScreenWidth = w;
	ScreenHeight = h;
	float aspectRatio = ((float)w) / h;
	float xScale = 1;
	float yScale = 1;

	if (aspectRatio > 1) {
		// Width > Height, so scale x accordinly.
		xScale *= aspectRatio;
	}
	else {
		// Height >= Width, so scale y accordingly.
		yScale = xScale / aspectRatio;
	}
	mouse_xAspect = xScale;
	mouse_yAspect = yScale;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1 * xScale, xScale, -1 * yScale, yScale);

	glMatrixMode(GL_MODELVIEW);
}

/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

													   //win32 global variabless
HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Context
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


							/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done = false;								// Bool Variable To Exit Loop

														// Create Our OpenGL Window
	if (!CreateGLWindow("Pixel Space", ScreenWidth, ScreenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stderr);
	__int64 lastFrame, currentFrame,cps,counts;
	QueryPerformanceCounter((LARGE_INTEGER *)&lastFrame); //Get initial Counts
	QueryPerformanceFrequency((LARGE_INTEGER *)&cps); // Counts per second
	while (!done)									// Loop That Runs While done=FALSE
	{
		QueryPerformanceCounter((LARGE_INTEGER *)&currentFrame);//
		counts = currentFrame - lastFrame;
		if (counts == 0) {
			counts = 1;
		}
		frameDuration = (double)counts/cps;
		counts = cps/counts;
		frameTimeOffset = (double)(targetFramerate/counts);
		framecounter ++;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				done = true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if (keys[VK_ESCAPE])
				done = true;
			update();
			display();					// Draw The Scene
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
			lastFrame = currentFrame;
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_CLOSE:								// Did We Receive A Close Message?
	{
		PostQuitMessage(0);						// Send A Quit Message
		return 0;								// Jump Back
	}
	break;

	case WM_SIZE:								// Resize The OpenGL Window
	{
		reshape(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;								// Jump Back
	}
	break;

	case WM_LBUTTONDOWN:
	{
		mouseClick = true;
	}
	break;


	case WM_LBUTTONUP:
	{
		mouseClick = false;
	}
	break;

	case WM_MOUSEMOVE:
	{
		mouse_x = ((LOWORD(lParam) - ScreenWidth / 2) / ((float)ScreenWidth) * 2) *mouse_xAspect; // Turn into view co-ordinates
		mouse_y = ((HIWORD(lParam) - ScreenHeight / 2) / ((float)ScreenHeight) * 2)*-1 * mouse_yAspect;
	}
	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
		keys[wParam] = true;					// If So, Mark It As TRUE
		return 0;								// Jump Back
	}
	break;
	case WM_KEYUP:								// Has A Key Been Released?
	{
		keys[wParam] = false;					// If So, Mark It As TRUE
		return 0;								// Jump Back
	}
	break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
*	title			- Title To Appear At The Top Of The Window				*
*	width			- Width Of The GL Window Or Fullscreen Mode				*
*	height			- Height Of The GL Window Or Fullscreen Mode			*/

bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

																	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();

	return true;									// Success
}

bool startSDLaudio() {

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}
	Mix_AllocateChannels(2000);
	return true;
}

void processKeys()
{
	if (keys[0x41]) {
		currentLevel.movePlayer(0.008);
	}
	if (keys[0x44]) {
		currentLevel.movePlayer(-0.008);
	}
	if (!keys[0x41] && !keys[0x44]) {
		currentLevel.movePlayer(0);
	}
	if (keys[VK_SPACE]) {
		currentLevel.playerJump();
	}
	if (mouseClick) {
		//mouseClick = false;
		//if (currentLevel.getClock() > 5) {
			currentLevel.playerAttack();
		//}
	}
}


