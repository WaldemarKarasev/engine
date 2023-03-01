#pragma once

#include <glm/vec2.hpp>
#include <memory>

#include "IGameObject.h"
#include "../../Renderer/SpriteAnimator.h"

namespace RendererEngine
{
	class Sprite;
}

class Tank : public IGameObject
{
public:
	enum class EOrientation 
	{
		Top, 
		Bottom,
		Left, 
		Right
	};

	Tank(std::shared_ptr<RendererEngine::Sprite> pSprite_top,
		std::shared_ptr<RendererEngine::Sprite> pSprite_left,
		std::shared_ptr<RendererEngine::Sprite> pSprite_right,
		std::shared_ptr<RendererEngine::Sprite> pSprite_bottom,
		const float velocity,
		const glm::vec2& position, 
		const glm::vec2& size);

	void render() const override;
	void setOrientation(const EOrientation eOrientation);
	void move(const bool move);
	void update(const uint64_t delta) override;

private:
	EOrientation m_eOrientation;
	std::shared_ptr<RendererEngine::Sprite> m_pSprite_top;
	std::shared_ptr<RendererEngine::Sprite> m_pSprite_left;
	std::shared_ptr<RendererEngine::Sprite> m_pSprite_right;
	std::shared_ptr<RendererEngine::Sprite> m_pSprite_bottom;
	RendererEngine::SpriteAnimator m_spriteAnimator_top;
	RendererEngine::SpriteAnimator m_spriteAnimator_left;
	RendererEngine::SpriteAnimator m_spriteAnimator_right;
	RendererEngine::SpriteAnimator m_spriteAnimator_bottom;


	bool m_move;
	float m_velocity;
	glm::vec2 m_moveOffset;
};