#ifndef JSingleton_h
#define JSingleton_h

template <typename T>
class JSingleton
{
public:

	static T* Inst();
	JSingleton(){};
	~JSingleton(){};
};

#endif
