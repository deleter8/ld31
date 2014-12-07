#ifndef _RESOURCEIMAGEMANAGER_H
#define _RESOURCEIMAGEMANAGER_H
#include <string>
#include "IResourceManager.h"
#include "model/Image.h"

class ResourceImageManager : public IResourceManager < ResourceImageManager, std::string, std::shared_ptr<Image> >
{
	friend class Singleton<ResourceImageManager>;	

	public:
	ResourceImageManager(){}
	virtual ~ResourceImageManager(){}

	protected:

	std::shared_ptr<Image> load (const char *filename, std::string keyName) override 
	{
		//std::shared_ptr<Image> ptr(new Image(32,32) , &ResourceImageManager::clean);
		std::shared_ptr<Image> ptr(new Image(32,32));
		return ptr;
	}
	void clean (std::shared_ptr<Image> *ptr) override 
	{
		std::cout << "deleting image ptr" << std::endl; 
		ptr->get()->print();
	}

};

#endif
