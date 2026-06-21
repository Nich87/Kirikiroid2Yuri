#pragma once
#include "axmol.h" // provides AX_SYNTHESIZE etc.
#include "extensions/axmol-ext.h" // provides ax::extension::ScrollView, ax::extension::TableView
#include "cocostudio/ActionTimeline/CSLoader.h"

class XKPageView;
class XKPageViewDelegate : public ax::extension::ScrollViewDelegate
{
public:
	virtual ~XKPageViewDelegate(){};
	XKPageViewDelegate(){};
	virtual ax::Size sizeForPerPage() = 0;
	virtual void pageViewDidScroll(XKPageView *pageView){};
};

class XKPageView : public ax::extension::ScrollView
{
public:
	static XKPageView *create(ax::Size size, XKPageViewDelegate *delegate);
	virtual bool init(ax::Size size, XKPageViewDelegate *delegate);
	ssize_t getCurPageIndex() const { return current_index; }
	void setCurPageIndex(ssize_t idx);
	ssize_t getPageCount() const { return pageCount; }
public:
	void setPageSize(const ax::Size &size) { pageSize = size; }
	virtual void setContentOffsetInDuration(ax::Vec2 offset, float dt);
	virtual void setContentOffset(ax::Vec2 offset);
	void setTouchEnabled(bool enabled);

private:
	virtual bool onTouchBegan(ax::Touch *touch, ax::Event *unusedEvent) override;
	virtual void onTouchMoved(ax::Touch *touch, ax::Event *unusedEvent) override;
	virtual void onTouchEnded(ax::Touch *touch, ax::Event *unusedEvent) override;

	void performedAnimatedScroll(float dt);
	int current_index;
	float current_offset;

	void adjust(float offset);
	ax::Size pageSize;
	AX_SYNTHESIZE(XKPageViewDelegate *, _delegate, Delegate);
public:
	int pageCount;
	void addPage(Node *node);
	ax::Node *getPageAtIndex(int index);
};

void TVPInitUIExtension();