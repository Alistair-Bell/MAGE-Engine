#include "mageAPI.h"

/* Impliments buffer related stuff */
void *mageBufferAllocate()
{
	return malloc(sizeof(struct MAGE_BUFFER_STRUCT));
}
void mageBufferInitialise(mageBuffer *buffer, const float *data, const uint32 quantity, const uint32 componentCount)
{
	buffer->ComponentCount = componentCount;
	#ifdef MAGE_OPENGL
		GLCall(glGenBuffers(1, &buffer->ID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer->ID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, quantity * sizeof(float), data, GL_STATIC_DRAW));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));	
	#endif
}
void mageBufferBind(mageBuffer *buffer)
{
	#ifdef MAGE_OPENGL
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer->ID));
	#endif
}
void mageBufferUnBind(mageBuffer *buffer)
{
	#ifdef MAGE_OPENGL
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));	
	#endif
}
void mageBufferDestroy(mageBuffer *buffer)
{
	
}
void *mageIndexBufferAllocate()
{
	return malloc(sizeof(struct MAGE_INDEX_BUFFER_STRUCT));
}
void mageIndexBufferInitialise(mageIndexBuffer *buffer, const uint16 *data, const uint32 quantity)
{
	buffer->ComponentCount = quantity;
	#ifdef MAGE_OPENGL
		GLCall(glGenBuffers(1, &buffer->ID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (quantity * sizeof(short)), data, GL_STATIC_DRAW));
	#endif
}
void mageIndexBufferBind(mageIndexBuffer *buffer)
{
	#ifdef MAGE_OPENGL
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ID));
	#endif
}
void mageIndexBufferUnBind(mageIndexBuffer *buffer)
{
	#ifdef MAGE_OPENGL
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	#endif
}
void mageIndexBufferDestroy(mageIndexBuffer *buffer)
{
	mageFreeMethod(buffer);	
}
void *mageVertexArrayAllocate()
{
	return malloc(sizeof(struct MAGE_VERTEX_ARRAY_STRUCT));
}
void mageVertexArrayInitialise(mageVertexArray *vertexArray)
{
	#ifdef MAGE_OPENGL
		GLCall(glGenVertexArrays(1, &vertexArray->ID));
	#endif
	
	vertexArray->Buffers = mageResizableListAllocate();
	mageResizableListInitialise(vertexArray->Buffers, sizeof(struct MAGE_BUFFER_STRUCT));
}
void mageVertexArrayPush(mageVertexArray *vertexArray, mageBuffer *buffer, const uint32 index)
{
	#ifdef MAGE_OPENGL
		mageVertexArrayBind(vertexArray);
		mageBufferBind(buffer);	
	
		GLCall(glEnableVertexAttribArray(vertexArray->ID));	
	
		GLCall(glVertexAttribPointer(index, buffer->ComponentCount, GL_FLOAT, GL_FALSE, 0, 0));
	
		mageBufferUnBind(buffer);
	#endif	

	mageResizableListPush(vertexArray->Buffers, buffer);

}
void mageVertexArrayBind(mageVertexArray *vertexArray)
{
	#ifdef MAGE_OPENGL
		GLCall(glBindVertexArray(vertexArray->ID));
	#endif
}
void mageVertexArrayUnBind(mageVertexArray *vertexArray)
{
	#ifdef MAGE_OPENGL
		GLCall(glBindVertexArray(0));	
	#endif
}
void mageVertexArrayDestroy(mageVertexArray *vertexArray)
{
	
	mageResizableListDestroy(vertexArray->Buffers);
}





































