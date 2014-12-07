#include <cstdlib>
#include <iostream>
#include "ResourceImageManager.h"
#include "ResourceFontManager.h"
//#include "ResourceAudioManager.h"
 
int main (int argv, char *argc[])
{
	
	ResourceImageManager::getInstance().add("/home/bj/somefile.img", "img1");
	ResourceImageManager::getInstance().add("/home/bj/somefile.img", "img2");
	ResourceImageManager::getInstance().add("/home/bj/somefile.img", "img1");

	ResourceFontManager::getInstance().add("/home/bj/font1", 1);
	ResourceFontManager::getInstance().add("/home/bj/font2", 2);


//	ResourceAudioManager::getInstance().add("/home/bj/audio", 1);

	auto img1 = ResourceImageManager::getInstance().get("img1");
	if(img1){
		std::cout << "Found img1 " << std::endl;
	}
	else {std::cout << "NOT FOUND imq1 " << std::endl;}


	ResourceImageManager::getInstance().remove("img2");
	auto img2 = ResourceImageManager::getInstance().get("img2");
	if(!img2) 
	{
		std::cout << "img2 dne" << std::endl;
	}
	ResourceFontManager::getInstance().cleanup();	
	return EXIT_SUCCESS;
}
