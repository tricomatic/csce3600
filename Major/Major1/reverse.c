#include "major1.h"

void reverse(unsigned int num)
{
	unsigned int numBits = sizeof(num) * 8;
	unsigned int numRev = 0;
	for(int i = 0;	i < numBits; i++)
	{
		if(num & (1 << i))
		{
			numRev |= 1 << ((numBits - 1) - i);
		}
	}

	printf("%u with bits reversed is %u\n", num, numRev);
}
