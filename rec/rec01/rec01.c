//Recitation Assignment 1
//Prompt the user for ASCII printable character and print out in dec, hex and bin digit. 
//(assume it is char)
#include <stdio.h>

void bitwise(int ch);
int main()
{
    unsigned char ch;
    printf("Enter an ASCII character: ");
    scanf("%c",&ch);
    printf("The ASCII value of %c is:", ch);
    printf("\n\tdec -- %d",ch);
    printf("\n\thex -- %x",ch);
    printf("\n\tbin -- ");
    bitwise(ch);
}

void bitwise(int ch)
{
    int k;
    for(int i = 7; i >= 0; i--)
    {
        k = ch >> i;
        if(k & 1) printf("1");
        else printf("0");
    }
    printf("\n");
}