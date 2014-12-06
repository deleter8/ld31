#ifndef _RESOURCEIMAGEMANAGER_H
#define _RESOURCEIMAGEMANAGER_H
#include <string>
#include "IResourceManager.h"
#include "model/Image.h"

class ResourceImageManager : public IResourceManager < ResourceImageManager, std::string, Image >
{
	friend class Singleton<ResourceImageManager>;	

	public:
	ResourceImageManager(){}
	~ResourceImageManager(){}

	protected:
	Image * load(const char *filename, std::string keyName) override
	{
		std::cout << filename << " " << keyName << std::endl;	
		Image *img = new Image(32,32);
		return img;
	}

};

#endif
