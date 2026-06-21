#pragma once
#ifdef WIN32
// posix io api
extern "C" {
	extern __int64 __cdecl lseek64(int _FileHandle, __int64 _Offset, int _Origin);
	extern void* valloc(int n);
	extern void vfree(void *p);
}
#endif
#if defined(__APPLE__)
#define lseek64 lseek
#endif