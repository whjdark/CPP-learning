#pragma once

#include <stdio.h>
#include <process.h>

static auto pid = _getpid();

inline void fake_ref(char* ptr) {
	if (pid == 0) {
		putchar(*ptr);
	}
}

template <typename T>
inline void dont_opt_away(T&& datum)
{
	fake_ref(reinterpret_cast<char*>(&datum));
}