# C++ Module 09: Exploring the Standard Template Library (STL)

[![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Build](https://img.shields.io/badge/Build-Make-brightgreen.svg)](https://www.gnu.org/software/make/)

This project is part of the C++ curriculum at 42 School. The objective of this module is to gain a deep, practical understanding of the C++ Standard Template Library (STL). The module consists of three distinct exercises, each requiring the use of different STL containers to solve a specific problem. The project mandates the use of at least four different containers across all exercises. For this implementation, I chose `std::map`, `std::stack`, `std::vector`, and `std::list`.

## Core Concepts Covered

*   **STL Containers:** Gaining hands-on experience with the properties, performance characteristics, and typical use cases of several fundamental containers:
    *   `std::map`: For storing and retrieving key-value data, automatically sorted by key.
    *   `std::stack`: A container adaptor for implementing Last-In, First-Out (LIFO) behavior.
    *   `std::vector`: A dynamic contiguous array, offering efficient random access.
    *   `std::list`: A doubly-linked list, providing fast insertions and deletions anywhere in the sequence.
*   **Performance Analysis:** Empirically comparing the performance of `std::vector` and `std::list` to understand the real-world trade-offs between random access with costly insertions (`vector`) versus sequential access with cheap insertions (`list`).
*   **Advanced Algorithms:** Implementing the complex **Ford-Johnson (Merge-Insertion) sort algorithm**, which is notable for its efficiency in terms of comparisons.
*   **Iterators:** Working with different iterator categories (Random Access for `vector`, Bidirectional for `list`) and understanding how their capabilities dictate algorithm implementation (e.g., `it + n` vs. `std::advance(it, n)`).
*   **C++ Core Language & Library Features:**
    *   Robust exception handling to manage runtime errors gracefully.
    *   File I/O using `std::ifstream` for parsing data from files.
    *   String manipulation and parsing.
    *   Performance timing using the `<chrono>` library.

## Learning Resources and References
- [std::map](https://en.cppreference.com/w/cpp/container/map.html)
- [std::stack](https://en.cppreference.com/w/cpp/container/stack.html)
- [std::vector](https://en.cppreference.com/w/cpp/container/vector.html)
- [std::list](https://en.cppreference.com/w/cpp/container/list.html)
- [Algorithms library](https://cppreference.com/w/cpp/algorithm.html)
- [Iterators and their usage](https://www.studyplan.dev/pro-cpp/iterators)
- [On the Average Case of MergeInsertion](https://link.springer.com/article/10.1007/s00224-020-09987-4) (read section 2.1: Description of MergeInsertion)
- [Merge Insertion Sort / Ford-Johnson Algorithm](https://github.com/decidedlyso/merge-insertion-sort)
  - Note that the example used in this source is for sorting in descending order; but the same concepts apply to sorting in ascending order.

## Project Structure
<p>
   The project is divided into three exercises:
  <img src="https://page-views-counter-534232554413.europe-west1.run.app/view?src=github.com&src_uri=/alimnaqvi/cpp_09" style="display: none;" />
</p>

*   `ex00/`: **Bitcoin Exchange**: A program that calculates the value of a Bitcoin amount on a specific date based on a historical price database.
*   `ex01/`: **Reverse Polish Notation**: A calculator that evaluates mathematical expressions written in RPN.
*   `ex02/`: **PmergeMe**: An implementation of the Ford-Johnson sorting algorithm, comparing the performance of `std::vector` and `std::list`.

Each directory contains a `Makefile`, a `main.cpp` for testing, and the necessary class header and implementation files.

## Implementation Details

### Exercise 00: Bitcoin Exchange

*   **Task:** Read a CSV database of historical Bitcoin prices. Then, process an input file containing dates and Bitcoin amounts, and for each entry, calculate the corresponding value in USD.
*   **Implementation:**
    *   The `BitcoinExchange` class uses a `std::map<std::string, float>` to store the price data. The `std::map` is ideal here because it automatically keeps the data sorted by date (the key), which is crucial for finding the correct price.
    *   The constructor takes an `istream` and parses the CSV database (`data.csv`). It performs validation on each line, ensuring correct format (`date,price`), valid dates, and valid floating-point numbers. Errors result in a `BadDatabaseFormat` exception.
    *   The core logic resides in the `getPriceOnDate(const std::string& date)` method. It leverages `std::map::lower_bound()` to efficiently find the first date in the database that is not less than the requested date.
        *   If an exact match is found, its price is returned.
        *   If no exact match is found, the iterator is decremented to find the closest *previous* date available in the database. This fulfills the requirement to use the price on that date if the exact date is missing.
        *   Edge cases, such as the database being empty or the requested date being earlier than any in the database, are handled with custom exceptions.
    *   The `main` function orchestrates the process: it opens the database and input files, creates the `BitcoinExchange` object, and then reads the input file line by line, parsing and validating each entry before printing the result or an error message.
*   **Key Concepts:** `std::map` for sorted key-value storage, `std::map::lower_bound` for efficient searching, file I/O with `std::ifstream`, string parsing, and custom exception handling.

### Exercise 01: Reverse Polish Notation

*   **Task:** Create a program that takes a single command-line argument—an RPN expression—and evaluates it to produce a single integer result.
*   **Implementation:**
    *   The `RPN` class encapsulates the logic, using a `std::stack<int>` as its core data structure. The stack is perfect for RPN evaluation due to its Last-In, First-Out (LIFO) nature.
    *   The `evaluate()` method processes the input expression string. It iterates through the string, tokenizing it into numbers and operators.
    *   When a number is encountered, it is pushed onto the stack.
    *   When an operator (`+`, `-`, `*`, `/`) is found, the top two numbers are popped from the stack. The operation is performed, and the result is pushed back onto the stack.
    *   The implementation includes robust error checking for all invalid scenarios:
        *   An operation is attempted with fewer than two numbers on the stack.
        *   Division by zero.
        *   The expression contains invalid characters.
        *   After the entire expression is processed, the stack does not contain exactly one number (the final result).
    *   Any error condition throws a custom exception with a descriptive message.
*   **Key Concepts:** `std::stack` for LIFO algorithm implementation, RPN evaluation logic, string tokenization, and comprehensive error handling with exceptions.

### Exercise 02: PmergeMe

*   **Task:** Implement the Ford-Johnson merge-insertion sort algorithm for a sequence of positive integers. The implementation must be provided for both `std::vector` and `std::list`, and the program must report and compare their execution times.
*   **Implementation:**
    *   The Ford-Johnson algorithm is implemented as a series of steps, adapted for both container types:
        1.  **Pairing:** The input sequence is grouped into pairs of numbers. Within each pair, the elements are sorted. Any leftover element (if the total count is odd) is set aside as a "straggler".
        2.  **Recursive Sort:** The pairs are recursively sorted based on their larger element using a standard merge sort. This creates a sorted main chain of the larger elements.
        3.  **Main Chain & Pending Elements:** The main chain is constructed by first inserting the smaller element of the very first pair, followed by all the larger elements from the sorted pairs. The remaining smaller elements and the straggler form a sequence of "pending" elements to be inserted.
        4.  **Jacobsthal Insertion Order:** To minimize comparisons, the pending elements are not inserted in sequence. Instead, an insertion order is generated based on **Jacobsthal numbers**. This sequence prioritizes inserting elements that are further apart first, which optimally narrows down the search space for subsequent insertions.
        5.  **Binary Search Insertion:** Each pending element is inserted into the sorted main chain using `std::lower_bound` to perform an efficient binary search for the correct insertion point.
    *   **Container Comparison:**
        *   **`std::vector`:** Benefits from fast random access, making `std::lower_bound` and accessing elements by index (from the Jacobsthal sequence) very efficient. However, each `insert()` operation can be costly (O(n)) as it may require shifting many subsequent elements.
        *   **`std::list`:** Suffers from slow sequential access (`std::advance` is O(n)), which makes the binary search traversal slower than in a vector. However, its `insert()` operation is extremely fast (O(1)) once the position is found, as it only involves rearranging pointers.
    *   A `Timer` class using `<chrono>` is used to precisely measure and report the time taken by each container to sort the same input, demonstrating the practical performance differences.
*   **Key Concepts:** Ford-Johnson algorithm, merge-insertion sort, Jacobsthal numbers, `std::vector` vs. `std::list` performance trade-offs, iterator categories, `std::lower_bound`, and performance measurement with `std::chrono`.

## How to Compile and Run

Each exercise is in its own directory and can be compiled independently using the provided `Makefile`.

1.  Navigate to the specific exercise directory (e.g., `ex00`, `ex01`, `ex02`).
    ```bash
    cd ex00
    ```
2.  Use `make` to compile the project. This will create an executable with the same name as the directory (`btc`, `RPN`, `PmergeMe`).
    ```bash
    make
    ```
3.  Run the executable with the required arguments.
    ```bash
    # Example for ex00
    ./btc input.txt

    # Example for ex01
    ./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"

    # Example for ex02
    ./PmergeMe 3 5 9 7 4
    ```

**Other Make Commands:**

*   `make clean`: Removes the object files and dependency files.
*   `make fclean`: Removes all compiled files, including the final executable.
*   `make re`: Re-compiles the project from scratch (`fclean` + `all`).
