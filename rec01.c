#include <stdio.h>

int main()
{

unsigned char userInput; //unsigned char variable

//inputing the character
printf("Enter a ASCII character: ");
scanf("%c", &userInput);

printf("\nThe decimal value of %c is: %d\n", userInput, userInput);
printf("The hexadecimal value of %c is: %x\n",userInput, userInput);
printf("The binary value of %c is: ",userInput);
for (int i = 7; i>=0;i--)
{
    printf("%d", (userInput>>i)& 1);//compares userInput to output binary counterpart
}
printf("\n");

return 0;
}//end of program