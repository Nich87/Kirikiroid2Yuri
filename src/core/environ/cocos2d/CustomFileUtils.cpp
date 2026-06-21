#include "CustomFileUtils.h"
#if AX_TARGET_PLATFORM == AX_PLATFORM_WIN32
#include "platform/FileUtils.h"
#elif AX_TARGET_PLATFORM == AX_PLATFORM_IOS
#import <Foundation/NSBundle.h>
#import "platform/apple/CCFileUtils-apple.h"
#elif AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID
#include "platform/android/FileUtils-android.h"
#elif AX_TARGET_PLATFORM == AX_PLATFORM_LINUX
#include "platform/linux/FileUtils-linux.h"
#endif
#ifdef MINIZIP_FROM_SYSTEM
#include <minizip/unzip.h>
#else // from our embedded sources
#include "3rdparty/unzip/unzip.h"
#endif
#include "ConfigManager/LocaleConfigManager.h"

namespace ax {

typedef
#if AX_TARGET_PLATFORM == AX_PLATFORM_WIN32
FileUtilsWin32
#elif AX_TARGET_PLATFORM == AX_PLATFORM_IOS
FileUtilsApple
#elif AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID
FileUtilsAndroid
#elif AX_TARGET_PLATFORM == AX_PLATFORM_LINUX
FileUtilsLinux
#else
FileUtils
#endif
FileUtilsInherit;

class CustomFileUtils : public FileUtilsInherit
{
public:
	CustomFileUtils();

	void addAutoSearchArchive(std::string_view path);
	virtual std::string fullPathForFilename(std::string_view filename) const override;
	virtual std::string getStringFromFile(std::string_view filename) const override;
	virtual Data getDataFromFile(std::string_view filename) const override;
	virtual bool isFileExistInternal(std::string_view strFilePath) const override;
	virtual bool isDirectoryExistInternal(std::string_view dirPath) const override;
	virtual bool init() override {
		return FileUtilsInherit::init();
	}

public:
	unsigned char* getFileData(std::string_view filename, const char* mode, ssize_t *size);

private:
	unsigned char* getFileDataFromArchive(std::string_view filename, ssize_t *size) const;

	mutable std::unordered_map<std::string, std::pair<unzFile, unz_file_pos> > _autoSearchArchive;
	mutable std::mutex _lock;
};

CustomFileUtils::CustomFileUtils()
{
}

void CustomFileUtils::addAutoSearchArchive(std::string_view path)
{
	auto pathStr = std::string(path);
	if (!this->isFileExist(path)) return;
	unzFile file = nullptr;
	file = unzOpen(pathStr.c_str());
	unz_file_info file_info;
	do {
		unz_file_pos entry;
		if (unzGetFilePos(file, &entry) == UNZ_OK) {
			char filename_inzip[1024];
			if (unzGetCurrentFileInfo(file, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0) == UNZ_OK) {
				_autoSearchArchive[filename_inzip] = std::make_pair(file, entry);
			}
		}
	} while (unzGoToNextFile(file) == UNZ_OK);
}

std::string CustomFileUtils::fullPathForFilename(std::string_view filename) const
{
	auto key = std::string(filename);
	auto it = _autoSearchArchive.find(key);
	if (_autoSearchArchive.end() != it) {
		return key;
	}
	return FileUtilsInherit::fullPathForFilename(filename);
}

unsigned char* CustomFileUtils::getFileData(std::string_view filename, const char* mode, ssize_t *size)
{
	unsigned char* ret = getFileDataFromArchive(filename, size);
	if (ret) return ret;
	auto data = FileUtilsInherit::getDataFromFile(filename);
	if (size) *size = data.getSize();
	unsigned char* buf = (unsigned char*)malloc(data.getSize());
	memcpy(buf, data.getBytes(), data.getSize());
	return buf;
}

bool CustomFileUtils::isFileExistInternal(std::string_view strFilePath) const
{
	auto key = std::string(strFilePath);
	auto it = _autoSearchArchive.find(key);
	if (_autoSearchArchive.end() != it) {
		return true;
	}
	return FileUtilsInherit::isFileExistInternal(strFilePath);
}

bool CustomFileUtils::isDirectoryExistInternal(std::string_view dirPath) const
{
	for (auto &it : _autoSearchArchive) {
		if (it.first.size() <= dirPath.size()) continue;
		if (!strncmp(it.first.c_str(), std::string(dirPath).c_str(), dirPath.size()) && it.first[dirPath.size()] == '/') {
			return true;
		}
	}
	// Cannot call FileUtilsInherit::isDirectoryExistInternal because it's private in axmol's platform-specific FileUtils.
	// Use stat() directly as fallback (same approach as FileUtilsAndroid::isDirectoryExistInternal).
	if (dirPath.empty()) return false;
	std::string path(dirPath);
	if (!path.empty() && path.back() == '/') path.pop_back();
	if (path.empty()) return false;
	struct stat st;
	if (stat(path.c_str(), &st) == 0) return S_ISDIR(st.st_mode);
	return false;
}

unsigned char* CustomFileUtils::getFileDataFromArchive(std::string_view filename, ssize_t *size) const
{
	auto key = std::string(filename);
	auto it = _autoSearchArchive.find(key);
	if (_autoSearchArchive.end() != it) {
		_lock.lock();
		if (unzGoToFilePos(it->second.first, &it->second.second) != UNZ_OK) return nullptr;
		unz_file_info fileInfo;
		if (unzGetCurrentFileInfo(it->second.first, &fileInfo, NULL, 0, NULL, 0, NULL, 0) != UNZ_OK) return nullptr;
		unsigned char *buffer = (unsigned char*)malloc(fileInfo.uncompressed_size);
		int readedSize = unzReadCurrentFile(it->second.first, buffer, static_cast<unsigned>(fileInfo.uncompressed_size));
		_lock.unlock();
		AXASSERT(readedSize == 0 || readedSize == (int)fileInfo.uncompressed_size, "the file size is wrong");
		*size = fileInfo.uncompressed_size;
		return buffer;
	}
	return nullptr;
}

ax::Data CustomFileUtils::getDataFromFile(std::string_view filename) const
{
	ssize_t size;
	unsigned char* buffer = getFileDataFromArchive(filename, &size);
	if (buffer) {
		Data ret;
		ret.fastSet(buffer, size);
		return ret;
	}
	return FileUtilsInherit::getDataFromFile(filename);
}

std::string CustomFileUtils::getStringFromFile(std::string_view filename) const
{
	Data data = getDataFromFile(filename);
	if (data.isNull())
		return "";

	std::string ret((const char*)data.getBytes(), data.getSize());
	return ret;
}

}

ax::FileUtils *TVPCreateCustomFileUtils() {
	ax::CustomFileUtils *ret = new ax::CustomFileUtils;
	ret->init();
	return ret;
}

void TVPAddAutoSearchArchive(const std::string &path)
{
	ax::CustomFileUtils *fileutils =static_cast<ax::CustomFileUtils*>(ax::FileUtils::getInstance());
	fileutils->addAutoSearchArchive(path);
}

#include "StorageImpl.h"
#include "Platform.h"
#include "ConfigManager/GlobalConfigManager.h"
#include "tinyxml2/tinyxml2.h"

using namespace ax;

static bool TVPCopyFolder(const std::string &from, const std::string &to) {
	if (!TVPCheckExistentLocalFolder(to) && !TVPCreateFolders(to)) {
		return false;
	}

	bool success = true;
	TVPListDir(from, [&](const std::string &_name, int mask) {
		if (_name == "." || _name == "..") return;
		if (!success) return;
		if (mask & S_IFREG) {
			success = TVPCopyFile(from + "/" + _name, to + "/" + _name);
		}
		else if (mask & S_IFDIR) {
			success = TVPCopyFolder(from + "/" + _name, to + "/" + _name);
		}
	});
	return success;
}

bool TVPCopyFile(const std::string &from, const std::string &to)
{
	FILE * ffrom = fopen(from.c_str(), "rb");
	if (!ffrom) { // try folder copy
		return TVPCopyFolder(from, to);
	}
	FILE * fto = fopen(to.c_str(), "wb");
	if (!fto) {
		if (ffrom) fclose(ffrom);
		return false;
	}
	const int bufSize = 1 * 1024 * 1024;
	std::vector<char> buffer; buffer.resize(bufSize);
	int readed = 0;
	while ((readed = fread(&buffer.front(), 1, bufSize, ffrom))) {
		fwrite(&buffer.front(), 1, readed, fto);
	}
	fclose(ffrom);
	fclose(fto);
	return true;
}

TVPSkinManager* TVPSkinManager::getInstance()
{
	static TVPSkinManager instance;
	return &instance;
}

void TVPSkinManager::InitSkin()
{
	std::string skinpath = GlobalConfigManager::GetInstance()->GetValue<std::string>("skin_path", "");
	if (!skinpath.empty()) {
		if (!Check(skinpath)) {
			TVPShowSimpleMessageBox(
				LocaleConfigManager::GetInstance()->GetText("invalid_skin_desc"),
				LocaleConfigManager::GetInstance()->GetText("invalid_skin"));
			Reset();
		} else {
			static_cast<CustomFileUtils*>(FileUtils::getInstance())->addAutoSearchArchive(skinpath);
		}
	}
}

static const char *_adapted_skin_version = "1.3.4";

bool TVPSkinManager::Check(const std::string &path)
{
	if (!ax::FileUtils::getInstance()->isFileExist(path)) {
		return false;
	}

	tinyxml2::XMLDocument doc;

	unzFile file = nullptr;
	file = unzOpen(path.c_str());
	unz_file_info file_info;
	do {
		unz_file_pos entry;
		if (unzGetFilePos(file, &entry) == UNZ_OK) {
			char filename_inzip[1024];
			if (unzGetCurrentFileInfo(file, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0) == UNZ_OK) {
				if (strcmp(filename_inzip, "meta.xml")) {
					if (unzGoToFilePos(&file, &entry) != UNZ_OK)
						break;
					unsigned char *buffer = (unsigned char*)malloc(file_info.uncompressed_size);
					int readedSize = unzReadCurrentFile(&file, buffer, static_cast<unsigned>(file_info.uncompressed_size));
					if (readedSize != (int)file_info.uncompressed_size) {
						free(buffer);
						break;
					}
					doc.Parse((char*)buffer);
					free(buffer);
					break;
				}
			}
		}
	} while (unzGoToNextFile(file) == UNZ_OK);
	unzClose(file);

	tinyxml2::XMLElement* root = doc.RootElement();
	if (!root)
		return false;

	const char *s = root->Attribute("version");
	if (!s)
		return false;
	
	return !strcmp(s, _adapted_skin_version);
}

void TVPSkinManager::Reset()
{
	GlobalConfigManager::GetInstance()->SetValue("skin_path", "");
	GlobalConfigManager::GetInstance()->SaveToFile();
}

bool TVPSkinManager::Use(const std::string &skin_path)
{
	GlobalConfigManager::GetInstance()->SetValue("skin_path", skin_path);
	GlobalConfigManager::GetInstance()->SaveToFile();
	return true;
}

bool TVPSkinManager::InstallAndUse(const std::string &skin_path)
{
	std::string path = FileUtils::getInstance()->getWritablePath() + "default.skin";
	FileUtils::getInstance()->removeFile(path);
	if (!TVPCopyFile(skin_path, path)) {
		return false;
	}
	GlobalConfigManager::GetInstance()->SetValue("skin_path", path);
	return true;
}
