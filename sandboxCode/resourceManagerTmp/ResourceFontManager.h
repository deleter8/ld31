#ifndef _RESOURCEFONTMANAGER_H
#define _RESOURCEFONTMANAGER_H
#include "IResourceManager.h"
#include "model/Font.h"

class ResourceFontManager : public IResourceManager <ResourceFontManager, int, Font *> 
{
	public:
	ResourceFontManager(){}
	virtual ~ResourceFontManager(){}

	protected:
	Font* load(const char *filename, int keyName) override
	{
		std::cout << filename << " " << keyName << std::endl;	
		Font *font= new Font(filename);
		return font;
	}
	void clean(Font** font) override 
	{
		std::cout << "deleting font ptr" << std::endl; 
		delete *font;
		*font = nullptr;
	}
};

#endif
