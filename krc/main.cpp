#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <locale.h>

#include <Windows.h>

#include "krc/krc.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void traceA(const char* s)
{
	OutputDebugStringA(s);
	printf(s);
}

void traceW(const wchar_t* s)
{
	OutputDebugStringW(s);
	wprintf(s);
}

void tracelnA(const char* s)
{
	traceA(s);
	traceA("\r\n");
}

void tracelnW(const wchar_t* s)
{
	traceW(s);
	traceW(L"\r\n");
}

void tracefA(const char* format, ...)
{
	char buffer[1024] = { 0 };
	int  max_count;
	int  length;

	va_list args;


	max_count = sizeof(buffer) - 1;

	va_start(args, format);
	length = vsnprintf(buffer, max_count, format, args);
	va_end(args);

	if (length > 0)
	{
		buffer[length] = '\0';

		traceA(buffer);
	}
}

void traceflnA(const char* format, ...)
{
	char buffer[1024] = { 0 };
	int  max_count;
	int  length;

	va_list args;


	max_count = sizeof(buffer) - 1;

	va_start(args, format);
	length = vsnprintf(buffer, max_count, format, args);
	va_end(args);

	if (length > 0)
	{
		buffer[length] = '\0';

		tracelnA(buffer);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test0(void)
{
	/*
	# arm
	C Source File = CP949
	char   (1) :  "가" = b0 a1 00
	wchar_t(2) : L"가" = b0 00 a1 00 00 00

	C Source File = utf8
	char   (1) :  "가" = ea b0 80 00
	wchar_t(2) : L"가" = 00 ac 00 00

	# msvc
	C Source File = CP949
	char   (1) :  "가" = b0 a1 00
	wchar_t(2) : L"가" = 00 ac 00 00

	C Source File = utf8
	char   (1) :  "가" = b0 a1 00
	wchar_t(2) : L"가" = 00 ac 00 00
	*/

	char    hangul_narrow[] = { "가" };
	wchar_t hangul_wide[] = { L"가" };

	unsigned char* p;
	int i;
	int count;


	count = sizeof(hangul_narrow);
	p = (unsigned char*)hangul_narrow;
	tracefA("char   (%d) :  \"GA\" = ", sizeof(char));
	for (i = 0u; i < count; i++)
	{
		tracefA("%02x ", p[i] & 0xFFu);
	}
	tracefA("\r\n");


	count = sizeof(hangul_wide);
	p = (unsigned char*)hangul_wide;
	tracefA("wchar_t(%d) : L\"GA\" = ", sizeof(wchar_t));
	for (i = 0u; i < count; i++)
	{
		tracefA("%02x ", p[i] & 0xFFu);
	}
	tracefA("\r\n");


	tracefA("\r\n");
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test1(void)
{
	//-----------------------------------------------------------------------
	// cp949 <-> unicode
	//-----------------------------------------------------------------------


	/////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	char    src_mbcs[] = { "A샾Zaz09가힣갂좤좥힋힍힣0ㄱㅎㅏㅣ1伽詰一龜豈刺切廓2。3я4" };
	wchar_t src_wcs[] = { L"A샾Zaz09가힣갂좤좥힋힍힣0ㄱㅎㅏㅣ1伽詰一龜豈刺切廓2。3я4" };
	char    dst_mbcs[1024];
	wchar_t dst_wcs[1024];


	char*    mbcs;
	wchar_t* utf8;
	int      mbcs_size;
	int      wcs_size;
	int      mbcs_len;
	int      wcs_len;


	//-----------------------------------------------------------------------
	//setlocale(LC_ALL, "kr-KR");


	//-----------------------------------------------------------------------
	mbcs = dst_mbcs;
	utf8 = dst_wcs;
	mbcs_size = sizeof(dst_mbcs);
	wcs_size = sizeof(dst_wcs);
	memset(mbcs, 0, mbcs_size);
	memset(utf8, 0, wcs_size);




	/////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	mbcs = src_mbcs;
	mbcs_size = sizeof(src_mbcs);
	mbcs_len = strlen(src_mbcs);
	utf8 = dst_wcs;
	wcs_size = sizeof(dst_wcs);
	wcs_len = krc_cp949_to_unicode(mbcs, mbcs_len, (krc_wchar_t*)utf8, wcs_size);
	tracelnA("//-----------------------------------------------------------------------");
	traceflnA("%d->%d", mbcs_len, wcs_len);
	tracelnA(mbcs);
	tracelnW(utf8);


	//-----------------------------------------------------------------------
	utf8 = src_wcs;
	wcs_size = sizeof(src_wcs);
	wcs_len = wcslen(src_wcs);
	mbcs = dst_mbcs;
	mbcs_size = sizeof(dst_mbcs);
	mbcs_len = krc_unicode_to_cp949((krc_wchar_t*)utf8, wcs_len, mbcs, mbcs_size);
	tracelnA("//-----------------------------------------------------------------------");
	traceflnA("%d->%d", wcs_len, mbcs_len);
	tracelnW(utf8);
	tracelnA(mbcs);




	/////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	memset(dst_mbcs, 0, sizeof(dst_mbcs));
	memset(dst_wcs, 0, sizeof(dst_wcs));

	memset(dst_mbcs, 0xFF, sizeof(dst_mbcs) - 1);
	memset(dst_wcs, 0xFF, sizeof(dst_wcs) - 2);



	/////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	mbcs = src_mbcs;
	mbcs_size = sizeof(src_mbcs);
	mbcs_len = 0;// strlen(src_mbcs);
	utf8 = dst_wcs;
	wcs_size = sizeof(dst_wcs);
	wcs_len = krc_cp949_to_unicode(mbcs, mbcs_len, (krc_wchar_t*)utf8, wcs_size);
	tracelnA("//-----------------------------------------------------------------------");
	traceflnA("%d->%d", 0, wcs_len); printf("\r\n");
	//tracelnA(mbcs);
	//tracelnW(utf8);


	/////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	utf8 = src_wcs;
	wcs_size = sizeof(src_wcs);
	wcs_len = 0;// wcslen(src_wcs);
	mbcs = dst_mbcs;
	mbcs_size = sizeof(dst_mbcs);
	mbcs_len = krc_unicode_to_cp949((krc_wchar_t*)utf8, wcs_len, mbcs, mbcs_size);
	tracelnA("//-----------------------------------------------------------------------");
	traceflnA("%d->%d", 0, mbcs_len); printf("\r\n");
	//tracelnW(utf8);
	//tracelnA(mbcs);



	/////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	mbcs = src_mbcs;
	mbcs_size = sizeof(src_mbcs);
	mbcs_len = strlen(src_mbcs);
	utf8 = 0;
	wcs_size = sizeof(dst_wcs);
	wcs_len = krc_cp949_to_unicode(mbcs, mbcs_len, (krc_wchar_t*)utf8, wcs_size);
	tracelnA("//-----------------------------------------------------------------------");
	traceflnA("%d->%d", 0, wcs_len); printf("\r\n");
	//tracelnA(mbcs);
	//tracelnW(utf8);


	/////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	utf8 = src_wcs;
	wcs_size = sizeof(src_wcs);
	wcs_len = wcslen(src_wcs);
	mbcs = 0;
	mbcs_size = sizeof(dst_mbcs);
	mbcs_len = krc_unicode_to_cp949((krc_wchar_t*)utf8, wcs_len, mbcs, mbcs_size);
	tracelnA("//-----------------------------------------------------------------------");
	traceflnA("%d->%d", 0, mbcs_len); printf("\r\n");
	//tracelnW(utf8);
	//tracelnA(mbcs);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <string>
#include <vector>

std::wstring mbcs_to_wcs(std::string input, UINT codepage=CP_ACP)
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

void test2(void)
{
	/////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	char test_mbcs_c_string[] = { "A샾Zaz09가힣갂좤좥힋힍힣0ㄱㅎㅏㅣ1伽詰一龜豈刺切廓2。3я4" };
	std::string test_utf8_cpp_string = mbcs_to_utf8(test_mbcs_c_string);

	char* src_mbcs_ptr = (char*)test_mbcs_c_string;
	int src_mbcs_len = strlen(test_mbcs_c_string);

	char* src_utf8_ptr = (char*)test_utf8_cpp_string.c_str();
	int src_utf8_len = (int)test_utf8_cpp_string.size();


	char dst_mbcs[1024];
	char dst_utf8[1024];

	char* mbcs;
	char* utf8;
	int   mbcs_size;
	int   utf8_size;
	int   mbcs_len;
	int   utf8_len;
	int   cmp;


	utf8 = src_utf8_ptr;
	utf8_size = src_utf8_len + 1;
	utf8_len = src_utf8_len;
	mbcs = dst_mbcs;
	mbcs_size = sizeof(dst_mbcs);
	mbcs_len = krc_utf8_to_cp949(utf8, utf8_len, mbcs, mbcs_size);
	cmp = memcmp(mbcs, src_mbcs_ptr, mbcs_len);
	if (0 == cmp)
	{
		if (mbcs_len == src_mbcs_len)
		{
			tracelnA("krc_utf8_to_cp949() ok");
		}
		else
		{
			tracelnA("krc_utf8_to_cp949() fail");
		}
	}
	else
	{
		tracelnA("krc_utf8_to_cp949() fail");
	}
	tracelnA("\r\n");


	mbcs = src_mbcs_ptr;
	mbcs_size = src_mbcs_len + 1;
	mbcs_len = src_mbcs_len;
	utf8 = dst_utf8;
	utf8_size = sizeof(dst_utf8);
	utf8_len = krc_cp949_to_utf8(mbcs, mbcs_size, utf8, utf8_size);
	cmp = memcmp(utf8, src_utf8_ptr, utf8_len);
	if (0 == cmp)
	{
		if (utf8_len == src_utf8_len)
		{
			tracelnA("krc_cp949_to_utf8() ok");
		}
		else
		{
			tracelnA("krc_cp949_to_utf8() fail");
		}
	}
	else
	{
		tracelnA("krc_cp949_to_utf8() fail");
	}
	tracelnA("\r\n");
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test(void)
{
	tracelnA("//***************************************************************************");
	tracelnA("// test0");
	tracelnA("//***************************************************************************");
	test0();
	tracelnA("\r\n");

	tracelnA("//***************************************************************************");
	tracelnA("// test1");
	tracelnA("//***************************************************************************");
	test1();
	tracelnA("\r\n");

	tracelnA("//***************************************************************************");
	tracelnA("// test2");
	tracelnA("//***************************************************************************");
	test2();
	tracelnA("\r\n");
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
int main()
{
	//---------------------------------------------------------
	/*
	가
	완성형 : 0xb0a1
	조합형 : 0x8861
	utf16  : 0xac00
	utf8   : 234(0xEA), 176(0xB0), 128(0x80)
	*/


    test();

    return 0;
}

