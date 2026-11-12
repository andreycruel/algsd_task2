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

std::vector<int> findAll(const std::string& text, const std::string& pattern) {
    std::vector<int> positions;

    if (pattern.empty() || pattern.length() > text.length()) {
        return positions;
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
            positions.push_back(shift);

            // Сдвиг по следующему символу сохраняет и перекрывающиеся вхождения.
            if (shift + patternLength < textLength) {
                unsigned char next =
                    static_cast<unsigned char>(text[shift + patternLength]);
                shift += patternLength - lastPosition[next];
            } else {
                ++shift;
            }
        } else {
            unsigned char mismatch =
                static_cast<unsigned char>(text[shift + patternIndex]);
            shift += std::max(1, patternIndex - lastPosition[mismatch]);
        }
    }

    return positions;
}

void printPositions(const std::vector<int>& positions) {
    std::cout << "[";
    for (int i = 0; i < static_cast<int>(positions.size()); ++i) {
        std::cout << positions[i];
        if (i + 1 < static_cast<int>(positions.size())) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";
}

int main() {
    const std::string text = "banana";
    const std::string pattern = "ana";

    std::cout << "Text: " << text << '\n';
    std::cout << "Pattern: " << pattern << '\n';
    std::cout << "First index: " << findFirst(text, pattern) << '\n';
    std::cout << "All indices: ";
    printPositions(findAll(text, pattern));

    return 0;
}
