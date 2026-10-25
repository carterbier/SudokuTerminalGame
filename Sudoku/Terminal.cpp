#include "Terminal.h"

#include <iostream>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

Terminal::Terminal()
{
	outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (outputHandle == INVALID_HANDLE_VALUE || outputHandle == nullptr)
		return;

	if (!GetConsoleMode(outputHandle, &originalMode))
		return;

	DWORD newMode = originalMode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;

	if (!SetConsoleMode(outputHandle, newMode))
		return;

	std::cout << "\x1b[?1049h" << std::flush;

	ready = true;
}

Terminal::~Terminal()
{
	if (ready)
	{
		std::cout << "\x1b[?1049l" << std::flush;
		SetConsoleMode(outputHandle, originalMode);
	}
}

bool Terminal::isReady() const
{
	return ready;
}

void Terminal::clearScreen()
{
	std::cout << "\x1b[2J\x1b[H" << std::flush;
}