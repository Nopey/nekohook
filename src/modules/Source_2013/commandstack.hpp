#pragma once
#include <stack>
#include <string>
#include <mutex>
std::stack<std::string> &command_stack();
extern std::mutex command_stack_mutex;
