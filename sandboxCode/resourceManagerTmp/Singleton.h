#ifndef _H0_SINGLETON_H
#define _H0_SINGLETON_H

#include <memory>
#include <thread>
#include <mutex>
/* 
 * =====================================================================================
 *        Class:  Singleton
 *  Description:  
 * =====================================================================================
 */
template < class T >
class Singleton 
{
	private:
		static std::unique_ptr<T> _instance;
		static std::once_flag _flag; 
	
	protected:
		Singleton () = default;
		Singleton ( const Singleton &other )=delete;
		Singleton ( Singleton &&other )=delete;
		Singleton& operator = ( const Singleton &other ) = delete;
		virtual ~Singleton (){};

	public:
		static T &getInstance()
		{
			std::call_once(_flag, [](){ _instance.reset(new T); });
			return *_instance.get();
		}
}; 
template <class T>
std::unique_ptr<T> Singleton<T>::_instance;
template<class T>
std::once_flag Singleton<T>::_flag;


#endif
