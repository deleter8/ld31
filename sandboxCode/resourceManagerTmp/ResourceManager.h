#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H
#include "Singleton.h"

#include "ResourceFontManager.h"
#include "ResourceImageManager.h"
#include "ResourceAudioManager.h"

class ResourceManager : public Singleton<ResourceManager>
{
	friend class Singleton<ResourceManager>;
	public:
		ResourceManager() 
			: _fontResources(new ResourceFontManager), 
			  _imageResources(new ResourceImageManager),
			  _audioResources(new ResourceAudioManager)
		{}
		ResourceManager(const ResourceManager& ) = delete;
		ResourceManager& operator=(const ResourceManager& ) = delete;
		ResourceManager(ResourceManager&& ) = delete;

	public:
		static ResourceManager& get() {return ResourceManager::getInstance();}
		ResourceFontManager& font(){return *_fontResources;}
		ResourceImageManager& image(){return *_imageResources;}
		ResourceAudioManager& audio(){return *_audioResources;}

	private:
		std::shared_ptr<ResourceFontManager> _fontResources;
		std::shared_ptr<ResourceImageManager> _imageResources;
		std::shared_ptr<ResourceAudioManager> _audioResources;
};
#endif
