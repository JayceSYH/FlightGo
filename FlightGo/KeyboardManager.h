#pragma once
#include "Listen.h"

class KeyState: virtual public KeyboardListenable {
public:
	enum E_KeyState { PRESSED, RELEASED };
private:
	unsigned key;
	E_KeyState state;
public:
	KeyState(unsigned key);
	virtual ~KeyState();
	void press();
	void release();
	E_KeyState getState() { return this->state;  };
};

class KeyboardManager
{
private:
	static const unsigned multiKey = -1;
	map<unsigned, KeyState*> keyMap;
	static KeyboardManager* instance;
	static KeyboardManager& getInstance();
	KeyboardManager();
public:
	~KeyboardManager();
	static void watchKeyState(unsigned key);
	static void watchAlphabetKeys();
	static void addKeyDownListener(KeyboardListener& listener, unsigned key);
	static void addKeyUpListener(KeyboardListener& lisener, unsigned key);
	static void addKeyListener(KeyboardListener& listener, unsigned key);
	static void addMultiKeyListener(KeyboardListener& listenr);
	static void addKeyDownListener(void(*listener)(string, unsigned), unsigned key);
	static void addKeyUpListener(void(*listener)(string, unsigned), unsigned key);
	static void addKeyListener(void(*listener)(string, unsigned), unsigned key);
	static void addMultiKeyListener(void(*listener)(string, unsigned));
	static void removeListeners(unsigned key);
	static void removeListener(int listenerId, unsigned key);
	static void removeAllListeners();
	static bool isPressed(unsigned key);
	static void onKey(string event, unsigned key);
};