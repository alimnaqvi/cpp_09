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
    int i {1};

    for ( std::string line; std::getline( input_stream, line ); )
    {
        trimWhitespace( line );

        auto comma_pos{ line.find( ',' ) };
        if ( comma_pos == std::string::npos )
            throw BadDatabaseFormat( "Invalid database CSV file. Error looking for comma on line " + std::to_string(i) + ": `" + line + '`' );

        // Skip header row
        if (i == 1)
            continue;

        auto date{ line.substr( 0, comma_pos ) };
        trimWhitespace( date );
        if ( !isValidDate( date ) )
            throw BadDatabaseFormat( "Invalid database CSV file. Invalid date on line " + std::to_string(i) + ": `" + line + '`' );

        std::size_t remaining_pos{};
        try
        {
            float price{ std::stof( line.substr( comma_pos + 1 ), &remaining_pos ) };
            m_btc_database[date] = price;
        }
        catch ( const std::exception& )
        {
            throw BadDatabaseFormat( "Invalid database CSV file. Error converting value to float on line " + std::to_string(i) + ": `" + line +
                                     '`' );
        }

        if ( remaining_pos != line.length() )
            throw BadDatabaseFormat( "Invalid database CSV file. Invalid (extra) data on line " + std::to_string(i) + ": `" + line + '`' );

        i++;
    }
}

// Get price on closest lower date. Throw InvalidDate exception on error
float BitcoinExchange::getPriceOnDate( const std::string& date )
{
    if ( !isValidDate( date ) )
        throw InvalidDate( date + " is not a valid date" );

    if ( m_btc_database.find( date ) != m_btc_database.end() )
        return m_btc_database[date];
    else
    {
        if ( date < m_btc_database.begin()->first )
            throw InvalidDate( date + " is before the earliest date in the datebase" );

        for ( auto it{ m_btc_database.begin() }; it != m_btc_database.end(); it++ )
        {
            auto next{ std::next( it ) };
            if ( next == m_btc_database.end() || next->first > date )
                return m_btc_database[it->first];
        }

        throw InvalidDate( "A strange error occurred when retrieving price closest to " + date );
    }
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

// Helper functions

bool isValidDate( const std::string& date )
{
    if ( date.length() != 10 )
        return false;

    std::size_t remaining_pos;
    try
    {
        [[maybe_unused]] auto year{ std::stoi( date.substr( 0, 4 ), &remaining_pos ) };
        if ( date[remaining_pos] != '-' )
            return false;
        auto month{ std::stoi( date.substr( remaining_pos + 1, 2 ), &remaining_pos ) };
        if ( date[remaining_pos] != '-' || month < 1 || month > 12 )
            return false;
        auto day{ std::stoi( date.substr( remaining_pos + 1, 2 ), &remaining_pos ) };
        if ( remaining_pos != date.length() || day < 1 || day > 31 )
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
