#include "PmergeMe.hpp"

// Main sorting logic with std::vector
std::vector<int> PmergeMe::sortVector( int argc, char** argv )
{
    std::vector vec{ parseArgsVec( argc, argv ) };

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
    std::vector<std::size_t> order_vec{ createJacobsthalOrderVec( remaining_vec.size() ) };

    // Use binary search to insert the remaining numbers into correct positions in the main chain
    for ( auto idx : order_vec )
    {
        // std::lower_bound conducts the binary search for us
        auto insertion_pos{ std::lower_bound( sorted_vec.begin(), sorted_vec.end(), remaining_vec[idx] ) };

        sorted_vec.insert( insertion_pos, remaining_vec[idx] );
    }

    return sorted_vec;
}

// Main sorting logic with std::list
std::list<int> PmergeMe::sortList( int argc, char** argv )
{
    std::list lst{ parseArgsList( argc, argv ) };

    // Nothing to be done if already sorted
    if ( std::is_sorted( lst.begin(), lst.end() ) )
        return lst;

    // Create pairs (leave out the odd one for now)
    std::list<Pair> pairs_list{ createSortedPairs( lst ) };

    // Merge sort the pairs
    pairs_list = mergeSortPairs( pairs_list );

    // Insertion into the main chain
    std::list<int> sorted_list;

    // First element in main chain is the smaller of the first pair
    sorted_list.push_back( pairs_list.front().second );

    // The rest of main chain consists of all greaters of the pairs because they are already sorted
    for ( auto& elem : pairs_list )
        sorted_list.push_back( elem.first );

    // Create list of remaining elements
    std::list<int> remaining_list;
    for ( auto it{ std::next( pairs_list.begin() ) }; it != pairs_list.end(); ++it )
        remaining_list.push_back( ( *it ).second );
    // Don't forget the left out odd
    if ( lst.size() % 2 != 0 )
        remaining_list.push_back( lst.back() );

    // FJ algorithm uses Jacobsthal numbers to determine the order of insertion
    std::list<std::size_t> order_list{ createJacobsthalOrderList( remaining_list.size() ) };

    // Use binary search to insert the remaining numbers into correct positions in the main chain
    for ( auto idx : order_list )
    {
        // std::list does not have operator[], so we use iterator to access the index
        auto it{ remaining_list.begin() };
        // Equivalent to remaining_list[idx] (as it were)
        std::advance( it, idx );

        // std::lower_bound conducts the binary search for us
        auto insertion_pos{ std::lower_bound( sorted_list.begin(), sorted_list.end(), *it ) };

        sorted_list.insert( insertion_pos, *it );
    }

    return sorted_list;
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

/*----------------Helper functions for vector sorting----------------*/

// Convert argv into std::vector; throw on invalid input
std::vector<int> parseArgsVec( int argc, char** argv )
{
    std::vector<int> vec;
    vec.reserve( argc - 1 );

    try
    {
        std::size_t remaining_pos;

        for ( int i{ 1 }; argv[i]; ++i )
        {
            vec.push_back( std::stoi( argv[i], &remaining_pos ) );

            // The arguments must be fully converted to (positive) integers to be considered valid
            if ( remaining_pos != std::strlen( argv[i] ) || vec.back() < 0 )
                throw std::exception();
        }
    }
    catch ( const std::exception& )
    {
        throw PmergeMe::InvalidArgument( "The list of numbers provided as arguments is not valid." );
    }

    return vec;
}

// Create a vector of pairs, first element is always >= the second; last odd element is not paired
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

// Create a vector of indexes (0-based) based on Jacobsthal numbers, similar to FJ algorithm
std::vector<std::size_t> createJacobsthalOrderVec( std::size_t size )
{
    // We hardcode cases of smaller size for efficiency
    if ( size == 0 )
        return {};
    if ( size == 1 )
        return { 0 };
    if ( size == 2 )
        return { 1, 0 };

    // Create a sequence of Jacobsthal numbers
    std::vector<std::size_t> jacobsthal_vec{ 3, 5 };
    while ( true )
    {
        std::size_t last_elem{ jacobsthal_vec[jacobsthal_vec.size() - 1] };
        std::size_t second_last_elem{ jacobsthal_vec[jacobsthal_vec.size() - 2] };
        std::size_t next_elem{ last_elem + ( 2 * second_last_elem ) };
        // +1 because we will subtract an index later (first of the pairs has already been inserted)
        if ( next_elem > size + 1 )
            break;
        jacobsthal_vec.push_back( next_elem );
    }

    // Create another vector (result) that fills the gaps in Jacobsthal numbers
    std::vector<std::size_t> order_vec;
    order_vec.reserve( size );
    // Prev represents the previously added Jacobsthal number
    // (starts with 1, because first of the pairs has already been inserted)
    std::size_t prev{ 1 };

    for ( std::size_t i{ 0 }; i < jacobsthal_vec.size(); ++i )
    {
        // Insert current Jacobsthal number
        order_vec.push_back( jacobsthal_vec[i] );

        // Insert the remaining indexes between current and previous Jacobsthal number
        for ( std::size_t j{ jacobsthal_vec[i] - 1 }; j > prev; --j )
            order_vec.push_back( j );

        // Current becomes prev for the next loop
        prev = jacobsthal_vec[i];
    }

    // If the largest Jacobsthal number added is smaller than the required size, add the remainder
    if ( size > order_vec.size() )
    {
        // +1 because we will subtract an index later (first of the pairs has already been inserted)
        for ( std::size_t i{ size + 1 }; i > prev; --i )
            order_vec.push_back( i );
    }

    // (just to be sure) For smaller N, make sure no extra Jacobsthal numbers have been added
    while ( size < order_vec.size() )
        order_vec.pop_back();

    // Subtract 2 from each element (one for already inserted pair, another for 0 based index)
    for ( auto& elem : order_vec )
        elem -= 2;

    return order_vec;
}

/*----------------Helper functions for list sorting----------------*/

// Convert argv into std::list; throw on invalid input
std::list<int> parseArgsList( int argc, char** argv )
{
    std::list<int> lst;
    (void)argc;

    try
    {
        std::size_t remaining_pos;

        for ( int i{ 1 }; argv[i]; ++i )
        {
            lst.push_back( std::stoi( argv[i], &remaining_pos ) );

            // The arguments must be fully converted to (positive) integers to be considered valid
            if ( remaining_pos != std::strlen( argv[i] ) || lst.back() < 0 )
                throw std::exception();
        }
    }
    catch ( const std::exception& )
    {
        throw PmergeMe::InvalidArgument( "The list of numbers provided as arguments is not valid." );
    }

    return lst;
}

// Create a list of pairs, first element is always >= the second; last odd element is not paired
std::list<Pair> createSortedPairs( const std::list<int>& lst )
{
    std::list<Pair> pairs_list;

    for ( size_t i{ 1 }; i < lst.size(); i += 2 )
    {
        // std::list does not have operator[], so we use iterator to access the index
        auto prev{ std::next( lst.begin(), i - 1 ) };
        auto cur{ std::next( lst.begin(), i ) };
        pairs_list.emplace_back( std::max( *prev, *cur ), std::min( *prev, *cur ) );
    }

    return pairs_list;
}

// Helper function for mergeSortPairs function
std::list<Pair> merge( std::list<Pair>& lst1, std::list<Pair>& lst2 )
{
    std::list<Pair> lst;

    while ( !lst1.empty() && !lst2.empty() )
    {
        if ( lst1.front().first > lst2.front().first )
        {
            lst.emplace_back( lst2.front() );
            lst2.erase( lst2.begin() );
        }
        else
        {
            lst.emplace_back( lst1.front() );
            lst1.erase( lst1.begin() );
        }
    }

    while ( !lst1.empty() )
    {
        lst.emplace_back( lst1.front() );
        lst1.erase( lst1.begin() );
    }

    while ( !lst2.empty() )
    {
        lst.emplace_back( lst2.front() );
        lst2.erase( lst2.begin() );
    }

    return lst;
}

// Use merge sort to sort pair_list according to their larger (first) element
std::list<Pair> mergeSortPairs( const std::list<Pair>& pairs_list )
{
    if ( pairs_list.size() == 1 )
        return pairs_list;

    auto mid_point_it{ pairs_list.begin() };
    std::advance( mid_point_it, pairs_list.size() / 2 );

    std::list<Pair> list_half_1{ pairs_list.begin(), mid_point_it };
    std::list<Pair> list_half_2{ mid_point_it, pairs_list.end() };

    list_half_1 = mergeSortPairs( list_half_1 );
    list_half_2 = mergeSortPairs( list_half_2 );

    return merge( list_half_1, list_half_2 );
}

// Create a list of indexes (0-based) based on Jacobsthal numbers, similar to FJ algorithm
std::list<std::size_t> createJacobsthalOrderList( std::size_t size )
{
    // We hardcode cases of smaller size for efficiency
    if ( size == 0 )
        return {};
    if ( size == 1 )
        return { 0 };
    if ( size == 2 )
        return { 1, 0 };

    // Create a sequence of Jacobsthal numbers
    std::list<std::size_t> jacobsthal_list{ 3, 5 };
    while ( true )
    {
        std::size_t last_elem{ jacobsthal_list.back() };
        std::size_t second_last_elem{ *( std::prev( jacobsthal_list.end(), 2 ) ) };
        std::size_t next_elem{ last_elem + ( 2 * second_last_elem ) };
        // +1 because we will subtract an index later (first of the pairs has already been inserted)
        if ( next_elem > size + 1 )
            break;
        jacobsthal_list.push_back( next_elem );
    }

    // Create another list (result) that fills the gaps in Jacobsthal numbers
    std::list<std::size_t> order_list;
    // Prev represents the previously added Jacobsthal number
    // (starts with 1, because first of the pairs has already been inserted)
    std::size_t prev{ 1 };

    for ( auto cur : jacobsthal_list )
    {
        // Insert current Jacobsthal number
        order_list.push_back( cur );

        // Insert the remaining indexes between current and previous Jacobsthal number
        for ( std::size_t j{ cur - 1 }; j > prev; --j )
            order_list.push_back( j );

        // Current becomes prev for the next loop
        prev = cur;
    }

    // If the largest Jacobsthal number added is smaller than the required size, add the remainder
    if ( size > order_list.size() )
    {
        // +1 because we will subtract an index later (first of the pairs has already been inserted)
        for ( std::size_t i{ size + 1 }; i > prev; --i )
            order_list.push_back( i );
    }

    // (just to be sure) For smaller N, make sure no extra Jacobsthal numbers have been added
    while ( size < order_list.size() )
        order_list.pop_back();

    // Subtract 2 from each element (one for already inserted pair, another for 0 based index)
    for ( auto& elem : order_list )
        elem -= 2;

    return order_list;
}
