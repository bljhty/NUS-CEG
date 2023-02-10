/**
 * CS1010 Semester 1 AY21/22
 * Assignment 2: Collatz 
 *
 * Read in a positive integer n from the standard input,
 * then, for each number k between number 1 .. n, transform
 * it into 1 based on the steps below:
 * - if k is even, k = k/2
 * - else, k = 3k + 1
 * Find the number k that requires the most number of steps
 * between 1 and n, and print both k and the number of steps
 * to standard output. If there are multiple numbers with
 * the same number of steps, we prefer to output the smaller
 * among these numbers.
 *
 * @file: collatz.c
 * @author: Bryan Lee (Group B08)
 */
#include "cs1010.h"
#include <math.h>

// finding the stopping time
long collatz(long y) {
  long stopping_time = 0;
  long x = y;
  while(x > 1) {
    if(x % 2 == 0) {
      x = x / 2;
    } else {
      x = (3 * x) + 1;
    }
    stopping_time += 1;
  } 
  return stopping_time;
}

// comparing stopping timee and printing out the one with the longest
void biggest(long n)
{ long l = 1;
  long largest = collatz(n);
  long y = 1;

  while (l < n) 
  {
    if (largest < collatz(l))
    {
      largest = collatz(l);
      l += 1;
      y = (l - 1);
    } else {
      l += 1;
    }
  }
  cs1010_println_long(y);
  cs1010_println_long(largest);
}


int main() 
{
  // input n
 long n = cs1010_read_long();
 
 biggest(n);
}

