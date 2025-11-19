#pragma once
#include <span>
#include <string>
#include "ZipRecord.h"

class RecordBuffer {
public:
	explicit RecordBuffer(std::span<const std::byte> payload);
	ZipRecord toZipRecord() const;
	std::size_t fieldCount() const;
	const std::string& fieldString(std::size_t i) const;
private:
	std::vector<std::string> fields_;
	static std::vector<std::string> parseCsv(std::string_view line);