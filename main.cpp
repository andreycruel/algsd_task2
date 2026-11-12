#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> makeBadCharacterTable(const std::string& pattern) {
    std::vector<int> lastPosition(256, -1);

    for (int i = 0; i < static_cast<int>(pattern.length()); ++i) {
        lastPosition[static_cast<unsigned char>(pattern[i])] = i;
    }

    return lastPosition;
}

int findFirst(const std::string& text, const std::string& pattern) {
    if (pattern.empty()) {
        return 0;
    }

    if (pattern.length() > text.length()) {
        return -1;
    }

    std::vector<int> lastPosition = makeBadCharacterTable(pattern);
    int textLength = static_cast<int>(text.length());
    int patternLength = static_cast<int>(pattern.length());
    int shift = 0;

    while (shift <= textLength - patternLength) {
        int patternIndex = patternLength - 1;

        while (patternIndex >= 0 &&
               pattern[patternIndex] == text[shift + patternIndex]) {
            --patternIndex;
        }

        if (patternIndex < 0) {
            return shift;
        }

        unsigned char mismatch =
            static_cast<unsigned char>(text[shift + patternIndex]);
        shift += std::max(1, patternIndex - lastPosition[mismatch]);
    }

    return -1;
}

int main() {
    const std::string text = "abracadabra";
    const std::string pattern = "cad";

    std::cout << "Text: " << text << '\n';
    std::cout << "Pattern: " << pattern << '\n';
    std::cout << "First index: " << findFirst(text, pattern) << '\n';

    return 0;
}
