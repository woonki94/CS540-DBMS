# External Memory Join Algorithm Implementation Assignment

## Objective
The objective of this assignment is to learn join implementation algorithms for relations and files on external memories. You are provided with two relations:
- Emp (eid (integer), ename (string), age (integer), salary (double))
- Dept (did (integer), dname (string), budget (double), managerid (integer))

## Implementation Requirements
- Implement the optimized sort-merge join algorithm for Dept ▷◁Dept.managerid=Emp.eid Emp in C/C++.
- Read input relations from CSV files (`Emp.csv` and `Dept.csv`) and assume they are in the current working directory.
- Store the join result in a new CSV file named `Join.csv`.
- Limit main memory usage to 22 pages.
- Use any data structure to represent and store pages in main memory and files.
- You can use/modify code from Assignment 4 for the sorting phase of the join.
- Choose the type (text or binary) of temporary files (runs).

## Input Files
- Each input relation is stored in a CSV file.
- Each tuple is in a separate line, and fields of each record are separated by commas.
- Input files `Emp.csv` and `Dept.csv` are assumed to be in the current working directory.

## Final Output
- The program must store the join result in a new CSV file named `Join.csv` in the current working directory.

## Main Memory Limitation
- The program can keep up to 22 pages in main memory at any time.
- Control, local, or temporary variables used in the program are excluded from this limit.

## Note
Ensure that your program adheres to the provided skeleton code and follows the specified requirements. Failure to do so may result in a loss of points.
