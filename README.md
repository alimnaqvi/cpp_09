# C++ Module 09: STL

This project is part of the curriculum at 42 school. The objective of this module is to deepen understanding of the C++ standard template library. Each of the 3 exercises must use a different container, and exercise 02 must use two different containers. So a minimum of 4 different containers must be used throughout the module. The choice of containers is free. I use std::map for exercise 00, std::stack (based on std::deque) for exercise 01, and std::vector and std::list for exercise 02.

Core concepts learned and applied:
- Exercise 00: Bitcoin Exchange
  - Using map
  - Iterating over map elements and accessing key and value
  - Usage of subscript operator for map and invalidation of its iterator
  - Map member functions such as lower_bound()
- Exercise 01: Reverse Polish Notation
  - Coming up with a stack-based algorithm to evaluate RPN expressions
- Exercise 02: PmergeMe
  - Implement the complex Ford–Johnson merge-insertion sort algorithm in C++ using two different containers
  - Differences and similarities between std::vector and std::list
  - Comparing efficiency of std::vector and std::list, especially trade-offs between random access and costly insertion
  - Random access iterators vs. forward/bidirectional iterators
  - Advancing iterators with operators vs. functions (e.g., it++ vs. std::advance(it, 1))
