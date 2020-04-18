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


/* Memory struct for debugging leaks */
typedef struct MAGE_MEMORY_STRUCT
{
	/* Location to the memory that was allocated */
	void *Location;
	/* Line that the memroy was allocated from */
	uint32 Line;
	/* File in which the memory was called from */
	const char *File;	
	/* Size in bytes */ 
	uint32 Size;
	/* Whether it has been freed */ 
	uint8 Freed;

} mageMemoryDebugPackage;

/* Allows debuging for malloc function calls */
extern void *mageMallocDebug(const uint64 size, const char *file, const uint32 line);
/* Allows debuging for the calloc function calls */
extern void *mageCallocDebug(const uint64 items, const uint64 size, const char *file, const uint32 line);
/* Allows debuging for the realloc function calls */
extern void *mageReallocDebug(void *memory, const uint64 size, const char *file, const uint32 line);

#ifdef __MAGE_MEMORY_DEBUG__
	#define malloc(s) mageMallocDebug(s, __FILE__, __LINE__);
	#define calloc(m, s) mageCallocDebug(m, s, __FILE__, __LINE__);
	#define realloc(m, s) mageReallocDebug(m, s, __FILE__, __LINE__);
#endif

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

} Vector2;

/* Intitalises the vector2 based on the values passed */
extern void mageVector2Initialise(Vector2 *vector2, const float value1, const float value2);
/* Adds the right vector2 to the left vector2 */
extern void mageVector2Add(Vector2 *left, const Vector2 *right);
/* Subtracts the right vector2 to the left vector2 */
extern void mageVector2Subtract(Vector2 *left, const Vector2 *right);
/* Multiplies the right vector2 to the left vector2 */
extern void mageVector2Multiply(Vector2 *left, const Vector2 *right);
/* Divides the right vector2 to the left vector2 */ 
extern void mageVector2Divide(Vector2 *left, const Vector2 *right);

typedef struct MAGE_VECTOR3_STRUCT
{
	float x;
	float y;
	float z;	

} Vector3;

/* Intitalises the vector3 based on the values passed */
extern void mageVector3Initialise(Vector3 *vector3, const float value1, const float value2, const float value3);
/* Adds the right vector3 to the left vector3 */
extern void mageVector3Add(Vector3 *left, const Vector3 *right);
/* Subtracts the right vector3 to the left vector3 */
extern void mageVector3Subtract(Vector3 *left, const Vector3 *right);
/* Multiplies the right vector3 to the left vector3 */
extern void mageVector3Multiply(Vector3 *left, const Vector3 *right);
/* Divides the right vector3 to the left vector3 */ 
extern void mageVector3Divide(Vector3 *left, const Vector3 *right);

typedef struct MAGE_VECTOR4_STRUCT
{
	float x;
	float y;
	float z;
	float w;	

} Vector4;

/* Intitalises the vector4 based on the values passed */
extern void mageVector4Initialise(Vector4 *vector4, const float value1, const float value2, const float value3, const float value4);
/* Initialses the vector4 by using values passed in from two vector2's */
extern void mageVector4InitialiseVector2(Vector4 *vector4, const Vector2 *left, const Vector2 *right);
/* Adds the right vector4 to the left vector4 */
extern void mageVector4Add(Vector4 *left, const Vector4 *right);
/* Subtracts the right vector4 to the left vector4 */
extern void mageVector4Subtract(Vector4 *left, const Vector4 *right);
/* Multiplies the right vector4 to the left vector4 */
extern void mageVector4Multiply(Vector4 *left, const Vector4 *right);
/* Divides the right vector4 to the left vector4 */ 
extern void mageVector4Divide(Vector4 *left, const Vector4 *right);

/* Stores sixteen floats using column majoring for opengl */
typedef struct MAGE_MATRIX4X4_STRUCT
{
	float Elements[16];

} Matrix4x4;

/* Initialses a matrix using four vector4's */
extern void mageMatrixInitialise(Matrix4x4 *matrix, const Vector4 *row0, const Vector4 *row1, const Vector4 *row2, const Vector4 *row3);
/* Multiplies the right matrix4x4 to the left matrix4x4 */
extern void mageMatrixMultiply(Matrix4x4 *left, const Matrix4x4 *right);

/* Graphics related functions */

typedef struct MAGE_RENDERER_STRUCT
{

} mageRenderer;












#endif
