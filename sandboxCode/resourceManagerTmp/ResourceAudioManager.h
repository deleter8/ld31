#ifndef _RESOURCEAUDIOMANAGER_H
#define _RESOURCEAUDIOMANAGER_H
#include "IResourceManager.h"
#include "model/Audio.h"
#include <chrono>
#include <thread>


class ResourceAudioManager : public IResourceManager <ResourceAudioManager, int, Audio >
{
	friend class Singleton<ResourceAudioManager>;	

	public:
	ResourceAudioManager(){}
	virtual ~ResourceAudioManager(){}

	protected:

	Audio * load(const char *filename, int keyName) override
	{
		std::cout << filename << " " << keyName << std::endl;	
		std::chrono::seconds dura( 10 );
		std::this_thread::sleep_for( dura );
		Audio *audio= new Audio(filename);
		return audio;
	}

	void clean(Audio* audio) override 
	{
	}

};

#endif
