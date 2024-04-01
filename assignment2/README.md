# External Storage Data Structure Implementation Assignment

## Objective
The objective of this assignment is to learn how to store information and implement data structures for external storage. You are provided with a relation Employee(id, name, bio, manager-id). The values of id and manager-id are integers each with fixed sizes of 8 bytes. The values of name and bio are character strings and take at most 200 and 500 bytes, respectively. The size of each page is 4096 bytes (4KB). The size of each record is less than the size of a page. 

## Implementation Requirements
- Read the input Employee relation from a CSV file (`Employee.csv`).
- Store the Employee relation in a new file on disk.
- Use one of the methods explained in lectures on storage management for storing variable-length records and page data structures.
- Implement searching functionality to find records with a given Employee id.
- Limit main memory usage to three pages during file creation and search.

## Input File
- The input relation is stored in a CSV file.
- Each tuple is in a separate line, and fields are separated by commas.
- The input CSV file is assumed to be in the current working directory and named `Employee.csv`.

## Data File Creation and Page Structure
- Store records of the input CSV file in a new data file named `EmployeeRelation`.
- Use methods explained in lectures for storing variable-length records and page data structures.

## Searching the Data File
- Accept an Employee id in the command line and search the file for records with the given id.
- Allow searching for records of multiple ids, one id at a time.

## Main Memory Limitation
- Keep up to three pages in main memory during file creation and search.

## Note
Ensure that your program adheres to the provided skeleton code and follows the specified requirements. Failure to do so may result in a loss of points.

