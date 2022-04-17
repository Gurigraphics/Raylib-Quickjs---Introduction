import * as raylibJS from 'raylibJS'

const ConfigFlag ={
	/** Set to show raylib logo at startup */
	FLAG_SHOW_LOGO          : 1,
	/** Set to run program in fullscreen */
	FLAG_FULLSCREEN_MODE    : 2,
	/** Set to allow resizable window */
	FLAG_WINDOW_RESIZABLE   : 4,
	/** Set to disable window decoration (frame and buttons) */
	FLAG_WINDOW_UNDECORATED : 8,
	/** Set to allow transparent window */
	FLAG_WINDOW_TRANSPARENT : 16,
	/** Set to create the window initially hidden */
	FLAG_WINDOW_HIDDEN      : 128,
	/** Set to try enabling MSAA 4X - Force anti-aliasing i.e. smooths the edges of images and makes them more pleasant */
	FLAG_MSAA_4X_HINT       : 32,
	/** Set to try enabling V-Sync on GPU */
	FLAG_VSYNC_HINT         : 64,
}   
  
const screenWidth = 1080; 
const screenHeight = 1920;  
const title = "Welcome to QuickJS+raylib!";

raylibJS.setConfigFlags(ConfigFlag.FLAG_WINDOW_RESIZABLE || ConfigFlag.FLAG_MSAA_4X_HINT);
 
raylibJS.initWindow(screenWidth, screenHeight, title)
raylibJS.setTargetFps(60);    
  
while (!raylibJS.windowShouldClose()) {

	raylibJS.beginDrawing(); 
	raylibJS.clearBackgroundRGBA(0, 255, 255, 255);
	raylibJS.endDrawing();
}

raylibJS.closeWindow();
  
