#pragma once

#include "IGameObject.h"

#include <memory>

namespace RendererEngine
{
	class Sprite;
}

class BrickWall : public IGameObject
{
public:

	BrickWall(const std::shared_ptr<RendererEngine::Sprite> pSprite, const glm::vec2& position, const glm::vec2& size, const float rotation);
	void render() const override;
	void update(const uint64_t delta) override;

private:

	std::shared_ptr<RendererEngine::Sprite> m_pCurrentSprite;

};