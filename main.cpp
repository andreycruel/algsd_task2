#include <algorithm>
#include <iostream>
#include <limits>
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

std::vector<int> findAllInRange(const std::string& text,
                                const std::string& pattern,
                                int startIndex,
                                int endIndex) {
    std::vector<int> positions;

    if (pattern.empty() || startIndex < 0 || endIndex < startIndex ||
        endIndex >= static_cast<int>(text.length())) {
        return positions;
    }

    int patternLength = static_cast<int>(pattern.length());
    if (patternLength > endIndex - startIndex + 1) {
        return positions;
    }

    std::vector<int> lastPosition = makeBadCharacterTable(pattern);
    int lastPossibleStart = endIndex - patternLength + 1;
    int shift = startIndex;

    while (shift <= lastPossibleStart) {
        int patternIndex = patternLength - 1;

        while (patternIndex >= 0 &&
               pattern[patternIndex] == text[shift + patternIndex]) {
            --patternIndex;
        }

        if (patternIndex < 0) {
            positions.push_back(shift);

            if (shift + patternLength <= endIndex) {
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

void printSearchResult(const std::string& text, const std::string& pattern,
                       int startIndex, int endIndex) {
    std::cout << "First index: " << findFirst(text, pattern) << '\n';
    std::cout << "All indices: ";
    printPositions(findAll(text, pattern));
    std::cout << "Indices in [" << startIndex << "; " << endIndex << "]: ";
    printPositions(findAllInRange(text, pattern, startIndex, endIndex));
}

void runDemonstration() {
    const std::string text =
        "std::move_iterator is an iterator adaptor which behaves exactly like "
        "the underlying iterator";
    const std::string pattern = "tor";

    std::cout << "Text: " << text << '\n';
    std::cout << "Pattern: " << pattern << '\n';
    printSearchResult(text, pattern, 17, 91);
    std::cout << "Indices in [28; 36]: ";
    printPositions(findAllInRange(text, pattern, 28, 36));
}

void runManualInput() {
    std::string text;
    std::string pattern;
    int startIndex;
    int endIndex;

    std::cout << "Enter text: ";
    std::getline(std::cin, text);
    std::cout << "Enter pattern: ";
    std::getline(std::cin, pattern);
    std::cout << "Enter inclusive range start and end: ";

    if (!(std::cin >> startIndex >> endIndex)) {
        std::cout << "Range must contain two integer indices.\n";
        return;
    }

    printSearchResult(text, pattern, startIndex, endIndex);
}

int main() {
    int mode;
    std::cout << "1 - demonstration, 2 - manual input: ";

    if (!(std::cin >> mode)) {
        std::cout << "Input error.\n";
        return 1;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (mode == 1) {
        runDemonstration();
    } else if (mode == 2) {
        runManualInput();
    } else {
        std::cout << "Unknown mode.\n";
        return 1;
    }

    return 0;
}
