/**
 * @file BlockPostalCode.cpp
 * @brief Implements the BlockPostalCode class.
 */

#include "BlockPostalCode.h"
#include "HeaderRecordPostalCodeItem.h"

/**
 * @brief Default constructor. Initializes an empty block with null links.
 */
BlockPostalCode::BlockPostalCode() : prevRBN(nullptr), nextRBN(nullptr) {}

/**
 * @brief Constructor that sets the data item for this block.
 * @param item The HeaderRecordPostalCodeItem to store in the block.
 */
BlockPostalCode::BlockPostalCode(const HeaderRecordPostalCodeItem &item) : data(item), prevRBN(nullptr), nextRBN(nullptr) {}

/**
 * @brief Constructor that sets data and predecessor/successor links.
 * @param item The header record stored in this block.
 * @param prevBlock Pointer to the previous block.
 * @param nextBlock Pointer to the next block.
 */
BlockPostalCode::BlockPostalCode(const HeaderRecordPostalCodeItem &item, BlockPostalCode *prevBlock, BlockPostalCode *nextBlock) : data(item), prevRBN(prevBlock), nextRBN(nextBlock) {}

/**
 * @brief Sets the header record stored in this block.
 * @param item The new HeaderRecordPostalCodeItem to store.
 */
void BlockPostalCode::setBlockItem(const HeaderRecordPostalCodeItem &item)
{
    data = item;
}

/**
 * @brief Sets the predecessor block link.
 * @param prevBlock Pointer to the previous block.
 */
void BlockPostalCode::setPrevRBN(BlockPostalCode *prevBlock)
{
    prevRBN = prevBlock;
}

/**
 * @brief Sets the successor block link.
 * @param nextBlock Pointer to the next block.
 */
void BlockPostalCode::setNextRBN(BlockPostalCode *nextBlock)
{
    nextRBN = nextBlock;
}

/**
 * @brief Retrieves the header record stored in this block.
 * @return The HeaderRecordPostalCodeItem stored inside the block.
 */
HeaderRecordPostalCodeItem BlockPostalCode::getBlockItem() const
{
    return data;
}

/**
 * @brief Gets the predecessor block pointer.
 * @return A pointer to the previous BlockPostalCode.
 */
BlockPostalCode *BlockPostalCode::getPrev() const
{
    return prevRBN;
}

/**
 * @brief Gets the successor block pointer.
 * @return A pointer to the next BlockPostalCode.
 */
BlockPostalCode *BlockPostalCode::getNext() const
{
    return nextRBN;
}