/*
 * Created by Aaron Helton on 5/28/22.
 */

#ifndef BRAINFUCKINTERPRETTER_BFMACHINE_HPP
#define BRAINFUCKINTERPRETTER_BFMACHINE_HPP

#include <cstdint>
#include <cstddef>

#define MAX_MEMORY_SIZE 30000

#define BF_RETURN_SUCCESS 0
#define BF_RETURN_INVALID_MEMORY_ADDRESS 1
#define BF_RETURN_INVALID_INSTRUCTION 2

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

#endif //BRAINFUCKINTERPRETTER_BFMACHINE_HPP
