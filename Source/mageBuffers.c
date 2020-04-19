#include "mageAPI.h"

/* Impliments buffer related stuff */
void *mageBufferAllocate()
{
	return malloc(sizeof(struct MAGE_BUFFER_STRUCT));
}
void mageBufferInitialise(mageBuffer *buffer, const float *data, const uint32 quantity, const uint32 componentCount)
{
	buffer->ComponentCount = componentCount;
	#ifdef __MAGE_OPENGL_
		glGenBuffers(1, &buffer->ID);
		glBindBuffer(GL_ARRAY_BUFFER, buffer->ID);
		glBufferData(GL_ARRAY_BUFFER, quantity * sizeof(float), data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);	
	#endif
}
void mageBufferBind(mageBuffer *buffer)
{
	#ifdef __MAGE_OPENGL_
		glBindBuffer(GL_ARRAY_BUFFER, buffer->ID);
	#endif
}
void mageBufferUnBind(mageBuffer *buffer)
{
	#ifdef __MAGE_OPENGL_
		glBindBuffer(GL_ARRAY_BUFFER, 0);	
	#endif
}
void mageBufferDestroy(mageBuffer *buffer, void (*freeMethod)(void *item))
{
	if (freeMethod == NULL)
		freeMethod = free;
	freeMethod(buffer);

}
void *mageIndexBufferAllocate()
{
	return malloc(sizeof(struct MAGE_INDEX_BUFFER_STRUCT));
}
void mageIndexBufferInitialise(mageIndexBuffer *buffer, const uint32 *data, const uint32 quantity)
{
	buffer->ComponentCount = quantity;
	#ifdef __MAGE_OPENGL_
		GLCall(glGenBuffers(1, &buffer->ID));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (quantity * sizeof(short)), data, GL_STATIC_DRAW);
	#endif
}
void mageIndexBufferBind(mageIndexBuffer *buffer)
{
	#ifdef __MAGE_OPENGL_
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ID);
	#endif
}
void mageIndexBufferUnBind(mageIndexBuffer *buffer)
{
	#ifdef __MAGE_OPENGL_
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	#endif
}
void mageIndexBufferDestroy(mageIndexBuffer *buffer, void (*freeMethod)(void *item))
{
	if (freeMethod == NULL)
		freeMethod = free;
	freeMethod(buffer);
}
