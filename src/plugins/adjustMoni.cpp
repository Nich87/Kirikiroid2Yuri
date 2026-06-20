//---------------------------------------------------------------------------
// AdjustMoni.dll stub for non-Windows platforms
//---------------------------------------------------------------------------
// The Windows plugin "AdjustMoni.dll" (adjustMonitor) adjusts a window or
// rectangle so that it does not fall outside the monitor it is on.
// Games scripted for the PC version call global.AdjustMoni(dic) and expect
// a dictionary with adjusted position/size keys to be returned.
//
// On Android there is no concept of multi-monitor window placement, so this
// stub simply returns the input dictionary unchanged.  This is enough to
// satisfy the game's save/UI scripts that reference the function.
//---------------------------------------------------------------------------
#include "ncbind/ncbind.hpp"

#define NCB_MODULE_NAME TJS_W("AdjustMoni.dll")

//---------------------------------------------------------------------------
// Global function object
//---------------------------------------------------------------------------
class tAdjustMoniFunction : public tTJSDispatch
{
	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if (membername) return TJS_E_MEMBERNOTFOUND;

		// global.AdjustMoni(dic)
		if (numparams < 1) return TJS_E_BADPARAMCOUNT;

		// No-op on Android: return the input dictionary as-is.
		if (result) {
			*result = *param[0];
		}
		return TJS_S_OK;
	}
} * AdjustMoniFunction;

//---------------------------------------------------------------------------
// Register global.AdjustMoni when AdjustMoni.dll is linked
//---------------------------------------------------------------------------
static void PostRegistCallback()
{
	iTJSDispatch2 * global = TVPGetScriptDispatch();
	if (!global) return;

	tTJSVariant val;
	AdjustMoniFunction = new tAdjustMoniFunction();
	val = tTJSVariant(AdjustMoniFunction);
	AdjustMoniFunction->Release();

	global->PropSet(
		TJS_MEMBERENSURE,
		TJS_W("AdjustMoni"),
		NULL,
		&val,
		global);

	global->Release();
	val.Clear();
}
NCB_POST_REGIST_CALLBACK(PostRegistCallback);
