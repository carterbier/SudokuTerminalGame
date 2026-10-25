#pragma once

#include <Windows.h>

class Terminal
{
public:
	Terminal();
	~Terminal();

	Terminal(const Terminal&) = delete;
	Terminal& operator=(const Terminal&) = delete;

	bool isReady() const;
	static void clearScreen();

private:
	HANDLE outputHandle = INVALID_HANDLE_VALUE;
	DWORD originalMode = 0;
	bool ready = false;
};