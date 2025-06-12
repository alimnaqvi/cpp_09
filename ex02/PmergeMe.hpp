#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <algorithm>
#include <chrono>  // std::chrono functions in Timer
#include <cstring> // std::strlen
#include <exception>
#include <iomanip> // std::fixed, std::setprecision
#include <iostream>
#include <list>
#include <string>
#include <utility> // std::pair
#include <vector>

using Pair = std::pair<int, int>;

class PmergeMe
{
  public:
    // OCF
    PmergeMe()                                   = default;
    PmergeMe( const PmergeMe& other )            = default;
    PmergeMe& operator=( const PmergeMe& other ) = default;
    ~PmergeMe()                                  = default;

    // Key member functions
    static std::vector<int> sortVector( int argc, char** argv );
    static std::list<int>   sortList( int argc, char** argv );

    // Exception class
    class InvalidArgument : public std::exception
    {
      public:
        InvalidArgument( std::string_view error );
        const char* what() const noexcept override;

      private:
        const std::string m_error{};
    };

    // Timer class
    class Timer
    {
      private:
        // Type aliases to make accessing nested type easier
        using Clock       = std::chrono::steady_clock;
        using MicroSecond = std::chrono::duration<double, std::ratio<1, 1000000>>;

        std::chrono::time_point<Clock> m_beginning{ Clock::now() };

      public:
        void reset()
        {
            m_beginning = Clock::now();
        }

        double elapsed() const
        {
            return std::chrono::duration_cast<MicroSecond>( Clock::now() - m_beginning ).count();
        }
    };
};

/*----------------Helper functions for vector sorting----------------*/

// Convert argv into std::vector; throw on invalid input
std::vector<int> parseArgsVec( int argc, char** argv );

// Create a vector of pairs, first element is always >= the second; last odd element is not paired
std::vector<Pair> createSortedPairs( const std::vector<int>& vec );

// Use merge sort to sort pair_vec according to their larger (first) element
std::vector<Pair> mergeSortPairs( const std::vector<Pair>& pairs_vec );

// Create a vector of indexes (0-based) based on Jacobsthal numbers, similar to FJ algorithm
std::vector<std::size_t> createJacobsthalOrderVec(std::size_t size);

/*----------------Helper functions for list sorting----------------*/

// Convert argv into std::vector; throw on invalid input
std::list<int> parseArgsList( int argc, char** argv );

// Create a list of pairs, first element is always >= the second; last odd element is not paired
std::list<Pair> createSortedPairs( const std::list<int>& lst );

// Use merge sort to sort pair_vec according to their larger (first) element
std::list<Pair> mergeSortPairs( const std::list<Pair>& pairs_vec );

// Create a list of indexes (0-based) based on Jacobsthal numbers, similar to FJ algorithm
std::list<std::size_t> createJacobsthalOrderList(std::size_t size);

#endif /* PMERGEME_HPP */
