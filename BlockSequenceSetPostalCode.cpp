#include "BlockSequenceSetPostalCode.h"
#include "BlockPostalCode.h"
#include "HeaderRecordPostalCodeItem.h"

/**
 * @file BlockSequenceSetPostalCode.cpp
 * @brief Implements the BlockSequenceSetPostalCode class.
 *
 * This class manages a sequence of BlockPostalCode nodes that together form
 * the block sequence set structure used for storing header postal code items.
 */

/**
 * @brief Default constructor. Creates an empty block sequence set.
 *
 * The head and tail block pointers are initialized to nullptr and the
 * item count is set to zero.
 */
BlockSequenceSetPostalCode::BlockSequenceSetPostalCode() : headBlock(nullptr), itemCount(0) {}

/**
 * @brief Gets the number of items currently stored in the sequence set.
 * @return The total count of HeaderRecordPostalCodeItem objects in the list.
 */
int BlockSequenceSetPostalCode::getCurrentSize() const
{
    return itemCount;
}

/**
 * @brief Returns the current head block by value.
 * @return A copy of the first BlockPostalCode in the sequence set.
 * @pre The sequence set must not be empty (headBlock != nullptr).
 */
BlockPostalCode BlockSequenceSetPostalCode::getHead() const
{
    return *headBlock;
}

/**
 * @brief Adds a new header postal code item to the end of the sequence set.
 *
 * The method allocates a new BlockPostalCode, stores the given header item,
 * and then links the new block into the doubly linked list that represents
 * the block sequence set. The predecessor and successor links of the tail
 * and new block are updated to maintain the structure.
 *
 * @param newHeaderPostalCodeItem The header record to add as a new block.
 * @return true if the block was successfully added.
 */
bool BlockSequenceSetPostalCode::add(const HeaderRecordPostalCodeItem &newHeaderPostalCodeItem)
{
    BlockPostalCode *newBlock = new BlockPostalCode();
    newBlock->setBlockItem(newHeaderPostalCodeItem);

    if (headBlock == nullptr || tailBlock == nullptr)
    {
        headBlock = newBlock;
        tailBlock = newBlock;
    }
    else if (headBlock == tailBlock)
    {
        tailBlock = newBlock;
        headBlock->setNextRBN(tailBlock);
        tailBlock->setPrevRBN(headBlock);
    }
    else
    {
        newBlock->setPrevRBN(tailBlock);
        tailBlock->setNextRBN(newBlock);
        tailBlock = newBlock;
    }

    itemCount++;

    return true;
}