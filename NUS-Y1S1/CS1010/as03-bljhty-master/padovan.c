/**
 * CS1010 Semester 1 AY21/22
 * Assignment 3: Padovan
 *
 * @file: padovan.c
 * @author: Bryan Lee (Group B08)
 */
#include "cs1010.h"
int main()
{
  long n = cs1010_read_long();
  long padovan[151]; // max no. is 150, so array shld cover from 0 - 150 so [151] not [150]

  padovan[0] = 1;
  padovan[1] = 0;
  padovan[2] = 0;

  for (long i = 3; i <= 150; i += 1)
  {
    padovan[i] = (padovan[i - 2] + padovan[i - 3]);
  }
  for (long k = n; k >= 0; k -= 1)
  {
    cs1010_println_long(padovan[k]);
  }
}
