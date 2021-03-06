#include "ds/util/string_util.h"

#include <sstream>
#include <fstream>
#include <codecvt>

using namespace std;
using namespace ds;

/* WIN32
 ******************************************************************/
#ifdef WIN32
#include <windows.h>
#include <time.h>
#include <iostream>

std::wstring wstr_from_str(const std::string& str, const UINT cp)
{
	if (str.empty()) return wstring();

	const int len = MultiByteToWideChar(cp, 0, str.c_str(), str.length(), 0, 0);
	if (!len) throw conversion_error();

	std::vector<wchar_t> wbuff(len + 1);
	// NOTE: this does not NULL terminate the string in wbuff, but this is ok
	//       since it was zero-initialized in the vector constructor
	if (!MultiByteToWideChar(cp, 0, str.c_str(), str.length(), &wbuff[0], len)) throw conversion_error();

	return &wbuff[0];
}

std::string str_from_wstr(const std::wstring& wstr, const UINT cp)
{
	if (wstr.empty()) return string();

	const int len = WideCharToMultiByte(cp, 0, wstr.c_str(), wstr.length(), 0, 0, 0, 0);
	if (!len) throw conversion_error();

	std::vector<char> abuff(len + 1);

	// NOTE: this does not NULL terminate the string in abuff, but this is ok
	//       since it was zero-initialized in the vector constructor
	if (!WideCharToMultiByte(cp, 0, wstr.c_str(), wstr.length(), &abuff[0], len, 0, 0)) throw conversion_error();

	return &abuff[0];
}

std::wstring		ds::wstr_from_utf8(const std::string& str)
{
	return wstr_from_str(str, CP_UTF8);
}

std::string			ds::utf8_from_wstr(const std::wstring& wstr)
{
	return str_from_wstr(wstr, CP_UTF8);
}


std::wstring ds::iso_8859_1_string_to_wstring(const std::string& input){
	// you may notice that this implementation is different than the reverse.
	// good eye.
	// wstring and string conversion is A TOTAL MOTHERFUCKER.
	// This way seems to keep some special characters, like accent marks.
	// the other way seems to throw A SILENT EXCEPTION FOR NO FUCKING REASON AND JUST SKIPS ON OUT OF YOUR CODE
	// LIKE A MOTHER FUCKER AND DOESN'T DO SHIT CAUSE WHAT THE FUCK.
	// So, yeah. 
	// Here you go.
	std::wostringstream conv;
	conv << input.c_str();
	return conv.str();
}

std::string ds::iso_8859_1_wstring_to_string(const std::wstring& input){
	typedef std::codecvt_utf8<wchar_t> convert_typeX;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(input);
}


#else
/* Linux equivalent?
 ******************************************************************/
std::wstring		ds::wstr_from_utf8(const std::string& src) {
	std::wstring dest;

	wchar_t w = 0;
	int bytes = 0;
	wchar_t err = L'?';
	for ( size_t i = 0; i < src.size(); i++ ) {
		unsigned char c = ( unsigned char )src[i];
		if ( c <= 0x7f ) { //first byte
			if ( bytes ) {
				dest.push_back( err );
				bytes = 0;
			}
			dest.push_back( ( wchar_t )c );
		}
		else if ( c <= 0xbf ) { //second/third/etc byte
			if ( bytes ) {
				w = ( ( w << 6 ) | ( c & 0x3f ) );
				bytes--;
				if ( bytes == 0 )
					dest.push_back( w );
			}
			else
				dest.push_back( err );
		}
		else if ( c <= 0xdf ) { //2byte sequence start
			bytes = 1;
			w = c & 0x1f;
		}
		else if ( c <= 0xef ) { //3byte sequence start
			bytes = 2;
			w = c & 0x0f;
		}
		else if ( c <= 0xf7 ) { //3byte sequence start
			bytes = 3;
			w = c & 0x07;
		}
		else {
			dest.push_back( err );
			bytes = 0;
		}
	}
	if ( bytes )
		dest.push_back( err );


	return dest;
}


std::string		ds::utf8_from_wstr(const std::wstring& src) {
	std::string dest;

	for ( size_t i = 0; i < src.size(); i++ ) {
		wchar_t w = src[i];
		if ( w <= 0x7f )
			dest.push_back( ( char )w );
		else if ( w <= 0x7ff ) {
			dest.push_back( 0xc0 | ( ( w >> 6 ) & 0x1f ) );
			dest.push_back( 0x80 | ( w & 0x3f ) );
		}
		else if ( w <= 0xffff ) {
			dest.push_back( 0xe0 | ( ( w >> 12 ) & 0x0f ) );
			dest.push_back( 0x80 | ( ( w >> 6 ) & 0x3f ) );
			dest.push_back( 0x80 | ( w & 0x3f ) );
		}
		else if ( w <= 0x10ffff ) {
			dest.push_back( 0xf0 | ( ( w >> 18 ) & 0x07 ) );
			dest.push_back( 0x80 | ( ( w >> 12 ) & 0x3f ) );
			dest.push_back( 0x80 | ( ( w >> 6 ) & 0x3f ) );
			dest.push_back( 0x80 | ( w & 0x3f ) );
		}
		else
			dest.push_back( '?' );
	}

	return dest;
}



#endif

//variation on a function found on stackoverflow
std::vector<std::string> ds::split( const std::string &str, const std::string &delimiters, bool dropEmpty )
{
	std::size_t pos;
	std::size_t lastPos = 0;
	std::vector<std::string> splitWords;

	while ( true )
	{
		pos = str.find( delimiters, lastPos );
		if ( pos == std::string::npos )
		{
			pos = str.length();

			if ( pos != lastPos || !dropEmpty )
				splitWords.push_back( std::string( str.data() + lastPos, pos-lastPos ) );

			break;
		}
		else
		{
			if ( pos != lastPos || !dropEmpty ) {
			  std::string tstr = std::string( str.data() + lastPos, pos-lastPos );
			  if (!tstr.empty())
				splitWords.push_back( tstr );
			  lastPos = pos + delimiters.size();
			  continue;
			}
		}

		lastPos = pos + 1;
	}

	return splitWords;
}

std::vector<std::wstring> ds::split( const std::wstring &str, const std::wstring &delimiters, bool dropEmpty )
{
	std::size_t pos;
	std::size_t lastPos = 0;
	std::vector<std::wstring> splitWords;

	while ( true )
	{
		pos = str.find( delimiters, lastPos );
		if ( pos == std::wstring::npos )
		{
			pos = str.length();

			if ( pos != lastPos || !dropEmpty )
				splitWords.push_back( std::wstring( str.data() + lastPos, pos-lastPos ) );

			break;
		}
		else
		{
			if ( pos != lastPos || !dropEmpty )
			{
				splitWords.push_back( std::wstring( str.data() + lastPos, pos-lastPos/* + delimiters.size()*/ ) );
				lastPos = pos + delimiters.size();
				continue;
			}
		}

		lastPos = pos + 1;
	}

	return splitWords;
}

std::vector<std::string> ds::partition( const std::string &str, const std::string &partitioner )
{
  std::vector<std::string> partitions;

  std::size_t pos;
  std::size_t lastPos = 0;

  while (lastPos != std::string::npos) {
	pos = str.find_first_of(partitioner, lastPos);
	if (pos != std::string::npos) {
	  if (pos == lastPos) {
		partitions.push_back(std::string(str.data() + lastPos, pos-lastPos+partitioner.size()));
		lastPos += partitioner.size();
		continue;
	  } else {
		partitions.push_back(std::string(str.data() + lastPos, pos-lastPos));
		partitions.push_back(partitioner);
		lastPos = pos;
		lastPos += partitioner.size();
		continue;
	  }
	} else {
	  pos = str.length();
	  if (pos != lastPos)
		partitions.push_back(std::string(str.data() + lastPos, pos-lastPos));
	  break;
	}
	lastPos += 1;
  }

  return partitions;
}

std::vector<std::string> ds::partition( const std::string &str, const std::vector<std::string> &partitioners )
{
  std::vector<std::string> partitions;
  partitions.push_back(str);

  for (auto it = partitioners.begin(), it2 = partitioners.end(); it != it2; ++it) {
	std::vector<std::string> tPartitions;
	for (auto itt = partitions.begin(), itt2 = partitions.end(); itt != itt2; ++itt) {
	  std::vector<std::string> splitWords = ds::partition(*itt, *it);

	  for (auto ittt = splitWords.begin(), ittt2 = splitWords.end(); ittt != ittt2; ++ittt) {
		tPartitions.push_back(*ittt);
	  }
	}
	partitions = tPartitions;
  }

  return partitions;
}

//std::vector<std::wstring> ds::partition( const std::wstring &str, const std::wstring &partitioner )
//{
//  std::vector<std::wstring> partitions;
//
//  std::size_t pos;
//  std::size_t lastPos = 0;
//
//  while (lastPos != std::wstring::npos) {
//    pos = str.find_first_of(partitioner, lastPos);
//    if (pos != std::wstring::npos) {
//      if (pos == lastPos) {
//        partitions.push_back(std::wstring(str.data() + lastPos, pos-lastPos+partitioner.size()));
//        lastPos += partitioner.size();
//        continue;
//      } else {
//        partitions.push_back(std::wstring(str.data() + lastPos, pos-lastPos));
//        partitions.push_back(partitioner);
//        lastPos = pos;
//        lastPos += partitioner.size();
//        continue;
//      }
//    } else {
//      pos = str.length();
//      if (pos != lastPos)
//        partitions.push_back(std::wstring(str.data() + lastPos, pos-lastPos));
//      break;
//    }
//    lastPos += 1;
//  }
//
//  return partitions;
//}

namespace {

bool strEqual(const wchar_t *str1, const wchar_t *str2, int size)
{
  for (int i = 0; i < size; ++i) {
	if (str1[i] != str2[i])
	  return false;
  }
  return true;
}

}

void ds::partition( const std::wstring &str, const std::wstring &partitioner, const Token &token, std::vector<Token> &partitions )
{
  int lastPos = token.pos;
  const int size = token.pos + token.size - (partitioner.size()-1);
  for (int i = token.pos; i < size; ++i) {
	if (strEqual(&str[i], partitioner.c_str(), partitioner.size())) {
	  if (i - lastPos > 0)
		partitions.push_back(Token(lastPos, i - lastPos));
	  partitions.push_back(Token(i, partitioner.size()));
	  lastPos = i + partitioner.size();
	}
  }
  if (lastPos != token.pos + token.size)
	partitions.push_back(Token(lastPos, token.pos + token.size - lastPos));
}

std::vector<std::wstring> ds::partition( const std::wstring &str, const std::vector<std::wstring> &partitioners )
{
  //clock_t start = clock();
  std::vector<Token> partitions;
  partitions.push_back(Token(0, str.size()));

  std::vector<Token> tPartitions;
  for (auto it = partitioners.begin(), it2 = partitioners.end(); it != it2; ++it) {
	for (auto itt = partitions.begin(), itt2 = partitions.end(); itt != itt2; ++itt) {
	  partition(str, *it, *itt, tPartitions);
	}
	partitions = tPartitions;
	tPartitions.clear();
  }

  std::vector<std::wstring> tokenPartitions;
  for (size_t i = 0; i < partitions.size(); ++i) {
	tokenPartitions.push_back(str.substr(partitions[i].pos, partitions[i].size));
  }

  //std::cout << "time taken: " << (double)(clock() - start) / CLOCKS_PER_SEC << std::endl;
  return tokenPartitions;
}
//std::vector<std::wstring> ds::partition( const std::wstring &str, const std::wstring &partitioner )
//{
//  std::vector<std::wstring> partitions;
//
//  int lastPos = 0;
//  const int size = str.size() - (partitioner.size()-1);
//  for (int i = 0; i < size; ++i) {
//    if (strEqual(&str[i], partitioner.c_str(), partitioner.size())) {
//      if (i - lastPos > 0)
//        partitions.push_back(str.substr(lastPos, i - lastPos));
//      partitions.push_back(str.substr(i, partitioner.size()));
//      lastPos = i + partitioner.size();
//    }
//  }
//  if (lastPos != str.size())
//    partitions.push_back(str.substr(lastPos, str.size() - lastPos));
//
//  return partitions;
//}
//
//std::vector<std::wstring> ds::partition( const std::wstring &str, const std::vector<std::wstring> &partitioners )
//{
//  std::vector<std::wstring> partitions;
//  partitions.push_back(str);
//
//  for (auto it = partitioners.begin(), it2 = partitioners.end(); it != it2; ++it) {
//    std::vector<std::wstring> tPartitions;
//    for (auto itt = partitions.begin(), itt2 = partitions.end(); itt != itt2; ++itt) {
//      std::vector<std::wstring> splitWords = ds::partition(*itt, *it);
//
//      for (auto ittt = splitWords.begin(), ittt2 = splitWords.end(); ittt != ittt2; ++ittt) {
//        tPartitions.push_back(*ittt);
//      }
//    }
//    partitions = tPartitions;
//  }
//
//  return partitions;
//}

int ds::find_count( const std::string &str, const std::string &token )
{
	std::size_t pos;
	std::size_t lastPos = 0;
	int count = 0;

	while ( true )
	{
		pos = str.find_first_of( token, lastPos );
		if ( pos == std::string::npos )
		{
			break;
		}
		
		++count;
		lastPos = pos + 1;
	}

	return count;
}

int ds::find_count( const std::wstring &str, const std::wstring &token )
{
	std::size_t pos;
	std::size_t lastPos = 0;
	int count = 0;

	while ( true )
	{
		pos = str.find_first_of( token, lastPos );
		if ( pos == std::wstring::npos )
		{
			break;
		}

		++count;
		lastPos = pos + 1;
	}

	return count;
}

void ds::replace( std::string &str, const std::string &oldToken, const std::string &newToken )
{
	std::size_t pos;
	std::size_t lastPos = 0;
	std::string tStr = str;
	str.clear();

	while ( true )
	{
		pos = tStr.find( oldToken, lastPos );
		if ( pos == std::string::npos )
		{
			pos = tStr.length();

			if ( pos != lastPos )
			{
				str += std::string( tStr.data() + lastPos, pos-lastPos );
				//str += newToken;
			}

			break;
		}
		else
		{
			str += std::string( tStr.data() + lastPos, pos-lastPos );
			str += newToken;
			lastPos = pos + oldToken.size();
			continue;
		}

		lastPos = pos + 1;
	}
}

void ds::replace( std::wstring &str, const std::wstring &oldToken, const std::wstring &newToken )
{
	std::size_t pos;
	std::size_t lastPos = 0;
	std::wstring tStr = str;
	str.clear();

	while ( true )
	{
		pos = tStr.find( oldToken, lastPos );
		if ( pos == std::wstring::npos )
		{
			pos = tStr.length();

			if ( pos != lastPos )
			{
				str += std::wstring( tStr.data() + lastPos, pos-lastPos );
				//str += newToken;
			}

			break;
		}
		else
		{
			if ( pos != lastPos )
			{
				str += std::wstring( tStr.data() + lastPos, pos-lastPos );
				str += newToken;
				lastPos = pos + oldToken.size();
				continue;
			}
		}

		lastPos = pos + 1;
	}
}

void ds::loadFileIntoString( const std::string &filename, std::string &destination )
{
	std::fstream filestr;
	filestr.open( filename.c_str(), std::fstream::in );
	if ( filestr.is_open() )
	{
		filestr.seekg( 0, std::fstream::end );
		unsigned count = static_cast<unsigned>(filestr.tellg());
		filestr.seekg( 0, std::fstream::beg );
		char *str = new char[count];
		if ( str )
		{
			filestr.read( str, count );
			count = static_cast<unsigned>(filestr.gcount());
			destination = std::string( str, count );
			//destination += '\0';
			delete [] str;
		}
		filestr.close();
	}
}

void ds::loadFileIntoString( const std::wstring &filename, std::wstring &destination )
{
	std::wfstream filestr;
	filestr.open( ds::utf8_from_wstr(filename).c_str(), std::wfstream::in );
	if ( filestr.is_open() )
	{
		filestr.seekg( 0, std::wfstream::end );
		unsigned count = static_cast<unsigned>(filestr.tellg());
		filestr.seekg( 0, std::wfstream::beg );
		wchar_t *str = new wchar_t[count];
		if ( str )
		{
			filestr.read( str, count );
			count = static_cast<unsigned>(filestr.gcount());
			destination = std::wstring( str, count );
			//destination += L'\0';
			delete [] str;
		}
		filestr.close();
	}
}

void ds::loadFileIntoStringByLine(const std::string& filename, const std::function<void(const std::string& line)>& func)
{
	if (!func) return;

//	std::cout << "Reading file " << filename << std::endl;
	std::string					f;
	ds::loadFileIntoString(filename, f);
	std::istringstream			lineBuf(f);
	if (f.empty()) {
//		std::cout << "\tERROR file does not exist or is empty" << std::endl;
		return;
	}

	while (lineBuf.good()) {
		std::string				out;
		getline(lineBuf, out);
		if (!out.empty()) {
//			std::cout << "\tsubscribe to " << out << std::endl;
			func(out);
		}
	}
}

void ds::loadFileIntoStringByLine(const std::wstring& filename, const std::function<void(const std::wstring& line)>& func)
{
	if (!func) return;

	//	std::cout << "Reading file " << filename << std::endl;
	std::wstring				f;
	ds::loadFileIntoString(filename, f);
	std::wistringstream			lineBuf(f);
	if (f.empty()) {
		//		std::cout << "\tERROR file does not exist or is empty" << std::endl;
		return;
	}

	while (lineBuf.good()) {
		std::wstring			out;
		getline(lineBuf, out);
		if (!out.empty()) {
			//			std::cout << "\tsubscribe to " << out << std::endl;
			func(out);
		}
	}
}

void ds::saveStringToFile( const std::string &filename, const std::string &src )
{
	std::fstream filestr;
	filestr.open( filename.c_str(), std::fstream::out );
	if ( filestr.is_open() )
	{
		filestr.write( src.c_str(), src.size() );
		filestr.close();
	}
}

void ds::saveStringToFile( const std::wstring &filename, const std::wstring &src )
{
	std::wfstream filestr;
	filestr.open( ds::utf8_from_wstr(filename).c_str(), std::wfstream::out );
	if ( filestr.is_open() )
	{
		filestr.write( src.c_str(), src.size() );
		filestr.close();
	}
}

void ds::tokenize(const std::string& input, const char delim, const std::function<void(const std::string&)>& f)
{
	try {
		std::istringstream			lineBuf(input);
		while (lineBuf.good()) {
			std::string				out;
			getline(lineBuf, out, delim);
			if (f) f(out);
		}
	} catch (std::exception&) {
	}
}

void ds::tokenize(const std::string& input, const std::function<void(const std::string&)>& f)
{
	tokenize(input, '\n', f);
}
