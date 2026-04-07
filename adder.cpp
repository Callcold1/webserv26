#include <iostream>
#include <sstream>

int main(int argc, char** argv){

    if (argc != 2)
        std::cout << "Invalid Number of Arguments";
    int one = 0;
    int two = 0;

    std::istringstream ss(argv[1]);
    if (ss >> one && ss >> two && ss.eof())
        std::cout << one + two << std::endl;
    else
        std::cout << "Invalid input parameters";
}