#include "commandstack.hpp"
std::mutex command_stack_mutex;
std::stack<std::string> &command_stack()
{
    static std::stack<std::string> stack;
    return stack;
}