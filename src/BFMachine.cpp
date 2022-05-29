/*
 * Created by Aaron Helton on 5/28/22.
 */

#include <BFMachine.hpp>
#include <cstring>
#include <iostream>

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
    this->error = BF_RETURN_SUCCESS;
}

int BFMachine::execute()
{
    if(!this->program)
        return BF_RETURN_SUCCESS;
    while(this->error == BF_RETURN_SUCCESS && program[programIndex] != '\0')
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
                            this->error = BF_RETURN_INVALID_INSTRUCTION;
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
                            this->error = BF_RETURN_INVALID_INSTRUCTION;
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
        this->error = BF_RETURN_INVALID_MEMORY_ADDRESS;
    }
}

void BFMachine::decrementDataPointer()
{
    if(this->dataIndex == 0)
    {
        this->error = BF_RETURN_INVALID_MEMORY_ADDRESS;
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
    std::cin.read(reinterpret_cast<char *>(&c), 1);
    this->memory[this->dataIndex] = reinterpret_cast<uint8_t>(c);
}