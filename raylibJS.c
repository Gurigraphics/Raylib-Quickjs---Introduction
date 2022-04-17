#include <quickjs.h>
#include <errno.h> 
#include <string.h>   
#include "cutils.h"
#include "stdio.h"    
#include "raylib.h"      

/*Setup init configuration flags (view FLAGS) 
  NOTE: If you want to use the default values, just call raylib_js_init()
*/ 
/*void SetConfigFlags(unsigned int flags); */                           
static JSValue rl_set_config_flags(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
	int flags;

	if (JS_ToInt32(ctx, &flags, argv[0]))
		return JS_EXCEPTION;

	SetConfigFlags((unsigned char)flags);

	return JS_UNDEFINED;
}

/* void SetTargetFPS(int fps);  
 * This function selects maximum number of FPS (maximum number of frames)
 * that will be guaranteed in both WaitForEndOfFrame() and EndFrame().
 *
 * NOTE: If the application doesn't call both, neither EndFrame() nor
 * WaitForEndOfFrame(), the FPS in the title bar will not be updated.
 */
static JSValue rl_set_target_fps(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
	int fps;

	if (JS_ToInt32(ctx, &fps, argv[0]))
		return JS_EXCEPTION;

	SetTargetFPS(fps);

	return JS_UNDEFINED;
}

 
/* Initialize window and OpenGL context 
 * NOTE: InitWindow() is called only once (the first time a window is created).
 * InitWindow() has to be called again everytime the window is re-created (re-init).
 */
/* void InitWindow(int width, int height, const char *title); */
static JSValue rl_init_window(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
	//verify args total
	if (argc != 3) {
		JS_ThrowInternalError(ctx, "initWindow: wrong number of arguments, expected 3");
		return JS_EXCEPTION;
	} 
 
	int w, h; 
	const char* title = NULL; 

	if (JS_ToInt32(ctx, &w, argv[0]))
		return JS_EXCEPTION;
	
	if (JS_ToInt32(ctx, &h, argv[1]))
		return JS_EXCEPTION;

	title = JS_ToCString(ctx, argv[2]);
	if (title == NULL)
		return JS_EXCEPTION;

	InitWindow(w, h, title);

	return JS_UNDEFINED;
} 

/* Check if KEY_ESCAPE pressed or Close icon pressed 
 * NOTE: This function is not updated when window is in fullscreen mode
 * Returns true if QUIT event happened
 */
/* bool WindowShouldClose(void); */
static JSValue rl_window_should_close(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
	return JS_NewBool(ctx, WindowShouldClose());
}

/* Close window and unload OpenGL context 
 * NOTE: This function doesn't return until the user closes the window
 */
/* void CloseWindow(void); */
static JSValue rl_close_window(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
	CloseWindow();
	return JS_UNDEFINED;
} 

/*Setup canvas (framebuffer) to start drawing 
  NOTE: This function must be called before any drawing command
*/
/*void BeginDrawing(void); */
static JSValue rl_begin_drawing(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
	BeginDrawing();
	return JS_UNDEFINED;
}

/*End canvas drawing and swap buffers (double buffering)
  NOTE: EndDrawing() must be called at the end of the rendering.
  This will perform a buffer swap on the GPU and make the rendered 
  image visible.
*/
/* void EndDrawing(void); */
static JSValue rl_end_drawing(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
	EndDrawing();
	return JS_UNDEFINED;
}

/* Set background color (framebuffer clear color)
 * NOTE: This function can change the background color of the
 *       current drawing target (framebuffer)
 * NOTE: This function has no effect on the current texture bound to the GPU.
 */
/*void ClearBackground(int r, int g, int b, int a); */
static JSValue rl_clear_background_rgba(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
	//verify args total
	if (argc != 4) {
		JS_ThrowInternalError(ctx, "clearBackgroundRGBA: wrong number of arguments, expected 4");
		return JS_EXCEPTION;
	}
	
	Color color = BLACK;  
	int r, g, b, a;

	if (JS_ToInt32(ctx, &r, argv[0]))
		return JS_EXCEPTION;
	if (JS_ToInt32(ctx, &g, argv[1]))
		return JS_EXCEPTION;
	if (JS_ToInt32(ctx, &b, argv[2]))
		return JS_EXCEPTION;
	if (JS_ToInt32(ctx, &a, argv[3]))
		return JS_EXCEPTION;

	color.r = (unsigned char)r;
	color.g = (unsigned char)g;
	color.b = (unsigned char)b;
	color.a = (unsigned char)a;

	ClearBackground(color);

	return JS_UNDEFINED;	 
} 


/* All functions list -> https://www.raylib.com/cheatsheet/cheatsheet.html */

const JSCFunctionListEntry js_raylibJS_funcs[] = {
 /* MODULE CORE | Window-related functions -------------------------------------------*/
	/* void InitWindow(int width, int height, const char *title); */
	JS_CFUNC_DEF("initWindow", 3, rl_init_window),
	/* bool WindowShouldClose(void);      */
 	JS_CFUNC_DEF("windowShouldClose", 1, rl_window_should_close),
	 /* void CloseWindow(void); */
	JS_CFUNC_DEF("closeWindow", 0, rl_close_window),
/* MODULE CORE | Misc. functions ----------------------------------------------------*/
	/*void SetConfigFlags(unsigned int flags); */
	JS_CFUNC_DEF("setConfigFlags", 1, rl_set_config_flags),
/* MODULE CORE | Timing-related functions -------------------------------------------*/
	/* void SetTargetFPS(int fps);*/
	JS_CFUNC_DEF("setTargetFps", 1, rl_set_target_fps),	

/* MODULE CORE | Drawing-related functions -------------------------------------------*/
	/*void BeginDrawing(void); */ 
	JS_CFUNC_DEF("beginDrawing", 0, rl_begin_drawing),
	/*void EndDrawing(void); */
	JS_CFUNC_DEF("endDrawing", 0, rl_end_drawing), 
	/* void ClearBackground(int r, int g, int b, int a); */
	JS_CFUNC_DEF("clearBackgroundRGBA", 1, rl_clear_background_rgba),
};  

int js_raylibJS_init(JSContext *ctx, JSModuleDef *m)
{
	return JS_SetModuleExportList(ctx, m, js_raylibJS_funcs, countof(js_raylibJS_funcs));
}

JSModuleDef *js_init_module_raylibJS(JSContext *ctx, const char *module_name)
{
  JSModuleDef *m;
  m = JS_NewCModule(ctx, module_name, js_raylibJS_init);
  if (!m) 
      return NULL;
  JS_AddModuleExportList(ctx, m, js_raylibJS_funcs, countof(js_raylibJS_funcs));
  return m;
}
