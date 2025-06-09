#include "BitcoinExchange.hpp"
#include <iomanip>

void generateResults( const BitcoinExchange& btc, std::ifstream& input_file )
{
    int line_num{ 0 };

    for ( std::string line; std::getline( input_file, line ); )
    {
        // Current line number
        ++line_num;

        trimWhitespace( line );

        // Look for pipe
        auto pipe_pos{ line.find( '|' ) };
        if ( pipe_pos == std::string::npos )
        {
            std::cout << "Error looking for pipe (|) on line " << line_num << ": `" << line << "`\n";
            continue;
        }

        // Extract date (substring before pipe)
        auto date{ line.substr( 0, pipe_pos ) };
        trimWhitespace( date );

        // Extract value / btc amount (substring after pipe)
        auto amount_str{ line.substr( pipe_pos + 1 ) };
        trimWhitespace( amount_str );

        // Skip header row
        if ( line_num == 1 && date == "date" && amount_str == "value" )
            continue;

        // Check if date is valid
        if ( !isValidDate( date ) )
        {
            std::cerr << "Error: Invalid date on line " << line_num << ": `" << line << "`\n";
            continue;
        }

        // Check if value / btc amount is valid
        std::size_t remaining_pos{};
        float       amount;
        try
        {
            amount = std::stof( amount_str, &remaining_pos );
        }
        catch ( const std::exception& )
        {
            std::cerr << "Invalid input file. Error converting value to float on line " << line_num << ": `" << line
                      << "`\n";
            continue;
        }

        if ( remaining_pos != amount_str.length() )
        {
            std::cerr << "Error: Invalid (extra) data on line " << line_num << ": `" << line << "`\n";
            continue;
        }

        // Should be between 0 and 1000
        if ( amount < 0 || amount > 1000 )
        {
            std::cerr << "Error: Value must be between 0 and 1000: Line " << line_num << ": `" << line << "`\n";
            continue;
        }

        // Print output based on datebase exchange rates
        try
        {
            auto price_on_date{ amount * btc.getPriceOnDate( date ) };
            std::cout << "Date:\t" << date << "\t|\tbtc amount:\t" << amount << "\t|\tPrice of amount:\t" << std::fixed
                      << std::setprecision( 2 ) << price_on_date << '\n';
        }
        catch ( const std::exception& e )
        {
            std::cerr << "Error: " << e.what() << ": Line " << line_num << ": `" << line << "`\n";
        }
    }
}

int main( int argc, char** argv )
{
    if ( argc != 2 )
    {
        std::cerr << "Usage: ./btc <input_file>" << '\n';
        return 1;
    }

    try
    {
        // Open database csv and instantiate BitcoinExchange
        std::ifstream db_file{ "./data.csv" };
        if ( !db_file )
        {
            std::cerr << "Error: Could not open database file: " << "./data.csv" << '\n';
            return 1;
        }
        BitcoinExchange btc{ db_file };

        // Open input file
        std::ifstream input_file{ argv[1] };
        if ( !input_file )
        {
            std::cerr << "Error: Could not open input file: " << argv[1] << '\n';
            return 1;
        }

        // Read input file line by line and print btc value on matching date
        generateResults( btc, input_file );
    }
    catch ( const std::exception& e )
    {
        std::cerr << "An exception occurred: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
