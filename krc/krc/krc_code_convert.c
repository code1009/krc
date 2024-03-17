﻿/////////////////////////////////////////////////////////////////////////////
//
// File: krc_code_convert.c
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define KRC_COMPILE 1





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "krc_config.h"
#include "krc_type.h"
#include "krc_api.h"
 
#include "krc_code_table.h"

#include "krc_code_type.h"
#include "krc_code_convert.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_uint_t krc_code_type_cp949(krc_char16_t ch)
{
	//-----------------------------------------------------------------------
	if (0x0000u <= ch && ch <= 0x007Fu)
	{
		return KRC_CODE_TYPE_ASCII_LOW;
	}

	if (0x0080u <= ch && ch <= 0x00FFu)
	{
		return KRC_CODE_TYPE_ASCII_HIGH;
	}


	//-----------------------------------------------------------------------
	if (0xA4A1u <= ch && ch <= 0xA4D3u)
	{
		if (0xA4A1u <= ch && ch <= 0xA4BEu)
		{
			return KRC_CODE_TYPE_HANGUL_JAMO_JAEUM;
		}
		if (0xA4BFu <= ch && ch <= 0xA4D3u)
		{
			return KRC_CODE_TYPE_HANGUL_JAMO_MOEUM;
		}
	}

	if (ch >= 0xB0A1u && ch <= 0xC8FEu)
	{
		return KRC_CODE_TYPE_HANGUL_11172;
	}
	if (ch >= 0x8141u && ch <= 0xA0FEu)
	{
		return KRC_CODE_TYPE_HANGUL_11172;
	}
	if (ch >= 0xA141u && ch <= 0xC5A0u)
	{
		//-------------------------------------------------------------------
		if (0xA1A1u <= ch && ch <= 0xACFEu)
		{
			return KRC_CODE_TYPE_SPECIAL_1128;
		}

		return KRC_CODE_TYPE_HANGUL_11172;
	}
	if (ch >= 0xC641u && ch <= 0xC652u)
	{
		return KRC_CODE_TYPE_HANGUL_11172;
	}


	//-----------------------------------------------------------------------
	if (0xA1A1u <= ch && ch <= 0xACFEu)
	{
		return KRC_CODE_TYPE_SPECIAL_1128;
	}

	if (0xCAA1u <= ch && ch <= 0xFDFEu)
	{
		return KRC_CODE_TYPE_HANJA_4888;
	}


	return KRC_CODE_TYPE_UNKNOWN;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_uint_t krc_char_type_unicode(krc_wchar_t ch)
{
	//-----------------------------------------------------------------------
	if (0x0000u <= ch && ch <= 0x007Fu)
	{
		return KRC_CODE_TYPE_ASCII_LOW;
	}

	if (0x0080u <= ch && ch <= 0x00FFu)
	{
		return KRC_CODE_TYPE_ASCII_HIGH;
	}


	//-----------------------------------------------------------------------
	if (0xAC00u <= ch && ch <= 0xD7A3u) // 0xAC00u+11172자
	{
		return KRC_CODE_TYPE_HANGUL_11172;
	}

	if (0x1100u <= ch && ch <= 0x11FFu)
	{
		if (0x1100u <= ch && ch <= 0x1112u) // 0x1100u+초성19자
		{
			return KRC_CODE_TYPE_HANGUL_JAMO_CHOSEONG;
		}
		if (0x1161u <= ch && ch <= 0x1175u) // 0x1161u+중성21자
		{
			return KRC_CODE_TYPE_HANGUL_JAMO_JUNGSEONG;
		}
		if (0x11A8u <= ch && ch <= 0x11C2u) // 0x11A8u+종성28자
		{
			return KRC_CODE_TYPE_HANGUL_JAMO_JONGSEONG;
		}
	}

	if (0x3131u <= ch && ch <= 0x3163u)
	{
		if (0x3131u <= ch && ch <= 0x314Eu) // 0x3131u+자음30자
		{
			return KRC_CODE_TYPE_HANGUL_JAMO_JAEUM;
		}
		if (0x314Fu <= ch && ch <= 0x3163u) // 0x314Fu+모음21자
		{
			return KRC_CODE_TYPE_HANGUL_JAMO_MOEUM;
		}
	}


	//-----------------------------------------------------------------------
	// 0x00A1u '¡' - 0x00FEu 'þ'
	// 0x0111u 'đ'  - 0x0451u 'ё'
	// 0x2015u '―' - 0x266Du '♭'
	// 0x3000u '　' - 0x33DDu '㏝'
	// 0xFF01u '！' - 0xFFE6u '￦'
	if (0x00A1u <= ch && ch <= 0x00FEu)
	{
		return KRC_CODE_TYPE_SPECIAL_1128;
	}
	if (0x0111u <= ch && ch <= 0x0451u)
	{
		return KRC_CODE_TYPE_SPECIAL_1128;
	}
	if (0x2015u <= ch && ch <= 0x266Du)
	{
		return KRC_CODE_TYPE_SPECIAL_1128;
	}
	if (0x3000u <= ch && ch <= 0x33DDu)
	{
		return KRC_CODE_TYPE_SPECIAL_1128;
	}
	if (0xFF01u <= ch && ch <= 0xFFE6u)
	{
		return KRC_CODE_TYPE_SPECIAL_1128;
	}


	//-----------------------------------------------------------------------
	// 0x4E00u '一' - 0x9F9Cu '龜'
	// 0xF900u '豈' - 0xF9FFu '刺'
	// 0xFA00u '切' - 0xFA0Bu '廓'
	if (0x4E00u <= ch && ch <= 0x9F9C)
	{
		return KRC_CODE_TYPE_HANJA_4888;
	}
	if (0xF900u <= ch && ch <= 0xF9FF)
	{
		return KRC_CODE_TYPE_HANJA_4888;
	}
	if (0xFA00u <= ch && ch <= 0xFA0B)
	{
		return KRC_CODE_TYPE_HANJA_4888;
	}


	return KRC_CODE_TYPE_UNKNOWN;
}
 




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_int_t krc_hangul_11172_index_cp949 (krc_char16_t ch)
{
	krc_uint_t offset;

	krc_int_t index;


	index = -1;


	//-----------------------------------------------------------------------
	// 완성형한글.한글2350자 
	//-----------------------------------------------------------------------

	// 0xB0A1u '가' - 0xC8FEu '힝'
	if (ch>=0xB0A1u && ch<=0xC8FEu)
	{
		offset = (ch-0xB0A0u)%0x0100u;
		if (offset>0u && offset<96u)
		{
			index = (ch-0xB0A0u)/0x0100u*94u + offset - 1u;
			return index;
		}
	}


	//-----------------------------------------------------------------------
	// 확장완성형한글.추가한글8822자
	//-----------------------------------------------------------------------

	// 5696 + 3108 + 18

	// 5696자
	// 0x8141u '갂' - 0xA0FEu '좤' 
	if (ch>=0x8141u && ch<=0xA0FEu)
	{
		offset = (ch-0x8140u)%0x0100u;
		if (offset>0u && offset<191u)
		{
			if (offset >= 0x001Bu && offset<= 0x0020u)
				return -1;

			if (offset >= 0x003Bu && offset<= 0x0040u)
				return -1;

			if      (offset>0x0040u) offset -= 12u;
			else if (offset>0x0020u) offset -= 6u;

			index  = (ch-0x8140u)/0x0100u*(192u-2u-6u-6u) + offset - 1u + 2350u;
			return index;
		}
	}

	// 3108자
	// 0xA141u '좥' - 0xC5A0u '힋' 
	if (ch>=0xA141u && ch<=0xC5A0u)
	{
		offset = (ch-0xA140u)%0x0100u;
		if (offset>0u && offset<97u)
		{
			if (offset >= 0x001Bu && offset<= 0x0020u)
				return -1;

			if (offset >= 0x003Bu && offset<= 0x0040u)
				return -1;

			if      (offset>0x0040) offset -= 12u;
			else if (offset>0x0020) offset -= 6u;

			index  = (ch-0xA140u)/0x0100u*(97u-6u-6u-1u) + offset - 1u + 5696u + 2350u;
			return index;
		}
	}

	// 18자
	// 0xC641u '힍' - 0xC652u '힣' 
	if (ch>=0xC641u && ch<=0xC652u)
	{
		index = ch - 0xC640u - 1u + 3108u + 5696u + 2350u;
		return index;
	}


	return index;
}

KRC_API krc_int_t krc_special_1128_index_cp949 (krc_char16_t ch)
{
	krc_uint_t offset;


	/*
	# 완성형한글.특수문자1128자: 0xA1A1u - 0xACFEu
	  12개영역 * 94문자 = 1128문자
	     0xA1A0u
	   + 0x0C00u(12*256)
	   = 0xFEA0u
	*/
	if (0xA1A1u<=ch && ch <= 0xACFEu)
	{
		offset = (ch-0xA1A0u)%0x0100u;

		if (offset>0u && offset<96u)
		{
			return ((ch-0xA1A0u)/0x0100u)*94u + offset -1u;
		}
	}

	return -1;
}

KRC_API krc_int_t krc_hanja_4888_index_cp949 (krc_char16_t ch)
{
	krc_uint_t offset;


	/*
	# 완성형한글.한자4888자: 0xCAA1u-0xFDFEu
	  52개영역 * 94문자 = 4888문자
	     0xCAA0u
	   + 0x3400u(52*256)
	   = 0xFEA0u
	*/
	if (0xCAA1u<=ch && ch <= 0xFDFEu)
	{
		offset = (ch-0xCAA0u)%0x0100u;

		if (offset>0u && offset<96u)
		{
			return ((ch-0xCAA0u)/0x0100u)*94u + offset - 1u;
		}
	}

	return -1;
}





/////////////////////////////////////////////////////////////////////////////
// 
// cp949_to_unicode 
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_bool_t krc_hangul_11172_cp949_to_unicode (krc_char16_t cp949, krc_wchar_t* unicode)
{
	krc_int_t index;

	
	// 유니코드 한글 낱글자 51자: 0x3131u 'ㄱ' - 0x3163u 'ㅣ'
	// 완성형   한글 낱글자 51자: 0xA4A1u 'ㄱ' - 0xA4D3u 'ㅣ'
	if (cp949>=0xA4A1u && cp949<=0xA4D3u)
	{
		*unicode = 0x3131u + cp949-0xA4A1u;
		return KRC_TRUE;
	}


	index = krc_hangul_11172_index_cp949 (cp949);
	if (index == -1)
		return KRC_FALSE;

	*unicode = _krc_code_table_hangul_11172_cp949_to_unicode[index];

	return KRC_TRUE;
}

KRC_API krc_bool_t krc_special_1128_cp949_to_unicode (krc_char16_t cp949, krc_wchar_t* unicode)
{
	krc_int_t index;


	index = krc_special_1128_index_cp949 (cp949);
	if (index == -1)
		return KRC_FALSE;

	*unicode = _krc_code_table_special_1128_cp949_to_unicode[index];

	return KRC_TRUE;
}

KRC_API krc_bool_t krc_hanja_4888_cp949_to_unicode (krc_char16_t cp949, krc_wchar_t* unicode)
{
	krc_int_t index;


	index = krc_hanja_4888_index_cp949 (cp949);
	if (index == -1)
		return KRC_FALSE;

	*unicode = _krc_code_table_hanja_4888_cp949_to_unicode[index];

	return KRC_TRUE;
}





/////////////////////////////////////////////////////////////////////////////
// 
// unicode_to_cp949
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_bool_t krc_hangul_11172_unicode_to_cp949 (krc_wchar_t unicode, krc_char16_t* cp949)
{
	krc_int_t index;


	// 유니코드 한글 낱글자 51자: 0x3131u 'ㄱ' - 0x3163u 'ㅣ'
	// 완성형   한글 낱글자 51자: 0xA4A1u 'ㄱ' - 0xA4D3u 'ㅣ'
	if (unicode>=0x3131u && unicode<=0x3163u)
	{
		*cp949 = 0xA4A1u + unicode-0x3131u;
		return KRC_TRUE;
	}


	// 유니코드 한글 11172자: 0xAC00u '가' - 0xD7A3u '힣'
	if (unicode>=0xAC00u && unicode<=0xD7A3u)
	{
		index = unicode - 0xAC00u;
		*cp949 = _krc_code_table_hangul_11172_unicode_to_cp949[index];
		return KRC_TRUE;
	}

	return KRC_FALSE;
}

KRC_API krc_bool_t krc_special_1128_unicode_to_cp949 (krc_wchar_t unicode, krc_char16_t* cp949)
{
	krc_int_t index;


	// 완성형한글.특수문자1128자: 0xA1A1u - 0xACFEu
	for (index=0; index<1128; index++)
	{
		if (unicode == _krc_code_table_special_1128_cp949_to_unicode[index])
		{
			*cp949 = 0xA1A1u + index/94u*0x0100u + index%94u;
			return KRC_TRUE;
		}
	}

	return KRC_FALSE;
}

KRC_API krc_bool_t krc_hanja_4888_unicode_to_cp949 (krc_wchar_t unicode, krc_char16_t* cp949)
{
	krc_int_t index;


	// 완성형한글.한자4888자: 0xCAA1u-0xFDFEu
	for (index=0; index<4888; index++)
	{
		if (unicode == _krc_code_table_hanja_4888_cp949_to_unicode[index])
		{
			*cp949 = 0xCAA1u + index/94u*0x0100u + index%94u;
			return KRC_TRUE;
		}
	}

	return KRC_FALSE;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _mbcs_ostream
{
	krc_char_t* pointer;
	krc_int_t length;
	krc_int_t offset;
}
mbcs_ostream;

static void mbcs_ostream_init(mbcs_ostream* ctx, krc_char_t* p, krc_int_t l)
{
	ctx->pointer = p;
	ctx->length = l;
	ctx->offset = 0;
}

static void mbcs_ostream_put_char(mbcs_ostream* ctx, krc_char_t c)
{
	if (ctx->offset < ctx->length)
	{
		*(ctx->pointer + ctx->offset) = c;
		ctx->offset++;
	}
}

static void mbcs_ostream_put_char8(mbcs_ostream* ctx, krc_char8_t c)
{
	if (ctx->offset < ctx->length)
	{
		*(ctx->pointer + ctx->offset) = (krc_char_t)c;
		ctx->offset++;
	}
}

static void mbcs_ostream_put_char16(mbcs_ostream* ctx, krc_char16_t c)
{
	if (ctx->offset+1 < ctx->length)
	{
		*(ctx->pointer + ctx->offset) = (krc_char_t)((c & 0xFF00u) >> 8u);
		ctx->offset++;

		*(ctx->pointer + ctx->offset) = (krc_char_t)(c & 0x00FFu);
		ctx->offset++;
	}
}

//===========================================================================
KRC_API krc_int_t krc_unicode_to_cp949(krc_wchar_t* wcs_string, krc_int_t wcs_length, krc_char_t* mbcs_string, krc_int_t mbcs_length)
{
	mbcs_ostream o;


	mbcs_ostream_init(&o, mbcs_string, mbcs_length);


	krc_wchar_t  wcs;
	krc_char8_t  ch1;
	krc_char16_t mbcs;

	
	krc_wchar_t* src;
	krc_int_t index;
	krc_int_t count;


	src = wcs_string;
	count = wcs_length;
	for (index = 0; index < count; index++)
	{
		wcs = *(src + index);
		
		if (wcs == 0x00)
		{
			mbcs_ostream_put_char8(&o, 0u);
			return o.offset;
		}
		else if (wcs < 0x0080u)
		{
			ch1 = (krc_char8_t)(wcs & 0x00FFu);

			mbcs_ostream_put_char8(&o, ch1);
		}
		else if (wcs < 0x0100u)
		{
			ch1 = (krc_char8_t)(wcs & 0x00FFu);

			mbcs_ostream_put_char8(&o, ch1);
		}

		else if (krc_hangul_11172_unicode_to_cp949  (wcs, &mbcs) == KRC_TRUE)
		{		
			mbcs_ostream_put_char16(&o, mbcs);
		}

		else if (krc_special_1128_unicode_to_cp949 (wcs, &mbcs) == KRC_TRUE)
		{		
			mbcs_ostream_put_char16(&o, mbcs);
		}

		else if (krc_hanja_4888_unicode_to_cp949   (wcs, &mbcs) == KRC_TRUE)
		{		
			mbcs_ostream_put_char16(&o, mbcs);
		}

		else
		{
			mbcs_ostream_put_char8(&o, 0x3Fu); // '?'
		}
	}
	mbcs_ostream_put_char8(&o, 0u);


	return o.offset;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _wcs_ostream
{
	krc_wchar_t* pointer;
	krc_int_t length;
	krc_int_t offset;
}
wcs_ostream;

static void wcs_ostream_init(wcs_ostream* ctx, krc_wchar_t* p, krc_int_t l)
{
	ctx->pointer = p;
	ctx->length = l;
	ctx->offset = 0;
}

static void wcs_ostream_put_wchar(wcs_ostream* ctx, krc_wchar_t c)
{
	if (ctx->offset < ctx->length)
	{
		*(ctx->pointer + ctx->offset) = c;
		ctx->offset++;
	}
}

//===========================================================================
KRC_API krc_int_t krc_cp949_to_unicode(krc_char_t* mbcs_string, krc_int_t mbcs_length, krc_wchar_t* wcs_string, krc_int_t wcs_length)
{
	wcs_ostream o;


	wcs_ostream_init(&o, wcs_string, wcs_length);


	krc_wchar_t  wcs;
	krc_char8_t  ch1;
	krc_char8_t  ch2;
	krc_char16_t mbcs;

	krc_char_t* src;
	krc_int_t index;
	krc_int_t count;


	src = mbcs_string;
	count = mbcs_length;
	for (index = 0; index < count; index++)
	{
		ch1 = (krc_char8_t)(*(src + index));


		if (ch1 == 0x00)
		{
			wcs_ostream_put_wchar(&o, 0u);
			return o.offset;
		}
		else if (ch1 < 0x80u)
		{
			// ASCII
			wcs_ostream_put_wchar(&o, ch1);
		}
		else
		{
			if ((index + 1) <= count)
			{
				ch2 = (krc_char8_t)(*(src + index + 1));
				mbcs = (ch1 << 8u) | ch2;


				if (krc_hangul_11172_cp949_to_unicode(mbcs, &wcs) == KRC_TRUE)
				{
					wcs_ostream_put_wchar(&o, wcs);
					index++;
				}

				else if (krc_special_1128_cp949_to_unicode(mbcs, &wcs) == KRC_TRUE)
				{
					wcs_ostream_put_wchar(&o, wcs);
					index++;
				}

				else if (krc_hanja_4888_cp949_to_unicode(mbcs, &wcs) == KRC_TRUE)
				{
					wcs_ostream_put_wchar(&o, wcs);
					index++;
				}

				else
				{
					wcs_ostream_put_wchar(&o, 0x003Fu); // '?'
				}
			}
			else
			{
				wcs_ostream_put_wchar(&o, 0x003Fu); // '?'
			}
		}
	}
	wcs_ostream_put_wchar(&o, 0u);


	return o.offset;
}
