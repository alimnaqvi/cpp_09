#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <exception>
#include <string>

class RPN
{
  public:
    // OCF
    RPN();
    RPN( const RPN& other );
    RPN& operator=( const RPN& other );
    ~RPN();

    // Key member function
    int evaluate(const std::string& expression);

    // Exception classes

    class DivisionByZero : public std::exception
    {
      public:
        DivisionByZero( std::string_view error );
        const char* what() const noexcept override;

      private:
        const std::string m_error{};
    };

    class InvalidExpression : public std::exception
    {
      public:
        InvalidExpression( std::string_view error );
        const char* what() const noexcept override;

      private:
        const std::string m_error{};
    };

  private:
    std::stack<int> m_nums;
};

// Helper functions
bool isOperator(const char c);
bool isSpace(const char c);

#endif /* RPN_HPP */
