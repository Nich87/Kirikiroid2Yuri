#pragma once
#include "BaseForm.h"

class TVPTipsHelpForm : public iTVPBaseForm {
	virtual void bindBodyController(const NodeMap &allNodes) override;
	ax::ui::ListView *_tipslist;

public:
	static TVPTipsHelpForm* create();
	static TVPTipsHelpForm* show(const char *tipName = nullptr);

	void setOneTip(const std::string &tipName);
	virtual void rearrangeLayout() override;
};