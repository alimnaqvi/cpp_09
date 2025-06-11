#include "PmergeMe.hpp"

// Key member functions

std::vector<int> PmergeMe::sortVector( char** argv )
{
    std::vector vec{ parseArgs<std::vector<int>>( argv ) };

    // Sorting logic

    if ( std::is_sorted( vec.begin(), vec.end() ) )
        return vec;

    std::vector<Pair> pairs_vec{ createSortedPairs( vec ) };

    pairs_vec = mergeSortPairs( pairs_vec );

    // Insertion (don't forget the left out odd)

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

std::vector<Pair> createSortedPairs( const std::vector<int>& vec )
{
    std::vector<Pair> pairs_vec;

    for ( auto it{ vec.begin() }; it + 1 != vec.end(); ++it )
    {
        pairs_vec.emplace_back( std::max( *it, *( it + 1 ) ), std::min( *it, *( it + 1 ) ) );
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
