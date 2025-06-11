#include "PmergeMe.hpp"
#include <chrono>
#include <iomanip>

int main( int argc, char** argv )
{
    if ( argc < 2 )
    {
        std::cout << "Usage: ./PmergeMe <a list of integers>" << '\n';
        return 1;
    }

    try
    {
        // Instantiating Timer class starts the timer
        PmergeMe::Timer timer;

        // Sort using std::vector
        std::vector vec{ PmergeMe::sortVector( argv ) };

        // Calculate time to sort using std::vector
        auto time_vec_sort{ timer.elapsed() };

        // Reset the timer
        timer.reset();

        // Sort using std::list
        std::list lst{ PmergeMe::sortList( argv ) };

        // Calculate time to sort using std::list
        auto time_list_sort{ timer.elapsed() };

        // Print unsorted numbers
        std::cout << "Before: ";
        for ( int i{ 1 }; argv[i]; ++i )
            std::cout << argv[i] << ' ';
        std::cout << '\n';

        // Print sorted numbers
        std::cout << "After: ";
        for ( const auto& elem : vec )
            std::cout << elem << ' ';
        std::cout << '\n';

        // Print times to sort
        std::cout << "Time to process a range of " << vec.size() << " elements with std::vector: " << std::fixed
                  << std::setprecision( 2 ) << time_vec_sort << " microseconds" << '\n';
        std::cout << "Time to process a range of " << lst.size() << " elements with std::list: " << std::fixed
                  << std::setprecision( 2 ) << time_list_sort << " microseconds" << '\n';
    }
    catch ( const std::exception& e )
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
