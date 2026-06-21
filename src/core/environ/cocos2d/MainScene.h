#pragma once

#include "2d/Scene.h"
#include "ui/UIWidget.h"
#include "base/IMEDelegate.h"

namespace ax {
	class Controller;
}

class TVPWindowLayer;
class TVPGameMainMenu;
class TVPMainScene : public ax::Scene, public ax::IMEDelegate
{
	TVPMainScene();
	static TVPMainScene *create();
	virtual void update(float delta) override;
	void initialize();
	friend class TVPAppDelegate;
public:
	static TVPMainScene* GetInstance();
	static TVPMainScene* CreateInstance();

	static void setMaskLayTouchBegain(const std::function<bool(ax::Touch *, ax::Event *)> &func);

	enum eEnterAni {
		eEnterAniNone,
		eEnterAniOverFromRight,
		eEnterFromBottom,
	};

	void pushUIForm(ax::Node *node, eEnterAni ani = eEnterAniOverFromRight);

	enum eLeaveAni {
		eLeaveAniNone,
		eLeaveAniLeaveFromLeft,
		eLeaveToBottom,
	};
	void popUIForm(ax::Node *node, eLeaveAni ani = eLeaveAniLeaveFromLeft);
	void popAllUIForm();

	void addLayer(TVPWindowLayer* lay);
	ax::Size getUINodeSize();
	ax::Size getGameNodeSize() { return GameNode->getContentSize(); }
	void rotateUI();

	bool startupFrom(const std::string &path);

	float getUIScale();

	void showWindowManagerOverlay(bool bVisible);

	void toggleVirtualMouseCursor();
	void showVirtualMouseCursor(bool bVisible);
	bool isVirtualMouseMode() const;

	virtual bool attachWithIME() override;
	virtual bool detachWithIME() override;

	static void onCharInput(int keyCode);
	static void onTextInput(const std::string &text);

	static float convertCursorScale(float cfgScale/*0 ~ 1*/);

private:
	void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
	void onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);

	bool onTouchBegan(ax::Touch *touch, ax::Event *event);
	void onTouchMoved(ax::Touch *touch, ax::Event *event);
	void onTouchEnded(ax::Touch *touch, ax::Event *event);
	void onTouchCancelled(ax::Touch *touch, ax::Event *event);

	void onAxisEvent(ax::Controller* ctrl, int code, ax::Event *e);
	void onPadKeyDown(ax::Controller* ctrl, int code, ax::Event *e);
	void onPadKeyUp(ax::Controller* ctrl, int code, ax::Event *e);
	void onPadKeyRepeat(ax::Controller* ctrl, int code, ax::Event *e);

	virtual bool canAttachWithIME() override;
	virtual bool canDetachWithIME() override;
	virtual void deleteBackward();
	virtual void insertText(const char * text, size_t len);

	void doStartup(float dt, std::string path);

	float ScreenRatio;
	ax::Size SceneSize, UISize;
	ax::Node *UINode, *GameNode;
	ax::EventListenerTouchOneByOne* _touchListener;
	TVPGameMainMenu *_gameMenu;
};