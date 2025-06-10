#include "PmergeMe.hpp"

// PmergeMe::PmergeMe()
// {
// }

// PmergeMe::PmergeMe(const PmergeMe& other)
// {
// }

// PmergeMe& PmergeMe::operator=(const PmergeMe& other)
// {
//     return *this;
// }

// PmergeMe::~PmergeMe()
// {
// }

// Key member functions

std::vector<int> PmergeMe::sortVector( char** argv )
{
    std::vector vec{ parseArgs<std::vector<int>>( argv ) };

    // Sorting logic

    return vec;
}

std::list<int> PmergeMe::sortList( char** argv )
{
    std::list lst{ parseArgs<std::list<int>>( argv ) };

    // Sorting logic

    return lst;
}

// Exception class

PmergeMe::InvalidArgument::InvalidArgument( std::string_view error )
    : m_error{ error }
{
}

const char* PmergeMe::InvalidArgument::what() const noexcept
{
    return m_error.c_str();
}
