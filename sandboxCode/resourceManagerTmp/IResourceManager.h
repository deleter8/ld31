#ifndef _IRESOURCE_H
#define _IRESOURCE_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <type_traits>
//#include "Singleton.h"

template<class MANAGER, typename KEY, class RESOURCE >
class IResourceManager //: public Singleton<MANAGER> 
{
	private:
		enum class ResourceState : short 
		{
			UNKNOWN = 0,
			NOTFOUND,
			LOADING,
			UNLOADING,
			LOADED,
			FAILED
		};
		struct ResourceItem 
		{
			ResourceItem(RESOURCE r, ResourceState cs) : resource(r), currentState(cs) {}
			RESOURCE resource;
			ResourceState currentState;
		};

	public:
		IResourceManager() {
			//TODO: only pointers and shared_ptrs should be allowed as RESOURCE type
			//static_assert(std::is_pointer<RESOURCE>::value, "Can only be used with pointer types"); 
			}
			
		virtual ~IResourceManager(){}
	protected:
		virtual RESOURCE load(const char *filename, KEY key) = 0;
		virtual void clean(RESOURCE* resource) = 0;
	public:
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  IResourceManager
		 *      Method:  IResourceManager :: add
		 * Description:  
		 *--------------------------------------------------------------------------------------
		 */
		void add(const char *filename, KEY key)
		{
			RESOURCE resource = get(key);
			//check if the resource is already loaded
			//TODO: Throw exception?
			if(resource != nullptr) {return;}
			//
			// We want to set the map up with a null value at first until we know the resource has loaded correctly.
			//
			std::unique_ptr<ResourceItem> item(new ResourceItem(nullptr, ResourceState::LOADING));
			_resourceMap[key] = std::move(item); // do not access 'item' object anymore

			// TODO: must implement asyn or object pooling
			resource = load(filename, key);
			if (resource) 
			{
				_resourceMap[key]->resource = resource;
				_resourceMap[key]->currentState = ResourceState::LOADED;
			}
			else 
			{
				_resourceMap[key]->currentState = ResourceState::FAILED;
			}
		}
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  IResourceManager
		 *      Method:  IResourceManager :: get 
		 * Description:  
		 *--------------------------------------------------------------------------------------
		 */
		RESOURCE get(const KEY key)
		{
			RESOURCE resource = NULL;
			auto it = _resourceMap.find(key);
			if (it != _resourceMap.end()) {resource = it->second->resource;}
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
			RESOURCE  resource = get(key);
			if (resource)
			{
				clean(&resource);
				_resourceMap.erase(_resourceMap.find(key));
			}
		}
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  IResourceManager
		 *      Method:  IResourceManager ::cleanup 
		 * Description:  
		 *--------------------------------------------------------------------------------------
		 */
		void cleanup()
		{
			std::for_each(_resourceMap.begin(), _resourceMap.end(), [this](std::pair<const KEY,std::unique_ptr<ResourceItem>> &element){clean(&(std::get<1>(element).get())->resource);});
			_resourceMap.clear();
		}
		/*
		 *--------------------------------------------------------------------------------------
		 *       Class:  IResourceManager
		 *      Method:  IResourceManager ::isLoaded
		 * Description:  
		 *--------------------------------------------------------------------------------------
		 */
		bool isLoaded(KEY key)
		{
			if(_resourceMap[key]->currentState == ResourceState::LOADED) {return true;}
			return false;
		}

	private:
		std::unordered_map< KEY, std::unique_ptr<ResourceItem> > _resourceMap;
};

#endif
