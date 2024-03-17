/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <locale.h>

#include <string>
#include <vector>

#include <Windows.h>

#include "../iconv/iconv.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*
# iconv 1.17 소스 포팅
config.h
iconv.c
iconv.h
libcharset.h
localcharset.c
localcharset.h
relocatable.c
*/



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool convert_charset
(
	const char* icharset, std::uint8_t* istring, std::uint32_t isize, std::uint32_t* irsize,
	const char* ocharset, std::uint8_t* ostring, std::uint32_t osize, std::uint32_t* owsize
)
{
	//--------------------------------------------------------------------------
	bool result;


	result = true;


	//--------------------------------------------------------------------------
	iconv_t descriptor;


	char* ipointer;
	::size_t ileft;

	char* opointer;
	::size_t oleft;

	::size_t converted;


	int rc;
	int error;


	ipointer = reinterpret_cast<char*>(istring);
	opointer = reinterpret_cast<char*>(ostring);
	ileft = isize;
	oleft = osize;

	descriptor = iconv_open(ocharset, icharset);
	if (((iconv_t)-1) == descriptor)
	{
		if (nullptr != irsize)
		{
			*irsize = 0u;
		}

		if (nullptr != owsize)
		{
			*owsize = 0u;
		}

		return false;
	}

	// iconv() 널문자 자동으로 안붙임 입력에 널문자 포함되어야 변환함
	converted = iconv(descriptor, &ipointer, &ileft, &opointer, &oleft);
	if (converted < 0)
	{
		error = errno;

		result = false;
	}

	rc = iconv_close(descriptor);
	if (rc < 0)
	{
		error = errno;

		result = false;
	}

	//--------------------------------------------------------------------------
	if (true == result)
	{
		if (nullptr != irsize)
		{
			*irsize = static_cast<std::uint32_t>(isize - ileft);
		}

		if (nullptr != owsize)
		{
			*owsize = static_cast<std::uint32_t>(osize - oleft);
		}
	}
	else
	{
		if (nullptr != irsize)
		{
			*irsize = 0u;
		}

		if (nullptr != owsize)
		{
			*owsize = 0u;
		}
	}

	return result;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::string iconv_utf8_string_to_johab_string(std::string input)
{
	std::string s;

	const char* icharset = "UTF-8";
	const char* ocharset = "JOHAB";

	std::uint8_t* istring;
	std::uint32_t  isize;
	std::uint32_t  irsize;
	std::uint8_t* ostring;
	std::uint32_t  osize;
	std::uint32_t  owsize;

	bool result;


	istring = reinterpret_cast<std::uint8_t*>(const_cast<char*>(const_cast<char*>(input.c_str())));
	isize = static_cast<std::uint32_t>(input.size() + 1);
	irsize = 0u;


	std::vector<char> buffer(isize * 4u);

	ostring = reinterpret_cast<std::uint8_t*>(&buffer[0]);
	osize = static_cast<std::uint32_t>(buffer.size() + 1);
	owsize = 0u;


	result = convert_charset(icharset, istring, isize, &irsize, ocharset, ostring, osize, &owsize);


	if (true == result)
	{
		s = &buffer[0];
	}

	return s;
}

std::string iconv_utf8_string_to_string(std::string input, const char* cs)
{
	std::string s;

	const char* icharset = "UTF-8";
	const char* ocharset = cs;

	std::uint8_t* istring;
	std::uint32_t  isize;
	std::uint32_t  irsize;
	std::uint8_t* ostring;
	std::uint32_t  osize;
	std::uint32_t  owsize;

	bool result;


	istring = reinterpret_cast<std::uint8_t*>(const_cast<char*>(input.c_str()));
	isize = static_cast<std::uint32_t>(input.size() + 1);
	irsize = 0u;


	std::vector<char> buffer(isize * 4u);

	ostring = reinterpret_cast<std::uint8_t*>(&buffer[0]);
	osize = static_cast<std::uint32_t>(buffer.size() + 1);
	owsize = 0u;


	result = convert_charset(icharset, istring, isize, &irsize, ocharset, ostring, osize, &owsize);


	if (true == result)
	{
		s = &buffer[0];
	}

	return s;
}

std::string iconv_string_to_utf8_string(std::string input, const char* cs)
{
	std::string s;

	const char* icharset = cs;
	const char* ocharset = "UTF-8";

	std::uint8_t* istring;
	std::uint32_t  isize;
	std::uint32_t  irsize;
	std::uint8_t* ostring;
	std::uint32_t  osize;
	std::uint32_t  owsize;

	bool result;


	istring = reinterpret_cast<std::uint8_t*>(const_cast<char*>(input.c_str()));
	isize = static_cast<std::uint32_t>(input.size() + 1);
	irsize = 0u;


	std::vector<char> buffer(isize * 4u);

	ostring = reinterpret_cast<std::uint8_t*>(&buffer[0]);
	osize = static_cast<std::uint32_t>(buffer.size() + 1);
	owsize = 0u;


	result = convert_charset(icharset, istring, isize, &irsize, ocharset, ostring, osize, &owsize);


	if (true == result)
	{
		s = &buffer[0];
	}

	return s;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring mbcs_to_wcs(std::string input, UINT codepage = CP_ACP)
{
	int len = MultiByteToWideChar(codepage, 0, input.c_str(), -1, NULL, 0);


	if (len > 0)
	{
		std::vector<wchar_t> buf(len);


		MultiByteToWideChar(codepage, 0, input.c_str(), -1, &buf[0], len);

		return std::wstring(&buf[0]);
	}

	return std::wstring();
}

std::string wcs_to_mbcs(std::wstring input, UINT codepage = CP_ACP)
{
	int len = WideCharToMultiByte(codepage, 0, input.c_str(), -1, NULL, 0, NULL, NULL);


	if (len > 0)
	{
		std::vector<char> buf(len);


		WideCharToMultiByte(codepage, 0, input.c_str(), -1, &buf[0], len, NULL, NULL);

		return std::string(&buf[0]);
	}

	return std::string();
}

std::string utf8_to_mbcs(std::string /*input*/utf8, UINT codepage = CP_ACP)
{
	//	std::string  utf8 ;
	std::wstring utf16;
	std::string  mbcs;


	//	utf8  = input;
	utf16 = mbcs_to_wcs(utf8, CP_UTF8);
	mbcs = wcs_to_mbcs(utf16, codepage);

	return mbcs;
}

std::string mbcs_to_utf8(std::string /*input*/mbcs, UINT codepage = CP_ACP)
{
	std::string  utf8;
	std::wstring utf16;
	//	std::string  mbcs ;


	//	mbcs  = input;
	utf16 = mbcs_to_wcs(mbcs, codepage);
	utf8 = wcs_to_mbcs(utf16, CP_UTF8);

	return utf8;
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::string get_directory_of_file_path(std::string file_path)
{
	std::string result;

	std::string::size_type index;


	index = file_path.find_last_of("\\/");
	if (std::string::npos == index)
	{
		return result;
	}


	result = file_path.substr(0, index + 1);

	return result;
}


std::string get_file_of_file_path(std::string file_path)
{
	std::string result;

	std::string::size_type index;


	index = file_path.find_last_of("\\/");
	if (std::string::npos == index)
	{
		return result;
	}


	result = file_path.substr(index + 1);

	return result;
}


//===========================================================================
std::string get_file_name_of_file_path(std::string file_path)
{
	std::string result;

	std::string::size_type index;


	std::string file;


	file = get_file_of_file_path(file_path);
	if (file.empty())
	{
		return result;
	}


	index = file.find_last_of(".");
	if (std::string::npos == index)
	{
		return result;
	}


	result = file.substr(0, index);

	return result;
}

std::string get_file_extension_of_file_path(std::string file_path)
{
	std::string result;

	std::string::size_type index;


	std::string file;


	file = get_file_of_file_path(file_path);
	if (file.empty())
	{
		return result;
	}


	index = file.find_last_of(".");
	if (std::string::npos == index)
	{
		return result;
	}


	result = file.substr(index);

	return result;
}

std::string get_file_extension_name_of_file_path(std::string file_path)
{
	std::string result;

	std::string::size_type index;


	std::string file;


	file = get_file_of_file_path(file_path);
	if (file.empty())
	{
		return result;
	}


	index = file.find_last_of(".");
	if (std::string::npos == index)
	{
		return result;
	}


	result = file.substr(index + 1);

	return result;
}

//===========================================================================
std::string get_directory_name_of_directory(std::string directory)
{
	std::string result;

	std::string::size_type index;


	index = directory.find_last_of("\\/");
	if (std::string::npos == index)
	{
		return result;
	}


	result = directory.substr(0, index);


	return result;
}

std::string get_parent_directory_of_directory(std::string directory)
{
	std::string result;

	std::string::size_type index;


	std::string directory_name;


	directory_name = get_directory_name_of_directory(directory);
	if (directory_name.empty())
	{
		return result;
	}


	index = directory_name.find_last_of("\\/");
	if (std::string::npos == index)
	{
		return result;
	}


	result = directory_name.substr(0, index + 1);


	return result;
}

std::string get_file_path_of_module(HINSTANCE hinstance)
{
	std::string result;


	char buffer[MAX_PATH];


	if (GetModuleFileNameA(hinstance, buffer, sizeof(buffer) / sizeof(char)) > 0)
	{
		result = buffer;
	}

	return result;
}

std::string get_file_path_of_current_process(void)
{
	return get_file_path_of_module(NULL);
}

std::string get_directory_of_current_process(void)
{
	return get_directory_of_file_path(get_file_path_of_current_process());
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <iostream>
#include <fstream>





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "../krc/krc/krc_type.h"
#include "../krc/krc/krc_code_table_hangul_11172_cp949_to_unicode.h"
#include "../krc/krc/krc_code_table_special_1128_cp949_to_unicode.h"
#include "../krc/krc/krc_code_table_hanja_4888_cp949_to_unicode.h"






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::uint8_t text_bom_utf16_be[] = { 0xFE, 0xFF };
std::uint8_t text_bom_utf16_le[] = { 0xFF, 0xFE };
std::uint8_t text_bom_utf8[] = { 0xEF, 0xBB, 0xBF };




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::string get_file_path(std::string file_name)
{
	std::string file_path;


	file_path = get_directory_of_current_process() + "../" + file_name;

	return file_path;
}

std::wstring read_ucs2_text_file(std::string file_name)
{
	std::wstring result;


	std::ifstream ifs(get_file_path(file_name), std::ios::binary);


	std::uint8_t text_bom[4];
	ifs.read(reinterpret_cast<char*>(&text_bom[0]), 2);
	if (0 != memcmp(text_bom, text_bom_utf16_le, 2))
	{
		ifs.close();
		return result;
	}


	std::uint16_t code;
	std::uint16_t i;


	for (i = 0U; !ifs.eof(); i++)
	{		
		ifs.read(reinterpret_cast<char*>(&code), 2);
		if (!ifs.eof())
		{
			result.push_back(code);
		}
	}


	ifs.close();


	return result;
}

void write_utf8_text_file(std::string file_name, std::string utf8)
{
	std::ofstream ofs(get_file_path(file_name), std::ios::binary);

	ofs.write(reinterpret_cast<char*>(&text_bom_utf8[0]), 3);

	ofs.write(reinterpret_cast<const char*>(utf8.c_str()), utf8.size());

	ofs.close();
}

std::wstring read_cp949_text_file(std::string file_name)
{
	std::wstring result;


	std::ifstream ifs(get_file_path(file_name), std::ios::binary);


	std::uint16_t code;
	std::uint16_t i;


	for (i = 0U; !ifs.eof(); i++)
	{
		ifs.read(reinterpret_cast<char*>(&code), 2);
		if (!ifs.eof())
		{
			result.push_back(code);
		}
	}


	ifs.close();


	return result;
}

void write_cp949_text_file(std::string file_name, std::string utf8)
{
	std::ofstream ofs(get_file_path(file_name), std::ios::binary);

	ofs.write(reinterpret_cast<const char*>(utf8.c_str()), utf8.size());

	ofs.close();
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void ucs2_code_generate_hangul_51(void)
{
	std::ofstream ofs(get_file_path("hangul_51.ucs2.txt"), std::ios::binary);
	ofs.write(reinterpret_cast<char*>(&text_bom_utf16_le[0]), sizeof(text_bom_utf16_le));



	// 유니코드 한글 낱글자 51자: 0x3131u 'ㄱ' - 0x3163u 'ㅣ'

	std::uint16_t code;
	std::uint16_t i;
	std::uint16_t count;

	
	count = 51;
	for (i = 0U; i < count; i++)
	{
		code = 0x3131 + i;
		ofs.write(reinterpret_cast<char*>(&code), 2);
	}


	ofs.close();
}

void ucs2_code_generate_hangul_11172(void)
{
	std::ofstream ofs(get_file_path("hangul_11172.ucs2.txt"), std::ios::binary);
	ofs.write(reinterpret_cast<char*>(&text_bom_utf16_le[0]), sizeof(text_bom_utf16_le));


	std::uint16_t code;
	std::uint16_t i;
	std::uint16_t count;


	count = 11172;
	for (i = 0U; i < count; i++)
	{
		code = 0xac00 + i;
		ofs.write(reinterpret_cast<char*>(&code), 2);
	}


	ofs.close();
}

void ucs2_code_generate_hanja_4888(void)
{
	std::ofstream ofs(get_file_path("hanja_4888.ucs2.txt"), std::ios::binary);
	ofs.write(reinterpret_cast<char*>(&text_bom_utf16_le[0]), sizeof(text_bom_utf16_le));


	std::uint16_t code;
	std::uint16_t i;
	std::uint16_t count;


	count = 4888;
	for (i = 0U; i < count; i++)
	{
		code = _krc_code_table_hanja_4888_cp949_to_unicode[i];
		ofs.write(reinterpret_cast<char*>(&code), 2);
	}


	ofs.close();
}

void ucs2_code_generate_special_1128(void)
{
	std::ofstream ofs(get_file_path("special_1128.ucs2.txt"), std::ios::binary);
	ofs.write(reinterpret_cast<char*>(&text_bom_utf16_le[0]), sizeof(text_bom_utf16_le));


	std::uint16_t code;
	std::uint16_t i;
	std::uint16_t count;


	count = 1128;
	for (i = 0U; i < count; i++)
	{
		code = _krc_code_table_special_1128_cp949_to_unicode[i];
		ofs.write(reinterpret_cast<char*>(&code), 2);
	}


	ofs.close();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void utf8_code_generate_hangul_51(void)
{
	std::wstring ucs2;
	std::string utf8;


	ucs2 = read_ucs2_text_file("hangul_51.ucs2.txt");
	utf8 = wcs_to_mbcs(ucs2, CP_UTF8);
	write_utf8_text_file("hangul_51.utf8.txt", utf8);
}

void utf8_code_generate_hangul_11172(void)
{
	std::wstring ucs2;
	std::string utf8;


	ucs2 = read_ucs2_text_file("hangul_11172.ucs2.txt");
	utf8 = wcs_to_mbcs(ucs2, CP_UTF8);
	write_utf8_text_file("hangul_11172.utf8.txt", utf8);
}

void utf8_code_generate_hanja_4888(void)
{
	std::wstring ucs2;
	std::string utf8;


	ucs2 = read_ucs2_text_file("hanja_4888.ucs2.txt");
	utf8 = wcs_to_mbcs(ucs2, CP_UTF8);
	write_utf8_text_file("hanja_4888.utf8.txt", utf8);
}

void utf8_code_generate_special_1128(void)
{
	std::wstring ucs2;
	std::string utf8;


	ucs2 = read_ucs2_text_file("special_1128.ucs2.txt");
	utf8 = wcs_to_mbcs(ucs2, CP_UTF8);
	write_utf8_text_file("special_1128.utf8.txt", utf8);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void cp949_code_generate_hangul_51(void)
{
	std::wstring ucs2;
	std::string mbcs;


	ucs2 = read_ucs2_text_file("hangul_51.ucs2.txt");
	mbcs = wcs_to_mbcs(ucs2);
	write_cp949_text_file("hangul_51.cp949.txt", mbcs);
}

void cp949_code_generate_hangul_11172(void)
{
	std::wstring ucs2;
	std::string mbcs;


	ucs2 = read_ucs2_text_file("hangul_11172.ucs2.txt");
	mbcs = wcs_to_mbcs(ucs2);
	write_cp949_text_file("hangul_11172.cp949.txt", mbcs);
}

void cp949_code_generate_hanja_4888(void)
{
	std::wstring ucs2;
	std::string mbcs;


	ucs2 = read_ucs2_text_file("hanja_4888.ucs2.txt");
	mbcs = wcs_to_mbcs(ucs2);
	write_cp949_text_file("hanja_4888.cp949.txt", mbcs);
}

void cp949_code_generate_special_1128(void)
{
	std::wstring ucs2;
	std::string mbcs;


	ucs2 = read_ucs2_text_file("special_1128.ucs2.txt");
	mbcs = wcs_to_mbcs(ucs2);
	write_cp949_text_file("special_1128.cp949.txt", mbcs);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void cp1316_code_generate_hangul_51(void)
{
	std::wstring ucs2;
	std::string mbcs;


	ucs2 = read_ucs2_text_file("hangul_51.ucs2.txt");
	mbcs = iconv_utf8_string_to_johab_string(wcs_to_mbcs(ucs2, CP_UTF8));
	//mbcs = wcs_to_mbcs(ucs2, 1316);
	write_cp949_text_file("hangul_51.cp1316.txt", mbcs);
}

void cp1316_code_generate_hangul_11172(void)
{
	std::wstring ucs2;
	std::string mbcs;


	ucs2 = read_ucs2_text_file("hangul_11172.ucs2.txt");
	mbcs = iconv_utf8_string_to_johab_string(wcs_to_mbcs(ucs2, CP_UTF8));
	//mbcs = wcs_to_mbcs(ucs2, 1316);
	write_cp949_text_file("hangul_11172.cp1316.txt", mbcs);
}

void cp1316_code_generate_hanja_4888(void)
{
	std::wstring ucs2;
	std::string mbcs;


	ucs2 = read_ucs2_text_file("hanja_4888.ucs2.txt");
	mbcs = iconv_utf8_string_to_johab_string(wcs_to_mbcs(ucs2, CP_UTF8));
	//mbcs = wcs_to_mbcs(ucs2, 1316);
	write_cp949_text_file("hanja_4888.cp1316.txt", mbcs);
}

void cp1316_code_generate_special_1128(void)
{
	std::wstring ucs2;
	std::string mbcs;


	ucs2 = read_ucs2_text_file("special_1128.ucs2.txt");
	mbcs = iconv_utf8_string_to_johab_string(wcs_to_mbcs(ucs2, CP_UTF8));
	//mbcs = wcs_to_mbcs(ucs2, 1316);
	write_cp949_text_file("special_1128.cp1316.txt", mbcs);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
int main()
{
	ucs2_code_generate_hangul_51();
	ucs2_code_generate_hangul_11172();
	ucs2_code_generate_hanja_4888();
	ucs2_code_generate_special_1128();

	utf8_code_generate_hangul_51();
	utf8_code_generate_hangul_11172();
	utf8_code_generate_hanja_4888();
	utf8_code_generate_special_1128();

	cp949_code_generate_hangul_51();
	cp949_code_generate_hangul_11172();
	cp949_code_generate_hanja_4888();
	cp949_code_generate_special_1128();

	cp1316_code_generate_hangul_51();
	cp1316_code_generate_hangul_11172();
	cp1316_code_generate_hanja_4888();
	cp1316_code_generate_special_1128();

	return 0;
}
