#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <cstring>
#include <exception>
#include <iostream>
#include <list>
#include <string>
#include <vector>

class PmergeMe
{
  public:
    // OCF
    PmergeMe()                                   = default;
    PmergeMe( const PmergeMe& other )            = default;
    PmergeMe& operator=( const PmergeMe& other ) = default;
    ~PmergeMe()                                  = default;

    // Key member functions
    static std::vector<int> sortVector( char** argv );
    static std::list<int>   sortList( char** argv );

    // Exception class
    class InvalidArgument : public std::exception
    {
      public:
        InvalidArgument( std::string_view error );
        const char* what() const noexcept override;

      private:
        const std::string m_error{};
    };
};

// To convert argv into std::vector or std::list
template <typename T>
T parseArgs( char** argv )
{
    T container;

    try
    {
        std::size_t remaining_pos;

        for ( int i{ 1 }; argv[i]; ++i )
        {
            container.push_back( std::stoi( argv[i], &remaining_pos ) );

            // The arguments must be fully converted to integers to be considered valid
            if ( remaining_pos != std::strlen( argv[i] ) )
                throw std::exception();
        }
    }
    catch ( const std::exception& )
    {
        throw PmergeMe::InvalidArgument( "The list of numbers provided as arguments is not valid." );
    }

    return container;
}

#endif /* PMERGEME_HPP */
