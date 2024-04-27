#include <iostream>
#include <ctime>
#include <chrono>
#include <string>
#include <regex>
#include <cstdlib>

// Function to execute shell commands and get output
std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

int main() {
    // Get system time
    std::time_t system_time = std::time(nullptr);
    std::cout << "System time: " << std::asctime(std::localtime(&system_time));

    // Get actual time using Chrony
    std::string chrony_output = exec("chronyc tracking");
    std::cout << "Chrony output:\n" << chrony_output << std::endl;

    // Extract actual time using regular expression
    std::regex time_regex(R"(Ref time \(UTC\)  : (.+?)\n)");
    std::smatch match;
    if (std::regex_search(chrony_output, match, time_regex)) {
        std::string actual_time_str = match[1];
        std::cout << "Actual time (via Chrony): " << actual_time_str;

        // Convert actual time to time_t
        struct tm tm;
        strptime(actual_time_str.c_str(), "%a %b %d %H:%M:%S %Y", &tm); // Adjust format based on the output
        std::time_t actual_time = mktime(&tm);

        // Calculate the time difference
        double diff_seconds = std::difftime(actual_time, system_time);
        std::cout << "Time difference: " << diff_seconds << " seconds" << std::endl;
    } else {
        std::cerr << "Error: Could not extract actual time from Chrony output." << std::endl;
        return 1;
    }

    return 0;
}
