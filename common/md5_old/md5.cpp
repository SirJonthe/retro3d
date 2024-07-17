// md5.cpp
// github.com/SirJonthe
// 2019

// Public domain

// THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#include "md5.h"

std::string md5(const std::string &message)
{
	return md5sum(message).hex();
}

md5sum::md5sum( void )
{
	for (uint32_t i = 0; i < 16; ++i) {
		m_digest[i] = 0;
	}
}

md5sum::md5sum(const std::string &message)
{
	calc_sum(message);
}

md5sum::md5sum(const char *message, uint64_t byte_size)
{
	calc_sum(message, byte_size);
}

md5sum &md5sum::operator=(const std::string &message)
{
	calc_sum(message);
	return *this;
}

uint32_t md5sum::leftrotate(uint32_t x, uint32_t c)
{
    return (x << c) | (x >> (32 - c));
}

void md5sum::calc_sum(std::string message)
{
	constexpr uint32_t CHUNK_BYTESIZE = 512 / CHAR_BIT;

	// Table of shift offsets
	constexpr uint32_t ShiftTable[CHUNK_BYTESIZE] = {
		7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
		5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
		4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
		6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
	};

	// Precomputed table of integer sines (in radians)
	constexpr uint32_t SineTable[CHUNK_BYTESIZE] = {
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	};

	// Initialization
	uint32_t *X = reinterpret_cast<uint32_t*>(m_digest);
	enum {a0,b0,c0,d0};
	X[a0] = 0x67452301; // A
	X[b0] = 0xefcdab89; // B
	X[c0] = 0x98badcfe; // C
	X[d0] = 0x10325476; // D

	// Compute padding.
	const uint64_t OriginalMessageSize = message.size();
	uint32_t PaddingSize = CHUNK_BYTESIZE - (OriginalMessageSize % CHUNK_BYTESIZE);
	if (PaddingSize < sizeof(uint64_t)) { // Padding must at least fit 64-bit number of bits of input message
		PaddingSize += CHUNK_BYTESIZE;
	}
	const uint64_t MessageSize = OriginalMessageSize + PaddingSize;
	
	// Resize the message if needed, all trailing zeroes.
	message.resize(MessageSize, 0);
	
	// If message is padded, add a first '1' to the padding.
	if (PaddingSize > 0) {
		message[OriginalMessageSize] = 0x80;
	}

	// Store size (64 bits) of original message in bits at the end of the message
	const uint64_t OriginalMessageBitSize = (OriginalMessageSize * CHAR_BIT);
	for (uint32_t i = 0; i < sizeof(uint64_t); ++i) {
		message[MessageSize - sizeof(uint64_t) + i] = reinterpret_cast<const char*>(&OriginalMessageBitSize)[i];
	}

	// Process every 512-bit chunk
	for (uint32_t chunk = 0; chunk < message.size(); chunk += CHUNK_BYTESIZE) {
		const uint32_t *M = reinterpret_cast<const uint32_t*>(message.c_str() + chunk);

		uint32_t A = X[a0];
		uint32_t B = X[b0];
		uint32_t C = X[c0];
		uint32_t D = X[d0];
		
		// Process every 64 bytes in chunk
		for (uint32_t i = 0; i < CHUNK_BYTESIZE; ++i) {
			uint32_t F = 0;
			uint32_t g = 0;
			if (i < 16) {
				F = (B & C) | ((~B) & D);
				g = i;
			} else if (i < 32) {
				F = (D & B) | ((~D) & C);
				g = (5*i + 1) % 16;
			} else if (i < 48) {
				F = B ^ C ^ D;
				g = (3*i + 5) % 16;
			} else {
				F = C ^ (B | (~D));
				g = (7*i) % 16;
			}
			
			F = F + A + SineTable[i] + M[g];
			A = D;
			D = C;
			C = B;
			B += leftrotate(F, ShiftTable[i]);
		}
		
		// Add to result
		X[a0] += A;
		X[b0] += B;
		X[c0] += C;
		X[d0] += D;
	}
}

void md5sum::calc_sum(const char *message, uint64_t byte_size)
{
	calc_sum(std::string(message, size_t(byte_size)));
}

std::string md5sum::hex( void ) const
{
	char out[33];
	for (uint32_t i = 0; i < 16; ++i) {
		sprintf(out + (i*2), "%02x", m_digest[i]);
	}
	out[32] = '\0';
	return std::string(out);
}

bool md5sum::operator==(const md5sum &r) const
{
	for (uint32_t i = 0; i < 16; ++i) {
		if (m_digest[i] != r.m_digest[i]) { return false; }
	}
	return true;
}

bool md5sum::operator!=(const md5sum &r) const
{
	for (uint32_t i = 0; i < 16; ++i) {
		if (m_digest[i] == r.m_digest[i]) { return false; }
	}
	return true;
}

md5sum::operator const uint8_t*( void ) const
{
	return m_digest;
}

