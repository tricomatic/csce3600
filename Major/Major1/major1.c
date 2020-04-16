//
//  Group-29-Major-1
//  CSCE 3600.001
//  Members:
//   James Colton Nicholas
//   Yafet Kubrom
//   Michael Rakestraw
//   Brandon Vojtkofsky
//

#include <stdio.h>
#include "major1.h"

int main()
{
    int choice; //stores user menu choice
    unsigned int num; //stores the user-chosen number
    while(1)
    {
        while(1)
        { //menu prompt
            printf("Enter the menu option for the operation to perform: ");
            printf("\n (1) POWER OF 2\n (2) REVERSE BITS\n (3) REPLACE BIT POSITION FROM MASK");
            printf("\n (4) PALINDROME\n (5) EXIT\n--> ");
            scanf("%d", &choice);
            if(choice > 0 && choice < 6) break; //if they enter a valid choice, break
	    printf("Error: Invalid option. Please try again.\n");
        }
        if (choice == 5) break; //if choice was to exit, break
        while(1)
        { //prompt for and store num
            printf("Enter a positive integer less than 2 billion: ");
            scanf("%u", &num);
            if(num < 2000000000 && num > 0) break;
        }

        switch(choice)
        { //switch for all other menu choices
            case 1:
                power(num);
                break;
            case 2:
                reverse(num);
                break;
            case 3:
                replace(num);
                break;
            case 4:
                palindrome(num);
                break;
            default: //if an invalid choice was entered
                printf("Invalid entry.Try again\n");
        }
    }
}
