#include "stdafx.h"
#include "KeyboardManager.h"
#include "Events.h"

KeyState::KeyState(unsigned key)
{
	this->key = key;
	this->state = E_KeyState::RELEASED;
}

KeyState::~KeyState()
{
}

void KeyState::press()
{
	this->state = E_KeyState::PRESSED;
}

void KeyState::release()
{
	this->state = E_KeyState::RELEASED;
}

KeyboardManager* KeyboardManager::instance = nullptr;

KeyboardManager & KeyboardManager::getInstance()
{
	if (KeyboardManager::instance == nullptr) {
		KeyboardManager::instance = new KeyboardManager();
	}

	return *KeyboardManager::instance;
}

KeyboardManager::KeyboardManager() {
	this->keyMap[multiKey] = new KeyState(multiKey);
}

KeyboardManager::~KeyboardManager()
{
	for (auto iter : keyMap) {
		delete iter.second;
	}
}

void KeyboardManager::watchKeyState(unsigned key)
{
	auto iter = getInstance().keyMap.find(key);
	if (iter == getInstance().keyMap.end()) {
		getInstance().keyMap[key] = new KeyState(key);
	}
}

void KeyboardManager::watchAlphabetKeys()
{
	for (auto key = 'A'; key <= 'Z'; ++key) {
		watchKeyState(key);
	}
}

void KeyboardManager::addKeyDownListener(KeyboardListener& listener, unsigned key)
{
	auto iter = getInstance().keyMap.find(key);
	if (iter == getInstance().keyMap.end()) {
		getInstance().keyMap[key] = new KeyState(key);
	}

	getInstance().keyMap[key]->addListener(listener, FG_KEYDOWN);
}

void KeyboardManager::addKeyUpListener(KeyboardListener & listener, unsigned key)
{
	auto iter = getInstance().keyMap.find(key);
	if (iter == getInstance().keyMap.end()) {
		getInstance().keyMap[key] = new KeyState(key);
	}

	getInstance().keyMap[key]->addListener(listener, FG_KEYUP);
}

void KeyboardManager::addKeyListener(KeyboardListener & listener, unsigned key)
{
	auto iter = getInstance().keyMap.find(key);
	if (iter == getInstance().keyMap.end()) {
		getInstance().keyMap[key] = new KeyState(key);
	}

	getInstance().keyMap[key]->addListener(listener, FG_KEYUP);
	getInstance().keyMap[key]->addListener(listener, FG_KEYDOWN);
}

void KeyboardManager::addMultiKeyListener(KeyboardListener & listener)
{
	getInstance().keyMap[multiKey]->addListener(listener, FG_KEYUP);
	getInstance().keyMap[multiKey]->addListener(listener, FG_KEYDOWN);
}

void KeyboardManager::addKeyDownListener(void(*listener)(string, unsigned), unsigned key)
{
	addKeyDownListener(KeyboardListener(listener), key);
}

void KeyboardManager::addKeyUpListener(void(*listener)(string, unsigned), unsigned key)
{
	addKeyUpListener(KeyboardListener(listener), key);
}

void KeyboardManager::addKeyListener(void(*listener)(string, unsigned), unsigned key)
{
	addKeyListener(KeyboardListener(listener), key);
}

void KeyboardManager::addMultiKeyListener(void(*listener)(string, unsigned))
{
	addMultiKeyListener(KeyboardListener(listener));
}

void KeyboardManager::removeListeners(unsigned key)
{
	auto iter = getInstance().keyMap.find(key);
	if (iter == getInstance().keyMap.end()) {
		getInstance().keyMap[key]->removeAll();
	}
}

void KeyboardManager::removeListener(int listenerId, unsigned key)
{
	auto iter = getInstance().keyMap.find(key);
	if (iter == getInstance().keyMap.end()) {
		getInstance().keyMap[key]->removeListener(listenerId, FG_KEYDOWN);
		getInstance().keyMap[key]->removeListener(listenerId, FG_KEYUP);
	}
}

void KeyboardManager::removeAllListeners()
{
	auto iter = getInstance().keyMap.begin();
	while (iter != getInstance().keyMap.end()) {
		iter->second->removeAll();
		++iter;
	}
}

bool KeyboardManager::isPressed(unsigned key)
{
	auto iter = getInstance().keyMap.find(key);
	if (iter != getInstance().keyMap.end()) {
		return iter->second->getState() == KeyState::E_KeyState::PRESSED;
	}
	return false;
}

void KeyboardManager::onKey(string event, unsigned key)
{
	getInstance().keyMap[multiKey]->notifyListeners(event, key);

	auto iter = getInstance().keyMap.find(key);
	if (iter != getInstance().keyMap.end()) {
		if (event == FG_KEYDOWN) {
			iter->second->press();
		}
		else if (event == FG_KEYUP) {
			iter->second->release();
		}
		iter->second->notifyListeners(event, key);
	}
}
