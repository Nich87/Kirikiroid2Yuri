//---------------------------------------------------------------------------
// nativeDialog.cpp
// Provides native Android dialog functions exposed to TJS scripts.
//
// This plugin registers global.nativeYesNoDialog(text) which shows a native
// Android AlertDialog with Yes/No buttons.  It blocks the calling thread
// (the GL thread) using the same JNI + condition_variable pattern as
// TVPShowSimpleMessageBox, delivering TVP events each cycle so the engine
// stays responsive during the wait.
//
// Intended use: game scripts that previously relied on modal script windows
// (which break on Android because the modal loop can't process touch
// events through the cocos2d layer) can call this native function instead.
//---------------------------------------------------------------------------
#include "ncbind/ncbind.hpp"
#include "Platform.h"

#ifdef __ANDROID__
#include <android/log.h>
#define NATIVEDLG_LOGI(...) __android_log_print(ANDROID_LOG_INFO, "nativeDialog", __VA_ARGS__)
#else
#define NATIVEDLG_LOGI(...) ((void)0)
#endif

#define NCB_MODULE_NAME TJS_W("nativeDialog.dll")

//---------------------------------------------------------------------------
// Global function object: nativeYesNoDialog(text) -> bool
// Shows a native Yes/No dialog and returns true for Yes, false for No.
//---------------------------------------------------------------------------
class tNativeYesNoFunction : public tTJSDispatch
{
	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if (membername) return TJS_E_MEMBERNOTFOUND;

		NATIVEDLG_LOGI("nativeYesNoDialog called with %d params", (int)numparams);

		if (numparams < 1) return TJS_E_BADPARAMCOUNT;

		// Get the message text from the first parameter
		ttstr text = *param[0];

		// Show native Yes/No dialog (returns 0 for Yes, 1 for No)
		int btn = TVPShowSimpleMessageBoxYesNo(text, ttstr(TJS_W("Confirm")));

		// Return true if Yes was pressed
		if (result) {
			*result = tTJSVariant(btn == 0 ? true : false);
		}

		return TJS_S_OK;
	}
} * NativeYesNoFunction;

//---------------------------------------------------------------------------
// Global function object: nativeMessageBox(text[, caption]) -> void
// Shows a native info/OK dialog.
//---------------------------------------------------------------------------
class tNativeMessageBoxFunction : public tTJSDispatch
{
	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if (membername) return TJS_E_MEMBERNOTFOUND;

		if (numparams < 1) return TJS_E_BADPARAMCOUNT;

		ttstr text = *param[0];
		ttstr caption(TJS_W("Message"));
		if (numparams >= 2) {
			caption = *param[1];
		}

		TVPShowSimpleMessageBox(text, caption);

		return TJS_S_OK;
	}
} * NativeMessageBoxFunction;

//---------------------------------------------------------------------------
// Register globals when nativeDialog.dll is linked
//---------------------------------------------------------------------------
static void PostRegistCallback()
{
	NATIVEDLG_LOGI("PostRegistCallback started");

	iTJSDispatch2 * global = TVPGetScriptDispatch();
	if (!global) {
		NATIVEDLG_LOGI("PostRegistCallback: global is null");
		return;
	}

	tTJSVariant val;

	// Register nativeYesNoDialog
	NativeYesNoFunction = new tNativeYesNoFunction();
	val = tTJSVariant(NativeYesNoFunction);
	NativeYesNoFunction->Release();
	global->PropSet(
		TJS_MEMBERENSURE,
		TJS_W("nativeYesNoDialog"),
		NULL,
		&val,
		global);
	NATIVEDLG_LOGI("PostRegistCallback: nativeYesNoDialog registered");

	// Register nativeMessageBox
	NativeMessageBoxFunction = new tNativeMessageBoxFunction();
	val = tTJSVariant(NativeMessageBoxFunction);
	NativeMessageBoxFunction->Release();
	global->PropSet(
		TJS_MEMBERENSURE,
		TJS_W("nativeMessageBox"),
		NULL,
		&val,
		global);
	NATIVEDLG_LOGI("PostRegistCallback: nativeMessageBox registered");

	global->Release();
	val.Clear();
}
NCB_POST_REGIST_CALLBACK(PostRegistCallback);
