#pragma once
#include "BaseForm.h"
#include "base/RefPtr.h"

class TVPMessageBoxForm : public iTVPBaseForm {
public:
	static void show(const std::string &caption, const std::string &text, int nBtns,
		const std::string *btnText, const std::function<void(int)> &callback);

	static void showYesNo(const std::string &caption, const std::string &text, const std::function<void(int)> &callback);

private:
	void init(const std::string &caption, const std::string &text, int nBtns,
		const std::string *btnText, const std::function<void(int)> &callback);
	virtual void bindBodyController(const NodeMap &allNodes) override;
	virtual void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);

	ax::ui::ScrollView *_textContainer; // "text"
	ax::Node *_btnList; // parent of "btn"
	ax::ui::Text* _title, *_textContent; // "content"
	ax::ui::Widget *_btnModel; // "btn"
	ax::ui::Button *_btnBody;

	std::function<void(int)> _callback;
};

class TVPSimpleProgressForm : public iTVPBaseForm {
public:
	static TVPSimpleProgressForm* create();

	void initButtons(const std::vector<std::pair<std::string, std::function<void(ax::Object*)> > > &vec);

	void setTitle(const std::string &text);
	void setContent(const std::string &text);
	void setPercentWithText(float percent);
	void setPercentWithText2(float percent);
	void setPercentOnly(float percent);
	void setPercentOnly2(float percent);
	void setPercentText(const std::string &text);
	void setPercentText2(const std::string &text);
	void setProgress2Visible(bool visible);

private:
	void bindBodyController(const NodeMap &allNodes) override;

	ax::ui::LoadingBar *_progressBar[2];
	ax::ui::Text *_textTitle, *_textContent, *_textProgress[2];
	std::vector<ax::ui::Button *> _vecButtons;
	ax::Node *_btnContainer;
	// button template
	ax::RefPtr<ax::ui::Widget> _btnCell;
	ax::ui::Button *_btnButton;
};
