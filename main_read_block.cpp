/**
 * @file main_read_block.cpp
 * @brief Reads a blocked sequence set (BSS) of postal codes and prints block records.
 */

#include <string>
#include "BlockPostalCode.h"
#include "BlockSequenceSetPostalCode.h"
#include "readHeaderPostalCodetoBSSBuffer.cpp"
#include <fstream>

using namespace std;

/**
 * @brief Program entry point.
 * Loads postal code data into a Block Sequence Set and prints each block.
 *
 * The output format for each block is:
 * @code
 * recordLength data prevZip nextZip
 * @endcode
 * where "NULL" is used when a link does not exist.
 *
 * @return int Exit status
 */
int main()
{
    string fileName = "us_postal_codes_length_indicated_header_record.txt"; ///< Input file containing header + length-indicated records

    BlockSequenceSetPostalCode myBlockSequenceSetPostalCode; ///< The full Block Sequence Set structure

    inputDatatoBlockSequenceSet(myBlockSequenceSetPostalCode, fileName); ///< Populate BSS from data file

    BlockPostalCode myBlock = myBlockSequenceSetPostalCode.getHead(); ///< Pointer to first block

    // Write first block
    string blockRecord = to_string(myBlock.getBlockItem().getRecordLength()) + " " +
                         myBlock.getBlockItem().getData() + " NULL " +
                         to_string(myBlock.getNext()->getBlockItem().getZip());

    cout << blockRecord << endl;

    myBlock = *myBlock.getNext(); ///< Move to next block

    // Loop through middle blocks
    while (myBlock.getNext() != nullptr)
    {
        blockRecord = to_string(myBlock.getBlockItem().getRecordLength()) + " " +
                      myBlock.getBlockItem().getData() + " " +
                      to_string(myBlock.getPrev()->getBlockItem().getZip()) + " " +
                      to_string(myBlock.getNext()->getBlockItem().getZip());

        cout << blockRecord << endl;

        myBlock = *myBlock.getNext(); ///< Advance to next block
    }

    // Write last block
    blockRecord = to_string(myBlock.getBlockItem().getRecordLength()) + " " +
                  myBlock.getBlockItem().getData() + " " +
                  to_string(myBlock.getPrev()->getBlockItem().getZip()) + " NULL";

    cout << blockRecord << endl;

    return 0;
}