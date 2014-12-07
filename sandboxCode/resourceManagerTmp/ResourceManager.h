#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include "ResourceFontManager.h"
#include "ResourceImageManager.h"


class ResourceManager : public Singleton<ResourceManager>
{
	public:
		static ResourceManager& get() {
			return ResourceManager::getInstance();
		}
		
	private:


};
#endif
