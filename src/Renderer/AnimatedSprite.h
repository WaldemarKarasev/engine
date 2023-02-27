#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "Sprite.h"

namespace RendererEngine {

	
	class AnimatedSprite : public Sprite
	{
	public:
		AnimatedSprite(const std::shared_ptr<Texture2D> pTexture,
			const std::string initialSubTexture,
			const std::shared_ptr<ShaderProgram> pShaderProgram);

		void insertState(std::string stateName, std::vector<std::pair<std::string, uint64_t>> subTexturesDuration);


		void setState(const std::string& newState);

		void update(uint64_t delta);

		void render(const glm::vec2& position, const glm::vec2& size, const float rotation) const override;
		
	private:
		std::map <std::string, std::vector<std::pair<std::string, uint64_t>>> m_statesMap;
		size_t m_currentFrame = 0;
		uint64_t m_currentAnimationTime = 0;
		std::map <std::string, std::vector<std::pair<std::string, uint64_t>>>::const_iterator m_pCurrentAnimationDuration;
		mutable bool m_dirty = false;
	};

}