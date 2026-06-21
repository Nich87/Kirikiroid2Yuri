//---------------------------------------------------------------------------
/*
	Risa [�肳]      alias �g���g��3 [kirikiri-3]
	 stands for "Risa Is a Stagecraft Architecture"
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
//! @file
//! @brief FreeType �t�H���g�h���C�o
//---------------------------------------------------------------------------
#ifndef _FREETYPE_H_
#define _FREETYPE_H_


#include "CharacterData.h"
#include "FreeTypeFace.h"
//#include "NativeFreeTypeDriver.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4819)
#endif
#include "ft2build.h"
#include "freetype/freetype.h"
// #include FT_FREETYPE_H
#ifdef _MSC_VER
#pragma warning(pop)
#endif

//---------------------------------------------------------------------------
#define	TVP_GET_FACE_INDEX_FROM_OPTIONS(x) ((x) & 0xff) //!< �I�v�V������������Face�C���f�b�N�X�����o���}�N��
#define	TVP_FACE_OPTIONS_FACE_INDEX(x)		((x) & 0xff) //!< Face�C���f�b�N�X���I�v�V���������ɕϊ�����}�N��
#define	TVP_FACE_OPTIONS_FILE				0x00010000 //!< �t�H���g���ł͂Ȃ��ăt�@�C�����ɂ��t�H���g�̎w����s��
#define TVP_FACE_OPTIONS_NO_HINTING			0x00020000 //!< �q���e�B���O���s��Ȃ�
#define TVP_FACE_OPTIONS_FORCE_AUTO_HINTING	0x00020000 //!< �����I�� auto hinting ���s��
#define TVP_FACE_OPTIONS_NO_ANTIALIASING	0x00040000 //!< �A���`�G�C���A�X���s��Ȃ�

//---------------------------------------------------------------------------
/**
 * FreeType �t�H���g face
 */
class tFreeTypeFace
{
	ttstr FontName;		//!< �t�H���g��
	tBaseFreeTypeFace * Face; //!< Face �I�u�W�F�N�g
	FT_Face FTFace; //!< FreeType Face �I�u�W�F�N�g
	tjs_uint32 Options; //!< �t���O

	typedef std::vector<FT_ULong> tGlyphIndexToCharcodeVector;
	tGlyphIndexToCharcodeVector * GlyphIndexToCharcodeVector;		//!< �O���t�C���f�b�N�X���當���R�[�h�ւ̕ϊ��}�b�v
	tjs_int Height;		//!< �t�H���g�T�C�Y(����) in pixel

	tjs_uint (*UnicodeToLocalChar)(tjs_char in); //!< SJIS�Ȃǂ�Unicode�ɕϊ�����֐�
	tjs_char (*LocalCharToUnicode)(tjs_uint in); //!< Unicode��SJIS�Ȃǂɕϊ�����֐�

	static inline tjs_int FT_PosToInt( tjs_int x ) { return (((x) + (1 << 5)) >> 6); }
public:
	tFreeTypeFace(const ttstr &fontname, tjs_uint32 options);
	~tFreeTypeFace();

	tjs_uint GetGlyphCount();
	tjs_char GetCharcodeFromGlyphIndex(tjs_uint index);

	void GetFaceNameList(std::vector<ttstr> &dest);

	const ttstr& GetFontName() const { return FontName; }

	tjs_int GetHeight() { return Height; }
	void SetHeight(int height);

	void SetOption( tjs_uint32 opt ) {
		Options |= opt;
	}
	void ClearOption( tjs_uint32 opt ) {
		Options &= ~opt;
	}
	bool GetOption( tjs_uint32 opt ) const {
		return (Options&opt) == opt;
	}
	tjs_char GetDefaultChar() const {
		return Face->GetDefaultChar();
	}
	tjs_char GetFirstChar() {
		FT_UInt gindex;
		return static_cast<tjs_char>( FT_Get_First_Char( FTFace, &gindex ) );
	}

	tjs_int GetAscent() const {
		tjs_int ppem = FTFace->size->metrics.y_ppem;
		tjs_int upe = FTFace->units_per_EM;
		return FTFace->ascender * ppem / upe;
	}
	void GetUnderline( tjs_int& pos, tjs_int& thickness ) const {
		tjs_int ppem = FTFace->size->metrics.y_ppem;
		tjs_int upe = FTFace->units_per_EM;
		tjs_int liney = 0; //�����̈ʒu
		tjs_int height = FT_PosToInt( FTFace->size->metrics.height );
		liney = ((FTFace->ascender-FTFace->underline_position) * ppem) / upe;
		thickness = (FTFace->underline_thickness * ppem) / upe;
		if( thickness < 1 ) thickness = 1;
		if( liney > height ) {
			liney = height - 1;
		}
		pos = liney;
	}
	void GetStrikeOut( tjs_int& pos, tjs_int& thickness ) const {
		tjs_int ppem = FTFace->size->metrics.y_ppem;
		tjs_int upe = FTFace->units_per_EM;
		thickness = FTFace->underline_thickness * ppem / upe;
		if( thickness < 1 ) thickness = 1;
		pos = FTFace->ascender * 7 * ppem / (10 * upe);
	}
	tTVPCharacterData * GetGlyphFromCharcode(tjs_char code);
	bool GetGlyphRectFromCharcode(struct tTVPRect& rt, tjs_char code, tjs_int& advancex, tjs_int& advancey );
	bool GetGlyphMetricsFromCharcode(tjs_char code, tGlyphMetrics & metrics);
	bool GetGlyphSizeFromCharcode(tjs_char code, tGlyphMetrics & metrics);

	const FT_Outline* GetOulineData(tjs_char code, float &w, float &h);
	tBaseFreeTypeFace *GetBaseFace() { return Face; }

private:
	bool LoadGlyphSlotFromCharcode(tjs_char code);
};
//---------------------------------------------------------------------------

#endif /*_FREETYPE_H_*/
