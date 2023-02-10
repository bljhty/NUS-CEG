/**
 * CS1010 Semester 1 AY21/22
 * Assignment 2: Rectangle
 *
 * Reads in two integer numbers corresponding to the width and the 
 * height of a rectangle.   Draw the rectangle with the given characters
 * to the standard output.
 *
 * @file: rectangle.c
 * @author: Bryan Lee (Group B08)
 */
#include "cs1010.h"

#define TOP_LEFT "╔"
#define TOP_RIGHT "╗"
#define BOTTOM_RIGHT "╝"
#define BOTTOM_LEFT "╚"
#define HORIZONTAL "═"
#define VERTICAL "║"

// printing the top protion of the rectangle
void top_rect(long w)
{
  cs1010_print_string(TOP_LEFT);

  for (long i = 0; i < w - 2; i += 1) 
  {
    cs1010_print_string(HORIZONTAL);
  }

  cs1010_print_string(TOP_RIGHT);
}

// printing the body of a reectangle
void body_rect(long w)
{
  cs1010_print_string(VERTICAL);

  for (long i = 0; i < w - 2; i += 1)
  {
    putchar(' ');
  }

  cs1010_print_string(VERTICAL);
}

// printing the lower ends of the rectangle
void btm_rect(long w)
{
  cs1010_print_string(BOTTOM_LEFT);

  for(long i = 0; i < w - 2; i += 1)
  {
    cs1010_print_string(HORIZONTAL);
  }

  cs1010_print_string(BOTTOM_RIGHT);
}


int main()
{
  // input width(w) and height(h) to draw out rectangle
  long w = cs1010_read_long();
  long h = cs1010_read_long();

  top_rect(w);
  putchar('\n');

  for (long i = 0; i < h - 2; i += 1)
  {
    body_rect(w);
    putchar('\n');
  }
  btm_rect(w);
  putchar('\n');
}

