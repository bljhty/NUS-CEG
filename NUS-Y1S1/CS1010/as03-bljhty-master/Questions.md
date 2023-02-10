
# Question 1: Days (7 marks)
----------------------------

COVID-19 restrictions have disrupted the routines and
structure of our daily life.  As home-based learning
and working-from-home become a norm, sometimes, days are
blended and blurred into one.

To remind ourselves of the passage of time, write a
program called `days` that reads in two integers from
the standard input, the first is the month (ranged from
1 to 12, inclusive) and the second is the day (ranged
from 1 to 31, inclusive).  The program then prints to the
standard output which day of the year it is.  For instance,
September 14 is the 257th day of the year.  If the input to
`days` is `9 14`, the program should print `257`.

Assume that the year is not a leap year.

Use a fixed-length array and loops to solve this problem
instead of conditional statements.  Solutions that use
conditional statements (in any form) will receive a 0.

Sample Runs
-----------
```
ooiwt@pe112:~/as03-ooiwt$ ./days
1 1
1
ooiwt@pe112:~/as03-ooiwt$ ./days
9 14
257
ooiwt@pe112:~/as03-ooiwt$ ./days
12 31
365
```

# Question 2: ID (7 marks)
--------------------------

Your NUS student id has a letter at the end. This letter is
called a check code and is a form of redundancy check used
for detecting errors, especially when your student id is
manually entered into a software application.

Your check code is calculated by:

1. Sum up the digits in your student id. Let the sum be N.
2. Divide N by 13, and take the remainder. Let the remainder
   be R.  
3. Look up the table below:

R  | Check Code
---|------------
0  | Y
1  | X
2  | W
3  | U
4  | R
5  | N
6  | M
7  | L
8  | J
9  | H
10 | E
11 | A
12 | B

Write a program `id.c` that reads in an integer containing
the digits of a student's id from the standard input. Print
out the check code to the standard output.

You can use the `putchar` function to print a single `char`
variable to the standard output.

Use a fixed-length array and loops to solve this problem 
instead of conditional statements.  Solutions that use 
conditional statements (in any form) will receive a 0.

Sample Run
----------
```
ooiwt@pe116:~/as03-ooiwt$ ./id
1933091
Y
ooiwt@pe116:~/as03-ooiwt$ ./id
3364497
E
ooiwt@pe116:~/as03-ooiwt$ ./id
1111111111111
Y
```

Question 3: Max (8 marks)
-------------------------

Write a program max that finds the maximum value from a list
of n integers L.

Instead of doing this with a loop, solve this question with
_recursion_. Write a function

```
long max(const long list[], long start, long end)
{
	:
}
```

that calls itself and return the maximum value among the
array elements list[start] .. list[end - 1]. It should split
the input list into two halves (roughly), find the maximum
of the left half, find the maximum of the right half, and
return the larger of these two maximums.

The program should read the following from the standard
inputs:

- The first number is a positive integer n.
- The next n numbers correspond to the list of integers L.

The program should then print to the largest value among
the inputs to the standard output.

n is guaranteed to be no more than 10000.  You may use a 
fixed-length array on the stack to store the inputs.

Note that you are not allowed to use loops of any kind in
your solution (`for`, `while`, `do-while`).  Solutions
that use loops will receive a 0.

Sample Runs
-----------

```
ooiwt@pe116:~/as03-ooiwt$ cat input
5
-5 3 1 8 2
ooiwt@pe116:~/as03-ooiwt$ ./max < input
8
```


Question 4: Padovan (8 marks)
-----------------------------

Padovan numbers are a sequence of numbers defined as
follows:

p(0) = 1
p(1) = 0
p(2) = 0

For n > 2:
  p(n) = p(n-2) + p(n-3)

You might notice the similarity between Padovan numbers 
and Fibonacci numbers.  Padovan numbers are named after 
Richard Padovan, an architect.  Unlike Fibonacci, Padovan
is "only" 88 years old and the sequence is named after him
only recently!

Write a program `padovan` that reads in a non-negative
integer n from the standard input, and writes to the 
standard output, all the Padovan numbers p(n), p(n-1),
.. p(0), in order (one number per line).

n is guaranteed to be 150 or less.

There are 3 marks allocated for efficiency for this 
question.  Avoid repetitively re-calculating the same
sequence of numbers over and over to earn these 3 marks.

Sample Runs
-----------

```
ooiwt@pe116:~/as03-ooiwt$ ./padovan
5
1
0
1
0
0
1
ooiwt@pe116:~/as03-ooiwt$ ./padovan
0
1
ooiwt@pe116:~/as03-ooiwt$ ./padovan
150
368560195659412891
278217860370802066
210020449144859290
158539746514553601
119678113856248465
90342335288610825
68197411225942776
51480702630305689
38861632658305136
29335778567637640
22144924062668049
16716708595637087
12619069972000553
9525854090667496
7190854504969591
5428215467030962
4097638623636534
3093215881333057
2334999585697905
1762639037938629
1330576843394428
1004422742303477
758216295635152
572360547759276
432062194544201
326154101090951
246206446668325
185855747875876
140298353215075
105908093453250
79947654422626
60350698792449
45557394660801
34390259761825
25960439030624
19596955630177
14793304131648
11167134898976
8429820731201
6363483400447
4803651498529
3626169232672
2737314167775
2066337330754
1559831901918
1177482265857
888855064897
670976837021
506505428836
382349636061
288627200960
217878227876
164471408185
124155792775
93722435101
70748973084
53406819691
40315615410
30433357674
22973462017
17342153393
13091204281
9882257736
7459895657
5631308624
4250949112
3208946545
2422362079
1828587033
1380359512
1042002567
786584466
593775046
448227521
338356945
255418101
192809420
145547525
109870576
82938844
62608681
47261895
35676949
26931732
20330163
15346786
11584946
8745217
6601569
4983377
3761840
2839729
2143648
1618192
1221537
922111
696081
525456
396655
299426
226030
170625
128801
97229
73396
55405
41824
31572
23833
17991
13581
10252
7739
5842
4410
3329
2513
1897
1432
1081
816
616
465
351
265
200
151
114
86
65
49
37
28
21
16
12
9
7
5
4
3
2
2
1
1
1
0
1
0
0
1
```
