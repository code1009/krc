/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <string>

//===========================================================================
#include <Windows.h>





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
void code_generate_hangule_51_ucs2(void)
{
	std::string file_path;


	file_path = get_directory_of_current_process() + "hangule_51.ucs2.txt";


	std::ofstream ofs(file_path, std::ios::binary);

	

	ofs.write(reinterpret_cast<char*>(&text_bom_utf16_le[0]), sizeof(text_bom_utf16_le));



	// 유니코드 한글 낱글자 51자: 0x3131u 'ㄱ' - 0x3163u 'ㅣ'
	// 완성형   한글 낱글자 51자: 0xA4A1u 'ㄱ' - 0xA4D3u 'ㅣ'

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

void code_generate_hangule_51_ucs2(void)
{
	std::string file_path;


	file_path = get_directory_of_current_process() + "hangule_51.ucs2.txt";


	std::ofstream ofs(file_path, std::ios::binary);



	ofs.write(reinterpret_cast<char*>(&text_bom_utf16_le[0]), sizeof(text_bom_utf16_le));



	// 유니코드 한글 낱글자 51자: 0x3131u 'ㄱ' - 0x3163u 'ㅣ'
	// 완성형   한글 낱글자 51자: 0xA4A1u 'ㄱ' - 0xA4D3u 'ㅣ'

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




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
int main()
{
	code_generate_hangule_51_ucs2();

	return 0;
}
