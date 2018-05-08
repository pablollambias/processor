#ifndef _ASSERT_H_
#define _ASSERT_H_
inline void _assert(const char* expression, const char* file, int line);
#ifdef NDEBUG
#define assert(EXPRESSION) ((void)0)
#else
#define assert(EXPRESSION) ((EXPRESSION) ? (void)0 : _assert(#EXPRESSION, __FILE__, __LINE__))
#endif
#endif
