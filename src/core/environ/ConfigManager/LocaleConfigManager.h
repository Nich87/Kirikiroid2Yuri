// multi language config mainly for ui
#pragma once
#include <unordered_map>
#include <string>
#include <vector>

namespace ax {
	namespace ui {
		class Text;
		class Button;
	}
}

class LocaleConfigManager {

	std::unordered_map<std::string, std::string> AllConfig; // tid->text in utf8

	bool ConfigUpdated;

	LocaleConfigManager();

	std::string GetFilePath();

public:
	static LocaleConfigManager* GetInstance();

	void Initialize(const std::string &sysLang);

	const std::string &GetText(const std::string &tid); // in utf8

	bool initText(ax::ui::Text *ctrl);
	bool initText(ax::ui::Button *ctrl);
	bool initText(ax::ui::Text *ctrl, const std::string &tid);
	bool initText(ax::ui::Button *ctrl, const std::string &tid);

private:

	std::string currentLangCode;
};