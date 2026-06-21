#pragma once
#include "axmol.h"
#include "extensions/axmol-ext.h"
#include "2d/Label.h"

class tTJSNI_BaseLayer;
class iTVPTexture2D;
class DebugViewLayerForm : public ax::Node, public ax::extension::TableViewDataSource {
public:
	static DebugViewLayerForm *create();

	virtual bool init() override;

private:
	virtual ax::Size tableCellSizeForIndex(ax::extension::TableView *table, ssize_t idx) override;
	virtual ax::extension::TableViewCell* tableCellAtIndex(ax::extension::TableView *table, ssize_t idx) override;
	virtual ssize_t numberOfCellsInTableView(ax::extension::TableView *table) override { return _layers.size(); }
	void onExitCallback();
	uint64_t addToLayerVec(int indent, const std::string &prefix, tTJSNI_BaseLayer* lay);

	class DebugViewLayerCell;
	struct LayerInfo {
		std::string Name;
		iTVPTexture2D *Texture;
		size_t VMemSize;
		int Indent;
	};

	ax::Label *_totalSize;
	ax::extension::TableView *_tableView;
	// pair<ident, layer>
	std::vector<LayerInfo> _layers;
};