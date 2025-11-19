#ifndef BLOCK_POSTAL_CODE
#define BLOCK_POSTAL_CODE

/**
 * @file BlockPostalCode.h
 * @brief Declares the BlockPostalCode class used in the postal-code block sequence set.
 *
 * This block stores a single postal-code header record and contains
 * predecessor and successor links (Relative Block Number style) using
 * raw pointers to represent the previous and next block.
 */

#include "HeaderRecordPostalCodeItem.h"

/**
 * @class BlockPostalCode
 * @brief Represents one block that stores a HeaderRecordPostalCodeItem.
 *
 * @details The block contains:
 *          - The postal-code header record stored as `data`
 *          - A predecessor pointer (`prev`) connecting to the previous block
 *          - A successor pointer (`next`) connecting to the next block
 *
 * These act as "predecessor & successor Relative Block Number links."
 */

class BlockPostalCode
{
private:
    ///< @brief The postal-code header record stored in this block.
    HeaderRecordPostalCodeItem data;

    /// @brief Predecessor Relative Block Number link.
    /// @details Points to the previous block in the block sequence set.
    BlockPostalCode *prevRBN;

    /// @brief Successor Relative Block Number link.
    /// @details Points to the next block in the block sequence set.
    BlockPostalCode *nextRBN;

public:
    /**
     * @brief Default constructor. Initializes an empty block with null links.
     */
    BlockPostalCode();

    /**
     * @brief Constructor that sets the data item for this block.
     * @param item The HeaderRecordPostalCodeItem to store in the block.
     */
    BlockPostalCode(const HeaderRecordPostalCodeItem &item);

    /**
     * @brief Constructor that sets data and predecessor/successor links.
     * @param item The header record stored in this block.
     * @param prevBlock Pointer to the previous block.
     * @param nextBlock Pointer to the next block.
     */
    BlockPostalCode(const HeaderRecordPostalCodeItem &item, BlockPostalCode *prevBlock, BlockPostalCode *nextBlock);

    /**
     * @brief Sets the header record stored in this block.
     * @param item The new HeaderRecordPostalCodeItem to store.
     */
    void setBlockItem(const HeaderRecordPostalCodeItem &item);

    /**
     * @brief Sets the predecessor block link.
     * @param prevBlock Pointer to the previous block.
     */
    void setPrevRBN(BlockPostalCode *prevBlock);

    /**
     * @brief Sets the successor block link.
     * @param nextBlock Pointer to the next block.
     */
    void setNextRBN(BlockPostalCode *nextBlock);

    /**
     * @brief Retrieves the header record stored in this block.
     * @return The HeaderRecordPostalCodeItem stored inside the block.
     */
    HeaderRecordPostalCodeItem getBlockItem() const;

    /**
     * @brief Gets the predecessor block pointer.
     * @return A pointer to the previous BlockPostalCode.
     */
    BlockPostalCode *getPrev() const;

    /**
     * @brief Gets the successor block pointer.
     * @return A pointer to the next BlockPostalCode.
     */
    BlockPostalCode *getNext() const;
};

#include "BlockPostalCode.cpp"
#endif