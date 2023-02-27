#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
namespace RendererEngine
{
	class ShaderProgram;
	class Texture2D;
	class Sprite;
	class AnimatedSprite;
}

class ResourceManager {
public:
	static void setExecutablePath(const std::string& executablePath);
	static void unloadResources();

	~ResourceManager() = delete;
	ResourceManager() = delete;
	
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(const ResourceManager&&) = delete;
	

	// ShaderPeogram loading and getting
	static std::shared_ptr<RendererEngine::ShaderProgram> loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	static std::shared_ptr<RendererEngine::ShaderProgram> getShaderProgram(const std::string& shaderName);

	// Textures loading and getting
	static std::shared_ptr<RendererEngine::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
	static std::shared_ptr<RendererEngine::Texture2D> getTexture(const std::string& textureName);

	// Sprites loading and getting
	static std::shared_ptr<RendererEngine::Sprite> loadSprite(const std::string& spriteName,
														const std::string& textureName,
														const std::string& shaderName,
														const std::string& subTextureName = "default");

	static std::shared_ptr<RendererEngine::Sprite> getSprite(const std::string& spriteName);

	// AnimatedSprites loading and getting
	static std::shared_ptr<RendererEngine::AnimatedSprite> loadAnimatedSprite(const std::string& spriteName,
																			  const std::string& textureName,
																			  const std::string& shaderName,
																			  const std::string& subTextureName = "default");

	static std::shared_ptr<RendererEngine::AnimatedSprite> getAnimatedSprite(const std::string& spriteName);

	// Texture Atlas (SubTextures) manipulation
	static std::shared_ptr<RendererEngine::Texture2D> loadTextureAtlas(const std::string textureName,
																 const std::string texturePath,
											        			 const std::vector<std::string> subTextures,
															     const unsigned int subTextureWidth,
															     const unsigned int subTextureHeight);


	static bool loadJSONResources(const std::string& JSONPath);

	static const std::vector<std::vector<std::string>>& getLevels() { return m_levels; }

private:
	// Path to shaders files
	static std::string getFileString(const std::string& relativeFilePath);

	typedef std::map<const std::string, std::shared_ptr<RendererEngine::ShaderProgram>> ShaderProgramsMap;
	static ShaderProgramsMap m_shaderPrograms;

	typedef std::map<const std::string, std::shared_ptr<RendererEngine::Texture2D>> TexturesMap;
	static TexturesMap m_textures;

	typedef std::map<const std::string, std::shared_ptr<RendererEngine::Sprite>> SpritesMap;
	static SpritesMap m_sprites;

	typedef std::map<const std::string, std::shared_ptr<RendererEngine::AnimatedSprite>> AnimatedSpritesMap;
	static AnimatedSpritesMap m_animatedSprites;

	static std::vector<std::vector<std::string>> m_levels;
	
	// Path to executable file (engine.exe)
	static std::string m_path;
};