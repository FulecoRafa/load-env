#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>

#include "validate.hpp"

class Defer {
    std::function<void()> m_func;
public:
    Defer(std::function<void()> f) : m_func(f) {}
    ~Defer() { m_func(); }
    Defer& operator=(const Defer&)=delete;
    Defer(const Defer&)=delete;
};

bool confirm(std::string command) {
    char answer;
    std::cout << command << "\nConfirm running this command? [Y/n]";
    std::cin >> answer;
    switch (answer) {
        case 'Y':
        case 'y':
            return true;
            break;
        default:
            return false;
            break;
    }
}

int read_env_file(std::stringstream &result, const char* filename) {
#ifdef DEBUG
    std::cerr<<"Reading file:" << filename << "\n";
#endif
    std::ifstream file(filename);
    Defer _([&file](){
        file.close();
    });
    std::string line;
    while(std::getline(file, line)) {
#ifdef DEBUG
        std::cerr << "Line: " << line << "\n";
#endif
        if (!validate_envvar(line)) {
            std::cerr << "Invalid line in " << filename << " : " << line << "\n";
            exit(1);
        }
        result << line << " ";
    }
    return 0;
}

typedef enum {
    ENV,
    CMD
} arg_t;

void handle_arg(std::stringstream &env, std::stringstream &command, char* arg, arg_t type) {
    if (type == arg_t::CMD) {
#ifdef DEBUG
    std::cerr<<"Interpret CMD: \n";
#endif
        command << arg << " ";
    } else {
#ifdef DEBUG
    std::cerr<<"Interpret ENV: \n";
#endif
        read_env_file(env, arg);
    }
}

int run_command(std::string cmd) {
    auto shell = std::getenv("SHELL");
    auto actual_command = std::string() + shell + " -c '" + cmd + "'";
#ifdef DEBUG
    std::cout<<"Actual command " << actual_command << "\n";
#endif
    system(actual_command.c_str());
    return true;
}

int main(int argc, char**argv) {
    if (argc <= 1) {
        std::cerr << "Expected arguments\n";
        exit(1);
    }

    std::stringstream env, command;
    arg_t active_type = arg_t::ENV;
    uint found_ddash(0);
    bool should_confirm(false);
    for (int i = 1; i < argc; i++) {
        auto arg = argv[i];
#ifdef DEBUG
        std::cerr << i << " -> `" << arg << "`\n";
#endif
        if (std::strncmp("--", arg, 3) == 0) {
            if (++found_ddash > 1) {
                std::cerr << "Expected a single `--`\n";
                exit(1);
            }
            active_type = arg_t::CMD;
        } else if (std::strncmp("--confirm", arg, 10) == 0) {
            should_confirm = true;
        } else {
            handle_arg(env, command, arg, active_type);
        }
    }
    if (found_ddash != 1 || command.str().length() == 0) {
        std::cerr << "No command provided\n";
        exit(1);
    }

    std::string result = env.str() + command.str();
#ifdef DEBUG
    std::cerr << "Command for shell `"<<std::getenv("SHELL")<<"`: " << result << "\n";
#endif

    if (should_confirm) {
        if (!confirm(result)) {
            exit(1);
        }
    }

    return run_command(result);
}
