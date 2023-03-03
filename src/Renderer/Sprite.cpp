#include "Sprite.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine {
	Sprite::Sprite(std::shared_ptr<Texture2D> pTexture,
		std::string initialSubTexture,
		std::shared_ptr<ShaderProgram> pShaderProgram)
		: m_pTexture(std::move(pTexture))
		, m_pShaderProgram(std::move(pShaderProgram))
		, m_lastFrameId(0)
	{
		const GLfloat vertexCoords[] = {
			// 1---2
			// |./.|
			// 0---3
			//
			//.X....Y coords
			//
			//1st triangle
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
		};

		auto subTexture = m_pTexture->getSubTexture(std::move(initialSubTexture));

		const GLfloat textureCoords[] = {
			//.U....V coords
			
			subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
			subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
			subTexture.rightTopUV.x, subTexture.rightTopUV.y,
			subTexture.rightTopUV.x, subTexture.leftBottomUV.y,
		};

		const GLuint indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		// vertext (points)
		m_vertexCoordsBuffer.init(vertexCoords, 2 * 4 * sizeof(GLfloat));
		VertexBufferLayout vertexCoordsLayoutFloat;
		vertexCoordsLayoutFloat.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayoutFloat);


		// texture
		m_textureCoordsBuffer.init(textureCoords, 2 * 4 * sizeof(GLfloat));
		VertexBufferLayout textureCoordsLayout;
		textureCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

		// EBO
		m_indexBuffer.init(indices, 6);
		
		// clearing data in buffers
		m_vertexArray.unbind();
		m_indexBuffer.unbind();
	}

	Sprite::~Sprite()
	{
	}

	void Sprite::render(const glm::vec2& position, const glm::vec2& size, const float rotation, const size_t frameId) const
	{
		if (m_lastFrameId != frameId)
		{
			m_lastFrameId = frameId;
			const FrameDescription& curretnFrameDescription = m_framesDescriptions[frameId];

			const GLfloat textureCoords[] = {
				//.U....V coords

				//1st triangle
				curretnFrameDescription.leftBottomUV.x, curretnFrameDescription.leftBottomUV.y,
				curretnFrameDescription.leftBottomUV.x, curretnFrameDescription.rightTopUV.y,
				curretnFrameDescription.rightTopUV.x,   curretnFrameDescription.rightTopUV.y,
				curretnFrameDescription.rightTopUV.x,   curretnFrameDescription.leftBottomUV.y,
			};

			m_textureCoordsBuffer.update(textureCoords, 2 * 4 * sizeof(GLfloat));
		}

		m_pShaderProgram->use();

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(position, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
		model = glm::scale(model, glm::vec3(size, 1.0f));

		m_pShaderProgram->setMatrix4("modelMat", model);
		//glBindVertexArray(m_VAO);
		m_vertexArray.bind();

		// in slot GL_TEXTURE0 bind texture m_pTexture
		glActiveTexture(GL_TEXTURE0);
		m_pTexture->bind();

		Renderer::draw(m_vertexArray, m_indexBuffer, *m_pShaderProgram);
		
		// clearing vertex array only for debuging
		//glBindVertexArray(0);
		//m_vertexArray.unbind();
	}

	void Sprite::insertFrames(std::vector<FrameDescription> framesDescriptions)
	{
		m_framesDescriptions = std::move(framesDescriptions);
	}

	uint64_t Sprite::getFrameDuration(const size_t frameId) const
	{
		return m_framesDescriptions[frameId].duration;
	}

	size_t Sprite::getFramesCount() const
	{
		return m_framesDescriptions.size();
	}
}