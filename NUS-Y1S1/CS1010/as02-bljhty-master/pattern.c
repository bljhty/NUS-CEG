/**
 * CS1010 Semester 1 AY21/22
 * Assignment 2: Pattern*
 * Read in two positive integers: an interval n (n >= 1) 
 * and the height of the triangle h.  Then draw a triangle
 * according to a given pattern.  
 *
 * @file: pattern.c
 * @author: bryan lee (Group B08)
 */
#include "cs1010.h"
#include <math.h>
#include <stdbool.h>

// checking for prime numbers
bool check_prime(long prime)
{
  long i = 2;
  while (i <= sqrt(prime))
  {
    // prime numbers can only be divisible by 1 or itself
    if ( prime % 2 == 0 || prime % i == 0)
    {
      return false;
    }
    i += 1;
  }
  return true;
}

// pattern to find the leading number of the start of each cell
long lead_number(long n, long row)
{
  return (((row * (row - 1)) / 2) * n) + 1;
}
// checking if a cell contains prime number given n, row and col
bool check_cell_prime(long n, long row, long cell)
{
  long leading_num = lead_number(n, row);
  long i = 0;
  long firstnum = leading_num + (cell - 1);

  while (i < n) 
  {
    long num = firstnum + (row * i);
    if (check_prime(num))
    {
      return true;
    }
    i += 1;
  }
  return false;
}

// printing of hashes if prime else blank spaces
void print_hash_empty(long n, long row, long cell)
{
  if (check_cell_prime(n, row, cell))
  {
    cs1010_print_string("#");
  } else {
    cs1010_print_string(" ");
  }
}

// printing of blank spaces before and after the triangle
void print_blanks(long h, long row)
{
  long i = 0;
  while (i < (h - row))
  {
    cs1010_print_string(" ");
    i += 1;
  }
}

// printing of final triangle
void print_triangle(long n, long row, long h)
{
  // printing of blank spaces before the triangle
  print_blanks(h, row);
  long x = 1;
  while (x <= ((2 * row) - 1))
  {
    // printing of hash and spaces within the triangle
    print_hash_empty(n, row, x);
    x += 1;
  }
  // printing of blank spaces after the triangle
  print_blanks(h, row);
  cs1010_println_string("");
}



int main()
{
  // input n and height of triangle
  long n = cs1010_read_long();
  long height = cs1010_read_long();

  long z = 1;
  while (z <= height) 
  {
    print_triangle(n, z, height);
    z += 1;
  }
}
