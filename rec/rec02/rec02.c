// swap two variables without using a third variable
// Name: Yafet Kubrom  EUID:11334602
#include<stdio.h>
#include<stdlib.h>

int main()
{
    int* int_ptr = (int*)malloc(2 * sizeof(int));
    if(!int_ptr) printf("Not enough memory available.\n");

    printf("Enter first integer: ");
    scanf("%d",&int_ptr[0]);
    printf("Enter second integer: ");
    scanf("%d",&int_ptr[1]);

    printf("Original Values: 1st =  %4d 2nd =  %4d\n",int_ptr[0],int_ptr[1]);
    int_ptr[0] = int_ptr[0] ^ int_ptr[1];
    int_ptr[1] = int_ptr[0] ^ int_ptr[1];
    int_ptr[0] = int_ptr[0] ^ int_ptr[1];
    printf("Swapped  Values: 1st =  %4d 2nd =  %4d\n",int_ptr[0],int_ptr[1]);
    free(int_ptr);
}
