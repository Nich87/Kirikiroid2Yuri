#pragma once

#include "2d/Node.h"
#include "2d/Label.h"
#include <deque>
#include <vector>
#include "tjsCommHead.h"

class TVPConsoleWindow : public ax::Node {
	TVPConsoleWindow();
public:
	static TVPConsoleWindow* create(int fontSize, ax::Node *parent);

	void addLine(const ttstr &line, ax::Color3B clr);

	void setFontSize(float size);

	virtual void visit(ax::Renderer *renderer, const ax::Mat4& parentTransform, uint32_t parentFlags) override;
private:
	float _fontSize;

	std::deque<ax::Label*> _dispLabels;
	std::vector<ax::Label*> _unusedLabels;
	std::deque<std::pair<ttstr, ax::Color3B> > _queuedLines;
	unsigned int _maxQueueSize;
};
