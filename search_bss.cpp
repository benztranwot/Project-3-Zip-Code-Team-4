#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

// Simple struct for an index entry: highest key in block, and its RBN
struct IndexEntry {
    int highestKey;
    int rbn;
};

// ---------- Helpers for parsing BSS lines ----------

// Given a line from block_sequence_set_data.txt, extract the ZIP code.
//
// Format your writer uses (one record per block):
//   recordLength SP zip,place,state,county,lat,lon SP prevLink SP nextLink
//
// We:
//   - read recordLength (everything before first space)
//   - grab the substring between firstSpace+1 and secondLastSpace as the CSV "data"
//   - ZIP is everything before the first comma in that data
int extractZipFromBssLine(const std::string &line) {
    // First space (after recordLength)
    std::size_t firstSpace = line.find(' ');
    if (firstSpace == std::string::npos) {
        throw std::runtime_error("Malformed BSS line (no first space): " + line);
    }

    // Last space (before nextLink)
    std::size_t lastSpace = line.rfind(' ');
    if (lastSpace == std::string::npos || lastSpace <= firstSpace) {
        throw std::runtime_error("Malformed BSS line (no last space): " + line);
    }

    // Second-to-last space (between data and prevLink)
    std::size_t secondLastSpace = line.rfind(' ', lastSpace - 1);
    if (secondLastSpace == std::string::npos || secondLastSpace <= firstSpace) {
        throw std::runtime_error("Malformed BSS line (no second-last space): " + line);
    }

    // CSV data substring
    std::string data = line.substr(firstSpace + 1, secondLastSpace - firstSpace - 1);

    // ZIP is before first comma in data
    std::size_t firstComma = data.find(',');
    if (firstComma == std::string::npos) {
        throw std::runtime_error("Malformed data section (no comma): " + data);
    }

    int zip = std::stoi(data.substr(0, firstComma));
    return zip;
}

// Read the RBN-th line (0-based) from the BSS data file
bool readBlockLineByRbn(const std::string &dataFile, int rbn, std::string &outLine) {
    std::ifstream in(dataFile);
    if (!in) {
        std::cerr << "Error: cannot open data file '" << dataFile << "'\n";
        return false;
    }

    std::string line;
    int current = 0;
    while (std::getline(in, line)) {
        if (current == rbn) {
            outLine = line;
            return true;
        }
        ++current;
    }
    return false; // RBN out of range
}

void printRecordFromBssLine(const std::string &line) {
    std::size_t firstSpace = line.find(' ');
    std::size_t lastSpace = line.rfind(' ');
    std::size_t secondLastSpace = line.rfind(' ', lastSpace - 1);

    if (firstSpace == std::string::npos ||
        lastSpace == std::string::npos ||
        secondLastSpace == std::string::npos) {
        std::cout << line << "\n"; // fallback
        return;
    }

    std::string data = line.substr(firstSpace + 1, secondLastSpace - firstSpace - 1);
    std::cout << "  Record: " << data << "\n";
}

std::vector<IndexEntry> buildIndexFromDataFile(const std::string &dataFile) {
    std::vector<IndexEntry> index;
    std::ifstream in(dataFile);
    if (!in) {
        throw std::runtime_error("Cannot open data file for index build: " + dataFile);
    }

    std::string line;
    int rbn = 0;
    while (std::getline(in, line)) {
        if (line.empty()) {
            ++rbn;
            continue;
        }
        int zip = extractZipFromBssLine(line);
        IndexEntry entry{zip, rbn};
        index.push_back(entry);
        ++rbn;
    }

    // Assuming BSS is in ascending zip order, index is already sorted by highestKey.
    // If not, you could std::sort by highestKey here.

    return index;
}

// Write index to a text file: "highestKey rbn" per line
void writeIndexToFile(const std::string &indexFile, const std::vector<IndexEntry> &index) {
    std::ofstream out(indexFile);
    if (!out) {
        throw std::runtime_error("Cannot open index file for writing: " + indexFile);
    }
    for (const auto &entry : index) {
        out << entry.highestKey << " " << entry.rbn << "\n";
    }
}

// Read index back from file into RAM
std::vector<IndexEntry> readIndexFromFile(const std::string &indexFile) {
    std::vector<IndexEntry> index;
    std::ifstream in(indexFile);
    if (!in) {
        throw std::runtime_error("Cannot open index file for reading: " + indexFile);
    }

    int key, rbn;
    while (in >> key >> rbn) {
        index.push_back(IndexEntry{key, rbn});
    }
    return index;
}

// Find the RBN of the block that should contain zipKey using the index.
// We find the first index entry with highestKey >= zipKey (like lower_bound).
// If none is found, return -1 (definitely not in file).
int findBlockRbnForZip(const std::vector<IndexEntry> &index, int zipKey) {
    int low = 0;
    int high = static_cast<int>(index.size()) - 1;
    int resultRbn = -1;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (index[mid].highestKey >= zipKey) {
            resultRbn = index[mid].rbn;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return resultRbn;
}

// ---------- Main program ----------

int main(int argc, char *argv[]) {
    // Defaults (satisfies #12 by being overridable from the command line)
    std::string dataFile  = "block_sequence_set_data.txt";
    std::string indexFile = "simple_index.txt";
    bool rebuildIndex     = false;
    std::vector<int> zipsToSearch;

    // Parse command line
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-D" && i + 1 < argc) {
            dataFile = argv[++i];
        } else if (arg == "-I" && i + 1 < argc) {
            indexFile = argv[++i];
        } else if (arg == "-buildIndex") {
            rebuildIndex = true;
        } else if (arg.rfind("-Z", 0) == 0 && arg.size() > 2) {
            int zip = std::stoi(arg.substr(2));
            zipsToSearch.push_back(zip);
        } else {
            std::cerr << "Warning: unrecognized argument '" << arg << "'\n";
        }
    }

    if (zipsToSearch.empty()) {
        std::cerr << "Usage: " << argv[0]
                  << " [-D dataFile] [-I indexFile] [-buildIndex] -Z56301 -Z56302 ...\n";
        return 1;
    }

    try {
        // Step 1: maybe build index in RAM and write it to disk
        if (rebuildIndex) {
            std::cout << "Building index from data file '" << dataFile << "'...\n";
            auto index = buildIndexFromDataFile(dataFile);
            writeIndexToFile(indexFile, index);
            std::cout << "Index written to '" << indexFile << "' (" << index.size()
                      << " entries).\n";
        }

        // Step 2: read index file back into RAM
        auto index = readIndexFromFile(indexFile);
        if (index.empty()) {
            std::cerr << "Error: index file '" << indexFile << "' is empty\n";
            return 1;
        }

        // Step 3: search for each requested Zip
        for (int zip : zipsToSearch) {
            std::cout << "Searching for ZIP " << zip << "...\n";

            int rbn = findBlockRbnForZip(index, zip);
            if (rbn == -1) {
                std::cout << "  Not found (ZIP is larger than any highestKey in index).\n";
                continue;
            }

            std::string blockLine;
            if (!readBlockLineByRbn(dataFile, rbn, blockLine)) {
                std::cout << "  Error: block RBN " << rbn
                          << " not found in data file '" << dataFile << "'.\n";
                continue;
            }

            // In your current design, each block contains one record, so just compare.
            int blockZip = extractZipFromBssLine(blockLine);
            if (blockZip == zip) {
                std::cout << "  Found in block RBN " << rbn << ":\n";
                printRecordFromBssLine(blockLine);
            } else {
                std::cout << "  Not found in block RBN " << rbn
                          << " (highestKey there is " << blockZip << ").\n";
            }
        }

    } catch (const std::exception &ex) {
        std::cerr << "Fatal error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
