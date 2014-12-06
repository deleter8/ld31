#ifndef _IRESOURCE_H
#define _IRESOURCE_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "Singleton.h"

template<class MANAGER, typename KEY, class RESOURCE >
class IResourceManager : public Singleton<MANAGER>
{
	//TODO: these should be bound to the resource instance	
	enum class ResourceManagerState : short
	{
		UNKNOWN = 0,
		NOTFOUND,
		LOADING,
		UNLOADING,
		LOADED,
	};

	public:
		IResourceManager() :_currentState(ResourceManagerState::UNKNOWN) {};
		//TODO: look into deallocators 
		virtual ~IResourceManager(){freeAll();}
	protected:
		virtual RESOURCE * load(const char *filename, KEY key) = 0;
	public:
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  IResourceManager
		 *      Method:  IResourceManager :: add
		 * Description:  
		 *--------------------------------------------------------------------------------------
		 */
		bool add(const char *filename, KEY key)
		{
			RESOURCE * resource = get(key);
			if(resource != nullptr) {return false;}
			_currentState = ResourceManagerState::LOADING;
			resource = load(filename, key);
			if (resource != nullptr) {
				_resourceMap[key] = resource;
				_currentState = ResourceManagerState::LOADED;
			}
			return true;
		}
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  IResourceManager
		 *      Method:  IResourceManager :: get 
		 * Description:  
		 *--------------------------------------------------------------------------------------
		 */
		RESOURCE * get(const KEY key)
		{
			RESOURCE *resource = NULL;
			auto it = _resourceMap.find(key);
			if (it != _resourceMap.end()) {resource = it->second;}
			return resource;
		}
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  IResourceManager
		 *      Method:  IResourceManager :: remove 
		 * Description:  
		 *--------------------------------------------------------------------------------------
		 */
		void remove(const KEY key)
		{
			RESOURCE *resource = get(key);
			if (resource)
			{
				delete resource;
				_resourceMap.erase(_resourceMap.find(key));
			}
		}
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  IResourceManager
		 *      Method:  IResourceManager :: freeAllItems 
		 * Description:  
		 *--------------------------------------------------------------------------------------
		 */
		void freeAll()
		{
			std::for_each(_resourceMap.begin(), _resourceMap.end(), [](std::pair<const KEY,RESOURCE *> &element){delete std::get<1>(element);});
			_resourceMap.clear();
		}
	private:
		//TODO: I can probably use shared_ptr<RESOURCE>
		std::unordered_map<KEY, RESOURCE *> _resourceMap;
		ResourceManagerState _currentState;
};

#endif
