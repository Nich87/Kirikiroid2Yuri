#pragma once
#include "BaseForm.h"
#include "ui/UIListView.h" // axmol replacement for CCTableView
#include "ui/UITextField.h"
#include "base/RefPtr.h"

class TVPListForm : public ax::Node {
public:
	virtual ~TVPListForm();;
	static TVPListForm * create(const std::vector<ax::ui::Widget*> &cells);

	void initFromInfo(const std::vector<ax::ui::Widget*> &cells);

	void show(); // for background fading

	void close();

private:
	bool onMaskTouchBegan(ax::Touch *t, ax::Event *);

	ax::Node *_root;
};

class TVPFileOperateMenu;
class TVPBaseFileSelectorForm : public iTVPBaseForm, public ax::extension::TableViewDataSource {
public:
	TVPBaseFileSelectorForm();
	virtual ~TVPBaseFileSelectorForm();

	virtual ax::Size tableCellSizeForIndex(ax::extension::TableView *table, ssize_t idx) override;
	virtual ax::extension::TableViewCell* tableCellAtIndex(ax::extension::TableView *table, ssize_t idx) override;
	virtual ssize_t numberOfCellsInTableView(ax::extension::TableView *table) override;
	virtual void onCellClicked(int idx);
	virtual void onCellLongPress(int idx);
	virtual void rearrangeLayout() override;
	static std::pair<std::string, std::string> PathSplit(const std::string &path);

protected:
	virtual void bindBodyController(const NodeMap &allNodes) override;
	virtual void bindHeaderController(const NodeMap &allNodes) override;

	void ListDir(std::string path);
	virtual void getShortCutDirList(std::vector<std::string> &pathlist);

	void onCellItemClicked(ax::Object *owner);
	void onTitleClicked(ax::Object *owner);
	void onBackClicked(ax::Object *owner);
	void _onCellClicked(int idx);

	ax::extension::TableView *FileList;
	ax::ui::Button *_title;

	ax::Node *_fileOperateMenuNode = nullptr;
	ax::Node *_fileOperateMenu = nullptr;
	ax::ui::ListView *_fileOperateMenulist;
	ax::RefPtr<ax::ui::Widget>
		_fileOperateCell_unselect,
		_fileOperateCell_view,
		_fileOperateCell_copy,
		_fileOperateCell_cut,
		_fileOperateCell_paste,
		_fileOperateCell_unpack,
		_fileOperateCell_repack, // TODO
		_fileOperateCell_delete,
		_fileOperateCell_rename,
		_fileOperateCell_sendto;
	
	std::vector<std::string> _clipboardForFileManager;
	std::string _clipboardPath;
	bool _clipboardForMoving = false;
	std::set<int> _selectedFileIndex;
	void onUnselectClicked(ax::Object *owner);
	void onViewClicked(ax::Object *owner);
	void onCopyClicked(ax::Object *owner);
	void onCutClicked(ax::Object *owner);
	void onPasteClicked(ax::Object *owner);
	void onUnpackClicked(ax::Object *owner);
	void onDeleteClicked(ax::Object *owner);
	void onSendToClicked(ax::Object *owner);
	void onBtnRenameClicked(ax::Object *owner);
	void updateFileMenu();
	void clearFileMenu();

	struct FileInfo {
		std::string FullPath;
		std::string NameForDisplay;
		std::string NameForCompare;
		bool IsDir;
		ax::Size CellSize;

		bool operator < (const FileInfo &rhs) const;
	};

	int RootPathLen = 1; // '/'
	std::vector<FileInfo> CurrentDirList;
	std::string ParentPath, CurrentPath;
	class FileItemCell;
	class FileItemCellImpl : public TTouchEventRouter {
	public:
		FileItemCellImpl() : _set(false), _owner(nullptr) {}
		
		static FileItemCellImpl *create(const char * filename, float width) {
			FileItemCellImpl* ret = new FileItemCellImpl();
			ret->autorelease();
			ret->init();
			ret->initFromFile(filename, width);
			return ret;
		}

		void initFromFile(const char * filename, float width);

		void setInfo(int idx, const FileInfo &info, bool selected, bool showSelect);

		void reset() {
			_set = false;
		}

		bool isSet() {
			return _set;
		}

		void setOwner(FileItemCell* owner) {
			_owner = owner;
		}

	private:
		void onClicked(ax::Object*);

		bool _set;
		ax::Size OrigCellModelSize, CellTextAreaSize, OrigCellTextSize;
		ax::ui::Text *FileNameNode;
		ax::Node *DirIcon, *_root, *BgOdd, *BgEven;
		ax::ui::CheckBox *SelectBox;
		FileItemCell *_owner;
	};
	ax::RefPtr<FileItemCellImpl> CellTemplateForSize;
	FileItemCellImpl* FetchCell(FileItemCellImpl* CellModel, ax::extension::TableView *table, ssize_t idx);

	class FileItemCell : public ax::extension::TableViewCell {
		typedef ax::extension::TableViewCell inherit;

	public:
		FileItemCell(TVPBaseFileSelectorForm *owner) : _owner(owner), _impl(nullptr) {}

		static FileItemCell *create(TVPBaseFileSelectorForm *owner) {
			FileItemCell *ret = new FileItemCell(owner);
			ret->init();
			ret->autorelease();
			return ret;
		}

		// retained
		FileItemCellImpl* detach() {
			FileItemCellImpl *ret = _impl;
			if (ret) {
				_impl = nullptr;
				ret->retain();
				ret->removeFromParentAndCleanup(false);
				ret->reset();
			}
			return ret;
		}

		// release
		void attach(FileItemCellImpl *impl) {
			_impl = impl;
			addChild(impl);
			setContentSize(impl->getContentSize());
			impl->setOwner(this);
			impl->release();
		}

		void onClicked() {
			_owner->_onCellClicked(getIdx());
		}

		void onLongPress() {
			_owner->onCellLongPress(getIdx());
		}

	private:
		FileItemCellImpl *_impl;
		TVPBaseFileSelectorForm *_owner;
	};
};

class TVPFileSelectorForm : public TVPBaseFileSelectorForm {
	typedef TVPBaseFileSelectorForm inherit;

public:
	static TVPFileSelectorForm *create(const std::string &initfilename, const std::string &initdir, bool issave);
	void initFromPath(const std::string &initfilename, const std::string &initdir, bool issave);
	void setOnClose(const std::function<void(const std::string &)> &func) { _funcOnClose = func; }

protected:
	virtual void bindFooterController(const NodeMap &allNodes) override;
	virtual void onCellClicked(int idx) override;
	void close();

	ax::ui::Button *_buttonOK, *_buttonCancel;
	ax::ui::TextField *_input;
	std::function<void(const std::string &)> _funcOnClose;
	std::string _result;
	bool _isSaveMode;
};
