#include <sstream>
#include "../common/MiniLib/MML/mmlMath.h"
#include "retro_reader.h"
#include <iostream>

void retro3d::Reader::UpdatePath(const std::string &file)
{
	m_file = file;

	size_t dir_sep1 = file.find_last_of('\\');
	size_t dir_sep2 = file.find_last_of('/');
	size_t dir_sep = mmlMax(dir_sep1, dir_sep2);
	if (dir_sep == std::string::npos) {
		dir_sep = mmlMin(dir_sep1, dir_sep2);
	}
	m_path = (dir_sep != std::string::npos) ? file.substr(0, dir_sep + 1) : "";
}

retro3d::Reader::Reader( void ) : m_in(nullptr), m_file(""), m_path(""), m_is_open(false)
{}

retro3d::Reader::~Reader( void )
{
	delete m_in;
}

bool retro3d::Reader::ReadFromFile(const std::string &file)
{
	UpdatePath(file);
	delete m_in;
	std::ifstream *fin = new std::ifstream;
	fin->open(file.c_str());
	m_in = fin;
	m_is_open = (fin->is_open() == true);
	if (m_is_open == false) {
		delete m_in;
		m_in = nullptr;
	}
	return m_is_open;
}

void retro3d::Reader::ReadFromMemory(const char *buf, uint64_t byte_count, const std::string &virtual_file)
{
	UpdatePath(virtual_file);
	delete m_in;
	std::istringstream *sin = new std::istringstream;
	sin->str() = std::string(buf, byte_count);
	m_in = sin;
	m_is_open = true;
}

void retro3d::Reader::Close( void )
{
	delete m_in;
	m_in = nullptr;
	m_file = "";
	m_path = "";
	m_is_open = false;
}

uint64_t retro3d::Reader::ReadBin(char *out, uint64_t byte_count)
{
	if (m_in == nullptr) { return 0; }
	const bool readstate = m_in->read(out, uint32_t(byte_count)) ? true : false;
	return readstate == true ? byte_count : (IsEnd() ? uint64_t(m_in->gcount()) : 0);
}

bool retro3d::Reader::ReadLine(std::string &out)
{
	if (m_in == nullptr) { return false; }
	return std::getline(*m_in, out) ? true : false;
}

bool retro3d::Reader::ReadWord(std::string &out)
{
	if (m_in == nullptr) { return false; }
	return ((*m_in) >> out) ? true : false;
}

bool retro3d::Reader::ReadChar(char &out)
{
	if (m_in == nullptr) { return false; }
	return ((*m_in) >> out) ? true : false;
}

bool retro3d::Reader::IsOpen( void ) const
{
	return m_is_open;
}

bool retro3d::Reader::IsEnd( void ) const
{
	return m_in != nullptr ? m_in->eof() : true;
}

const std::string &retro3d::Reader::GetFile( void ) const
{
	return m_file;
}

const std::string &retro3d::Reader::GetPath( void ) const
{
	return m_path;
}
