#include "RPN.hpp"

RPN::RPN()
{
}

RPN::RPN( const RPN& other )
    : m_nums{ other.m_nums }
{
}

RPN& RPN::operator=( const RPN& other )
{
    if ( this != &other )
        m_nums = other.m_nums;

    return *this;
}

RPN::~RPN()
{
}

int performOperation( int first, int second, char operator_char )
{
    if ( operator_char == '+' )
        return first + second;
    if ( operator_char == '-' )
        return first - second;
    if ( operator_char == '*' )
        return first * second;
    if ( operator_char == '/' )
    {
        if ( second == 0 )
            throw RPN::DivisionByZero( "Cannot perform division by zero: " + std::to_string( first ) + '/' +
                                       std::to_string( second ) );
        return first / second;
    }
    // Should never be reached but just in case
    throw RPN::InvalidExpression( "Operator is not valid" );
}

// #include <iostream>
// #include <vector>
// #include <algorithm>
// void printStack( std::stack<int> stack )
// {
//     std::vector<int> vec;
//     vec.reserve(stack.size());

//     for (auto i {stack.size()}; i > 0; --i)
//     {
//         vec.push_back(stack.top());
//         stack.pop();
//     }

//     std::reverse(vec.begin(), vec.end());

//     std::cout << "{ ";
//     for ( const auto& elem : vec )
//         std::cout << elem << ' ';
//     std::cout << "}\n";
// }

int RPN::evaluate( const std::string& expression )
{
    std::string cur_expression{ expression };

    while ( true )
    {
        std::size_t remaining_pos{ 0 };
        try
        {
            m_nums.push( std::stoi( cur_expression, &remaining_pos ) );

            // Erase used number and trailing whitespace
            while ( remaining_pos < cur_expression.length() && isSpace( cur_expression[remaining_pos] ) )
                ++remaining_pos;
            cur_expression.erase( 0, remaining_pos );

            // Exit loop if expression string is exhausted
            if ( cur_expression.empty() )
                break;
        }
        catch ( const std::exception& )
        {
            // Skip whitespace
            while ( remaining_pos < cur_expression.length() && isSpace( cur_expression[remaining_pos] ) )
                ++remaining_pos;

            // if a valid operator is present, evaluate and replace stack
            if ( remaining_pos < cur_expression.length() && isOperator( cur_expression[remaining_pos] ) )
            {
                // Stack must have at least two numbers to perform any operation
                if ( m_nums.size() < 2 )
                    throw InvalidExpression(
                        "Invalid expression (operation cannot be performed on fewer than 2 numbers): " + expression );

                // Get the last 2 numbers in the stack to perform operation on
                int second{ m_nums.top() };
                m_nums.pop();
                int first{ m_nums.top() };
                m_nums.pop();

                // Perform operation
                m_nums.push( performOperation( first, second, cur_expression[remaining_pos] ) );

                // Erase used operator and trailing whitespace
                ++remaining_pos;
                while ( remaining_pos < cur_expression.length() && isSpace( cur_expression[remaining_pos] ) )
                    ++remaining_pos;
                cur_expression.erase( 0, remaining_pos );

                // Exit loop if expression string is exhausted
                if ( cur_expression.empty() )
                    break;
            }
            else if ( remaining_pos == cur_expression.length() )
                break;
            else
                throw InvalidExpression( "Invalid expression (unknown operator / element found): " + expression );
        }
    }

    // Nothing should be remaining in the cur_expression but just to be sure
    if ( !cur_expression.empty() )
        throw InvalidExpression( "Something went wrong (unevaluated string) while evaluating expression: " +
                                 expression );

    // Stack size should be 1 and that is our result
    if ( m_nums.size() != 1 )
        throw InvalidExpression( "Something went wrong (stack size) while evaluating expression: " + expression );

    int result{ m_nums.top() };
    m_nums.pop();

    return result;
}

// Exception classes

RPN::DivisionByZero::DivisionByZero( std::string_view error )
    : m_error{ error }
{
}

const char* RPN::DivisionByZero::what() const noexcept
{
    return m_error.c_str();
}

RPN::InvalidExpression::InvalidExpression( std::string_view error )
    : m_error{ error }
{
}

const char* RPN::InvalidExpression::what() const noexcept
{
    return m_error.c_str();
}

// Helper functions

bool isOperator( const char c )
{
    return ( c == '+' || c == '-' || c == '*' || c == '/' );
}

bool isSpace( const char c )
{
    return ( c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\r' || c == '\f' );
}
