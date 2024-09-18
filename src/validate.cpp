#include <regex>

const std::regex envvar_reg("[a-zA-Z]+=[a-zA-Z]+", std::regex_constants::ECMAScript);

bool validate_envvar(std::string &var) {
    return std::regex_search(var, envvar_reg);
}
