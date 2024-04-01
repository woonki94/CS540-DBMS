# External Memory Index Structure Implementation Assignment

## Objective
The objective of this assignment is to learn how to implement index structures on data stored in external memories. You are provided with a relation Employee(id, name, bio, manager-id). The values of id and manager-id are integers each with the fixed sizes of 8 bytes. The values of name and bio are character strings and take at most 200 and 500 bytes, respectively. The size of each page is 4096 bytes (4KB). The size of each record is less than the size of a page.

## Implementation Requirements
- Create a hash index file for relation Employee using attribute id.
- Enable users to search the created index by providing the id of a record.

## Input File
- The input relation is stored in a CSV file.
- Each tuple is in a separate line, and fields of each record are separated by commas.
- The input CSV file is assumed to be in the current working directory and named `Employee.csv`.

## Index File Creation
- Read the input Employee relation and build a linear hash index for the relation using attribute id.
- Store the hash index in a binary data file named `EmployeeIndex` in the current working directory.
- Use methods explained in lectures for storing variable-length records and page data structures.
- Use hash function h = idmod216.
- Increment the value of n if the average number of records per each page exceeds 70% of the page capacity.

## Main Memory Limitation
- During index creation, the program can keep up to three pages plus the directory of the hash index in main memory at any time.

## Searching the Index File
- After finishing the index creation, the program must accept an Employee id in its command line and search the index file for all records of the given id.
- During the lookup operation, the program may use up to three pages plus the directory of the hash index in main memory at any time.
- The user may search for records of multiple ids, one id at a time.

## Note
Ensure that your program adheres to the provided skeleton code and follows the specified requirements. Failure to do so may result in a loss of points.

