#pragma once
#include "AppManager.h"
#include "FlightObject.h"

class MainApp : virtual public App {
private:
	Timer mainTimer;
	EnemyFlight* enemyModel;
public:
	virtual void initApp();
	virtual void updateView(LayoutView& mainLayout);
	virtual ~MainApp() {};
	void initHeroFlight();
	void initEnemyFlight();
	void initSources();
	void LoadImageAsCanvas(const WCHAR* filename, string id);
	void LoadImageSource(const WCHAR* filename, string id);
	void LoadRegionSource(string filename, string regionName, string id);
	void enemyGenerator();
};