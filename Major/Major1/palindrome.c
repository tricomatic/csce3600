// Name................Yafet Kubrom
// Course Section......CSCE 3600.001
// Date................02/17/2020
// Description
// .....This code contains a function that takes integer less than 2 billion and check if its binary version
//  is palindrome(same backward and forward ) using bitwise operation.
#include "major1.h"

void palindrome(unsigned int num)
{
    int a = 0, b = 31;	// used for shifting 1

// for loop to display the binary representation of num.
    printf("The binary representation is: ");
    for(int i = 31; i >= 0; i--)
    {
        printf("%d", (num & (1 << i)) >> i);
    }
    printf("\n");

// while loop to check if num is palindrome. 
    while(num & (1 << a) && num & (1 << b) || !(num & (1 << a)) && !(num & (1 << b)))
    {
        a++;
        b--;
        if (a == 16)
        {
            printf("%d is a palindrome\n", num);
            return;
        }
    }
    printf("%d is not a palindrome\n", num);
    printf("\n");

}
