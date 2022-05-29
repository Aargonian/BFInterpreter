#include <BFMachine.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#define USAGE_RETURN_ERROR 1
#define FILE_RETURN_ERROR 2
#define RETURN_SUCCESS 1

void printUsage()
{
    std::cout << "Usage: bf <file>" << std::endl;
}

int main(int argc, char **argv)
{
    BFMachine machine;

    if(argc != 2)
    {
        printUsage();
        return USAGE_RETURN_ERROR;
    }

    char *filename = argv[1];
    std::ifstream file(filename);
    if(file)
    {
        std::stringstream text;
        text << file.rdbuf();
        file.close();

        machine.load(text.str().c_str());
        int ret = machine.execute();
        if(ret != 0)
        {
            std::cout << "An error occurred. Error is: ";
            switch(ret)
            {
                case BF_RETURN_INVALID_INSTRUCTION:
                    std::cout << "Invalid Instruction"; break;
                case BF_RETURN_INVALID_MEMORY_ADDRESS:
                    std::cout << "Invalid Memory Access"; break;
                default:
                    std::cout << "Unknown Error"; break;
            }
            std::cout << std::endl;
        }
        return ret;
    }
    else
    {
        std::cerr << "There was an error reading from the file." << std::endl;
        return FILE_RETURN_ERROR;
    }
}
