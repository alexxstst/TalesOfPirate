#include "stdafx.h"
#include "xString.h"

long StringGet(_TCHAR *out, long out_max, _TCHAR *in, long *in_from, _TCHAR *end_list, long end_len)
{
	long offset=-1;    // set offset of get string to -1 for the first do process
	long i;    // temp variable

	--(*in_from);   // dec (*in_from) for the first do process
	do
	{
		out[++offset]=in[++(*in_from)];
		for(i=end_len-1;i>=0;--i)
		{
			if(out[offset]==end_list[i])
			{
				out[offset]=0x00;
				break;
			}
		}
	}while(out[offset] && offset<out_max);
	return offset;
}

void StringSkipCompartment(_TCHAR *in, long *in_from, _TCHAR *skip_list, long skip_len)
{
	long i;    // temp variable

	while(in[(*in_from)])
	{
		for(i=skip_len-1;i>=0;--i)
		{
			if(in[(*in_from)]==skip_list[i])    
				break;
		}
		if(i<0) break;  // dismatch skip conditions, finished
		else ++(*in_from);  // match skip conditions, skip it
	}
}
