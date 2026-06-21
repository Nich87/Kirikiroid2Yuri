#include "YUVSprite.h"

using namespace ax;

TVPYUVSprite::~TVPYUVSprite()
{
	AX_SAFE_RELEASE(_textureU);
	AX_SAFE_RELEASE(_textureV);
}

TVPYUVSprite* TVPYUVSprite::create()
{
	TVPYUVSprite* sprite = new TVPYUVSprite;
	if (sprite && sprite->init()) {
		sprite->autorelease();
		return sprite;
	}
	AX_SAFE_DELETE(sprite);
	return nullptr;
}

bool TVPYUVSprite::init()
{
	auto tex = new ax::Texture2D;
	if (!initWithTexture(tex)) {
		AX_SAFE_RELEASE(tex);
		return false;
	}
	tex->release();

	setProgramState(ax::backend::ProgramType::VIDEO_TEXTURE_I420);

	_textureU = new ax::Texture2D;
	_textureU->retain();
	_textureV = new ax::Texture2D;
	_textureV->retain();

	_textureU->setAliasTexParameters();
	_textureV->setAliasTexParameters();

	return true;
}

void TVPYUVSprite::updateTextureDataInternal(ax::Texture2D *pTex, const void* data, int width, int height, ax::PixelFormat pixfmt)
{
	const ax::Size &size = pTex->getContentSize();
	ax::Size videoSize(width, height);
	if (size.width != videoSize.width || size.height != videoSize.height || pTex->getPixelFormat() != pixfmt) {
		ssize_t datasize = width * height;
		switch (pixfmt) {
		case ax::PixelFormat::BGRA8:
		case ax::PixelFormat::RGBA8:
			datasize *= 4; break;
		case ax::PixelFormat::RGB8:
			datasize *= 3; break;
		case ax::PixelFormat::RGB565:
		case ax::PixelFormat::RGBA4:
			datasize *= 2; break;
		default:
			break;
		}
		pTex->initWithData(data, datasize, pixfmt, width, height, false);
	} else {
		ssize_t datasize = width * height;
		switch (pixfmt) {
		case ax::PixelFormat::BGRA8:
		case ax::PixelFormat::RGBA8:
			datasize *= 4; break;
		case ax::PixelFormat::RGB8:
			datasize *= 3; break;
		case ax::PixelFormat::RGB565:
		case ax::PixelFormat::RGBA4:
			datasize *= 2; break;
		default:
			break;
		}
		pTex->updateWithData(data, datasize, pixfmt, pixfmt, width, height, false, 0);
	}
}

void TVPYUVSprite::updateTextureData(const void* data, int width, int height)
{
	ax::Texture2D *pTex = getTexture();
	const ax::Size &size = pTex->getContentSize();
	updateTextureDataInternal(pTex, data, width, height, ax::PixelFormat::RGBA8);
	if (size.width != width || size.height != height) {
		setTextureRect(ax::Rect(0, 0, width, height));
	}
	setProgramState(ax::backend::ProgramType::POSITION_TEXTURE_COLOR);
}

void TVPYUVSprite::updateTextureData(const void* Y, int YW, int YH, const void* U, int UW, int UH, const void* V, int VW, int VH)
{
	ax::Texture2D *pTex = getTexture();
	const ax::Size &size = pTex->getContentSize();
	updateTextureDataInternal(pTex, Y, YW, YH, ax::PixelFormat::R8);
	updateTextureDataInternal(_textureU, U, UW, UH, ax::PixelFormat::R8);
	updateTextureDataInternal(_textureV, V, VW, VH, ax::PixelFormat::R8);
	if (size.width != YW || size.height != YH) {
		setTextureRect(ax::Rect(0, 0, YW, YH));
	}

	setProgramState(ax::backend::ProgramType::VIDEO_TEXTURE_I420);

	auto ps = getProgramState();
	if (ps) {
		ps->setTexture(ps->getUniformLocation("u_tex1"), 1, _textureU->getBackendTexture());
		ps->setTexture(ps->getUniformLocation("u_tex2"), 2, _textureV->getBackendTexture());

		const ax::Mat4 colorTransform{
			1.16438356f,   0.00000000f,    1.79265225f,     0.0f,
			1.16438356f,  -0.213237017f, - 0.533004045f,    0.0f,
			1.16438356f,   2.11241937f,    0.00000000f,     0.0f,
			0.0627451017f, 0.501960814f,   0.501960814f,    0.0f
		};
		ps->setUniform(ps->getUniformLocation("colorTransform"), &colorTransform, sizeof(colorTransform));
	}
}
