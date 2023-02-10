/**
 * CS1010 Semester 1 AY21/22
 * Assignment 3: ID
 *
 * @file: id.c
 * @author: Bryan Lee (Group B08)
 */
#include "cs1010.h"

long id(long number)
{
  long num = number;
  long sum = 0;

  // loop to sum the numbers of student id
  while (num != 0)
  {
    // % 10 to obtain last digit
    long remainder = num % 10;

    // add the last digit with the digit before it
    sum = sum + remainder;

    // divide by 10 after 
    num = num / 10;
  }
  return sum;
}

int main()
{
  long number = cs1010_read_long();

  // initialising array with respect to the letters according to its value
  char code[13] = {'Y', 'X', 'W', 'U', 'R', 'N', 'M', 'L', 'J', 'H', 'E', 'A', 'B'};

  // obtaining the remainder when divided by 13 to print out letter corresponding to the code
  long R = id(number) % 13;

  // printing of letter corresponding to R
  putchar(code[R]);
}
