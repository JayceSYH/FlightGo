#pragma once
#include<list>
#include<map>
#include<string>
#include<functional>
#include<algorithm>
#include "TemplateTools.h"
using namespace std;

enum ListenerState { INVALID, VALID };
template <class... Params>
class Listener
{
protected:
	typedef void(*F)(string event, Params...);
	typedef std::function<void(string, Params...)> FLambda;
	FLambda lambda;
	int generateId() { static int idGenerator = 0; return idGenerator++; }
	int id;
	ListenerState state;
public:
	Listener(FLambda lambda) {
		this->id = generateId();
		this->lambda = lambda;
		this->state = ListenerState::VALID;
	}

	Listener() {
		this->id = generateId();
		this->state = ListenerState::VALID;
	}

	Listener& operator=(const FLambda& lambda) {
		this->lambda = lambda;
		return *this;
	}

	operator FLambda() {
		return this->lambda;;
	}

	FLambda& operator *() {
		return this->lambda;
	}

	void operator()() {
		this->lambda();
	}

	bool operator==(const Listener<Params...>& listener) {
		return listener.id == this->id;
	}

	bool operator!=(Listener<Params...>& listener) {
		return listener.id != this->id;
	}

	void notify(string event,Params... params) {
		this->lambda(event, unpack(params)...);
	}

	int getListenerId() {
		return this->id;
	};
	
	void setValid() {
		this->state = ListenerState::VALID;
	}

	void setInvalid() {
		this->state = ListenerState::INVALID;
	}

	bool isValid() {
		return this->state == ListenerState::VALID;
	}
};


template <class... Params>
class Listenable {
private:
	typedef  Listener<Params...> T__Listener;
	map<string, list<T__Listener>> eventListeners;
public:
	virtual Listenable* clone() { return new Listenable<Params...>(*this); };
	 virtual void addListener(T__Listener listener, string event);
	 virtual void removeListener(T__Listener listener, string event);
	 virtual void removeListener(int listenerId, string event);
	 virtual void removeAll();
	 virtual void clearInvalid(string event);
	 virtual void notifyListeners(string event, Params... params);
	 virtual bool operator==(Listenable<Params...>& listenable);
	 virtual bool operator!=(Listenable<Params...>& listenable);
};

template<class... Params>
void Listenable<Params...>::addListener(T__Listener listener, string event)
{
	listener.setValid();
	if (eventListeners.find(event) == eventListeners.end()) {
		auto *listeners = new list<T__Listener>();
		listeners->push_back(listener);
		eventListeners[event] = *listeners;
	}
	else {
		auto& listeners = eventListeners[event];
		int thisId = listener.getListenerId();
		auto iter = find_if(listeners.begin(), listeners.end(), [thisId](T__Listener lstn)->bool { return lstn.getListenerId() == thisId; });
		if (iter != listeners.end()) {
			*iter = listener;
		}
		else {
			listeners.push_back(listener);
			eventListeners[event] = listeners;
		}
	}
}

template<class ...Params>
inline void Listenable<Params...>::removeListener(T__Listener listener, string event)
{
	removeListener(listener.getListenerId(), event);
}

template<class ...Params>
inline void Listenable<Params...>::removeListener(int listenerId, string event)
{
	auto iter = this->eventListeners.find(event);
	if (iter != this->eventListeners.end()) {
		auto listiter = iter->second.begin();
		while (listiter != iter->second.end()) {
			if (listiter->getListenerId() == listenerId) {
				listiter->setInvalid();
				break;
			}
		}
	}
}

template<class ...Params>
inline void Listenable<Params...>::removeAll()
{
	this->eventListeners.clear();
}

template<class ...Params>
inline void Listenable<Params...>::clearInvalid(string event)
{
	auto iter = eventListeners.find(event);
	if (iter != eventListeners.end()) {
		list<T__Listener>& ref = iter->second;
		auto l_iter = ref.begin();
		while ((l_iter = find_if(ref.begin(), ref.end(), [](T__Listener lstn)->bool { return !lstn.isValid(); })) != ref.end()) {
			ref.erase(l_iter);
			break;
		}
	}
}

template<class ...Params>
inline void Listenable<Params...>::notifyListeners(string event, Params ...params)
{
	clearInvalid(event);
	if (eventListeners.find(event) != eventListeners.end()) {
		for (auto listener : eventListeners[event]) {
			listener.notify(event, unpack(params)...);
		}
	}
}

template<class ...Params>
inline bool Listenable<Params...>::operator==(Listenable<Params...>& listenable)
{
	return this == &listenable;
}

template<class ...Params>
inline bool Listenable<Params...>::operator!=(Listenable<Params...>& listenable)
{
	return this != &listenable;
}


/*************Common Listenable***************/
typedef Listenable<CPoint> ClickListenable;
typedef Listenable<UINT> KeyboardListenable;


/**************Common Listeners****************/
typedef Listener<CPoint> ClickListener;
typedef Listener<UINT> KeyboardListener;