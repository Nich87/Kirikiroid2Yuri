#pragma once
#include "axmol.h"
#include "extensions/axmol-ext.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include <unordered_map>

namespace cocostudio {
	namespace timeline {
		class ActionTimeline;
	}
}

class NodeMap : public std::unordered_map<std::string, ax::Node*> {
protected:
	const char *FileName;
	void onLoadError(const std::string &name) const;

public:
	NodeMap();
	NodeMap(const char *filename, ax::Node* node);
	template<typename T = ax::Node>
	T *findController(const std::string &name, bool notice = true) const {
		ax::Node *node = findController<ax::Node>(name, notice);
		if (node) {
			T *ret = dynamic_cast<T*>(node);
			if (!ret) {
				onLoadError(name);
			}
			return ret;
		}
		return nullptr;
	}
	ax::ui::Widget *findWidget(const std::string &name, bool notice = true) const {
		return findController<ax::ui::Widget>(name, notice);
	}
	void initFromNode(ax::Node* node);
};
template<> ax::Node *NodeMap::findController<ax::Node>(const std::string &name, bool notice) const;

class CSBReader : public NodeMap {
public:
	ax::Node* Load(const char *filename);
};

class iTVPBaseForm : public ax::Node {
public:
	iTVPBaseForm()
		: RootNode(nullptr){}
	virtual ~iTVPBaseForm();

	void Show();

	virtual void rearrangeLayout();
	virtual void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);

protected:
	bool initFromFile(const char *navibar, const char *body, const char *bottombar, ax::Node *parent = nullptr);
	bool initFromFile(const char *body) {
		return initFromFile(nullptr, body, nullptr);
	}

	virtual void bindBodyController(const NodeMap &allNodes) {}
	virtual void bindFooterController(const NodeMap &allNodes) {}
	virtual void bindHeaderController(const NodeMap &allNodes) {}

	ax::ui::Widget *RootNode;

	struct {
		//ax::ui::Button *Title;
		ax::ui::Button *Left;
		ax::ui::Widget *Right;
		ax::Node *Root;
	} NaviBar;

	struct {
		//ax::ui::ListView *Panel;
		ax::Node *Root;
	} BottomBar;
};

class TTouchEventRouter : public ax::ui::Widget {
public:
	typedef std::function<void(ax::ui::Widget::TouchEventType event,
		ax::ui::Widget* sender, ax::Touch *touch)> EventFunc;

	static TTouchEventRouter *create() {
		TTouchEventRouter * ret = new TTouchEventRouter;
		ret->init();
		ret->autorelease();
		return ret;
	}

	void setEventFunc(const EventFunc &func) {
		_func = func;
	}

	virtual void interceptTouchEvent(ax::ui::Widget::TouchEventType event,
		ax::ui::Widget* sender, ax::Touch *touch) override {
		if (_func) _func(event, sender, touch);
	}

private:
	EventFunc _func;
};

class TCommonTableCell : public ax::extension::TableViewCell {
	typedef ax::extension::TableViewCell inherit;

protected: // must be inherited
	TCommonTableCell() : _router(nullptr) {}

public:
	virtual ~TCommonTableCell() {
		if (_router) _router->release();
	}

	virtual void setContentSize(const ax::Size& contentSize) {
		inherit::setContentSize(contentSize);
		if (_router) _router->setContentSize(contentSize);
	}

	virtual bool init() {
		bool ret = inherit::init();
		_router = TTouchEventRouter::create();
		return ret;
	}

protected:
	TTouchEventRouter *_router;
};

class iTVPFloatForm : public iTVPBaseForm {
public:
	virtual void rearrangeLayout() override;
};

void ReloadTableViewAndKeepPos(ax::extension::TableView *pTableView);
