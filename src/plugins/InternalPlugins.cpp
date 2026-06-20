#include "ncbind/ncbind.hpp"
//extern void InitPlugin_CSVParser();

void TVPLoadInternalPlugins()
{
    ncbAutoRegister::AllRegist();
	ncbAutoRegister::LoadModule(TJS_W("xp3filter.dll"));
	ncbAutoRegister::LoadModule(TJS_W("AdjustMoni.dll"));
	ncbAutoRegister::LoadModule(TJS_W("nativeDialog.dll"));
    //InitPlugin_CSVParser();
}

void TVPUnloadInternalPlugins()
{
    ncbAutoRegister::AllUnregist();
}

bool TVPLoadInternalPlugin(const ttstr &_name)
{
	return ncbAutoRegister::LoadModule(TVPExtractStorageName(_name));
}