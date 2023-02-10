/**
 * CS1010 Semester 1 AY21/22
 * Assignment 3: Max
 *
 * Read in a sequence of numbers and recursively find
 * the maximum.
 *
 * @file: max.c
 * @author: Bryan Lee (Group B08)
 */
#include "cs1010.h"

long max(const long list[], long start, long end)
{
  // recursion ending clause - if only 1 number return it
  if (start == end)
  {
    return list[start];
  }
  // finding the middle of the list
  long middle = (start + end) / 2;
  // from the left side of the list look for max no.
  long left_max = max(list, start, middle);
  // from the right side of the list look for max no.
  long right_max = max(list, middle + 1, end);

  // compare left vs right and obtain max
  if (left_max > right_max)
  {
    return left_max;
  }
  return right_max;
}

int main()
{
  // since n < 10000, initialise L[10000]
  long L[10000];
  long x = cs1010_read_long();

  // loop to read in array L[ ] 
  for (long i = 0; i < x; i += 1)
  {
    L[i] = cs1010_read_long();
  }
  // printing of output
  cs1010_println_long(max(L, 0, x - 1));
}
