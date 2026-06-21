#pragma once
#include "BaseForm.h"

class TVPGameMainMenu : public ax::Node {
public:
	TVPGameMainMenu(GLubyte opa);
	static TVPGameMainMenu *create(GLubyte opa);

	virtual bool init() override;

	void setMouseIcon(bool bMouse);

	void shrink();
	void expand();
	void toggle();
	void shrinkWithTime(float dur);
	bool isShrinked();

private:
	bool onHandlerTouchBegan(ax::Touch *touch, ax::Event *unusedEvent);
	void onHandlerTouchMoved(ax::Touch *touch, ax::Event *unusedEvent);
	void onHandlerTouchEnded(ax::Touch *touch, ax::Event *unusedEvent);
	void onHandlerTouchCancelled(ax::Touch *touch, ax::Event *unusedEvent);

	bool onBackgroundTouchBegan(ax::Touch *touch, ax::Event *unusedEvent);
	void onBackgroundTouchMoved(ax::Touch *touch, ax::Event *unusedEvent);
	void onBackgroundTouchEnded(ax::Touch *touch, ax::Event *unusedEvent);
	void onBackgroundTouchCancelled(ax::Touch *touch, ax::Event *unusedEvent);

	GLubyte _handler_inactive_opacity;
	bool _hitted;
	bool _shrinked;
	bool _draggingX, _draggingY;

	ax::Node *_root;
	ax::Node *_handler;
	ax::Node *_icon_touch;
	ax::Node *_icon_mouse;

	ax::Vec2 _touchBeganPosition;
	ax::Vec2 _touchMovePosition;
	ax::Vec2 _touchEndPosition;

	unsigned int _touchBeganTime;
};