#include "RPN.hpp"
#include <iostream>

int main( int argc, char** argv )
{
    if ( argc != 2 )
    {
        std::cerr << "Usage: ./RPN <expression to evaluate>" << '\n';
        return 1;
    }

    try
    {
        RPN  rpn;
        auto result{ rpn.evaluate( argv[1] ) };
        std::cout << result << '\n';
    }
    catch ( const std::exception& e )
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
