#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <locale.h>

#include <Windows.h>

#include "krc/krc.h"

void print_GA(void)
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
	printf("char   (%d) :  \"GA\" = ", sizeof(char));
	for (i = 0u; i < count; i++)
	{
		printf("%02x ", p[i] & 0xFFu);
	}
	printf("\r\n");


	count = sizeof(hangul_wide);
	p = (unsigned char*)hangul_wide;
	printf("wchar_t(%d) : L\"GA\" = ", sizeof(wchar_t));
	for (i = 0u; i < count; i++)
	{
		printf("%02x ", p[i] & 0xFFu);
	}
	printf("\r\n");


	printf("\r\n");
}

void printlnA(const char* s)
{
	OutputDebugStringA(s); OutputDebugStringA("\r\n");
	printf(s); printf("\r\n");
}

void printlnW(const wchar_t* s)
{
	OutputDebugStringW(s); OutputDebugStringW(L"\r\n");
	wprintf(s); wprintf(L"\r\n");
}

void test1(void)
{
	//-----------------------------------------------------------------------
	// cp949 <-> unicode
	//-----------------------------------------------------------------------


	/////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	char    mbcs_string[] = "AZaz09가힣갂좤좥힋힍힣0ㄱㅎㅏㅣ1伽詰一龜豈刺切廓2。3я4";
	wchar_t wcs_string[] = L"AZaz09가힣갂좤좥힋힍힣0ㄱㅎㅏㅣ1伽詰一龜豈刺切廓2。3я4";

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
	wcs_length = krc_cp949_to_unicode(mbcs_string, strlen(mbcs_string), wcs, 1024);
	printlnA("//-----------------------------------------------------------------------");
	printf("%d->%d", strlen(mbcs_string), wcs_length); printf("\r\n");
	printlnA(mbcs_string);
	printlnW(wcs);


	//-----------------------------------------------------------------------
	mbcs_length = krc_unicode_to_cp949(wcs_string, wcslen(wcs_string), mbcs, 1024);
	printlnA("//-----------------------------------------------------------------------");
	printf("%d->%d", wcslen(wcs_string), mbcs_length); printf("\r\n");
	printlnW(wcs_string);
	printlnA(mbcs);




	/////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	memset(mbcs, 0, sizeof(mbcs));
	memset(wcs, 0, sizeof(wcs));

	memset(mbcs, 0xFF, sizeof(mbcs) - 1);
	memset(wcs, 0xFF, sizeof(wcs) - 2);



	/////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	wcs_length = krc_cp949_to_unicode(mbcs_string, 0, wcs, 0);
	printlnA("//-----------------------------------------------------------------------");
	printf("%d->%d", 0, wcs_length); printf("\r\n");
	printlnA(mbcs_string);
	printlnW(wcs);


	/////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------
	mbcs_length = krc_unicode_to_cp949(wcs_string, 0, mbcs, 0);
	printlnA("//-----------------------------------------------------------------------");
	printf("%d->%d", 0, mbcs_length); printf("\r\n");
	printlnW(wcs_string);
	printlnA(mbcs);
}

void test(void)
{
	print_GA();
	test1();
}

int main()
{
    test();

    return 0;
}

