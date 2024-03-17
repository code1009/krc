﻿#ifndef __krc_code_convert__h__
#define __krc_code_convert__h__

/////////////////////////////////////////////////////////////////////////////
//
// File: krc_code_convert.h
//
// Created by MOON, Eui-kwon.
// Created on Nov-14th, 2019.
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
KRC_API krc_uint_t krc_code_type_cp949(krc_uchar16_t ch);
KRC_API krc_uint_t krc_char_type_unicode(krc_wchar_t ch);

KRC_API krc_int_t krc_hangul_11172_index_cp949(krc_uchar16_t ch);
KRC_API krc_int_t krc_special_1128_index_cp949(krc_uchar16_t ch);
KRC_API krc_int_t krc_hanja_4888_index_cp949(krc_uchar16_t ch);

KRC_API krc_bool_t krc_hangul_11172_cp949_to_unicode(krc_uchar16_t cp949, krc_wchar_t* unicode);
KRC_API krc_bool_t krc_special_1128_cp949_to_unicode(krc_uchar16_t cp949, krc_wchar_t* unicode);
KRC_API krc_bool_t krc_hanja_4888_cp949_to_unicode(krc_uchar16_t cp949, krc_wchar_t* unicode);
KRC_API krc_int_t krc_cp949_to_unicode(krc_char_t* mbcs_string, krc_int_t mbcs_length, krc_wchar_t* wcs_string, krc_int_t wcs_length);

KRC_API krc_bool_t krc_hangul_11172_unicode_to_cp949(krc_wchar_t unicode, krc_uchar16_t* cp949);
KRC_API krc_bool_t krc_special_1128_unicode_to_cp949(krc_wchar_t unicode, krc_uchar16_t* cp949);
KRC_API krc_bool_t krc_hanja_4888_unicode_to_cp949(krc_wchar_t unicode, krc_uchar16_t* cp949);
KRC_API krc_int_t krc_unicode_to_cp949(krc_wchar_t* wcs_string, krc_int_t wcs_length, krc_char_t* mbcs_string, krc_int_t mbcs_length);





#endif




