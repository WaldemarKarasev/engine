#pragma once

#include <glad/glad.h>
#include <vector>

namespace RenderEngine
{
	struct VertrexBufferLayoutElement
	{
		GLuint count;
		GLenum type;
		GLboolean normalized;
		unsigned int size;
	};

	class VertexBufferLayout	{
	public:
		VertexBufferLayout();

		void reserveElements(const size_t count);
		unsigned int getStride() const { return m_stride; }
		void addElementLayoutFloat(const unsigned int count, const bool normalized);
		const std::vector<VertrexBufferLayoutElement>& getLayoutElements() const { return m_layoutElements; }

	private:
		std::vector<VertrexBufferLayoutElement> m_layoutElements;
		unsigned int m_stride;

	};
}