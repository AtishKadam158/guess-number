#include "utilities.h"

void error(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
    exit(EXIT_FAILURE);
}
