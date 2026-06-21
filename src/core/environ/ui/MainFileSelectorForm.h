#pragma once
#include "FileSelectorForm.h"

namespace ax {
	class LayerColor;
}

class TVPMainFileSelectorForm : public TVPBaseFileSelectorForm {
	typedef TVPBaseFileSelectorForm inherit;
public:
	virtual void bindBodyController(const NodeMap &allNodes) override;

	void show();

	static TVPMainFileSelectorForm *create();

	void initFromFile();

	virtual void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);

protected:
	TVPMainFileSelectorForm();
	virtual void onEnter() override;
	bool CheckDir(const std::string &path);

	virtual void onCellClicked(int idx) override;
	virtual void getShortCutDirList(std::vector<std::string> &pathlist) override;

	void startup(const std::string &path);
	void doStartup(const std::string &path);

	void showMenu(ax::Object*);

	void hideMenu(ax::Object*);

	bool isMenuShowed();
	bool isMenuShrinked();

	void onShowPreferenceConfigAt(const std::string &path);

	void ListHistory();

	class HistoryCell : public ax::ui::Widget {
	public:
		static HistoryCell *create(const std::string &fullpath, const std::string &prefix, const std::string &pathname,
			const std::string &filename) {
			HistoryCell* ret = new HistoryCell();
			ret->autorelease();
			ret->init();
			ret->initInfo(fullpath, prefix, pathname, filename);
			return ret;
		}

		void initInfo(const std::string &fullpath, const std::string &prefix, const std::string &pathname, const std::string &filename);
		void initFunction(const ccWidgetClickCallback &funcDel, const ccWidgetClickCallback &funcJump,
			const ccWidgetClickCallback &funcConf, const ccWidgetClickCallback &funcPlay);

		void rearrangeLayout();
		const std::string &getFullpath() { return _fullpath; }

	private:
		virtual void onSizeChanged() override;

		ax::ui::ScrollView *_scrollview;
		ax::ui::Widget *_btn_delete, *_btn_jump, *_btn_conf, *_btn_play;
		ax::ui::Text* _prefix, *_path, *_file;
		ax::Node *_panel_delete, *_root = nullptr;
		std::string _fullpath;
	};

	void RemoveHistoryCell(ax::Object*, HistoryCell* cell);

	std::string _lastpath;
	ax::ui::Widget *_touchHideMenu;
	ax::ui::ListView *_menuList, *_historyList = nullptr;
	ax::LayerColor* _mask;
	ax::Node *_menu, *_fileList = nullptr;
	ax::Node *newLocalPref, *localPref;
	ax::Size sizeNewLocalPref, sizeLocalPref;
};
