#ifndef COMMON_H
#define COMMON_H

#define IN
#define OUT

#define GLOBAL
#define PRIVATE static

#define OPTIONAL

#define UNUSED(X) (void)X

#ifdef _DEBUG
#define IF_DEBUG(X) X
#else
#define IF_DEBUG(X)
#endif

#ifndef __FUNCTION__
#define __FUNCTION__
#endif // !__FUNCTION__

#define _STR(X) #X
#define _STR_EXPAND(X) _STR(X)
#define _DEBUG_MSG(MSG) __FILE__ " [" _STR_EXPAND(__LINE__) "] >>>  " MSG

#define BIT(X) 1 << X

#endif
