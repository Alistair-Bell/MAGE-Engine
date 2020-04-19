#ifndef __MAGE_API__
#define __MAGE_API__

/* Contains definitions for all the mage related api functions */
/* All source files reference this header implimenting a section of the api */

#include "mageCore.h"

/* Typedefs for integers */

typedef unsigned char byte;
typedef char sint8;
typedef unsigned char uint8;
typedef short sint16;
typedef unsigned short uint16;
typedef int sint32;
typedef unsigned int uint32;
typedef long sint64;
typedef unsigned long uint64;

/* Engine related defines */ 

typedef struct MAGE_RESIZABLE_LIST_STRUCT
{
	/* Pointer array to the elements stored */
	void **Elements;
	/* Count of all of the elements stored */
	uint32 Quantity;
	/* Size of each element in bytes */
	uint32 ElementSize;

} mageResizableList;

/* Allocates the memory for the struct */
extern void *mageResizableListAllocate();
/* Initialises the list populating the members of the struct */
extern void mageResizableListInitialise(mageResizableList *resizableList, const uint32 size);
/* Pushes an element in the array resizing its memory */
extern void mageResizableListPush(mageResizableList *resizableList, void *item);
/* Pops the last element pushed onto the array */
extern void mageResizableListPop(mageResizableList *resizableList, void (*freeMethod)(void* item));
/* Destroys the resizable list and a function handlers its memory */
extern void mageResizableListDestroy(mageResizableList *resizableList, void (*freeMethod)(void* item));
/* Basic memory handler for the resizable list methods */
extern void mageResizableListBasicDestroyHandler(void *item);

/* File utilities */


/* Dumps the file contents into the buffer allowing for the buffer to be reallocated pushing the return value to the integer passed in */
extern void mageFileReadContents(const char *file, char *buffer, const uint8 reallocatable, uint8 *success);
/* Dumps the contents of the buffer into the file allowing the file to be cleaned or not pushing the return value to the integer passed in*/
extern void mageFileDumpContents(const char *file, const char *buffer, const uint8 clean, uint8 *success);



/* Window related functions */

typedef struct MAGE_WINDOW_STRUCT
{
	/* Width of the window */
	sint32 Width;
	/* Height of the window */
	sint32 Height;
	/* Title of the window */
	const char *Title;
	/* Instaniated GLFW context */
	GLFWwindow *Context;
	/* Whether the window is running */
	uint8 Running;	
} mageWindow;

/* Allocates the space for the window */ 
extern void mageWindowAllocate(mageWindow *window);
/* Initialises the window */
extern uint8 mageWindowInitialise(mageWindow *window, const sint32 xResolution, const sint32 yResolution, const char *title);
/* Swaps the glfw window's buffers */
extern void mageWindowSwapBuffers(mageWindow *window);
/* Frees itself and the members */
extern void mageWindowDestroy(mageWindow *window); 

/* Input related functions */

/* Puts the mouse position into the values passed into the function */
extern void mageGetMousePosition(mageWindow *window, double *x, double *y);
/* Puts the mouse x position into the the value passed into the function */
extern void mageGetMousePositionX(mageWindow *window, double *x);
/* Puts the mouse y position into the value passed into the function */ 
extern void mageGetMousePositionY(mageWindow *window, double *y);
/* Puts the state of both the mouse buttons int the values passed into the function */
extern void mageGetMouseButtonStates(mageWindow *window, sint32 *left, sint32 *right);
/* Puts whether the left mouse button is being clicked into the value passed into the function */
extern void mageGetMouseButtonLeftClick(mageWindow *window, uint8 *state);
/* Puts whether the right mouse button is being clicked into the value passed into the function */
extern void mageGetMouseButtonRightClick(mageWindow *window, uint8 *state);
/* Puts whether the mouse is inside the window into the value passed into the function */
extern void mageGetMouseInsideContext(mageWindow *window, uint8 *state);
/* Sets the mouse position based on the values passed into the function */
extern void mageSetMousePosition(mageWindow *window, const double x, const double y);
/* Gets whether the key is being pressed down */
extern void mageGetKeyDown(mageWindow *window, const sint32 key, uint8 *state);
/* Gets whether the key is not being pressed down */
extern void mageGetKeyNotDown(mageWindow *window, const sint32 key, uint8 *state);

/* Math related functions */

/* Stores two floats */
typedef struct MAGE_VECTOR2_STRUCT
{
	float x;
	float y;

} mageVector2;

/* Intitalises the vector2 based on the values passed */
extern void mageVector2Initialise(mageVector2 *vector2, const float value1, const float value2);
/* Adds the right vector2 to the left vector2 */
extern void mageVector2Add(mageVector2 *left, const mageVector2 *right);
/* Subtracts the right vector2 to the left vector2 */
extern void mageVector2Subtract(mageVector2 *left, const mageVector2 *right);
/* Multiplies the right vector2 to the left vector2 */
extern void magemageVector2Multiply(mageVector2 *left, const mageVector2 *right);
/* Divides the right vector2 to the left vector2 */ 
extern void mageVector2Divide(mageVector2 *left, const mageVector2 *right);

typedef struct MAGE_VECTOR3_STRUCT
{
	float x;
	float y;
	float z;	

} mageVector3;

/* Intitalises the vector3 based on the values passed */
extern void mageVector3Initialise(mageVector3 *vector3, const float value1, const float value2, const float value3);
/* Adds the right vector3 to the left vector3 */
extern void mageVector3Add(mageVector3 *left, const mageVector3 *right);
/* Subtracts the right vector3 to the left vector3 */
extern void mageVector3Subtract(mageVector3 *left, const mageVector3 *right);
/* Multiplies the right vector3 to the left vector3 */
extern void mageVector3Multiply(mageVector3 *left, const mageVector3 *right);
/* Divides the right vector3 to the left vector3 */ 
extern void mageVector3Divide(mageVector3 *left, const mageVector3 *right);

typedef struct MAGE_VECTOR4_STRUCT
{
	float x;
	float y;
	float z;
	float w;	

} mageVector4;

/* Intitalises the vector4 based on the values passed */
extern void mageVector4Initialise(mageVector4 *vector4, const float value1, const float value2, const float value3, const float value4);
/* Initialses the vector4 by using values passed in from two vector2's */
extern void mageVector4InitialisemageVector2(mageVector4 *vector4, const mageVector2 *left, const mageVector2 *right);
/* Adds the right vector4 to the left vector4 */
extern void mageVector4Add(mageVector4 *left, const mageVector4 *right);
/* Subtracts the right vector4 to the left vector4 */
extern void mageVector4Subtract(mageVector4 *left, const mageVector4 *right);
/* Multiplies the right vector4 to the left vector4 */
extern void mageVector4Multiply(mageVector4 *left, const mageVector4 *right);
/* Divides the right vector4 to the left vector4 */ 
extern void mageVector4Divide(mageVector4 *left, const mageVector4 *right);

/* Stores sixteen floats using column majoring for opengl */
typedef struct MAGE_MATRIX4X4_STRUCT
{
	float Elements[16];

} mageMatrix4x4;

/* Initialses a matrix using four vector4's */
extern void mageMatrixInitialise(mageMatrix4x4 *matrix, const mageVector4 *row0, const mageVector4 *row1, const mageVector4 *row2, const mageVector4 *row3);
/* Multiplies the right matrix4x4 to the left matrix4x4 */
extern void mageMatrixMultiply(mageMatrix4x4 *left, const mageMatrix4x4 *right);


/* OpenGL related functions */

#ifdef __MAGE_OPENGL_
	#define GLCall(x) mageGLClearError();\
		x;\
		mageGLLogError(#x, __FILE__, __LINE__);
#endif

/* OpenGL debugging function */
extern void mageGLClearError();
/* OpenGL call wrapper */
extern void mageGLLogError(const char *function, const char *file, const sint32 line);

/* OpenGL buffers */ 

typedef struct MAGE_BUFFER_STRUCT
{
	/* Buffer `reference` used by opengl */
	uint32 ID;
	
	/* Count of the buffer components */
	uint32 ComponentCount;

} mageBuffer;

/* Allocates the memory for the buffer */
extern void *mageBufferAllocate();
/* Initialses the buffer suppling the values and the counts required by opengl */
extern void mageBufferInitialise(mageBuffer *buffer, const float *data, const uint32 quantity, const uint32 componentCount);
/* Binds the buffer */
extern void mageBufferBind(mageBuffer *buffer);
/* Unbinds the buffer */
extern void mageBufferUnbind(mageBuffer *buffer);
/* Destroys the buffer freeing the memory */
extern void mageBufferDestroy(mageBuffer *buffer, void (*freeMethod)(void *item));

typedef struct MAGE_INDEX_BUFFER_STRUCT
{
	/* Index buffer `reference` used by opengl */
	uint32 ID;
	
	/* Count of the index buffer components */
	uint32 ComponentCount;

} mageIndexBuffer;

/* Allocates the memroy for the index buffer */
extern void *mageIndexBufferAllocate();
/* Initialses the buffer suppling the values and the counts required by opengl */
extern void mageIndexBufferInitialise(mageIndexBuffer *buffer, const uint32 *data, const uint32 quantity);
/* Binds the index buffer */
extern void mageIndexBufferBind(mageIndexBuffer *buffer);
/* Unbinds the index buffer */
extern void mageIndexBufferUnBind(mageIndexBuffer *buffer); 
/* Destroys the index buffer freeing the memory */  
extern void mageIndexBufferDestroy(mageIndexBuffer *buffer, void (*freeMethod)(void *item)); 

typedef struct MAGE_VERTEX_ARRAY_STRUCT
{
	/* Buffer `reference` used by opengl */
	uint32 ID;

	/* List of the buffers stored */
	mageResizableList *Buffers;

} mageVertexArray;

/* Allocates the memroy for the vertex array */
extern void *mageVertexArrayAllocate();
/* Initialses the vertex array */
extern void mageVertexArrayInitialise(mageVertexArray *vertexArray);
/* Pushes the buffer to the list */
extern void mageVertexArrayPush(mageVertexArray *vertexArray, mageBuffer *buffer);
/* Binds the vertex array */
extern void mageVertexArrayBind(mageVertexArray *vertexArray);
/* Unbinds the vertex array */
extern void mageVertexArrayUnBind(mageVertexArray *vertexArray);
/* Destroys the vertex array and the buffers stored freeing the memory */
extern void mageVertexArrayDestroy(mageVertexArray *vertexArray, void (*freeMethod)(void *item));


/* Graphics related functions */

typedef struct MAGE_RENDERABLE_STRUCT
{
	mageVector3 Scale;
	mageVector3 Position;
	

} mageRenderable;

/* Allocates the memory for the renderable */
extern void *mageRenderableAllocate();
/* Intitalises the renderable suppling a scale position */
extern void mageRenderableInitialse(mageRenderable *renderable, const mageVector3 scale, const mageVector3 position);
/* Destroys the renderable freeing the memory */
extern void mageRenderableDestroy(mageRenderable *renderable, void (*freeMethod)(void *item));

/* Renderer related functions */

typedef struct MAGE_RENDERER_STRUCT
{
	mageResizableList *PipeLine;
	uint8 Flushable;
	void (*FreeMethod)(void *item);

} mageRenderer;

/* Allocates the memory for the renderer */
extern void *mageRendererAllocate();
/* Initialses the renderer allowing the list not to be flushed after render */
extern void mageRendererInitialises(mageRenderer *renderer, void (*freeMethod)(void *item), const uint8 flushContents);
/* Destroys the renderer freeing the memory */
extern void mageRendererDestroy(mageRenderer *renderer);
/* Render function overriden depending on the api used */
extern void mageRender2D(mageRenderer *renderer); 

#ifdef __MAGE_OPENGL_
	#define mageRender2D(x) { mageRenderOpenGL2D(x); }
#elif
	#define mageRender2D(x) { mageRenderVulkan2D(x); }
#elif
	#define mageRender2D(x) { mageRenderDirextX2D(x); }
#else
	#error Undefined Rendering API
#endif

/* Pushes a renderable to the pipeline */
extern void mageRenderPush(mageRenderer *renderer, mageRenderable *renderable);
/* Renderers 2D renderables using opengl */
extern void mageRenderOpenGL2D(mageRenderer *renderer);
/* Renderers 2D renderables using vulkan */
extern void mageRenderVulkan2D(mageRenderer *renderer);
/* Renderers 2D renderables using directx */
extern void mageRenderDirextX2D(mageRenderer *renderer);






#endif

