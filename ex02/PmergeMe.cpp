#include "PmergeMe.hpp"

// Sorting with std::vector

std::vector<int> PmergeMe::sortVector( char** argv )
{
    std::vector vec{ parseArgs<std::vector<int>>( argv ) };

    // Sorting logic

    // Nothing to be done if already sorted
    if ( std::is_sorted( vec.begin(), vec.end() ) )
        return vec;

    // Create pairs (leave out the odd one for now)
    std::vector<Pair> pairs_vec{ createSortedPairs( vec ) };

    // Merge sort the pairs
    pairs_vec = mergeSortPairs( pairs_vec );

    // Insertion into the main chain
    std::vector<int> sorted_vec;
    sorted_vec.reserve( vec.size() );

    // First element in main chain is the smaller of the first pair
    sorted_vec.push_back( pairs_vec[0].second );

    // The rest of main chain consists of all greaters of the pairs because they are already sorted
    for ( auto& elem : pairs_vec )
        sorted_vec.push_back( elem.first );

    // Create vector of remaining elements
    std::vector<int> remaining_vec;
    remaining_vec.reserve( pairs_vec.size() );
    for ( auto it{ pairs_vec.begin() + 1 }; it != pairs_vec.end(); ++it )
        remaining_vec.push_back( ( *it ).second );
    // Don't forget the left out odd
    if ( vec.size() % 2 != 0 )
        remaining_vec.push_back( vec.back() );

    // FJ algorithm uses Jacobsthal numbers to determine the order of insertion
    std::vector<std::size_t> order_vec{ createJacobsthalOrder( remaining_vec.size() ) };

    // Use binary search to insert the remaining numbers into correct positions in the main chain
    for ( auto idx : order_vec )
    {
        // std::lower_bound conducts the binary search for us
        auto insertion_pos{ std::lower_bound( sorted_vec.begin(), sorted_vec.end(), remaining_vec[idx] ) };

        sorted_vec.insert( insertion_pos, remaining_vec[idx] );
    }

    return sorted_vec;
}

// Sorting with std::list

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

// Helper functions for vector sorting

std::vector<Pair> createSortedPairs( const std::vector<int>& vec )
{
    std::vector<Pair> pairs_vec;

    for ( size_t i{ 1 }; i < vec.size(); i += 2 )
    {
        pairs_vec.emplace_back( std::max( vec[i - 1], vec[i] ), std::min( vec[i - 1], vec[i] ) );
    }

    return pairs_vec;
}

// Helper function for mergeSortPairs function
std::vector<Pair> merge( std::vector<Pair>& vec1, std::vector<Pair>& vec2 )
{
    std::vector<Pair> vec;
    vec.reserve( vec1.size() + vec2.size() );

    while ( !vec1.empty() && !vec2.empty() )
    {
        if ( vec1[0].first > vec2[0].first )
        {
            vec.emplace_back( vec2[0] );
            vec2.erase( vec2.begin() );
        }
        else
        {
            vec.emplace_back( vec1[0] );
            vec1.erase( vec1.begin() );
        }
    }

    while ( !vec1.empty() )
    {
        vec.emplace_back( vec1[0] );
        vec1.erase( vec1.begin() );
    }

    while ( !vec2.empty() )
    {
        vec.emplace_back( vec2[0] );
        vec2.erase( vec2.begin() );
    }

    return vec;
}

// Use merge sort to sort pair_vec according to their larger (first) element
std::vector<Pair> mergeSortPairs( const std::vector<Pair>& pairs_vec )
{
    if ( pairs_vec.size() == 1 )
        return pairs_vec;

    auto mid_point_it{ pairs_vec.begin() + ( pairs_vec.size() / 2 ) };

    std::vector<Pair> vec_half_1{ pairs_vec.begin(), mid_point_it };
    std::vector<Pair> vec_half_2{ mid_point_it, pairs_vec.end() };

    vec_half_1 = mergeSortPairs( vec_half_1 );
    vec_half_2 = mergeSortPairs( vec_half_2 );

    return merge( vec_half_1, vec_half_2 );
}

// Create a vector of indexes based on Jacobsthal numbers, similar to FJ algorithm
std::vector<std::size_t> createJacobsthalOrder(std::size_t size)
{
    std::vector<std::size_t> jacobsthal_vec{3, 5};
    // Create a sequence of Jacobsthal numbers
    while (true)
    {
        std::size_t next {jacobsthal_vec[jacobsthal_vec.size() - 1] + (2 * jacobsthal_vec[jacobsthal_vec.size() - 2])};
        // +1 because we will subtract an index later (first of the pairs has already been inserted)
        if (next > size + 1)
            break;
        jacobsthal_vec.push_back(next);
    }
    
    // Create another vector (result) that fills the gaps in Jacobsthal numbers
    std::vector<std::size_t> order_vec;
    order_vec.reserve(size);
    // Prev represents the previously added Jacobsthal number
    // (starts with 1, because first of the pairs has already been inserted)
    std::size_t prev{1};

    for (std::size_t i {0}; i < jacobsthal_vec.size(); ++i)
    {
        // Insert current Jacobsthal number
        order_vec.push_back(jacobsthal_vec[i]);

        // Insert the remaining indexes between current and previous Jacobsthal number
        for (std::size_t j {jacobsthal_vec[i] - 1}; j > prev; --j)
            order_vec.push_back(j);
        
        // Current becomes prev for the next loop
        prev = jacobsthal_vec[i];
    }
    
    // If the largest Jacobsthal number added is smaller than the required size, add the remainder
    if (size > order_vec.size())
    {
        // +1 because we will subtract an index later (first of the pairs has already been inserted)
        for (std::size_t i {size + 1}; i > prev; --i)
            order_vec.push_back(i);
    }

    // Subtract 2 from each element (one for already inserted pair, another for 0 based index)
    for (auto& elem: order_vec)
        elem -= 2;

    return order_vec;
}
