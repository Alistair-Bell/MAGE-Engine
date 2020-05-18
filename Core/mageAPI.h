#ifndef __MAGE_API__
#define __MAGE_API__

#include "mageCore.h"

typedef MAGE_API signed char int8;
typedef MAGE_API unsigned char uint8;
typedef MAGE_API signed short int16;
typedef MAGE_API unsigned short uint16;
typedef MAGE_API signed int int32;
typedef MAGE_API unsigned int uint32;
typedef MAGE_API signed long int64;
typedef MAGE_API unsigned long uint64;
typedef MAGE_API uint8 byte;


/*!************************
	@brief Initialises the dependencies of the library and other important stuff
	@param success A pointer where the success of the function will be dumped
	@return Nothing
**************************/
extern MAGE_API void mageEngineInitialise(uint8 *success);
/*!************************ 
	@brief Free method used by the destory methods throughout the API
	@param item pointer to a block of memory which will be freed
	@return Nothing
**************************/
extern MAGE_API void mageFreeMethod(void *item);
/*!************************ 
	@brief Allocation method used by the allocate methods throughout the API
	@param size The size of the allocation in bytes
	@return Void pointer to the block of memory allocated
**************************/
extern MAGE_API void *mageAllocationMethod(const uint64 size);
/*!************************
	@brief Tries to dump the contents into a pointer passed in
	@param contents The contents to dump in
	@param state The block of memory to dump the contents to
**************************/
extern MAGE_API void mageTryDumpSuccess(uint8 contents, uint8 *state);
/*!************************
	@brief Logs to the console and writes to an output
	@param user Core = 0 | Client = 1
	@param severity Inform = 0 | Warning = 1 | Error = 2 | Fatal Error = 3 (causes an assert break)
	@param line Line where it is being called from
	@param file File where it is being called from
	@param format Formatting using the printf style
	@param ... Values for the formatting 
	@return Nothing
**************************/
extern MAGE_API void mageLogMessage(const uint8 user, const uint8 severity, const uint32 line, const char *file, const char *format, ...);
/*!************************
	@brief Resets the color of the console
	@return Nothing 
**************************/
extern MAGE_API void mageLogReset();
/*!************************
	@brief Resizable list for storing varying amounts of one type
	@warning The list can only store 4294967295 numbers
**************************/
typedef struct MAGE_API  MAGE_RESIZABLE_LIST_STRUCT
{
	/*!************************
		@brief Array of void pointers to the element stored
	**************************/
	void **Elements;
	/*!************************
		@brief Count of the amount of elements stored
	**************************/
	uint32 Quantity;
	/*!************************ 
		@brief Size of each element in bytes 
	**************************/
	uint32 ElementSize;

} mageResizableList;

/*!************************
	@brief Allocates a block of memory for the resizable list
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
**************************/
extern MAGE_API void *mageResizableListAllocate();
/*!************************
	@brief Initialises the resizable list populating its members
	@param resizableList A pointer to a instance of a resizable list
	@param size Size of the element being stored in bytes
	@return Nothing
**************************/
extern MAGE_API void mageResizableListInitialise(mageResizableList *resizableList, const uint32 size);
/*!************************
	@brief Pushes element to the resizable list
	@param resizableList A pointer to a instance of a resizable list
	@param item A pointer to the block of memory that will be pushed to the elements
	@return Nothing
	@warning If the item's size in bytes is greater than the size previously specified errors will occur
	@warning Passing a uninitialised list will make erorrs occur
**************************/
extern MAGE_API void mageResizableListPush(mageResizableList *resizableList, void *item);
/*!************************
	@brief Pops the last element pushed onto the array allowing the client to handle the memory 
	@param resizableList A pointer to a instance of a resizable list
	@return Nothing
**************************/
extern MAGE_API void mageResizableListPop(mageResizableList *resizableList);
/*!************************ 
	@brief Frees all the elements
	@param resizableList A pointer to a instance of a resizable list
	@return Nothing
**************************/
extern MAGE_API void mageResizableListFreeElements(mageResizableList *resizableList);
/*!************************ 
	@brief Destroys the resizable list freeing itelsf
	@param resizableList A pointer to a instance of a resizable list
	@return Nothing
	@warning If the array not allocated on the heap then do not call this method
**************************/
extern MAGE_API void mageResizableListDestroy(mageResizableList *resizableList);

/*!************************
	@brief Stores a pair of values
**************************/
typedef struct MAGE_API  MAGE_PAIR_STRUCT
{
	/*!************************
		@brief A pointer to the first value stored
	**************************/
	void *First;	
	/*!************************
		@brief A pointer to the second value stored
	**************************/
	void *Second;
	/*!************************
		@brief The size of the first value in bytes
	**************************/
	uint32 FirstSize;
	/*!************************
		@brief The size of the first value in bytes
	**************************/
	uint32 SecondSize;

} magePair;

/*!************************ 
	@brief Allocates a block of memory for the pair
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
**************************/
extern MAGE_API void *magePairAllocate();
/*!************************ 
	@brief Initialises the pair populating the members
	@param pair A pointer to a instance of a pair
	@param firstSize Size of the first value in bytes
	@param secondSize Size of the second value in bytes
	@return Nothing
**************************/
extern MAGE_API void magePairInitialise(magePair *pair, const uint32 firstSize, const uint32 secondSize);
/*!************************ 
	@brief Sets the first value in the pair
	@param pair A pointer to a instance of a pair
	@param item The item that will be pushed to the first value		
	@return Nothing
**************************/
extern MAGE_API void magePairSetFirst(magePair *pair, void *item);
/*!************************ 
	@brief Sets the second value in the pair
	@param pair A pointer to a instance of a pair
	@param item The item that will be pushed to the second value	
	@return Nothing
	@warning If the item passed in is bigger than the size specified then problems will occur	
**************************/
extern MAGE_API void magePairSetSecond(magePair *pair, void *item);
/*!************************ 
	@brief Sets both values in the pair
	@param pair A pointer to a instance of a pair
	@param first The item that will be pushed to the first value	
	@param second The item that will be pushed to the second value	
	@return Nothing
**************************/
extern MAGE_API void magePairSetBoth(magePair *pair, void *first, void *second);
/*!************************ 
	@brief Gets the first value copying it into the buffer
	@param pair A pointer to a instance of a pair
	@param buffer Pointer to a block of memory where the contents will be dumped
	@param reallocatable If true the program will resize the buffer to prevent overflow
	@return Nothing
	@warning If reallocatable is true passing an unallocated buffer will cause a segmentation fault
**************************/
extern MAGE_API void magePairGetFist(magePair *pair, void *buffer, uint8 reallocatable);
/*!************************ 
	@brief Gets the second value copying it into the buffer
	@param pair A pointer to a instance of a pair
	@param buffer Pointer to a block of memory where the contents will be dumped
	@param reallocatable If true the program will resize the buffer to prevent overflow
	@return Nothing
	@warning If reallocatable is true passing an unallocated buffer will cause a segmentation fault
**************************/
extern MAGE_API void magePairGetSecond(magePair *pair, void *buffer, uint8 reallocatable);
/*!************************ 
	@brief Sets both values in the pair
	@param pair A pointer to a instance of a pair
	@param buffer1 Pointer to a block of memory where the contents of first will be dumped
	@param buffer2 Pointer to a block of memory where the contents of second will be dumped	
	@param reallocatable If true the program will resize the buffer to prevent overflow
	@return Nothing
	@warning If reallocatable is true passing an unallocated buffer will cause a segmentation fault
**************************/
extern MAGE_API void magePairGetBoth(magePair *pair, void *buffer1, void *buffer2, uint8 reallocatable);
/*!************************
	@brief Resizes the memory buffer of the pair's first buffer
	@param pair A pointer to a instance of a pair
	@param newSize The new size of the buffer
	@return Nothing
**************************/
extern MAGE_API void magePairResizeFirst(magePair *pair, const uint32 newSize);
/*!************************
	@brief Resizes the memory buffer of the pair's second buffer
	@param pair A pointer to a instance of a pair
	@param newSize The new size of the buffer
	@return Nothing
**************************/
extern MAGE_API void magePairResizeSecond(magePair *pair, const uint32 newSize);
/*!************************
	@brief Resizes the memory buffer of the both pair's value buffers
	@param pair A pointer to a instance of a pair
	@param newFirstSize The new size of the first buffer
	@param newSecondSize The new size of the second buffer
	@return Nothing
**************************/
extern MAGE_API void magePairResizeBoth(magePair *pair, const uint32 newFirstSize, const uint32 newSecondSize);
/*!************************
	@brief Frees the first and second item
	@param pair A pointer to a instance of a pair
	@return Nothing
**************************/
extern MAGE_API void magePairFree(magePair *pair);
/*!************************ 
	@brief Destroys the pair freeing itelsf
	@param pair A pointer to a instance of a pair
	@return Nothing
	@warning If the array not allocated on the heap then do not call this method
**************************/
extern MAGE_API void magePairDestroy(magePair *pair);

/*!************************
	@brief Stores a list of pairs
	@see magePair
**************************/
typedef struct MAGE_API MAGE_DICTIONARY_STRUCT
{
	/*!************************
		@brief List of all all the pairs being stored
	**************************/
	mageResizableList *Elements;
	
} mageDictionary;

/*!************************ 
	@brief Allocates a block of memory for the dictionary
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
**************************/
extern MAGE_API void *mageDictionaryAllocate();
/*!************************ 
	@brief Initialises the dictionary
	@param dictionary A pointer to a instance of a dictionary
	@return Nothing
**************************/
extern MAGE_API void mageDictionaryInitialise(mageDictionary *dictionary);
/*!************************ 
	@brief Pushes a pair to the dictionary
	@param dictionary A pointer to a instance of a dictionary
	@param pair A pointer to the pair to be pushed
	@return Nothing
**************************/
extern MAGE_API void mageDictionaryPush(mageDictionary *dictionary, magePair *pair);
/*!************************
	@brief Pops the last pair pushed
	@param dictionary A pointer to a instance of a dictionary
	@return Nothing
**************************/
extern MAGE_API void mageDictionaryPop(mageDictionary *dictionary);
/*!************************
	@brief Dumps a copy of the first pair in the list
	@param dictionary A pointer to a instance of a dictionary
	@param buffer Where the coppy will be dumped
	@return Nothing
**************************/
extern MAGE_API void mageDictionaryFetch(mageDictionary *dictionary, magePair *buffer);
/*!************************
	@brief Dumps a copy of the pair specified by index in the list to the buffer
	@param dictionary A pointer to a instance of a dictionary
	@param buffer Where the coppy will be dumped
	@param index The index in the list of pairs
	@return Nothing
	@warning Passing an invalid index will cause a memory error
**************************/
extern MAGE_API void mageDictionaryFetchIndex(mageDictionary *dictionary, magePair *buffer, const uint32 index);


/*!************************
	@brief Reads the files contents dumping the contents into the buffer passed int
	@param file Path to the file 
	@param buffer Pointer to a char buffer where the contents will be dumped
	@param reallocatable If true the program will resize the buffer to prevent overflow
	@param success A pointer where the success of the function will be dumped
	@return Nothing
	@warning If the buffer is too small then the data will be potentially incomplete
**************************/
extern MAGE_API void mageFileReadContents(const char *file, char *buffer, const uint8 reallocatable, uint8 *success);
/*!************************
	@brief Dumps the contents of a buffer to a file
	@param file Path to the file 
	@param buffer Pointer to a char buffer containing the contents to be dumped to the file
	@param clean If true the program will clean the contents of the file before dumping
	@param success A pointer where the success of the function will be dumped
	@return Nothing
**************************/
extern MAGE_API void mageFileDumpContents(const char *file, const char *buffer, const uint8 clean, uint8 *success);


/*!************************ 
	@brief Stores 2 floats within the struct
**************************/
typedef struct MAGE_API MAGE_VECTOR2_STRUCT
{
	/*!************************ 
		@brief First value
	**************************/
	float x;
	/*!************************ 
		@brief Second value
	**************************/
	float y;

} mageVector2;

/*!************************ 
	@brief Allocates a block of memory for the vector2
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
**************************/ 
extern MAGE_API void *mageVector2Allocate();
/*!************************ 
	@brief Intitalises the vector2 based on the values passed 
	@param vector2 A pointer to a instance of a vector2
	@param value1 The value which x will be set to
	@param value2 The value which y will be set to
	@return Nothing
**************************/
extern MAGE_API void mageVector2Initialise(mageVector2 *vector2, const float value1, const float value2);
/*!************************ 
	@brief Adds the right vector2 to the left vector2 
	@param left A pointer to a instance of a vector2 which will be modified
	@param right A pointer to a instance of a vector2 which will add the left
	@return Nothing 	
**************************/
extern MAGE_API void mageVector2Add(mageVector2 *left, const mageVector2 *right);
/*!************************ 
	@brief Subtracts the right vector2 to the left vector2 
	@param left A pointer to a instance of a vector2 which will be modified
	@param right A pointer to a instance of a vector2 which will subtract the left
	@return Nothing 	
**************************/
extern MAGE_API void mageVector2Subtract(mageVector2 *left, const mageVector2 *right);
/*!************************ 
	@brief Multiplies the right vector2 to the left vector2 
	@param left A pointer to a instance of a vector2 which will be modified
	@param right A pointer to a instance of a vector2 which will multiply the left
	@return Nothing 	
**************************/
extern MAGE_API void magemageVector2Multiply(mageVector2 *left, const mageVector2 *right);
/*!************************ 
	@brief Divides the right vector2 to the left vector2 
	@param left A pointer to a instance of a vector2 which will be modified
	@param right A pointer to a instance of a vector2 which will divide the left
	@return Nothing 	
**************************/
extern MAGE_API void mageVector2Divide(mageVector2 *left, const mageVector2 *right);
/*!************************ 
	@brief Destroys the vector2 freeing itelsf
	@param vector A pointer to a instance of a vector2
	@return Nothing
	@warning If the vector2 not allocated on the heap then do not call this method
**************************/
extern MAGE_API void mageVector2Destroy(mageVector2 *vector);

/*!************************ 
	@brief Stores 3 floats within the struct
**************************/
typedef struct MAGE_API MAGE_VECTOR3_STRUCT
{
	/*!************************ 
		@brief First value
	**************************/
	float x;
	/*!************************ 
		@brief Second value 
	**************************/
	float y;
	/*!************************ 
		@brief Third value
	**************************/
	float z;	

} mageVector3;

/*!************************ 
	@brief Allocates a block of memory for the vector3
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
**************************/ 
extern MAGE_API void *mageVector3Allocate();
/*!************************ 
	@brief Intitalises the vector3 based on the values passed 
	@param vector3 A pointer to a instance of a vector3
	@param value1 The value which x will be set to
	@param value2 The value which y will be set to
	@param value3 The value which z will be set to
	@return Nothing
**************************/
extern MAGE_API void mageVector3Initialise(mageVector3 *vector3, const float value1, const float value2, const float value3);
/*!************************ 
	@brief Adds the right vector3 to the left vector3 
	@param left A pointer to a instance of a vector3 which will be modified
	@param right A pointer to a instance of a vector3 which will add the left
	@return Nothing 	
**************************/
extern MAGE_API void mageVector3Add(mageVector3 *left, const mageVector3 *right);
/*!************************ 
	@brief Subtracts the right vector3 to the left vector3 
	@param left A pointer to a instance of a vector3 which will be modified
	@param right A pointer to a instance of a vector3 which will subtract the left
	@return Nothing 	
**************************/
extern MAGE_API void mageVector3Subtract(mageVector3 *left, const mageVector3 *right);
/*!************************ 
	@brief Multiplies the right vector3 to the left vector3 
	@param left A pointer to a instance of a vector3 which will be modified
	@param right A pointer to a instance of a vector3 which will multiply the left
	@return Nothing 	
**************************/
extern MAGE_API void mageVector3Multiply(mageVector3 *left, const mageVector3 *right);
/*!************************ 
	@brief Divides the right vector3 to the left vector3 
	@param left A pointer to a instance of a vector3 which will be modified
	@param right A pointer to a instance of a vector3 which will divide the left
	@return Nothing 	
**************************/
extern MAGE_API void mageVector3Divide(mageVector3 *left, const mageVector3 *right);
/*!************************
	@brief Destroys the vector3 freeing itelsf
	@param vector A pointer to a instance of a vector3
	@return Nothing
	@warning If the vector3 is not allocated on the heap then do not call this method
**************************/
extern MAGE_API void mageVector3Destroy(mageVector3 *vector);

/*!************************ 
	@brief Stores 4 floats within the struct
**************************/
typedef struct MAGE_API MAGE_VECTOR4_STRUCT
{
	/*!************************ 
		@brief First value
	**************************/
	float x;
	/*!************************ 
		@brief Second value
	**************************/
	float y;
	/*!************************ 
		@brief Third value
	**************************/
	float z;
	/*!************************ 
		@brief Fourth value
	**************************/
	float w;	

} mageVector4;

/*!************************ 
	@brief Allocates a block of memory for the vector4
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
**************************/ 
extern MAGE_API void *mageVector4Allocate();
/*!************************ 
	@brief Intitalises the vector3 based on the values passed 
	@param vector4 A pointer to a instance of a vector4
	@param value1 The value which x will be set to
	@param value2 The value which y will be set to
	@param value3 The value which z will be set to
	@param value4 The value which w will be set to
	@return Nothing
**************************/
extern MAGE_API void mageVector4Initialise(mageVector4 *vector4, const float value1, const float value2, const float value3, const float value4);
/*!************************ 
	@brief Intitalises the vector4 based 2 vector2's
	@param vector4 A pointer to a instance of a vector4
	@param left The vector's x and y will set the vector4's x and y 
	@param right The vector's x and y will set the vector4's z and w
	@return Nothing
**************************/
extern MAGE_API void mageVector4InitialiseVector2(mageVector4 *vector4, const mageVector2 *left, const mageVector2 *right);
/*!************************ 
	@brief Adds the right vector4 to the left vector4 
	@param left A pointer to a instance of a vector4 which will be modified
	@param right A pointer to a instance of a vector4 which will add the left
	@return Nothing 	
**************************/
extern MAGE_API void mageVector4Add(mageVector4 *left, const mageVector4 *right);
/*!************************ 
	@brief Subtracts the right vector4 to the left vector4 
	@param left A pointer to a instance of a vector4 which will be modified
	@param right A pointer to a instance of a vector4 which will subtract the left
	@return Nothing 	
**************************/
extern MAGE_API void mageVector4Subtract(mageVector4 *left, const mageVector4 *right);
/*!************************ 
	@brief Multiplies the right vector4 to the left vector4 
	@param left A pointer to a instance of a vector4 which will be modified
	@param right A pointer to a instance of a vector4 which will multiply the left
	@return Nothing 	
**************************/
extern MAGE_API void mageVector4Multiply(mageVector4 *left, const mageVector4 *right);
/*!************************ 
	@brief Divides the right vector4 to the left vector4 
	@param left A pointer to a instance of a vector4 which will be modified
	@param right A pointer to a instance of a vector4 which will divide the left
	@return Nothing 	
**************************/
extern MAGE_API void mageVector4Divide(mageVector4 *left, const mageVector4 *right);
/*!************************
	@brief Destroys the vector4 freeing itelsf
	@param vector A pointer to a instance of a vector4
	@return Nothing
	@warning If the vector4 is not allocated on the heap then do not call this method
**************************/
extern MAGE_API void mageVector4Destroy(mageVector4 *vector);

/*!************************
	@brief Stores 16 floating point numbers
	@brief When writing shaders in GLSL matrix4x4 is called mat4
	@warning This implimentation uses column majoring
	@see https://en.wikipedia.org/wiki/Row-_and_column-major_order
**************************/
typedef struct MAGE_API MAGE_MATRIX4X4
{
	/*!************************
		@brief Stores 16 floats
	**************************/
	float Elements[16];

} mageMatrix4x4;

/*!************************ 
	@brief Allocates a block of memory for the matrix
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
**************************/ 
extern MAGE_API void *mageMatrix4x4Allocate();
/*!************************
	@brief Sets all the matrix's elements to 0.0f 	
	@param matrix A pointer to a instance of a matrix
	@return Nothing
**************************/
extern MAGE_API void mageMatrix4x4Default(mageMatrix4x4 *matrix);
/*!************************
	@brief Initialises the matrix based on 4 vector4s
	@param matrix A pointer to a instance of a matrix
	@param column0 A pointer to a insatnce of a vector4 which the matrix's 0 index row will be set to.
				   Elements[0] = column0.x | Elements[4] = column0.y Elements[8] = column0.z | Elements[12] = column0.w
	@param column1 A pointer to a insatnce of a vector4 which the matrix's 1 index row will be set to.
				   Elements[1] = column1.x | Elements[5] = column1.y Elements[9] = column1.z | Elements[13] = column1.w
	@param column2 A pointer to a insatnce of a vector4 which the matrix's 2 index row will be set to.
				   Elements[2] = column1.x | Elements[6] = column1.y Elements[10] = column1.z | Elements[14] = column1.w
	@param column3 A pointer to a insatnce of a vector4 which the matrix's 3 index row will be set to.
				   Elements[3] = column1.x | Elements[7] = column1.y Elements[11] = column1.z | Elements[15] = column1.w
	@return Nothing
**************************/
extern MAGE_API void mageMatrix4x4InitialiseVector4(mageMatrix4x4 *matrix, const mageVector4 *column0, const mageVector4 *column1, const mageVector4 *column2, const mageVector4 *column3);
/*!************************
	@brief Initialises the matrix based on passing in a set number of floats
	@param matrix A pointer to a instance of a matrix
	@param elements A array of floating points
	@param count Count of the amount of floats being passed in
	@return Nothing
**************************/
extern MAGE_API void mageMatrix4x4InitialiseArray(mageMatrix4x4 *matrix, const float *elements, const uint8 count);
/*!************************ 
	@brief Initialises the matrix using a diagonal
	@param matrix A pointer to a instance of a matrix
	@param diagonal Sets the diagonal elements to the value passed in
	@return Nothing
**************************/
extern MAGE_API void mageMatrix4x4InitialiseDiagonal(mageMatrix4x4 *matrix, const float diagonal);
/*!************************
`	@brief Multiplies the left matrix by the right matrix
	@param left The left matrix
	@param right The right matrix
	@param result The result where the calculation will be dumped
	@return Nothing
**************************/
extern MAGE_API void mageMatrix4x4Multiply(const mageMatrix4x4 *left, const mageMatrix4x4 *right, mageMatrix4x4 *result);
/*!************************
	@brief Applies a perspective matrix to the matrix
	@param matrix A pointer to a instance of a matrix
	@param fov The angle the rays get cas
	@param aspectRatio The aspect ratio of the screen 
	@param near The nearest plane to clip
	@param far The furthest plane to clip
	@return Nothing
	@see https://en.wikipedia.org/wiki/3D_projection
**************************/
extern MAGE_API void mageMatrix4x4Perspective(mageMatrix4x4 *matrix, const float fov, const float aspectRatio, const float near, const float far);
/*!************************
	@brief Applies a orthographic matrix to the matrix
	@param matrix A pointer to a instance of a matrix
	@param left The leftmost plane
	@param right The rightmost plane
	@param bottom The bottommost plane
	@param top The topmost plan
	@param near The nearest plane included to the start of the projection
	@param far The furthest plane included to the start of the projection
	@return Nothing
	@see https://en.wikipedia.org/wiki/3D_projection
**************************/
extern MAGE_API void mageMatrix4x4Orthographic(mageMatrix4x4 *matrix, const float left, const float right, const float bottom, const float top, const float near, const float far);
/*!************************
	@brief Translates the matrix
	@param matrix A pointer to a instance of a matrix
	@param translation The translation to be applied
	@return Nothing
**************************/
extern MAGE_API void mageMatrix4x4Translation(mageMatrix4x4 *matrix, const mageVector3 *translation);
/*!************************
	@brief Rotates a matrix using a axis and angle
	@param matrix A pointer to a instance of a matrix
	@param angle The angle of rotation
	@param axis Axis to be used for rotation
	@return Nothing
**************************/
extern MAGE_API void mageMatrix4x4Rotation(mageMatrix4x4 *matrix, const float angle, const mageVector3 *axis);
/*!************************
	@brief Scales a matrix using 3 floats (vector3)
	@param matrix A pointer to a instance of a matrix
	@param scale The scale the matrix will be applying
	@return Nothing
**************************/
extern MAGE_API void mageMatrix4x4Scale(mageMatrix4x4 *matrix, const mageVector3 *scale);
/*!************************
	@brief Inverts using matrix maths
	@param matrix A pointer to a instance of a matrix
	@return Nothing
**************************/
extern MAGE_API void mageMatrix4x4Invert(mageMatrix4x4 *matrix);
/*!************************
	@brief Destroys the matrix freeing itelsf
	@param matrix A pointer to a instance of a matrix
	@return Nothing
	@warning If the matrix is not allocated on the heap then do not call this method
**************************/
extern MAGE_API void mageMatrix4x4Destroy(mageMatrix4x4 *matrix);
/*!************************
	@brief Converts degrees into radians
	@param degrees The degrees to convert to radians
	@param result A pointer to where the result will be dumped
	@return Nothing
**************************/
extern MAGE_API void mageToRadians(const float degrees, float *result);


/*!************************
	@brief Window context used by the engine to render to
	@brief The window can either use SDL2 or GLFW3 as its framework
**************************/
typedef struct MAGE_API MAGE_WINDOW_STRUCT
{
	/*!************************
		@brief Width of the window context
	**************************/
	int32 Width;
	/*!************************
		@brief Height of the window context
	**************************/
	int32 Height;
	/*!************************
		@brief Title of the window context
	**************************/
	const char *Title;
	
	#if defined(MAGE_SDL2)
		/*!************************
			@brief Window context using the SDL2 framework
		**************************/
		SDL_Window *Context;
		/*!************************
			@brief SDL2 event poller context
		**************************/
		SDL_Event Events;
	#elif defined(MAGE_GLFW)
		/*!************************ 
			@brief Window context using the GLFW3 framework
		**************************/		
		GLFWwindow *Context;
		/*!************************
			@brief Renderer used by the application
		**************************/
	#endif
	
	/*!************************
		@brief A flag whether the window is running
	**************************/
	uint8 Running;	

} mageWindow;

/*!************************ 
	@brief Allocates a block of memory for the game window
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
**************************/ 
extern MAGE_API void *mageWindowAllocate();
/*!************************ 
	@brief Initialises the game window populating its members
	@param window A pointer to a instance of a window
	@param xResolution Width of the window created
	@param yResolution Height of thw window created
	@param title Title of the window
	@param success A pointer where the success of the function will be dumped
	@return Nothing
	@warning The window will not be constructed if the libraries fail to be initialsied
**************************/
extern MAGE_API void mageWindowInitialise(mageWindow *window, const int32 xResolution, const int32 yResolution, const char *title, uint8 *success);
/*!************************
	@brief Swaps the buffers of the window
	@param window A pointer to a instance of a window
	@return Nothing
**************************/
extern MAGE_API void mageWindowSwapBuffers(mageWindow *window);
/*!************************
	@brief Terminates the window API
	@param window A pointer to a instance of a window
	@return Nothing
**************************/
extern MAGE_API void mageWindowTerminate(mageWindow *window);
/*!************************ 
	@brief Destroys the resizable list freeing itelsf
	@param window A pointer to a instance of a window
	@return Nothing
	@warning If the window not allocated on the heap then do not call this method
**************************/
extern MAGE_API void mageWindowDestroy(mageWindow *window); 
/*!************************ 
	@brief Gets the mouse's x and y coordinates 
	@param window A pointer to a instance of a window
	@param x A pointer to where the x coordinate will be dumped 
	@param y A pointer to where the x coordinate will be dumped 
	@return Nothing
**************************/
extern MAGE_API void mageGetMousePosition(mageWindow *window, double *x, double *y);
/*!************************ 
	@brief Gets the mouse's x coordinate 
	@param window A pointer to a instance of a window
	@param x A pointer to where the x coordinate will be dumped 
	@return Nothing
**************************/
extern MAGE_API void mageGetMousePositionX(mageWindow *window, double *x);
/*!************************ 
	@brief Gets the mouse's y coordinate 
	@param window A pointer to a instance of a window
	@param y A pointer to where the y coordinate will be dumped 
	@return Nothing
**************************/
extern MAGE_API void mageGetMousePositionY(mageWindow *window, double *y);
/*!************************ 
	@brief Gets whether the left mouse button is being clicked
	@param window A pointer to a instance of a window
	@param state A pointer to whether its true will be dumped 
	@return Nothing
**************************/
extern MAGE_API void mageGetMouseButtonLeftClick(mageWindow *window, uint8 *state);
/*!************************ 
	@brief Gets whether the right mouse button is being clicked
	@param window A pointer to a instance of a window
	@param state A pointer to whether its true will be dumped 
	@return Nothing
**************************/
extern MAGE_API void mageGetMouseButtonRightClick(mageWindow *window, uint8 *state);
/*!************************ 
	@brief Gets whether the mouse is inside the window context
	@param window A pointer to a instance of a window
	@param state A pointer to whether its true will be dumped 
	@return Nothing
**************************/
extern MAGE_API void mageGetMouseInsideContext(mageWindow *window, uint8 *state);
/*!************************ 
	@brief Sets the mouse x and y to a specific location
	@param window A pointer to a instance of a window
	@param x Location the mouses x coordinate will be set at
	@param y Location the mouses y coordinate will be set at
	@return Nothing
**************************/
extern MAGE_API void mageSetMousePosition(mageWindow *window, const double x, const double y);
/*!************************ 
	@brief Gets whether the key is being pressed down
	@param window A pointer to a instance of a window
	@param key A keycode for the key that is being tested
	@param state A pointer to whether its true will be dumped 
	@return Nothing
**************************/
extern MAGE_API void mageGetKeyDown(mageWindow *window, const int32 key, uint8 *state);
/*!************************ 
	@brief Gets whether the key is not being pressed down
	@param window A pointer to a instance of a window
	@param key A keycode for the key that is being tested
	@param state A pointer to whether its true will be dumped 
	@return Nothing
**************************/
extern MAGE_API void mageGetKeyNotDown(mageWindow *window, const int32 key, uint8 *state);

/*!************************
	@brief Object that renders and is pushed to the renderer pipeline
**************************/
typedef struct MAGE_API MAGE_RENDERABLE_STRUCT
{
	/*!************************ 
		@brief How the renderable will scale 
	**************************/
	mageVector3 Scale;
	/*!************************ 
		@brief Position in the scene
	**************************/
	mageVector3 Position;

} mageRenderable;
/*!************************ 
	@brief Allocates a block of memory for the renderable
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
**************************/
extern MAGE_API void *mageRenderableAllocate();
/*!************************ 
	@brief Intitalises the renderable based on  a scale and position
	@param renderable A pointer to a instance of a renderable
	@param scale Scale of the surface that will be rendered
	@param position Position in the world the object will be at
	@warning Negative values for the scale may cause issues
**************************/
extern MAGE_API void mageRenderableInitialse(mageRenderable *renderable, mageVector3 scale, mageVector3 position);
/*!************************
	@brief Destroys the renderable freeing itelsf
	@param renderable A pointer to a instance of a renderable
	@return Nothing
	@warning If the renderable is not allocated on the heap then do not call this method
**************************/
extern MAGE_API void mageRenderableDestroy(mageRenderable *renderable);

/*!************************
	@brief Hanldes vulkan stuff
**************************/
typedef struct MAGE_API MAGE_VULKAN_HANDLER_STRUCT
{
	#if defined(MAGE_VULKAN)

		VkPhysicalDeviceMemoryProperties MemoryProperties;

		VkDevice Device;

		VkPhysicalDevice PhysicalDevice;

		VkPhysicalDeviceProperties PhysicalProperties;

		VkCommandPool CommandPool;
	
		VkSemaphore Semaphore;

		VkSurfaceKHR Surface;

		VkQueue GraphicsQueue;

		VkFence Fence; 

		VkInstance Instance;

		uint32 GraphicsFamilyIndex;

	#endif

} mageVulkanHandler;

/*!************************ 
	@brief Allocates a block of memory for the handler
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
**************************/
extern MAGE_API void *mageVulkanHandlerAllocate();
/*!************************
	@brief Destroys the renderer freeing itelsf
	@param handler A pointer to a instance of a vulkan handler
	@param window A pointer to a instance of a window
	@param success A pointer where the success of the function will be dumped
	@return Nothing
**************************/
extern MAGE_API void mageVulkanHandlerInitialise(mageVulkanHandler *handler, mageWindow *window, uint8 *success);


/*!************************
	@brief Renderer that renders objects pushed into the pipeline
**************************/
typedef struct MAGE_API MAGE_RENDERER_STRUCT
{
	#if defined(MAGE_VULKAN)

		/*!************************
			@brief Handler for the vulkan setup
		**************************/
		mageVulkanHandler Handler;
	
	#endif

} mageRenderer;

/*!************************ 
	@brief Allocates a block of memory for the renderer
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
**************************/ 
extern MAGE_API void *mageRendererAllocate();
/*!************************
	@brief Initialises the renderer
	@param renderer A pointer to a instance of a renderer
	@param window A pointer to a instance of a window
	@param success A pointer where the success of the function will be dumped
	@return Nothing
**************************/
extern MAGE_API void mageRendererInitialise(mageRenderer *renderer,  mageWindow *window, uint8 *success);
/*!************************
	@brief Destroys the renderer freeing itelsf
	@param renderer A pointer to a instance of a renderer
	@return Nothing
	@warning If the renderer is not allocated on the heap then do not call this method
**************************/
extern MAGE_API void mageRendererDestroy(mageRenderer *renderer);
/*!************************
	@brief Camera used for the scene
**************************/
typedef struct MAGE_API MAGE_CAMERA_STRUCT
{
	/*!************************
		@brief The flags of the camera
	**************************/
	uint8 Flags;
	/*!************************
		@brief The position within the world
	 **************************/
	mageVector3 Position;
	/*!************************
		@brief The projection matrix of the camera 
	**************************/
	mageMatrix4x4 ProjectionMatrix;
	/*!************************
		@brief The view matrix of the camera 
	**************************/
	mageMatrix4x4 ViewMatrix;
	/*!************************
		@brief The view projection matrix of the camera 
	**************************/
	mageMatrix4x4 ViewProjectionMatrix;
	/*!************************
		@brief Rotation the camera is at
	**************************/
	float Rotation;

} mageCamera;

/*!************************ 
	@brief Allocates a block of memory for the camera
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
**************************/ 
extern MAGE_API void *mageCameraAllocate();
/*!************************
	@brief Initialises the camera for use
	@param camera A pointer to a instance of a camera
	@param position The position within the world
	@param rotation Rotation of the camera
	@param flags The flags 
**************************/
extern MAGE_API void mageCameraInitialise(mageCamera *camera, const mageVector3 position, const float rotation, const uint8 flags);
/*!************************ 
	@brief Calculates the projection for the matrix
	@param camera A pointer to a instance of a camera
	@param left Left value used for the view calculations
	@param right Right value used for the view calculations
	@param bottom Bottom value used for the view calculations
	@param top Top value used for the view calculations
	@return Nothing
**************************/ 
extern MAGE_API void mageCameraOrthographicSetProjection(mageCamera *camera, const float left, const float right, const float bottom, const float top);
/*!************************
	@brief recalculates the projection for the matrix
	@param camera A pointer to a instance of a camera
	@return Nothing
**************************/
extern MAGE_API void mageCameraOrthographicRecalculateViewMatrix(mageCamera *camera);
/*!************************
	@brief Gets the value of the bit using the index provided
	@param camera A pointer to a instance of a camera
	@param bit The index of the bit being retrieved
	@param value Where the value of the bit will be dumped
	@return Nothing
	@warning Passing in an invalid bit will cause a possible wrong result
**************************/
extern MAGE_API void mageCameraGetFlag(mageCamera *camera, const uint8 bit, uint8 *value);
/*!************************
	@brief Sets the camera flag using the index provided
	@param camera A pointer to a instance of a camera
	@param bit The index of the bit being retrieved
	@param value Where the value of the bit will be dumped
	@return Nothing
	@warning Passing in an invalid bit will still cause a bit to be changed
	@warning Passing in a number greater that 1 through value will cause errors
**************************/
extern MAGE_API void mageCameraSetFlag(mageCamera *camera, const uint8 bit, uint8 value);

/*!************************
	@brief Scene stores all objects cameras and renderers
**************************/
typedef struct MAGE_API MAGE_SCENE_STRUCT
{
	/*!************************
		@brief The cameras used in the scene
	**************************/	
	mageResizableList *Cameras;
	/*!************************
		@brief Pointer to the main camera of the scene
	**************************/
	mageCamera *MasterCamera;


} mageScene;

/*!************************ 
	@brief Allocates a block of memory for the scene
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
**************************/ 
extern MAGE_API void *mageSceneAllocate();
/*!************************
	@brief Initialises the scene
	@param scene A pointer to a instance of a scene
	@return Nothing
**************************/
extern MAGE_API void mageSceneInitialise(mageScene *scene);
/*!************************
	@brief Binds the master camera to the scene
	@param scene A pointer to a instance of a scene
	@param camera A pointer to a instance of a camera
	@return Nothing
**************************/
extern MAGE_API void mageSceneBindMasterCamera(mageScene *scene, mageCamera *camera);
/*!************************
	@brief Adds a camera to the scene
	@param scene A pointer to a instance of a scene
	@param cameras A pointer to an array of instances of a camera
	@param count The count of cameras to push
	@return Nothing
**************************/
extern MAGE_API void mageScenePushCameras(mageScene *scene, mageCamera **cameras);

/*!************************
	@brief Constructer used for the application
**************************/
typedef struct MAGE_API MAGE_APPLICATION_PROPS_STRUCT
{
	/*!************************
		@brief Application version
	**************************/
	double Version;
	/*!************************
		@brief The width of the applications window
	**************************/
	uint32 Width;
	/*!************************
		@brief The height of the applications window
	**************************/
	uint32 Height;
	/*!************************
		@brief The name of the application 
	**************************/
	char *Name;

} mageApplicationProps;

/*!************************
	@brief Wrapper for all the objects
**************************/
typedef struct MAGE_API MAGE_APPLICATION_STRUCT
{
	/*!************************
	@brief The renderer used by the application
	**************************/
	mageRenderer *Renderer;
	/*!************************
	@brief The current scene loaded by the application
	**************************/
	mageScene *CurrentScene;
	/*!************************
	@brief The window used by the application
	**************************/
	mageWindow *Window;


} mageApplication;

/*!************************ 
	@brief Allocates a block of memory for the application
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
**************************/ 
extern MAGE_API void *mageApplicationAllocate();
/*!************************
	@brief Initialises the application
	@param application A pointer to a instance of a scene
	@param props The props used to construct the application and its members
	@param success A pointer where the success of the function will be dumped
	@return Nothing
**************************/
extern MAGE_API void mageApplicationInitialise(mageApplication *application, const mageApplicationProps *props, uint8 *success);
/*!************************
	@brief Loads a scene for the application
	@param application A pointer to a instance of a scene
	@param scene A pointer to a instance of a scene
	@return Nothing
**************************/
extern MAGE_API void mageApplicationLoadScene(mageApplication *application, mageScene *scene);








#endif  


