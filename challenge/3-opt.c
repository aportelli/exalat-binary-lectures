#include <stdint.h>
#include <stdlib.h>

uint32_t hash(const char *s) 
{
	uint32_t   crc = 0xFFFFFFFF;
	const char *pt = s;
  
	while (*pt) 
  {
		char ch = *pt;

		for(size_t j = 0; j < 8; ++j)
    {
			uint32_t b = (ch^crc)&1;
			crc >>=1;
			if(b) crc=crc^0xEDB88320;
			ch >>=1;
		}
    pt++;
	}
	
	return ~crc;
}
