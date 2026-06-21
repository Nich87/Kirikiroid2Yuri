#pragma once
#include "axmol.h"

class TVPYUVSprite : public ax::Sprite {
	ax::Texture2D* _textureU = nullptr;
	ax::Texture2D* _textureV = nullptr;

	void updateTextureDataInternal(ax::Texture2D *pTex, const void* data, int width, int height,
		ax::PixelFormat pixfmt);

public:
	virtual ~TVPYUVSprite();

	static TVPYUVSprite* create();

	bool init() override;

	void updateTextureData(const void* data, int width, int height);

	void updateTextureData(
		const void* Y, int YW, int YH,
		const void* U, int UW, int UH,
		const void* V, int VW, int VH);
};
