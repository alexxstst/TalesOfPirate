#ifndef XSTRING_H
#define XSTRING_H

#include "TChar.h"

// (_TCHAR *in)(long *in_from)
// (_TCHAR *end_list)
long StringGet(_TCHAR *out, long out_max, _TCHAR *in, long *in_from, _TCHAR *end_list, long end_len);


// (_TCHAR *in)(long *in_from)(_TCHAR *end_list)
void StringSkipCompartment(_TCHAR *in, long *in_from, _TCHAR *skip_list, long skip_len);





















#endif
