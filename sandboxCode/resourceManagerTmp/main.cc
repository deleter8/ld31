#include <cstdlib>
#include <iostream>
#include "ResourceManager.h"
 
int main (int argv, char *argc[])
{
	ResourceManager::get().image().add("/home/bj/somefile.img", "img1");
	ResourceManager::get().image().add("/home/bj/somefile.img", "img2");
	ResourceManager::get().image().add("/home/bj/somefile.img", "img1");
	ResourceManager::get().font().add("/home/bj/font1", 1);
	ResourceManager::get().font().add("/home/bj/font2", 2);
	ResourceManager::get().audio().add("/home/bj/audio", 1);

	if(ResourceManager::get().image().isLoaded("img1") ){
		auto img1 = ResourceManager::get().image().get("img1");
		if(img1){
			img1.get()->print();
		}
		else {std::cout << "NOT FOUND imq1 " << std::endl;}
	}
	
	/*  

//	ResourceAudioManager::getInstance().add("/home/bj/audio", 1);

	auto img1 = ResourceImageManager::getInstance().get("img1");
	if(img1){
		img1.get()->print();
	}
	else {std::cout << "NOT FOUND imq1 " << std::endl;}

	ResourceImageManager::getInstance().remove("img2");
	auto img2 = ResourceImageManager::getInstance().get("img2");
	if(!img2) 
	{
		std::cout << "img2 dne" << std::endl;
	}

	ResourceFontManager::getInstance().get(1)->print();
	ResourceFontManager::getInstance().cleanup();	
	*/

	return EXIT_SUCCESS;
}
