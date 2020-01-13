#ifndef RETRO_READER_H
#define RETRO_READER_H

#include <cstdint>
#include <string>
#include <fstream>

namespace retro3d
{

class Reader
{
private:
	std::istream *m_in;
	std::string   m_file;
	std::string   m_path;
	bool          m_is_open;

private:
	void UpdatePath(const std::string &file);

public:
	Reader( void );
	~Reader( void );

	bool ReadFromFile(const std::string &file);
	void ReadFromMemory(const char *buf, uint64_t byte_count, const std::string &virtual_file = "");
	void Close( void );

	void SkipTo(uint64_t byte_loc);
	uint64_t ReadBin(char *out, uint64_t byte_count);
	bool ReadLine(std::string &out);
	bool ReadWord(std::string &out);
	bool ReadChar(char &out);
	template < typename type_t >
	bool Read(type_t &out);

	bool IsOpen( void ) const;
	bool IsEnd( void ) const;

	const std::string &GetFile( void ) const;
	const std::string &GetPath( void ) const;
};

}

template < typename type_t >
bool retro3d::Reader::Read(type_t &out)
{
	if (m_in == nullptr) { return false; }
	return ((*m_in) >> out) ? true : false;
}

#endif // RETRO_READER_H
