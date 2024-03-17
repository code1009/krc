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
	char    mbcs_string[] = { "AZaz09가힣갂좤좥힋힍힣0ㄱㅎㅏㅣ1伽詰一龜豈刺切廓2。3я4" };
	wchar_t wcs_string[] = { L"AZaz09가힣갂좤좥힋힍힣0ㄱㅎㅏㅣ1伽詰一龜豈刺切廓2。3я4" };

	char    mbcs[56+1];
	wchar_t wcs[56+1];
	int     mbcs_length;
	int     wcs_length;


	//-----------------------------------------------------------------------
	//setlocale(LC_ALL, "kr-KR");


	//-----------------------------------------------------------------------
	memset(mbcs, 0, sizeof(mbcs));
	memset(wcs, 0, sizeof(wcs));




	/////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	wcs_length = krc_cp949_to_unicode(&mbcs_string[0], strlen(mbcs_string), (krc_wchar_t*)&wcs[0], 1024);
	tracelnA("//-----------------------------------------------------------------------");
	traceflnA("%d->%d", strlen(mbcs_string), wcs_length);
	tracelnA(mbcs_string);
	tracelnW(wcs);


	//-----------------------------------------------------------------------
	mbcs_length = krc_unicode_to_cp949((krc_wchar_t*)&wcs_string[0], wcslen(wcs_string), &mbcs[0], 1024);
	tracelnA("//-----------------------------------------------------------------------");
	traceflnA("%d->%d", wcslen(wcs_string), mbcs_length);
	tracelnW(wcs_string);
	tracelnA(mbcs);




	/////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	memset(mbcs, 0, sizeof(mbcs));
	memset(wcs, 0, sizeof(wcs));

	memset(mbcs, 0xFF, sizeof(mbcs) - 1);
	memset(wcs, 0xFF, sizeof(wcs) - 2);



	/////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	wcs_length = krc_cp949_to_unicode(&mbcs_string[0], 0, (krc_wchar_t*)&wcs[0], 0);
	tracelnA("//-----------------------------------------------------------------------");
	traceflnA("%d->%d", 0, wcs_length); printf("\r\n");
	tracelnA(mbcs_string);
	tracelnW(wcs);


	/////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	mbcs_length = krc_unicode_to_cp949((krc_wchar_t*)&wcs_string[0], 0, &mbcs[0], 0);
	tracelnA("//-----------------------------------------------------------------------");
	traceflnA("%d->%d", 0, mbcs_length); printf("\r\n");
	tracelnW(wcs_string);
	tracelnA(mbcs);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test2(void)
{

}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test(void)
{
	tracelnA("//***************************************************************************");
	test0();
	tracelnA("\r\n");

	tracelnA("//***************************************************************************");
	test1();
	tracelnA("\r\n");

	tracelnA("//***************************************************************************");
	test2();
	tracelnA("\r\n");
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
int main()
{
    test();

    return 0;
}

