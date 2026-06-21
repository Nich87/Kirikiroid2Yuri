#include "tjsTypes.h"
#include "tvpinputdefs.h"
#include "axmol.h"

int TVPConvertMouseBtnToVKCode(tTVPMouseButton _mouseBtn);
int TVPConvertKeyCodeToVKCode(ax::EventKeyboard::KeyCode keyCode);
int TVPConvertPadKeyCodeToVKCode(int keyCode);
const std::unordered_map<std::string, int> &TVPGetVKCodeNameMap();
std::string TVPGetVKCodeName(int keyCode);