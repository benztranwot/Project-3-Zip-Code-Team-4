#ifndef BLOCK_SEQUENCE_SET_POSTAL_CODE
#define BLOCK_SEQUENCE_SET_POSTAL_CODE

/**
 * @file BlockSequenceSetPostalCode.h
 * @brief Declares the BlockSequenceSetPostalCode class which manages a doubly linked
 *        sequence of BlockPostalCode nodes.
 *
 * This class forms the Block Sequence Set (BSS) structure used to store postal
 * header records in a linked-block format. Each block is connected using
 * predecessor/successor links similar to Relative Block Number (RBN) behavior.
 */

#include "BlockPostalCode.h"
#include "HeaderRecordPostalCodeItem.h"

/**
 * @class BlockSequenceSetPostalCode
 * @brief Manages a linked list of BlockPostalCode objects.
 *
 * @details
 * Each BlockPostalCode contains:
 *   - A HeaderRecordPostalCodeItem
 *   - A predecessor pointer (prev)
 *   - A successor pointer (next)
 *
 * The BlockSequenceSetPostalCode class stores:
 *   - headBlock → pointer to the first block
 *   - tailBlock → pointer to the last block
 *   - itemCount → number of stored blocks
 *
 * New blocks are appended to the end (tail), maintaining predecessor and
 * successor relationships.
 */
class BlockSequenceSetPostalCode
{
private:
    BlockPostalCode *headBlock; ///< Pointer to the first block in the sequence.
    BlockPostalCode *tailBlock; ///< Pointer to the last block in the sequence.
    int itemCount;              ///< Total number of blocks stored.

public:
    /**
     * @brief Default constructor. Creates an empty block sequence set.
     */
    BlockSequenceSetPostalCode();

    /**
     * @brief Adds a new header postal code item as a BlockPostalCode.
     * @param newHeaderPostalCodeItem The item to insert into a new block.
     * @return true if the block was successfully created and linked.
     */
    bool add(const HeaderRecordPostalCodeItem &newHeaderPostalCodeItem);

    /**
     * @brief Retrieves the head block by value.
     * @return A copy of the first BlockPostalCode in the sequence.
     */
    BlockPostalCode getHead() const;

    /**
     * @brief Gets the number of blocks stored in the sequence.
     * @return The total count of blocks.
     */
    int getCurrentSize() const;
};

#include "BlockSequenceSetPostalCode.cpp"
#endif