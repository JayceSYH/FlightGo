#include "stdafx.h"
#include "Drawable.h"
#include "Canvas.h"


bool ListenableDrawer::operator==(ListenableDrawer& listenableDrawer)
{
	return this == &listenableDrawer;
}

bool ListenableDrawer::operator!=(ListenableDrawer& listenableDrawer)
{
	return this != &listenableDrawer;
}
