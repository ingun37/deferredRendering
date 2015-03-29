#include "JSingleton.h"

template <typename T>
T* JSingleton<T>::Inst()
{
	static T* instance = NULL;

	if( instance == NULL )
		instance = new T();
	return instance;
}