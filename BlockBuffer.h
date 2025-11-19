#pragma once
#include <cstdint>
#include <vector>
#include <span>

class BlockBuffer {
public:
	explicit BlockBuffer(std::size_t blockSize)
		: blockSize_(blockSize), buffer_(), currentBlock_(), position_(0) {
		buffer_.reserve(blockSize_);
	}
	void loadBlock(std::span<const std::byte> data) {
		if (data.size() != blockSize_) {
			throw std::runtime_error("Data size does not match block size");
		}
		buffer_.assign(data.begin(), data.end());
		currentBlock_ = buffer_.data();
		position_ = 0;
	}
	bool isAvailable() const;
	std::uint16_t recordCount() const;
	int32_t prevRBN() const;
	int32_t nextRBN() const;
	std::span<const std::byte> recordSpan(std::size_t i) const;
	std::size_t blockSize() const { return blockSize_; }

private:
	std::size_t blockSize_;
	std::vector<std::byte> buffer_;
	std::uint16_t recordCount_;
	int32_t prevRBN_, nextRBN_;
	std::vector<std::uint16_t> recordOffsets_;

	void parseHeaderAndOffsets();
};