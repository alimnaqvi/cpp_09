#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange( const BitcoinExchange& other )
    : m_btc_database{ other.m_btc_database }
{
}

BitcoinExchange& BitcoinExchange::operator=( const BitcoinExchange& other )
{
    if ( this != &other )
        m_btc_database = other.m_btc_database;

    return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

// Constructor to initialize the database with an input csv; DatabaseFormatError exception on error
BitcoinExchange::BitcoinExchange( std::istream& input_stream )
{
    int line_num{ 0 };

    for ( std::string line; std::getline( input_stream, line ); )
    {
        // Current line number
        ++line_num;

        trimWhitespace( line );

        // Look for comma
        auto comma_pos{ line.find( ',' ) };
        if ( comma_pos == std::string::npos )
            throw BadDatabaseFormat( "Invalid database CSV file. Error looking for comma on line " +
                                     std::to_string( line_num ) + ": `" + line + '`' );

        // Extract date (substring before comma)
        auto date{ line.substr( 0, comma_pos ) };
        trimWhitespace( date );

        // Extract price/exchange_rate (substring after comma)
        auto price_str{ line.substr( comma_pos + 1 ) };
        trimWhitespace( price_str );

        // Skip header row
        if ( line_num == 1 && date == "date" && price_str == "exchange_rate" )
            continue;

        // Check if date is valid
        if ( !isValidDate( date ) )
            throw BadDatabaseFormat( "Invalid database CSV file. Invalid date on line " + std::to_string( line_num ) +
                                     ": `" + line + '`' );

        // Check if price/exchange_rate is valid and add to map
        std::size_t remaining_pos{};
        try
        {
            float price{ std::stof( price_str, &remaining_pos ) };
            m_btc_database[date] = price;
        }
        catch ( const std::exception& )
        {
            throw BadDatabaseFormat( "Invalid database CSV file. Error converting value to float on line " +
                                     std::to_string( line_num ) + ": `" + line + '`' );
        }

        if ( remaining_pos != price_str.length() )
            throw BadDatabaseFormat( "Invalid database CSV file. Invalid (extra) data on line " +
                                     std::to_string( line_num ) + ": `" + line + '`' );
    }
}

// Get price on closest lower date. Throw InvalidDate exception on error
float BitcoinExchange::getPriceOnDate( const std::string& date ) const
{
    if ( !isValidDate( date ) )
        throw InvalidDate( date + " is not a valid date" );

    if ( m_btc_database.empty() )
        throw RetrievalError( "Database is empty" );

    // Find the first element with key not less than `date`
    auto it{ m_btc_database.lower_bound( date ) };

    // Exact match has been found
    if ( it != m_btc_database.end() && it->first == date )
        return it->second;

    // Earlier date is not available
    if ( it == m_btc_database.begin() )
        throw InvalidDate( date + " is before the earliest date in the datebase" );

    // Go to previous date (map is ordered)
    --it;
    return it->second;
}

// Exception classes

BitcoinExchange::BadDatabaseFormat::BadDatabaseFormat( std::string_view error )
    : m_error{ error }
{
}

const char* BitcoinExchange::BadDatabaseFormat::what() const noexcept
{
    return m_error.c_str();
}

BitcoinExchange::InvalidDate::InvalidDate( std::string_view error )
    : m_error{ error }
{
}

const char* BitcoinExchange::InvalidDate::what() const noexcept
{
    return m_error.c_str();
}

BitcoinExchange::RetrievalError::RetrievalError( std::string_view error )
    : m_error{ error }
{
}

const char* BitcoinExchange::RetrievalError::what() const noexcept
{
    return m_error.c_str();
}

// Helper functions

bool isValidDate( const std::string& date )
{
    if ( date.length() != 10 )
        return false;

    std::size_t remaining_pos;
    try
    {
        // First 4 characters in the string must be numbers (year), followed by '-'
        [[maybe_unused]] auto year{ std::stoi( date.substr( 0, 4 ), &remaining_pos ) };
        if ( date[remaining_pos] != '-' )
            return false;

        // Next 2 characters in the string must be numbers (month), followed by '-'
        auto date_chopped_year{ date.substr( remaining_pos + 1 ) };
        auto month{ std::stoi( date_chopped_year, &remaining_pos ) };
        if ( date_chopped_year[remaining_pos] != '-' || month < 1 || month > 12 )
            return false;

        // Next 2 characters in the string must be numbers (day), followed by nothing
        auto date_chopped_month{ date_chopped_year.substr( remaining_pos + 1 ) };
        auto day{ std::stoi( date_chopped_month, &remaining_pos ) };
        if ( remaining_pos != date_chopped_month.length() || day < 1 || day > 31 )
            return false;
    }
    catch ( const std::exception& )
    {
        return false;
    }

    return true;
}

void trimWhitespace( std::string& str, const std::string& whitespace )
{
    const auto firstNonSpace = str.find_first_not_of( whitespace );

    if ( firstNonSpace == std::string::npos )
    {
        str.clear();
        return;
    }

    const auto lastNonSpace = str.find_last_not_of( whitespace );
    const auto strLen       = lastNonSpace - firstNonSpace + 1;

    str = str.substr( firstNonSpace, strLen );
}
