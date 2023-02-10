/**
 * CS1010 Semester 1 AY21/22
 * Assignment 2: Prime
 *
 * Reads in an integer n from standard input and print 
 * the smallest prime larger or equal to n.
 *
 * @file: prime.c
 * @author: Bryan Lee (Group B08)
 */
#include "cs1010.h"
#include <math.h>
#include <stdbool.h>

// checking for prime number given n
bool is_prime(long n) {
  long i = 2;
  while (i <= sqrt(n)) {
    if (n % i == 0) {
      return false;
    } 
    i += 1;
  }
  return true;
} 
// looking for the smallest prime number = or greater than n
long next_prime (long n) {
  long prime = n;
  while (!is_prime(prime)) {
    prime += 1;
  }
  return prime;
}

int main()
{
  long n = cs1010_read_long();
  cs1010_println_long(next_prime(n));
}
