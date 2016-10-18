#include "stdafx.h"
#include "AppManager.h"
#include "LayoutView.h"
#include "FlightGoView.h"
#include "ResourceManager.h"
#include "ImageUtils.h"


AppManager AppManager::instance = AppManager();

CDC * AppManager::getDC()
{
	return viewInstance->GetDC();
}

AppManager::~AppManager()
{
	delete this->app;
}

AppManager & AppManager::getInstance()
{
	return AppManager::instance;
}

void AppManager::registerView(CFlightGoView * view)
{
	this->viewInstance = view;
}

void AppManager::registerApp(App * app)
{
	this->app = app;
	initApp();
}

void AppManager::initApp()
{
	this->app->initApp();
	this->drawerInit = true;
}

LayoutView & AppManager::getLayoutView()
{
	return this->viewInstance->getMainLayout();
}

CSize AppManager::getViewSize()
{
	return viewInstance->getSize();
}

void AppManager::updateView()
{
	this->app->updateView(this->getLayoutView());
}

Canvas * AppManager::createCanvas(int width, int height)
{
	return new Canvas(height, width, getDC());
}

Canvas * AppManager::createCanvas(CSize & size)
{
	return createCanvas(size.cx, size.cy);
}

Canvas * AppManager::createCanvas(CImage& image)
{
	return new Canvas(image, getDC());
}

Canvas * AppManager::createCanvas(Image & image)
{
	return new Canvas(image, getDC());
}

void AppManager::redrawWindow()
{
	this->viewInstance->Invalidate();
	this->viewInstance->UpdateWindow();
}

bool AppManager::isDrawerInited()
{
	return drawerInit;
}

void App::redrawWindow()
{
	AppManager::getInstance().redrawWindow();
}

CRect App::getViewRect()
{ 
	CSize size = AppManager::getInstance().getLayoutView().getSize();
	return CRect(0, 0, size.cx, size.cy);
}

Canvas * App::createCanvas(int width, int height)
{
	return AppManager::getInstance().createCanvas(width, height);
}

Canvas * App::createCanvas(Image & image)
{
	return AppManager::getInstance().createCanvas(image);
}

Canvas * App::createCanvas(CImage & image)
{
	return AppManager::getInstance().createCanvas(image);
}
