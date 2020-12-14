#pragma once

#include "column.h"
#include "../base/string_view.h"
#include <utility>
#include <vector>

namespace clickhouse {

/**
 * Represents column of fixed-length strings.
 */
class ColumnFixedString : public Column {
public:
    using ValueType = string_view;

    explicit ColumnFixedString(size_t n);

    template <typename Values>
    ColumnFixedString(size_t n, const Values & values)
        : ColumnFixedString(n)
    {
        for (const auto & v : values)
            Append(v);
    }

    /// Appends one element to the column.
    void Append(string_view str);

    /// Returns element at given row number.
    string_view At(size_t n) const;

    /// Returns element at given row number.
    string_view operator [] (size_t n) const;

    /// Returns the max size of the fixed string
    size_t FixedSize() const;

public:
    /// Appends content of given column to the end of current one.
    void Append(ColumnRef column) override;

    /// Loads column data from input stream.
    bool Load(CodedInputStream* input, size_t rows) override;

    /// Saves column data to output stream.
    void Save(CodedOutputStream* output) override;

    /// Clear column data .
    void Clear() override;

    /// Returns count of rows in the column.
    size_t Size() const override;

    /// Makes slice of the current column.
    ColumnRef Slice(size_t begin, size_t len) override;

    void Swap(Column& other) override;

    ItemView GetItem(size_t) const override;

private:
    size_t string_size_;
    std::string data_;
};

/**
 * Represents column of variable-length strings.
 */
class ColumnString : public Column {
public:
    // Type this column takes as argument of Append and returns with At() and operator[]
    using ValueType = string_view;

    ColumnString();
    ~ColumnString();

    explicit ColumnString(const std::vector<std::string> & data);
    ColumnString& operator=(const ColumnString&) = delete;
    ColumnString(const ColumnString&) = delete;

    /// Appends one element to the column.
    void Append(string_view str);

    /// Returns element at given row number.
    string_view At(size_t n) const;

    /// Returns element at given row number.
    string_view operator [] (size_t n) const;

public:
    /// Appends content of given column to the end of current one.
    void Append(ColumnRef column) override;

    /// Loads column data from input stream.
    bool Load(CodedInputStream* input, size_t rows) override;

    /// Saves column data to output stream.
    void Save(CodedOutputStream* output) override;

    /// Clear column data .
    void Clear() override;

    /// Returns count of rows in the column.
    size_t Size() const override;

    /// Makes slice of the current column.
    ColumnRef Slice(size_t begin, size_t len) override;
    void Swap(Column& other) override;
    ItemView GetItem(size_t) const override;

private:
    void AppendUnsafe(string_view);

private:
    struct Block;

    std::vector<string_view> items_;
    std::vector<Block> blocks_;
};

}
