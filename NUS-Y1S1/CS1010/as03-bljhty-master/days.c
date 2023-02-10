/**
 * CS1010 Semester 1 AY21/22
 * Assignment 3: Days
 *
 * @file: days.c
 * @author: Bryan Lee (Group B08)
 */
#include "cs1010.h"

long time(long d, long m)
{
  // initialising month corresponding to the days in each month, jan = 0 as we are only taking in days for jan
  long month[12] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};
  long days = 0;

  for (long i = 0; i < m; i += 1)
  {
    days = days + month[i];
  }
  return days + d;
}

int main() 
{
  long d = cs1010_read_long();
  long m = cs1010_read_long();

  cs1010_println_long(time(m, d));
}
