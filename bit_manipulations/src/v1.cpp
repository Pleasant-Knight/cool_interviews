#include <cstdint>
#include <iostream>

/** Note: uint_fast8_t is unsigned char */
constexpr std::uint_fast16_t mask0{ 0b0000'0001 }; // represents bit 0
constexpr std::uint_fast16_t mask1{ 0b0000'0010 }; // represents bit 1
constexpr std::uint_fast16_t mask2{ 0b0000'0100 }; // represents bit 2 
constexpr std::uint_fast16_t mask3{ 0b0000'1000 }; // represents bit 3
constexpr std::uint_fast16_t mask4{ 0b0001'0000 }; // represents bit 4
constexpr std::uint_fast16_t mask5{ 0b0010'0000 }; // represents bit 5
constexpr std::uint_fast16_t mask6{ 0b0100'0000 }; // represents bit 6
constexpr std::uint_fast16_t mask7{ 0b1000'0000 }; // represents bit 7

/** create mask from bit position i to bit position b and add b into a. */
uint32_t createMask(uint32_t a, uint32_t b, int i, int j) {
	uint32_t mask = 0xFFFFFFFF;
	std::cout << std::hex << std::uppercase;
	// clear bits from 0 to j
	mask >>= j;
	mask <<= j;
	std::cout << "mask_0 is 0X" << (mask) << " and ~(mask_0) is 0X" << ~(mask) << "\n";
	// clear bits from i + 1 to 31
	mask <<= (31 - i); // if you dont want i's bit, then use 32 instead of 31
	mask >>= (31 - i); // if you dont want i's bit, then use 32 instead of 31
	std::cout << "mask_1 is 0X" << (mask) << " and ~(mask_1) is 0X" << ~(mask) << "\n";

	auto t = ((b & mask));

	std::cout << "temp is 0X" << t << "\n";
	t |= a;
	std::cout << "result is 0X" << t << "\n";
	
	std::cin.get();
	return t;
}



int main(int argc, char *argv[]) {
	std::uint_fast8_t flags{ 0b0000'0101 }; // 8 bits in size means room for 8 flags
		
	//std::cout << "bit 0 is " << ((flags & mask0) ? "on\n" : "off\n");
	//std::cout << "bit 1 is " << ((flags & mask1) ? "on\n" : "off\n");
	
	//std::cout << "mask_1 is " << (uint8_t)(mask1) << " and ~(mask_1) is " << (uint8_t)~(mask1) << "\n";
	std::cout << std::hex << std::uppercase;
	//std::cout << "mask_1 is 0X" << (mask1) << " and ~(mask_1) is 0X" << ~(mask1) << "\n";

	uint32_t A = 0XCD00FC25, B = 0XF02173AF;
	int i = 7, j = 3;
	auto m = createMask(A, B, i, j);

	
	
	
	
	//std::cin.get();
	
	return 0;
}