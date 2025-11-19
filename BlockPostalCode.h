#ifndef BLOCK_POSTAL_CODE
#define BLOCK_POSTAL_CODE

#include "HeaderRecordPostalCodeItem.h"

class BlockPostalCode
{
private:
    HeaderRecordPostalCodeItem data;
    BlockPostalCode *prevRBN;
    BlockPostalCode *nextRBN;

public:
    BlockPostalCode();
    BlockPostalCode(const HeaderRecordPostalCodeItem &item);
    BlockPostalCode(const HeaderRecordPostalCodeItem &item, BlockPostalCode *prevBlock, BlockPostalCode *nextBlock);

    void setBlockItem(const HeaderRecordPostalCodeItem &item);
    void setPrevRBN(BlockPostalCode *prevBlock);
    void setNextRBN(BlockPostalCode *nextBlock);

    HeaderRecordPostalCodeItem getBlockItem() const;
    BlockPostalCode *getPrev() const;
    BlockPostalCode *getNext() const;
};

#include "BlockPostalCode.cpp"
#endif