#include "BitcoinExchange.hpp"
#include <fstream>

int main()
{
    try
    {
        std::ifstream in_file{ "./data.csv" };
        if ( !in_file )
        {
            std::cout << "Could not open database file" << '\n';
            return 1;
        }
        BitcoinExchange btc{in_file};

        std::string date{"2011-01-01"};
        std::cout << "Price on " << date << " is: " << btc.getPriceOnDate(date) << '\n';
    }
    catch ( const std::exception& e )
    {
        std::cerr << "An exception occurred: " << e.what() << '\n';
    }

    return 0;
}
