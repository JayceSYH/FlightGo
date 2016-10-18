#pragma once
#include<list>
#include<string>
#include<atlimage.h>
#include<algorithm>
using namespace std;

template <class C>
class CategoryResourceManager {
private:
	class ResWithId {
	public:
		string id;
		C* resource;
		ResWithId(C* res, string id):resource(res), id(id){}
	};
	static CategoryResourceManager<C>* instance;
private:
	list<ResWithId> resourceList;
	CategoryResourceManager(){}
public:
	static CategoryResourceManager<C>& getInstance();
	~CategoryResourceManager();
	void addResource(C* resource, string id);
	C& getResource(string id);
};

template <class C>
CategoryResourceManager<C>* CategoryResourceManager<C>::instance = nullptr;

template<class C>
inline CategoryResourceManager<C>& CategoryResourceManager<C>::getInstance()
{
	if (CategoryResourceManager<C>::instance == nullptr) {
		CategoryResourceManager<C>::instance = new CategoryResourceManager<C>();
	}

	return *CategoryResourceManager<C>::instance;
}

template<class C>
inline CategoryResourceManager<C>::~CategoryResourceManager()
{
	for (auto resWithId : resourceList) {
		delete resWithId.resource;
	}
}

template<class C>
inline void CategoryResourceManager<C>::addResource(C * resource, string id)
{
	this->resourceList.push_back(ResWithId(resource, id));
}

template<class C>
inline C& CategoryResourceManager<C>::getResource(string id)
{
	ResWithId& ref =  *find_if(this->resourceList.begin(), this->resourceList.end(), [&id](auto rwi)->bool {return rwi.id == id; });
	return *ref.resource;
}



class ResourceManager
{
public:
	template<class C>
	static void addResource(C* res, string id);
	template<class C>
	static C& getResource(string id);
};

template<class C>
inline void ResourceManager::addResource(C * res, string id)
{
	CategoryResourceManager<C>::getInstance().addResource(res, id);
}

template<class C>
inline C & ResourceManager::getResource(string id)
{
	return CategoryResourceManager<C>::getInstance().getResource(id);
}
