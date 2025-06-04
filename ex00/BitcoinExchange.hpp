#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <algorithm>
#include <exception>
#include <iostream>
#include <istream>
#include <map>
#include <string>

// class Date
// {
//   public:
//     // OCF
//     Date();
//     Date( const Date& other );
//     Date& operator=( const Date& other );
//     ~Date();

//     // Takes date string in Year-Month-Day format; InvalidDate exception otherwise
//     Date(std::string_view date_str);

//     // Getter
//     std::string getDateString();

//   private:
//     int m_year;
//     int m_month;
//     int m_day;
// };

class BitcoinExchange
{
  public:
    // OCF
    BitcoinExchange();
    BitcoinExchange( const BitcoinExchange& other );
    BitcoinExchange& operator=( const BitcoinExchange& other );
    ~BitcoinExchange();

    // Constructor to initialize the database with an input csv; DatabaseFormatError exception on error
    BitcoinExchange( std::istream& input_stream );

    // Get price on closest lower date. Throw InvalidDate exception on error
    float getPriceOnDate( const std::string& date );

    // Exception classes

    class BadDatabaseFormat : public std::exception
    {
      public:
        BadDatabaseFormat( std::string_view error );
        const char* what() const noexcept override;

      private:
        const std::string m_error{};
    };

    class InvalidDate : public std::exception
    {
      public:
        InvalidDate( std::string_view error );
        const char* what() const noexcept override;

      private:
        const std::string m_error{};
    };

    // class RetrievalError : public std::exception
    // {
    //   public:
    //     RetrievalError( std::string_view error );
    //     const char* what() const noexcept override;

    //   private:
    //     const std::string m_error{};
    // };

  private:
    std::map<std::string, float> m_btc_database;
};

// Helper functions
bool isValidDate( const std::string& date );
void trimWhitespace( std::string& str, const std::string& whitespace = " \t\n\r\f\v" );

#endif /* BITCOINEXCHANGE_HPP */
