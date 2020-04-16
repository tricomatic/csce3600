//
//  power.c
//  Group-29-Major-1
//
//  Author: James Colton Nicholas
//  EUID: jcn0080
//  Course: 3600.001
//
//  The power funtion will taken in a given "num" for which it will then
//      determine, using bitwise operations, if it is a power of 2. If it
//      IS a power of 2, the user will be prompted of suhc, otherwise the
//      user will be notified of that and given next greatest power of 2.
//

#include "major1.h"

void power(unsigned int num)
{ //this is the power function!
    
    unsigned int mask = 1; //this mask is used to check if a specific bit is equal to 1
    unsigned int numOnes = 0; //this is used to store the number of ones in a binary number
    for(unsigned int i = 0; i < 31; i++)
    { //this loop will count the number of ones in the binary form of num
        if(mask & num)
        { //if the mask at current position equals binary number in current position
            numOnes++;
            if(numOnes > 1) // we are just checking to find one 1,
                break;      //   so we can break out of the loop if we find more
        }
        mask <<= 1; //moves mask one bit to the left
    }
    
    if(numOnes == 1)
    {
        printf("%u is a power of 2!\n", num);
    }
    else
    {
        printf("%u is not a power of 2.\n", num);
        
        for(unsigned int i = 31; i > 0; i--)
            if(1 << i & num) // this loop will essentially find the leftmost 1 in the binary number
            {                //   and then make the number to the left of it equal to 1 and the rest 0
                mask = 1 << (i+1);
                printf("The next power of 2 is: %u\n", mask);
                break;
            }
    }
    
    printf("\n"); //for formatting
    
}
