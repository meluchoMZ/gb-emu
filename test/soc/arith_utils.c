/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "../../src/soc/arith_utils.h"

#include <ctest_api.h>

/** 8 bit carry utils **/

TEST(CarryUtils, compute8BitCarryCarries, "Computes 8 bit carry that should generate carry bit")
{
	uint8_t a = 0xFF;
	uint8_t b = 0xFE;
	ASSERT_TRUE(compute8BitCarry(a, b));
}

TEST(CarryUtils, compute8BitCarryDoesntCarry, "Computes 8 bit carry that should not generate carry bit")
{
	uint8_t a = 0x0F;
	uint8_t b = 0xEE;
	ASSERT_FALSE(compute8BitCarry(a, b));
}

TEST(CarryUtils, compute8BitCarryMaxU8IntSum, "Computes 8 bit carry between the maximum values")
{
	uint8_t a = 0xFF;
	ASSERT_TRUE(compute8BitCarry(a, a));
}

TEST(CarryUtils, compute8BitCarrySumOfTwoHalfSizes, "Computes 8 bit carry between u8 maxInt/2")
{
	uint8_t a = 0x80;
	ASSERT_TRUE(compute8BitCarry(a, a));
}

TEST(CarryUtils, compute8BitCarryTotalMaxint, "Computes 8 bit carry between maxint -1 and 1")
{
	uint8_t a = 0xFE;
	uint8_t b = 0x01;
	ASSERT_FALSE(compute8BitCarry(a, b));
}

TEST(CarryUtils, compute8BitHalfCarryCarries, "Computes 8 bit sumn that should generate a half carry bit")
{
	uint8_t a = 0x08;
	ASSERT_TRUE(compute8BitHalfCarry(a, a));
}

TEST(CarryUtils, compute8BitHalfCarryDoesntCarry, "Computes 8 bit sum that should not generate a half carry bit")
{
	uint8_t a = 0x0F;
	uint8_t b = 0x00;
	ASSERT_FALSE(compute8BitHalfCarry(a, b));
}

TEST(CarryUtils, compute8BitHalfCarryBiggerThan16, "Computes 8 bit sum that should generate a half carry bit when both operands are bigger than 0x10")
{
	uint8_t a = 0x2A;
	uint8_t b = 0x17;
	ASSERT_TRUE(compute8BitHalfCarry(a, b));
}

TEST(CarryUtils, compute8BitBorrowingBorrows, "Computes 8 bit substraction that should generate bit borrowing")
{
	uint8_t a = 0x02;
	uint8_t b = 0x05;
	ASSERT_TRUE(compute8BitBorrowing(a, b));
}

TEST(CarryUtils, compute8BitBorrowingBorrowsZeroAndOne, "Computes 8 bit substraction that should generate bit borrowing borrowing from zero")
{
	uint8_t a = 0x00;
	uint8_t b = 0x01;
	ASSERT_TRUE(compute8BitBorrowing(a, b));
}

TEST(CarryUtils, compute8BitBorrowingDoesntBorrow, "Computes 8 bit substraction that should not generate bit borrowing")
{
	uint8_t a = 0x06;
	uint8_t b = 0x02;
	ASSERT_FALSE(compute8BitBorrowing(a, b));
}

TEST(CarryUtils, compute8BitBorrowingDoesntBorrowZero, "Computes 8 bit substraction that shoudl not generate bit borrworing when substracting zero")
{
	uint8_t a = 0x03;
	uint8_t b = 0x00;
	ASSERT_FALSE(compute8BitBorrowing(a, b));
}

TEST(CarryUtils, compute8BitHalfBitBorrowingBorrows, "Computes 8 bit substraction that should generate half bit borrowing")
{
	uint8_t a = 0x12;
	uint8_t b = 0x05;
	ASSERT_TRUE(compute8BitHalfBitBorrowing(a, b));
}

TEST(CarryUtils, compute8BitHalfBitBorrowringBorrowsZeroedLowHalf, "Computes 8 bit substraction that should generate half bit borrowing when the second hex digit is zero")
{
	uint8_t a = 0x20;
	uint8_t b = 0x01;
	ASSERT_TRUE(compute8BitHalfBitBorrowing(a, b));
}

TEST(CarryUtils, compute8BitHalfBitBorrowingDoesntBorrow, "Computes 8 bit substraction that should not generate half bit borrowing")
{
	uint8_t a = 0x28;
	uint8_t b = 0x13;
	ASSERT_FALSE(compute8BitHalfBitBorrowing(a, b));
}

/** 16 bit carry utils **/

TEST(CarryUtils, compute16BitCarryCarries, "Computes 16 bit carry that should generate carry bit")
{
	uint16_t a = 0xF0F0;
	uint16_t b = 0xF0E0;
	ASSERT_TRUE(compute16BitCarry(a, b));
}

TEST(CarryUtils, compute16BitCarryDoesntCarry, "Computes 16 bit carry that should not generate carry bit")
{
	uint16_t a = 0x00F0;
	uint16_t b = 0xE0E0;
	ASSERT_FALSE(compute16BitCarry(a, b));
}

TEST(CarryUtils, compute16BitCarryMaxU16IntSum, "Computes 16 bit carry between the maximum values")
{
	uint16_t a = 0xFFFF;
	ASSERT_TRUE(compute16BitCarry(a, a));
}

TEST(CarryUtils, compute16BitCarrySumOfTwoHalfSizes, "Computes 16 bit carry between u16 maxInt/2")
{
	uint16_t a = 0x8000;
	ASSERT_TRUE(compute16BitCarry(a, a));
}

TEST(CarryUtils, compute16BitCarryTotalMaxint, "Computes 16 bit carry between maxint -1 and 1")
{
	uint16_t a = 0xFFFE;
	uint16_t b = 0x0001;
	ASSERT_FALSE(compute16BitCarry(a, b));
}

TEST(CarryUtils, compute16BitHalfCarryCarries, "Computes 16 bit sumn that should generate a half carry bit")
{
	uint16_t a = 0x0080;
	ASSERT_TRUE(compute16BitHalfCarry(a, a));
}

TEST(CarryUtils, compute16BitHalfCarryDoesntCarry, "Computes 16 bit sum that should not generate a half carry bit")
{
	uint16_t a = 0x00F0;
	uint16_t b = 0x0000;
	ASSERT_FALSE(compute16BitHalfCarry(a, b));
}

TEST(CarryUtils, compute16BitHalfCarryBiggerThan4096, "Computes 16 bit sum that should generate a half carry bit when both operands are bigger than 0x1000")
{
	uint16_t a = 0x20A0;
	uint16_t b = 0x1070;
	ASSERT_TRUE(compute16BitHalfCarry(a, b));
}

TEST(CarryUtils, compute16BitBorrowingBorrows, "Computes 16 bit substraction that should generate bit borrowing")
{
	uint16_t a = 0x0020;
	uint16_t b = 0x0050;
	ASSERT_TRUE(compute16BitBorrowing(a, b));
}

TEST(CarryUtils, compute16BitBorrowingBorrowsZeroAndOne, "Computes 16 bit substraction that should generate bit borrowing borrowing from zero")
{
	uint16_t a = 0x0000;
	uint16_t b = 0x0010;
	ASSERT_TRUE(compute16BitBorrowing(a, b));
}

TEST(CarryUtils, compute16BitBorrowingDoesntBorrow, "Computes 16 bit substraction that should not generate bit borrowing")
{
	uint16_t a = 0x0060;
	uint16_t b = 0x0020;
	ASSERT_FALSE(compute16BitBorrowing(a, b));
}

TEST(CarryUtils, compute16BitBorrowingDoesntBorrowZero, "Computes 16 bit substraction that shoudl not generate bit borrworing when substracting zero")
{
	uint8_t a = 0x0030;
	uint8_t b = 0x0000;
	ASSERT_FALSE(compute16BitBorrowing(a, b));
}

TEST(CarryUtils, compute16BitHalfBitBorrowingBorrows, "Computes 16 bit substraction that should generate half bit borrowing")
{
	uint16_t a = 0x1020;
	uint16_t b = 0x0050;
	ASSERT_TRUE(compute16BitHalfBitBorrowing(a, b));
}

TEST(CarryUtils, compute16BitHalfBitBorrowringBorrowsZeroedLowHalf, "Computes 16 bit substraction that should generate half bit borrowing when the second hex digit is zero")
{
	uint16_t a = 0x0200;
	uint16_t b = 0x0010;
	ASSERT_TRUE(compute16BitHalfBitBorrowing(a, b));
}

TEST(CarryUtils, compute16BitHalfBitBorrowingDoesntBorrow, "Computes 16 bit substraction that should not generate half bit borrowing")
{
	uint16_t a = 0x0208;
	uint16_t b = 0x0103;
	ASSERT_FALSE(compute16BitHalfBitBorrowing(a, b));
}
