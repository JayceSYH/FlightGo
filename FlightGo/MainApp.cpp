#include "stdafx.h"
#include "AppManager.h"
#include "MainApp.h"
#include "LayoutView.h"
#include"CommonActors.h"
#include "RegionLoader.h"
#include "CommonViewObjects.h"
#include <iostream>
#include <sstream>

void MainApp::initApp()
{

	//set main timer
	this->mainTimer = TimerManager::createTimer();
	this->mainTimer.addEvent(TimerListener([this](string event)->void { redrawWindow(); }));
	this->mainTimer.addEvent(TimerListener([this](string event)->void { this->enemyGenerator(); }));
	mainTimer.startTimer(10);

	//set key watch
	KeyboardManager::watchAlphabetKeys();

	initSources();
	initHeroFlight();
	initEnemyFlight();
}

void MainApp::updateView(LayoutView & mainLayout)
{
	mainLayout.clearView();
	
	ViewObjectManager::addToLayout(mainLayout);
}

void MainApp::initHeroFlight()
{
	/*init hero flight*/
	SimpleRegion& hero_region = ResourceManager::getResource<SimpleRegion>("hero_region");
	Canvas& hero_canvas = ResourceManager::getResource<Canvas>("hero_flight");
	HeroFlight *hero = new HeroFlight(StaticCanvasView(hero_canvas), CSize(44, 50), getViewRect(), CPoint(0, getViewRect().bottom-50));
	hero->setRegionWithPosOffset(hero_region);
	hero->setObjectType("hero_flight");
	hero->addActor(new KeyboardControlActor(*hero));
	hero->setSpeed(3);
	ViewObjectManager::addViewObject(hero);

	/*init hero flight collision actor*/
	FlightCollisionActor* collisionActor = new FlightCollisionActor(*hero);
	collisionActor->addExcludeType("hero_bullet");
	hero->addActor(collisionActor);

	/*init hero missle*/
	SimpleRegion& region = ResourceManager::getResource<SimpleRegion>("hero_bullet_region");
	Canvas& missle_canvas = ResourceManager::getResource<Canvas>("simple_missle");
	BulletObject* simple_missle = new BulletObject(StaticCanvasView(missle_canvas), CSize(10, 15), getViewRect());
	simple_missle->setObjectType("hero_bullet");
	simple_missle->setRegionWithPosOffset(region);
	simple_missle->addExcludeType("hero_flight");
	simple_missle->addExcludeType("enemy_bullet");
	
	/*init bullet actor with simple missle*/
	BulletShooterActor* bulletActor = new BulletShooterActor(*hero, simple_missle);
	bulletActor->bindKey('J');
	bulletActor->setBulletSpeed(0, -4);
	hero->addActor(bulletActor);
}

void MainApp::initEnemyFlight()
{
	/*init enemy flight*/
	Canvas& canvas = ResourceManager::getResource<Canvas>("enemy_flight");
	EnemyFlight* enemy = new EnemyFlight(StaticCanvasView(canvas), CSize(97, 100), getViewRect());

	enemy->setInitFunction([this](EnemyFlight& enemy) {

		/*set Region*/
		SimpleRegion& enemy_region = ResourceManager::getResource<SimpleRegion>("enemy_region");
		enemy.setRegionWithPosOffset(enemy_region);
		enemy.setObjectType("enemy_flight");

		/*collision actor*/
		FlightCollisionActor *collisionActor = new FlightCollisionActor(enemy, false);
		collisionActor->addCollisionType("hero_bullet");
		collisionActor->addExcludeType("enemy_bullet");
		enemy.addActor(collisionActor);

		/*automovement actor*/
		AutoMovementActor* automove = new AutoMovementActor(enemy, 0, 2);
		enemy.addActor(automove);

		/*bullet object*/
		SimpleRegion& bullet_region = ResourceManager::getResource<SimpleRegion>("enemy_bullet_region");
		Canvas& missile_canvas = ResourceManager::getResource<Canvas>("simple_missile");
		BulletObject* bullet = new BulletObject(StaticCanvasView(missile_canvas), CSize(20, 30), getViewRect());
		bullet->setObjectType("enemy_bullet");
		bullet->setRegionWithPosOffset(bullet_region);
		bullet->addExcludeType("enemy_flight");
		bullet->addTargetType("hero_flight");

		/*bullet actor*/
		BulletShooterActor* bulletActor = new BulletShooterActor(enemy, bullet, true);
		bulletActor->setBulletSpeed(0, 4);
		bulletActor->setCooldownTime(40);
		enemy.addActor(bulletActor);
	});

	this->enemyModel = enemy;
}

void MainApp::LoadImageAsCanvas(const WCHAR* filename, string id) {
	Image* image = new Image(filename);
	Canvas* canvas = createCanvas(*image);
	ResourceManager::addResource(canvas, id);
	delete image;
	image = nullptr;
}

void MainApp::LoadImageSource(const WCHAR * filename, string id)
{
	Image* image = new Image(filename);
	ResourceManager::addResource(image, id);
}

void MainApp::LoadRegionSource(string filename, string regionName, string id)
{
	SimpleRegion* region = RegionLoader::loadRegion(filename, regionName);
	ResourceManager::addResource(region, id);
}

void MainApp::enemyGenerator()
{
	static CSize size = enemyModel->getSize();
	static int cooldownTime = enemyModel->getSize().cy / 2;
	static int leftCooldownTime = 100;
	if (--leftCooldownTime <= 0) {
		int xpos = ((double)rand()) / RAND_MAX * (getViewRect().Width() - size.cx);
		EnemyFlight* enemy = enemyModel->divide();
		enemy->setPos(CPoint(xpos, -size.cy + 1));
		ViewObjectManager::addViewObject(enemy);
		leftCooldownTime = cooldownTime;
	}
}

void MainApp::initSources()
{
	/*hero flight*/
	LoadImageAsCanvas(_T("res//flight3.png"), "hero_flight");
	/*missle*/
	LoadImageAsCanvas(_T("res//bullet.png"), "simple_missle");
	/*enemy flight*/
	LoadImageAsCanvas(_T("res//enemy_flight.png"), "enemy_flight");
	/*enemy missile*/
	LoadImageAsCanvas(_T("res//simple_missile.png"), "simple_missile");

	/*hero region*/
	LoadRegionSource("res//config.ini", "hero", "hero_region");
	/*load hero bullet region*/
	LoadRegionSource("res//config.ini", "hero_bullet", "hero_bullet_region");
	/*load enemy region*/
	LoadRegionSource("res//config.ini", "enemy", "enemy_region");
	/*load enemy bullet region*/
	LoadRegionSource("res//config.ini", "enemy_bullet", "enemy_bullet_region");
}
