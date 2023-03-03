#pragma once

#include "VertexArray.h";
#include "IndexBuffer.h"
#include "ShaderProgram.h"

#include <string>

namespace RenderEngine
{
	class VertexArray;
	class IndexBuffer;
	class ShaderProgram;

	class Renderer
	{
	public:
		static void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shaderProgram);
		static void setClearColor(float r, float g, float b, float alpha);
		static void clear();
		static void setViewport(unsigned  int width, unsigned int height, unsigned int leftOffset = 0, unsigned int bottomOffset = 0);
		
		static std::string getRendererStr();
		static std::string getVersionStr();
	};
}