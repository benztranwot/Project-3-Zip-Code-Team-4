/**
 * @file main_write_block.cpp
 * @brief Writes a blocked sequence set (BSS) of postal codes to a file.
 *
 * Output format:
 * @code
 * HeaderRecord Data PrevZip NextZip
 * @endcode
 */

#include <string>
#include "BlockPostalCode.h"
#include "BlockSequenceSetPostalCode.h"
#include "readHeaderPostalCodetoBSSBuffer.cpp"
#include <fstream>

using namespace std;

/**
 * @brief Program entry point.
 * Loads postal code data into a Block Sequence Set and writes
 * block data to block_sequence_set_data.txt.
 *
 * Block records include record length, data, previous block ZIP, and next block ZIP.
 * "NULL" is written where a link does not exist.
 *
 * @return int Exit status
 */
int main()
{
    string fileName = "us_postal_codes_length_indicated_header_record.txt"; ///< Input file for BSS population

    BlockSequenceSetPostalCode myBlockSequenceSetPostalCode; ///< BSS object storing all blocks

    inputDatatoBlockSequenceSet(myBlockSequenceSetPostalCode, fileName); ///< Fill BSS from input file

    ofstream outputFile("block_sequence_set_data.txt"); ///< Output file for block sequence set

    BlockPostalCode myBlock = myBlockSequenceSetPostalCode.getHead(); ///< First block

    // Write first block
    string blockRecord = to_string(myBlock.getBlockItem().getRecordLength()) + " " +
                         myBlock.getBlockItem().getData() + " NULL " +
                         to_string(myBlock.getNext()->getBlockItem().getZip());

    outputFile << blockRecord << endl;

    myBlock = *myBlock.getNext(); ///< Move to next block

    // Loop through middle blocks
    while (myBlock.getNext() != nullptr)
    {
        blockRecord = to_string(myBlock.getBlockItem().getRecordLength()) + " " +
                      myBlock.getBlockItem().getData() + " " +
                      to_string(myBlock.getPrev()->getBlockItem().getZip()) + " " +
                      to_string(myBlock.getNext()->getBlockItem().getZip());

        outputFile << blockRecord << endl;

        myBlock = *myBlock.getNext(); ///< Advance to next block
    }

    // Write last block
    blockRecord = to_string(myBlock.getBlockItem().getRecordLength()) + " " +
                  myBlock.getBlockItem().getData() + " " +
                  to_string(myBlock.getPrev()->getBlockItem().getZip()) + " NULL";

    outputFile << blockRecord << endl;

    outputFile.close();

    return 0;
}