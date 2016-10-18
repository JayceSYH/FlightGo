#pragma once
#include "LayoutView.h"
#include "ResourceManager.h"
#include "TimerManager.h"
#include "KeyboardManager.h"
#include"ViewObjectManager.h"
#include <list>
#include<map>
#include<string>
class CFlightGoView;

class App
{
private:
public:
	virtual void initApp() = 0;
	virtual void updateView(LayoutView& mainLayout) = 0;
	virtual ~App() {};
	void redrawWindow();
	CRect getViewRect();
	template<class... Params>
	void addListener(string event, Listener<Params...> listener);
	template <class... Params>
	void addListener(string event, void(*func)(string, Params...));
	Canvas* createCanvas(int width, int height);
	Canvas* createCanvas(Image& image);
	Canvas* createCanvas(CImage& image);
};

class AppManager
{
private:
	static AppManager instance;
private:
	CFlightGoView* viewInstance;
	CDC* getDC();
	App* app;
	bool drawerInit;
public:
	AppManager() :drawerInit(false) {};
	~AppManager();
	static AppManager& getInstance();
	void registerView(CFlightGoView* view);
	void registerApp(App* app);
	void initApp();
	LayoutView& getLayoutView();
	CSize getViewSize();
	void updateView();
	Canvas* createCanvas(int width, int height);
	Canvas* createCanvas(CSize& size);
	Canvas* createCanvas(CImage& image);
	Canvas* createCanvas(Image& image);
	void redrawWindow();
	bool isDrawerInited();
};

template<class ...Params>
inline void App::addListener(string event, Listener<Params...> listener)
{
	if (event == FG_TIMER) {
		assert("Please use TimerManager to register timer events" && 0);
	}
	AppManager::getInstance().getLayoutView().addListener(listener, event);
}

template<class ...Params>
inline void App::addListener(string event, void(*func)(string, Params...))
{
	addListener(event, Listener<Params...>(func));
}

