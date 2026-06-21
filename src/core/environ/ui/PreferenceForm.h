#pragma once
#include "BaseForm.h"
#include "ui/UIWidget.h"
#include "ConfigManager/GlobalConfigManager.h"

namespace tinyxml2 {
	class XMLElement;
}

class iTVPPreferenceInfo;
struct tPreferenceScreen {
	tPreferenceScreen() {}
	tPreferenceScreen(const std::string &title, const std::initializer_list<iTVPPreferenceInfo*> &elem)
		: Title(title), Preferences(elem) {}
	~tPreferenceScreen() { clear(); }
	void clear();
	void init(const std::initializer_list<iTVPPreferenceInfo*> &elem) {
		Preferences = elem;
	}
	std::string Title; // as tid
	std::vector<iTVPPreferenceInfo*> Preferences;
};

class iPreferenceItem;

class iTVPPreferenceInfo {
public:
	virtual iPreferenceItem *createItem(int idx) = 0;
	virtual void InitDefaultConfig() {}
	virtual tPreferenceScreen* GetSubScreenInfo() { return nullptr; }

	iTVPPreferenceInfo() {}
	iTVPPreferenceInfo(const std::string &cap, const std::string &key) : Caption(cap), Key(key) {}
	virtual ~iTVPPreferenceInfo() {}

	std::string Caption;
	std::string Key; // in config
};

template<typename T>
struct tTVPPreferenceInfo : public iTVPPreferenceInfo {
public:
	tTVPPreferenceInfo(const std::string &cap, const std::string &key, const T& defval)
		: iTVPPreferenceInfo(cap, key), DefaultValue(defval) {}

	T DefaultValue;
	// 	iTVPPreferenceInfo(const std::string &cap, const std::string &key, const std::string &defval)
// 		: Caption(cap), Key(key), DefaultValue(defval), SubPrefScreen(nullptr) {}
// 	iTVPPreferenceInfo(const std::string &cap, tPreferenceScreen* subscr = nullptr, bool hidden = false)
// 		: Caption(cap), SubPrefScreen(subscr) {}
// 	~iTVPPreferenceInfo() {
// 		if (SubPrefScreen) delete SubPrefScreen;
// 	}

	virtual void InitDefaultConfig() {
		if (!Key.empty())
			GlobalConfigManager::GetInstance()->GetValue<T>(Key, DefaultValue);
	}

//	tPreferenceScreen *SubPrefScreen = nullptr; // eTypeSubPref
};

class TVPPreferenceForm : public iTVPBaseForm {
protected:
	void initPref(const tPreferenceScreen *config);
	virtual void bindBodyController(const NodeMap &allNodes) override;
	virtual void bindHeaderController(const NodeMap &allNodes) override;

	const tPreferenceScreen *Config = nullptr;
	ax::ui::ListView *PrefList;
	ax::ui::Button *_title;
};

class iPreferenceItem : public ax::ui::Widget {
public:
	void initFromInfo(int idx, ax::Size size, const std::string& title); // not tid

protected:
	virtual void initController(const NodeMap &allNodes) = 0;
	virtual const char *getUIFileName() const = 0;
	ax::ui::Text *_title;
	ax::Node *BgOdd, *BgEven;
};

template<typename TArg>
class tPreferenceItem : public iPreferenceItem {
public:
	typedef TArg ValueType;
	std::function<TArg()> _getter;
	std::function<void(TArg)> _setter;
};

template<typename T> // factory function
T* CreatePreferenceItem(int idx, const ax::Size &size, const std::string &title, const std::function<void(T*)> &initer) {
	T *ret = new T;
	ret->autorelease();
	initer(ret);
	ret->initFromInfo(idx, size, title);
	return ret;
}

template<typename T>
T* CreatePreferenceItem(int idx, const ax::Size &size, const std::string &title) {
	T *ret = new T;
	ret->autorelease();
	ret->initFromInfo(idx, size, title);
	return ret;
}

class tPreferenceItemCheckBox : public tPreferenceItem<bool> {
public:
	tPreferenceItemCheckBox();

protected:
	virtual void initController(const NodeMap &allNodes) override;
	virtual const char* getUIFileName() const override;
	virtual void onPressStateChangedToNormal() override;
	virtual void onPressStateChangedToPressed() override;
	ax::ui::CheckBox *checkbox;
	ax::Node *highlight;
};

class tPreferenceItemWithHighlight : public iPreferenceItem {
protected:
	virtual void initController(const NodeMap &allNodes) override;

	virtual void onPressStateChangedToNormal() override;
	virtual void onPressStateChangedToPressed() override;

	ax::Node *highlight = nullptr;
};

class tPreferenceItemSubDir : public tPreferenceItemWithHighlight {
public:
	tPreferenceItemSubDir();

protected:
	virtual const char* getUIFileName() const override;
};

class tPreferenceItemConstant : public tPreferenceItemSubDir {
protected:
	virtual void initController(const NodeMap &allNodes) override;
};

class tPreferenceItemSelectListInfo {
public:
	virtual const std::vector<std::pair<std::string, std::string> >& getListInfo() = 0;
};

class tPreferenceItemSelectList : public tPreferenceItem<std::string> {
public:
	tPreferenceItemSelectList();

	void initInfo(tPreferenceItemSelectListInfo* info) { CurInfo = info; }

protected:
	virtual void initController(const NodeMap &allNodes) override;
	virtual const char* getUIFileName() const override;

	virtual void onPressStateChangedToNormal() override;
	virtual void onPressStateChangedToPressed() override;

	void showForm(ax::Object*);
	void updateHightlight();

	ax::Node *highlight = nullptr;
	ax::ui::Text *selected = nullptr;
	tPreferenceItemSelectListInfo* CurInfo;
	std::string highlightTid;
};

class tPreferenceItemFileSelect : public tPreferenceItem<std::string> {
protected:
	virtual void initController(const NodeMap &allNodes) override;
	virtual const char* getUIFileName() const override;

	virtual void onPressStateChangedToNormal() override;
	virtual void onPressStateChangedToPressed() override;

	void showForm(ax::Object*);
	void updateHightlight();

	ax::Node *highlight = nullptr;
	ax::ui::Text *selected = nullptr;
	std::string highlightTid;
};

class tPreferenceItemKeyValPair : public tPreferenceItem<std::pair<std::string,std::string> > {
public:
	tPreferenceItemKeyValPair();

protected:
	virtual void initController(const NodeMap &allNodes) override;
	virtual const char* getUIFileName() const override;

	virtual void onPressStateChangedToNormal() override;
	virtual void onPressStateChangedToPressed() override;

	void showInput(ax::Object*);
	void updateText();

	ax::Node *highlight;
	ax::ui::Text *selected;
};

class TVPCustomPreferenceForm : public iTVPBaseForm {
public:
	static TVPCustomPreferenceForm *create(const std::string &tid_title, int count,
		const std::function<std::pair<std::string, std::string>(int)> &getter,
		const std::function<void(int, const std::pair<std::string, std::string>&)> &setter);

protected:
	void initFromInfo(const std::string &tid_title, int count,
		const std::function<std::pair<std::string, std::string>(int)> &getter,
		const std::function<void(int, const std::pair<std::string, std::string>&)> &setter);

	virtual void bindBodyController(const NodeMap &allNodes) override;
	virtual void bindHeaderController(const NodeMap &allNodes) override;

	std::function<std::pair<std::string, std::string>(int)> _getter;
	std::function<void(int, const std::pair<std::string, std::string>&)> _setter;
	ax::ui::ListView *_listview;
	ax::ui::Button *_title;
};

class iPreferenceItemSlider : public tPreferenceItem<float> {
	typedef tPreferenceItem<float> inherit;

public:
	iPreferenceItemSlider(float r) : _resetValue(r) {}
	virtual void initController(const NodeMap &allNodes) override;

protected:
	ax::ui::Slider* _slider;
	ax::ui::Button *_reset;
	float _resetValue;
};

class tPreferenceItemCursorSlider : public iPreferenceItemSlider {
	typedef iPreferenceItemSlider inherit;

public:
	tPreferenceItemCursorSlider(float r, const std::function<float(float)> &f)
		: iPreferenceItemSlider(r), _curScaleConv(f) {}

protected:
	virtual void initController(const NodeMap &allNodes) override;
	virtual const char* getUIFileName() const override;

	virtual void onEnter() override;

	ax::Node *_icon;
	ax::Node *_cursor;
	std::function<float(float)> _curScaleConv;
};

class tPreferenceItemTextSlider : public iPreferenceItemSlider {
	typedef iPreferenceItemSlider inherit;

public:
	tPreferenceItemTextSlider(float r, const std::function<std::string(float)> &f)
		: iPreferenceItemSlider(r), _strScaleConv(f) {}

protected:
	virtual void initController(const NodeMap &allNodes) override;
	virtual const char* getUIFileName() const override;

	ax::ui::Text *_text;
	std::function<std::string(float)> _strScaleConv;
};

class tPreferenceItemDeletable : public iPreferenceItem {
	virtual void initController(const NodeMap &allNodes) override;
	virtual const char* getUIFileName() const override;
	void onTouchEvent(ax::Object*, ax::ui::Widget::TouchEventType);
	void walkTouchEvent(ax::ui::Widget* node);

	ax::ui::Widget *_deleteIcon;
	ax::ui::ScrollView *_scrollview;

public:
	std::function<void(tPreferenceItemDeletable*)> _onDelete;
};

class tPreferenceItemKeyMap : public tPreferenceItemDeletable {
public:
	std::pair<int, int> _keypair;

	void initData(int k, int v, int idx, const ax::Size &size);
};

class KeyMapPreferenceForm : public TVPPreferenceForm {
	iSysConfigManager* _mgr;
	KeyMapPreferenceForm(iSysConfigManager* mgr);
	void initData();

public:
	static KeyMapPreferenceForm* create(iSysConfigManager* mgr);
};
