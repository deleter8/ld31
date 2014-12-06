#ifndef _RESOURCEFONTMANAGER_H
#define _RESOURCEFONTMANAGER_H
#include "IResourceManager.h"
#include "model/Font.h"
#include <chrono>
#include <thread>

class ResourceFontManager : public IResourceManager <ResourceFontManager, int, Font >
{
	friend class Singleton<ResourceFontManager>;	

	public:
	ResourceFontManager(){}
	~ResourceFontManager(){}

	protected:
	Font * load(const char *filename, int keyName) override
	{
		std::cout << filename << " " << keyName << std::endl;	
		std::chrono::seconds dura( 10 );
		std::this_thread::sleep_for( dura );
		Font *font= new Font(filename);
		return font;
	}
};

#endif
