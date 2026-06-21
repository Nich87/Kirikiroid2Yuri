#pragma once
#include "BaseForm.h"
#include "extension/UIExtension.h"

class iTVPHalfScreenForm : public iTVPBaseForm {
public:
	virtual void rearrangeLayout() override;
};

class TVPSelectListForm : public iTVPHalfScreenForm {
public:
	static TVPSelectListForm *create(const std::vector<std::string> &info,
		const std::string &highlight_tid, const std::function<void(int)> &funcok);

protected:
	virtual void bindBodyController(const NodeMap &allNodes) override;
	void initWithInfo(const std::vector<std::string> &info, const std::string &highlight_tid);

	std::function<void(int)> FuncOK;

	XKPageView *pageView;
};

class TVPTextPairInputForm : public iTVPHalfScreenForm {
public:
	static TVPTextPairInputForm *create(const std::string &text1,
		const std::string &text2, const std::function<void(const std::string &, const std::string &)> &funcok);

private:
	virtual void bindBodyController(const NodeMap &allNodes) override;
	void initWithInfo(const std::string &text1, const std::string &text2);

	std::function<void(const std::string &, const std::string &)> FuncOK;

	ax::ui::TextField *input1, *input2;
};

class TVPKeyPairSelectForm : public TVPSelectListForm {
	typedef TVPSelectListForm inherit;

	ax::EventListenerKeyboard* _keylistener = nullptr;
	std::vector<std::string> _keyinfo;
	std::function<void(int)> _funcok;

public:
	static TVPKeyPairSelectForm *create(const std::function<void(int/*keycode*/)> &funcok);

	virtual ~TVPKeyPairSelectForm();

	void initWithInfo();

	void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
	void onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
};