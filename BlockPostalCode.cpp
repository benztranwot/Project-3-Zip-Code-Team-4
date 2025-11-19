#include "BlockPostalCode.h"
#include "HeaderRecordPostalCodeItem.h"

BlockPostalCode::BlockPostalCode() : prevRBN(nullptr), nextRBN(nullptr) {}

BlockPostalCode::BlockPostalCode(const HeaderRecordPostalCodeItem &item) : data(item), prevRBN(nullptr), nextRBN(nullptr) {}

BlockPostalCode::BlockPostalCode(const HeaderRecordPostalCodeItem &item, BlockPostalCode *prevBlock, BlockPostalCode *nextBlock) : data(item), prevRBN(prevBlock), nextRBN(nextBlock) {}

void BlockPostalCode::setBlockItem(const HeaderRecordPostalCodeItem &item)
{
    data = item;
}

void BlockPostalCode::setPrev(BlockPostalCode *prevBlock)
{
    prevRBN = prevBlock;
}

void BlockPostalCode::setNext(BlockPostalCode *nextBlock)
{
    nextRBN = nextBlock;
}

HeaderRecordPostalCodeItem BlockPostalCode::getBlockItem() const
{
    return data;
}

BlockPostalCode *BlockPostalCode::getPrev() const
{
    return prevRBN;
}

BlockPostalCode *BlockPostalCode::getNext() const
{
    return nextRBN;
}