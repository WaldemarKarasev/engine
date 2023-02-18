#include "AnimatedSprite.h"
#include "Texture2D.h"

namespace Renderer {
	AnimatedSprite::AnimatedSprite(const std::shared_ptr<Texture2D> pTexture,
		const std::string initialSubTexture,
		const std::shared_ptr<ShaderProgram> pShaderProgram,
		const glm::vec2& position,
		const glm::vec2& size,
		const float rotation)
		: Sprite(std::move(pTexture), std::move(initialSubTexture), std::move(pShaderProgram), position, size, rotation)
	{
		m_pCurrentAnimationDuration = m_statesMap.end();
	}

	void AnimatedSprite::insertState(std::string stateName, std::vector<std::pair<std::string, uint64_t>> subTexturesDuration)
	{
		m_statesMap.emplace(std::move(stateName), std::move(subTexturesDuration));
	}

	void AnimatedSprite::setState(const std::string& newState)
	{
		auto it = m_statesMap.find(newState);
		if (it == m_statesMap.end())
		{
			std::cout << "Can't find animation state: " << newState << std::endl;
			return;
		}

		if (it != m_pCurrentAnimationDuration)
		{
			m_currentFrame = 0;
			m_currentAnimationTime = 0;
			m_pCurrentAnimationDuration = it;
			m_dirty = true;
		}

	}

	void AnimatedSprite::update(uint64_t delta)
	{
		if (m_pCurrentAnimationDuration != m_statesMap.end())
		{
			m_currentAnimationTime += delta;

			while (m_currentAnimationTime >= m_pCurrentAnimationDuration->second[m_currentFrame].second)
			{
				m_currentAnimationTime -= m_pCurrentAnimationDuration->second[m_currentFrame].second;
				++m_currentFrame;
				m_dirty = true;

				if (m_currentFrame == m_pCurrentAnimationDuration->second.size())
				{
					m_currentFrame = 0;
				}
			}
			
		}
	}



	void AnimatedSprite::render() const
	{
		if (m_dirty)
		{
			auto subTexture = m_pTexture->getSubTexture(m_pCurrentAnimationDuration->second[m_currentFrame].first);

			const GLfloat textureCoords[] = {
				//.U....V coords

				//1st triangle
				subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
				subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
				subTexture.rightTopUV.x,   subTexture.rightTopUV.y,

				// 2nd triangle
				subTexture.rightTopUV.x,   subTexture.rightTopUV.y,
				subTexture.rightTopUV.x,   subTexture.leftBottomUV.y,
				subTexture.leftBottomUV.x, subTexture.leftBottomUV.y
			};

			glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordsVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textureCoords), &textureCoords);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			m_dirty = false;
		}
		Sprite::render();
	}

}