#pragma once
#include "Listen.h"
class TimerManager;


typedef Listener<> TimerListener;
typedef Listenable<> TimerListenable;
typedef UINT_PTR TimerId;
typedef std::function<void(string)> TimerFunc;

class Timer {
private:
	friend TimerManager;
	int* timerId;
	long* timeGap;
	enum TimerState { TIMER_STOP, TIMER_START, TIMER_DESTROYED }; //logic state of a timer in app
	TimerState* state;
	TimerListenable* timerListeners;
	enum RealState { STARTED, KILLED };
	RealState realState;//real state of a timer in mfc
	clock_t *clocker;
public:
	Timer() {}
	Timer(int timerId);
	void startTimer(long timeGap);
	void stopTimer();
	void destroyTimer();
	void addEvent(TimerListener listener);
	void addEvent(TimerFunc func);
	void removeEvent(TimerListener listener);
	void removeEvent(int listenerId);
	void notifyListeners();
	int getTimeGap();
	void addDisposableEvent(TimerFunc func);
};

class TimerManager {
private:
	friend Timer;
	map<int, Timer> timers;
	static void destroyTimer(int timerId);
	static void destroyTimers();
	static void killTimer(CWnd* wnd);
	static void startTimer(CWnd* wnd);
	static void notifyListeners();
	int generateTimerId();
	static TimerManager* instance;
	TimerManager() {};
public:
	~TimerManager();
	static TimerManager& getInstance();
	static Timer& createTimer();
	static Timer& getTimer(int timerId);
	static void onTimer(CWnd* wnd);
	static void addDelayedDisposableEvent(TimerFunc func, int millisecond);
};