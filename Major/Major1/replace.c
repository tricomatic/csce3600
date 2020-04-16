//Author: Brandon Vojtkofsky bmv0045
//Date: 2/9/2020
#include <stdio.h>
#include <stdlib.h>
#include "major1.h"

void replace(unsigned int orig)
{
    unsigned int mask = 0;
    int pos = -1;
    
    while(mask < 1 || mask >= 3000000000)
    {
        printf("Enter a new integer between 0 and 3 billion: ");
        scanf("%u", &mask);
        //endl;
    }
    
    while(pos < 0 || pos > 31)
    {
        printf("\nEnter the position of the bit to be replaced (0-31): ");
        scanf("%d", &pos);
        
    }
    
    unsigned int pbit = 0, pmask = 0;//2 single bit indicator values to use for flip
    
    //making sure pbit only contains the bit in mask that we want
    pbit = mask >> pos;
    pbit = pbit << 31;
    pbit = pbit >> 31-pos;
    
    if((orig >> pos) & 1 == (mask >> pos) & 1)//if both are 1 at that pos
    {
        printf("\nAfter replacement, the new number is %u\n\n", orig);
        return;
    }
    else if(!((orig >> pos) & 1) && !((mask >> pos) & 1))//if both are 0 at that pos
    {
        printf("\nAfter replacement, the new number is %u\n\n", orig);
        return;
    }
    else
    {
        
        if((orig >> pos) & 1)
        {
            //assuming orig is 1 at that bit...
            pmask = ~pmask;
            pmask = pmask >> pos;
            pmask = pmask << 31;
            pmask = pmask >> 31 - pos;//as with pbit
            pmask = ~pmask;
            
            //if(!((orig >> pos) & 1))
            {
                orig = orig ^ pmask;//gives us the inverse of our new #
                orig = ~orig;//flips it to the correct #
                printf("\nAfter replacement, the new number is %u\n\n", orig);
            }
        }
        else
        {
            //assuming orig is 0 at that bit...
            if((pbit >> pos) & 1)
            {
                orig = orig ^ pbit;//change the bit to a 1 if pbit is 1
                printf("\nAfter replacement, the new number is %u\n\n", orig);
            }
            /*else
             {
             orig = orig & (~pbit);
             }*/
        }
        
        printf("\n");
        return;
    }
}

//testing bench
/*int main()
 {
 unsigned int inin = 5;
 int i = 31;
 
 inin = replace(inin);
 
 unsigned char mask;
 
 printf("\nBinary: ");
 
 while(i >= 0)
 {
 mask = inin & 0;
 
 mask = (inin >> i);
 
 if(mask & 1)
 {
 printf("1");
 }
 else
 {
 printf("0");
 }
 
 i = i-1;
 }
 
 printf("\nuin = %u\n",inin);
 
 return 0;
 }*/
