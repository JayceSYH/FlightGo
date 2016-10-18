#include "stdafx.h"
#include "TimerManager.h"
#include "Events.h"
#include "exceptions.h"

Timer::Timer(int timerId)
{
	this->timerId = new int;
	*this->timerId = timerId;
	this->state = new TimerState;
	*this->state = TimerState::TIMER_STOP;
	this->realState = RealState::KILLED;
	this->timeGap = new long;
	*this->timeGap = 1000;
	this->clocker = new clock_t;
	*this->clocker = clock();
	this->timerListeners = new TimerListenable();
}

void Timer::startTimer(long timeGap)
{
	*this->state = TimerState::TIMER_START;
	*this->timeGap = timeGap;
}

void Timer::stopTimer()
{
	*this->state = TimerState::TIMER_STOP;
}

void Timer::destroyTimer()
{
	*this->state = TimerState::TIMER_DESTROYED;
}

void Timer::addEvent(TimerListener  listener)
{
	this->timerListeners->addListener(listener, FG_TIMER);
}

void Timer::addEvent(TimerFunc func)
{
	addEvent(TimerListener(func));
}

void Timer::addDisposableEvent(TimerFunc func)
{
	TimerListener listener;
	int id = listener.getListenerId();
	auto lambda = [this, func, id](string event)->void { func(event); removeEvent(id); };
	listener = lambda;
	this->timerListeners->addListener(listener, FG_TIMER);
}

void Timer::removeEvent(TimerListener  listener)
{
	this->timerListeners->removeListener(listener, FG_TIMER);
}

void Timer::removeEvent(int listenerId)
{
	this->timerListeners->removeListener(listenerId, FG_TIMER);
}

void Timer::notifyListeners()
{
	clock_t now = clock();
	if (now - *clocker >= *timeGap) {
		this->timerListeners->notifyListeners(FG_TIMER);
		*this->clocker = now;
	}
}

int Timer::getTimeGap()
{
	return *this->timeGap;
}


TimerManager* TimerManager::instance = nullptr;

void TimerManager::destroyTimer(int timerId)
{
	auto iter = getInstance().timers.find(timerId);
	if (iter != getInstance().timers.end()) {
		Timer timer = iter->second;
		delete timer.state;
		delete timer.timeGap;
		delete timer.timerListeners;
		getInstance().timers.erase(iter);
	}
}

void TimerManager::destroyTimers()
{
	auto iter = getInstance().timers.begin();
	list<int> tobeDstroyed;
	//filter
	while (iter != getInstance().timers.end()) {
		Timer& timer = iter->second;
		if (*timer.state == Timer::TimerState::TIMER_DESTROYED) {
			tobeDstroyed.push_back(*timer.timerId);
		}
		++iter;
	}

	//destroy
	auto iter2 = tobeDstroyed.begin();
	while (iter2 != tobeDstroyed.end()) {
		destroyTimer(*iter2);
		++iter2;
	}
}

void TimerManager::killTimer(CWnd* wnd) {
	auto iter = getInstance().timers.begin();
	while (iter != getInstance().timers.end()) {
		Timer& timer = iter->second;
		if (timer.realState == Timer::RealState::STARTED && 
			(*timer.state == Timer::TimerState::TIMER_STOP || *timer.state == Timer::TimerState::TIMER_DESTROYED)) {
			wnd->KillTimer(*timer.timerId);
			timer.realState = Timer::RealState::KILLED;
		}
		++iter;
	}
}

void TimerManager::startTimer(CWnd* wnd) {
	auto iter = getInstance().timers.begin();
	while (iter != getInstance().timers.end()) {
		Timer& timer = iter->second;
		if (timer.realState == Timer::RealState::KILLED && *timer.state == Timer::TimerState::TIMER_START) {
			wnd->SetTimer(*timer.timerId, *timer.timeGap, nullptr);
			timer.realState = Timer::RealState::STARTED;
		}
		++iter;
	}
}

void TimerManager::notifyListeners()
{
	auto iter = getInstance().timers.begin();
	while (iter != getInstance().timers.end()) {
		Timer& timer = iter->second;
		if (*timer.state == Timer::TimerState::TIMER_START && timer.realState == Timer::RealState::STARTED) {
			timer.notifyListeners();
		}
		++iter;
	}
}

int TimerManager::generateTimerId()
{
	static int count = 0;
	return count++;
}

TimerManager::~TimerManager()
{
	while (timers.begin() != timers.end()) {
		destroyTimer(*(timers.begin()->second.timerId));
	}
}

TimerManager & TimerManager::getInstance()
{
	if (TimerManager::instance == nullptr) {
		TimerManager::instance = new TimerManager();
	}

	return *TimerManager::instance;
}

Timer & TimerManager::createTimer()
{
	int timerId = getInstance().generateTimerId();
	Timer timer(timerId);
	timer.realState = Timer::RealState::KILLED;
	getInstance().timers[timerId] = timer;
	return getInstance().timers[timerId];
}

Timer & TimerManager::getTimer(int timerId)
{
	auto iter = getInstance().timers.find(timerId);
	if (iter != getInstance().timers.end()) {
		return iter->second;
	}

	throw TimerNotExistException(timerId);
}

void TimerManager::onTimer(CWnd * wnd)
{
	killTimer(wnd);
	notifyListeners();
	startTimer(wnd);
	destroyTimers();
}

void TimerManager::addDelayedDisposableEvent(TimerFunc func, int millisesoncd)
{
	Timer& timer = createTimer();
	timer.addEvent(
		[&timer, func](string event)->void {
		func(event);
		timer.destroyTimer(); 
	}
	);
	timer.startTimer(millisesoncd);
}
