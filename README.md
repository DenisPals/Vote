# Vote - A C program that runs a runoff election
## Complexity
This Vote program, contrary to a plurality vote will run a runoff election iif no candidate has more than 50% of the votes and if any candidate has the majority of the votes (more than 50%), he will be declared the winner. The process repeats itself until a candidate can be declared winner.
This is a course related project and part of the problemset in Harvards CS50x week 3.

The challenge was:
* The implementation of the vote function. 
* The implementation of the tabulate function. 
* The implementation of the print_winner function. 
* The implementation of the find_min function. 
* The implementation of the is_tie function. 
* The implementation of the eliminate function. 


## File
**runoff.c**
Runoff.c contains the entire code, written in the C programming language. For simplicity the program uses the cs50 library, which cuts down the complexity of string manipulation and other areas at the current progress of the course. This complexities are added back at later point in course and are covered extensively.

## How to run
To run this program, a C compiler must be installed and the cs50 library must be included. The best way is to run it is to use a Linux enviroment like WSL for windows (if not using Linux) and to run 
> ./runoff Alice Bob Charlie

for example. The user will then be prompted with the number of voters 
> Number of voters: 

and can determine howmany people are taking part in the voting process.

## Output
```
./runoff Alice Bob Charlie

Number of voters: 5
Rank 1: Alice
Rank 2: Bob
Rank 3: Charlie

Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob

Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice

Rank 1: Bob
Rank 2: Alice
Rank 3: Charlie

Rank 1: Charlie
Rank 2: Alice
Rank 3: Bob

Alice
```