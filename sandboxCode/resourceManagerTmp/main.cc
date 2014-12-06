#include <cstdlib>
#include <iostream>
#include <thread>
#include <mutex>
#include "ResourceImageManager.h"
#include "ResourceFontManager.h"

void addImage(std::string imagePath, std::string key)
{
	if( !ResourceImageManager::getInstance().add(imagePath.c_str(), key) )
	{
		std::cout<< "failed to insert " << key << std::endl;
	}
}
 
int main (int argv, char *argc[])
{
	addImage("/home/bj/somefile.img", "img1");
	addImage("/home/bj/somefile.img", "img2");
	addImage("/home/bj/somefile.img", "img1");

	//TODO: work on asyn loading
	ResourceFontManager::getInstance().add("/home/bj/font1", 1);
	ResourceFontManager::getInstance().add("/home/bj/font2", 2);

	auto *img1 = ResourceImageManager::getInstance().get("img1");

	if(!img1)
	{
		std::cout << "Unable to find img1" << std::endl;	
	}
	else
	{
		std::cout << "Found img1" << std::endl;
	}

	ResourceImageManager::getInstance().remove("img2");
	auto *img2 = ResourceImageManager::getInstance().get("img2");
	if(!img2) 
	{
		std::cout << "img2 dne" << std::endl;
	}
	
	return EXIT_SUCCESS;
}
