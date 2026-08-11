# CS 300: Data Structures and Algorithms

This repository contains portfolio artifacts from my CS 300 course. Project One demonstrates my analysis of the run-time and memory requirements of vectors, hash tables, and binary search trees. Project Two contains a working C++ course-planning program that loads course data, stores it in a binary search tree, and prints the courses in alphanumeric order.

## What was the problem you were solving in the projects for this course?

The problem I was solving was how to create a program that would help academic advisors access course information. The program needed to load course data from a file, validate it, print all courses in alphanumeric order, and display the title and prerequisites of a selected course. In Project One, I evaluated different data structures and developed pseudocode. In Project Two, I used that design to create the working C++ program.

## How did you approach the problem?

I approached the problem by comparing a vector, hash table, and binary search tree. I considered the run-time efficiency, memory usage, searching process, and ability of each structure to print courses in order. Understanding data structures is important because the structure selected affects how efficiently a program stores, organizes, searches, and displays information. I selected a binary search tree because an in-order traversal naturally prints the courses in alphanumeric order while also supporting efficient searches when the tree is reasonably balanced.

## How did you overcome roadblocks?

Some of the roadblocks I encountered involved reading and validating the input file, connecting prerequisites to the correct courses, and making sure the course list printed in the proper order. I overcame these challenges by separating the program into smaller functions and testing each part individually. I also reviewed the program output after making changes, which helped me identify errors without having to search through the entire program at once.

## How has your work expanded your approach to designing software and developing programs?

This work taught me to think about program design and efficiency before beginning to write code. I now understand that selecting a data structure should be based on the requirements of the program rather than simply choosing the first available option. Creating pseudocode and analyzing the expected run time helped me plan the program, recognize possible problems, and make better design decisions before completing the C++ implementation.

## How has your work evolved the way you write maintainable, readable, and adaptable programs?

My work in this course helped me improve how I organize and document my programs. I used descriptive names, consistent formatting, focused functions, and comments to make the code easier to understand. I separated file loading, validation, searching, sorting, printing, and menu operations so that each part had a clear purpose. This structure makes the program easier to test, maintain, and adapt if additional features are needed later.
