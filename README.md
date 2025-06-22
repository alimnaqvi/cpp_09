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

## Learning Resources and References
- [std::map](https://en.cppreference.com/w/cpp/container/map.html)
- [std::stack](https://en.cppreference.com/w/cpp/container/stack.html)
- [std::vector](https://en.cppreference.com/w/cpp/container/vector.html)
- [std::list](https://en.cppreference.com/w/cpp/container/list.html)
- [Algorithms library](https://cppreference.com/w/cpp/algorithm.html)
- [Iterators and their usage](https://www.studyplan.dev/pro-cpp/iterators)
- [On the Average Case of MergeInsertion](https://link.springer.com/article/10.1007/s00224-020-09987-4) (read section 2.1: Description of MergeInsertion)
- [Merge Insertion Sort / Ford-Johnson Algorithm](https://github.com/decidedlyso/merge-insertion-sort)
  - Note that the example used in this source is for sorting in descending order; but the same concepts apply to sorting in ascending order
