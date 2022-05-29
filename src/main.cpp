#include <BFMachine.hpp>
#include <iostream>

int main()
{
    BFMachine machine;
    const char* instructions =
        "[ This program prints \"Hello World!\" and a newline to the screen, its\n"
        "  length is 106 active command characters. [It is not the shortest.]\n"
        "\n"
        "  This loop is an \"initial comment loop\", a simple way of adding a comment\n"
        "  to a BF program such that you don't have to worry about any command\n"
        "  characters. Any \".\", \",\", \"+\", \"-\", \"<\" and \">\" characters are simply\n"
        "  ignored, the \"[\" and \"]\" characters just have to be balanced. This\n"
        "  loop and the commands it contains are ignored because the current cell\n"
        "  defaults to a value of 0; the 0 value causes this loop to be skipped.\n"
        "]\n"
        "++++++++               Set Cell #0 to 8\n"
        "[\n"
        "    >++++               Add 4 to Cell #1; this will always set Cell #1 to 4\n"
        "    [                   as the cell will be cleared by the loop\n"
        "        >++             Add 2 to Cell #2\n"
        "        >+++            Add 3 to Cell #3\n"
        "        >+++            Add 3 to Cell #4\n"
        "        >+              Add 1 to Cell #5\n"
        "        <<<<-           Decrement the loop counter in Cell #1\n"
        "    ]                   Loop until Cell #1 is zero; number of iterations is 4\n"
        "    >+                  Add 1 to Cell #2\n"
        "    >+                  Add 1 to Cell #3\n"
        "    >-                  Subtract 1 from Cell #4\n"
        "    >>+                 Add 1 to Cell #6\n"
        "    [<]                 Move back to the first zero cell you find; this will\n"
        "                        be Cell #1 which was cleared by the previous loop\n"
        "    <-                  Decrement the loop Counter in Cell #0\n"
        "]                       Loop until Cell #0 is zero; number of iterations is 8\n"
        "\n"
        "The result of this is:\n"
        "Cell no :   0   1   2   3   4   5   6\n"
        "Contents:   0   0  72 104  88  32   8\n"
        "Pointer :   ^\n"
        "\n"
        ">>.                     Cell #2 has value 72 which is 'H'\n"
        ">---.                   Subtract 3 from Cell #3 to get 101 which is 'e'\n"
        "+++++++..+++.           Likewise for 'llo' from Cell #3\n"
        ">>.                     Cell #5 is 32 for the space\n"
        "<-.                     Subtract 1 from Cell #4 for 87 to give a 'W'\n"
        "<.                      Cell #3 was set to 'o' from the end of 'Hello'\n"
        "+++.------.--------.    Cell #3 for 'rl' and 'd'\n"
        ">>+.                    Add 1 to Cell #5 gives us an exclamation point\n"
        ">++.                    And finally a newline from Cell #6";

    machine.load(instructions);
    int ret = machine.execute();
    if(ret != 0)
    {
        std::cout << "An error occurred. Error is: ";
        switch(ret)
        {
            case RETURN_INVALID_INSTRUCTION:
                std::cout << "Invalid Instruction"; break;
            case RETURN_INVALID_MEMORY_ADDRESS:
                std::cout << "Invalid Memory Access"; break;
            default:
                std::cout << "Unknown Error"; break;
        }
        std::cout << std::endl;
        std::cout.flush();
    }
    return ret;
}
