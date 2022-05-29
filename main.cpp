#include <iostream>
#include <string>

#define MAX_MEMORY_SIZE 30000

#define RETURN_SUCCESS 0
#define RETURN_INVALID_MEMORY_ADDRESS 1
#define RETURN_INVALID_INSTRUCTION 2

class BFMachine
{
    public:
        BFMachine();
        ~BFMachine();
        void load(const char *instructions);
        int execute();
        void reset();

    private:
        uint8_t* memory;
        const char *program{};
        size_t programIndex{};
        size_t dataIndex{};
        int error{};

        void increment();
        void decrement();
        void incrementDataPointer();
        void decrementDataPointer();
        void printByte();
        void readByte();
};

BFMachine::BFMachine()
{
    this->memory = new uint8_t[MAX_MEMORY_SIZE];
    this->reset();
}

BFMachine::~BFMachine()
{
    delete[] memory;
    memory = nullptr;
}

void BFMachine::load(const char *instructions)
{
    this->program = instructions;
}

void BFMachine::reset()
{
    memset(this->memory, 0, sizeof(uint8_t)*MAX_MEMORY_SIZE);
    this->dataIndex = 0;
    this->programIndex = 0;
    this->program = nullptr;
    this->error = RETURN_SUCCESS;
}

int BFMachine::execute()
{
    if(!this->program)
        return RETURN_SUCCESS;
    while(this->error == RETURN_SUCCESS && program[programIndex] != '\0')
    {
        unsigned char command = program[programIndex];
        switch(command)
        {
            case '>': incrementDataPointer(); break;
            case '<': decrementDataPointer(); break;
            case '+': increment(); break;
            case '-': decrement(); break;
            case '.': printByte(); break;
            case ',': readByte(); break;
            case '[':
                if(this->memory[this->dataIndex] == 0)
                {
                    uint8_t depth = 1;
                    while(this->program[programIndex] != ']' || depth != 0)
                    {
                        programIndex++;
                        if(program[programIndex] == '\0')
                        {
                            this->error = RETURN_INVALID_INSTRUCTION;
                            break;
                        }
                        if(program[programIndex] == '[')
                            depth++;
                        if(program[programIndex] == ']')
                            depth--;
                    }
                }
                break;
            case ']':
                if(this->memory[this->dataIndex] != 0)
                {
                    uint8_t depth = 1;
                    while(this->program[programIndex] != '[' || depth != 0)
                    {
                        if(programIndex == 0)
                        {
                            this->error = RETURN_INVALID_INSTRUCTION;
                            break;
                        }
                        programIndex--;
                        if(program[programIndex] == '[')
                            depth--;
                        if(program[programIndex] == ']')
                            depth++;
                    }
                }
                break;
            default:
                break;
        }
        programIndex++;
    }
    return this->error;
}

void BFMachine::increment()
{
    this->memory[this->dataIndex]++;
}

void BFMachine::decrement()
{
    this->memory[this->dataIndex]--;
}

void BFMachine::incrementDataPointer()
{
    this->dataIndex++;
    if(this->dataIndex >= MAX_MEMORY_SIZE)
    {
        this->error = RETURN_INVALID_MEMORY_ADDRESS;
    }
}

void BFMachine::decrementDataPointer()
{
    if(this->dataIndex == 0)
    {
        this->error = RETURN_INVALID_MEMORY_ADDRESS;
        return;
    }
    this->dataIndex--;
}

void BFMachine::printByte()
{
    auto c = reinterpret_cast<unsigned char>(this->memory[this->dataIndex]);
    std::cout << c;
}

void BFMachine::readByte()
{
    unsigned char c;
    std::cin >> c;
    this->memory[this->dataIndex] = reinterpret_cast<uint8_t>(c);
}

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
