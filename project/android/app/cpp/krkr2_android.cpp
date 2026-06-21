/* Include the SDL main definition header */
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "AppDelegate.h"
#include "MainScene.h"
#include "ConfigManager/GlobalConfigManager.h"
#include "Application.h"

// Forward declarations
extern void Android_PushEvents(const std::function<void()>& func);
extern class tTVPApplication* Application;

/*******************************************************************************
                 Functions called by JNI
*******************************************************************************/
#include <string.h>
#include <string>
#include <condition_variable>
#include <mutex>
#include "breakpad/client/linux/handler/exception_handler.h"
#include "breakpad/client/linux/handler/minidump_descriptor.h"

//std::string Android_GetDumpStoragePath();

static bool __DumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,
	void* context, bool succeeded)
{
	return succeeded;
}

extern bool TVPSystemUninitCalled;

static bool __DumpFilter(void *data) {
	if(TVPSystemUninitCalled) return false; // if trying exit system, ignore all exception
	return true;
}


//static void __InitAndroidDump() {
//    static google_breakpad::MinidumpDescriptor descriptor(Android_GetDumpStoragePath());
//	static google_breakpad::ExceptionHandler eh(descriptor, __DumpFilter, __DumpCallback,
//		NULL, true, -1);
//}

void axmol_android_app_init (JNIEnv* env) { // for axmol engine
//	__InitAndroidDump();
	__android_log_print(ANDROID_LOG_INFO,"## krkr2yuri","in cocos_android_app_init");
	static TVPAppDelegate *pAppDelegate = new TVPAppDelegate();
}

namespace kr2android {
	extern std::condition_variable MessageBoxCond;
	extern std::mutex MessageBoxLock;
	extern int MsgBoxRet;
    extern std::string MessageBoxRetText;
}
void Android_PushEvents(const std::function<void()> &func);
using namespace kr2android;
extern "C" {
	JNIEXPORT void JNICALL Java_org_tvp_kirikiri2_KR2Activity_initDump(JNIEnv* env, jclass cls, jstring path) {
		const char* pszPath = env->GetStringUTFChars(path, NULL);
		if (pszPath && *pszPath) {
			static google_breakpad::MinidumpDescriptor descriptor(pszPath);
			static google_breakpad::ExceptionHandler eh(descriptor, __DumpFilter, __DumpCallback,
				NULL, true, -1);
		}
		env->ReleaseStringUTFChars(path, pszPath);
	}
	
	JNIEXPORT void JNICALL Java_org_tvp_kirikiri2_KR2Activity_onMessageBoxOK(JNIEnv* env, jclass cls, jint nButton) {
		MsgBoxRet = nButton;
		MessageBoxCond.notify_one();
	}
    
	JNIEXPORT void JNICALL Java_org_tvp_kirikiri2_KR2Activity_onMessageBoxText(JNIEnv* env, jclass cls, jstring text) {
		const char* pszText = env->GetStringUTFChars(text, NULL);
		if (pszText && *pszText) {
            MessageBoxRetText = pszText;
		}
		env->ReleaseStringUTFChars(text, pszText);
	}

	JNIEXPORT void JNICALL Java_org_tvp_kirikiri2_KR2Activity_nativeTouchesBegin(JNIEnv * env, jclass clazz, jint id, jfloat x, jfloat y) {
		intptr_t idlong = id;
		Android_PushEvents([idlong, x, y](){
			ax::Director::getInstance()->getGLView()->handleTouchesBegin(1, (intptr_t*)&idlong, (float*)&x, (float*)&y);
		});
	}

	JNIEXPORT void JNICALL Java_org_tvp_kirikiri2_KR2Activity_nativeTouchesEnd(JNIEnv * env, jclass clazz, jint id, jfloat x, jfloat y) {
		intptr_t idlong = id;
		Android_PushEvents([idlong, x, y](){
			ax::Director::getInstance()->getGLView()->handleTouchesEnd(1, (intptr_t*)&idlong, (float*)&x, (float*)&y);
		});
	}

	JNIEXPORT void JNICALL Java_org_tvp_kirikiri2_KR2Activity_nativeTouchesMove(JNIEnv * env, jclass clazz, jintArray ids, jfloatArray xs, jfloatArray ys) {
		int size = env->GetArrayLength(ids);
		if (size == 1) {
			intptr_t idlong;
			jint id;
			jfloat x;
			jfloat y;
			env->GetIntArrayRegion(ids, 0, size, &id);
			env->GetFloatArrayRegion(xs, 0, size, &x);
			env->GetFloatArrayRegion(ys, 0, size, &y);
			idlong = id;
			Android_PushEvents([idlong, x, y](){
				ax::Director::getInstance()->getGLView()->handleTouchesMove(1, (intptr_t*)&idlong, (float*)&x, (float*)&y);
			});
			return;
		}
		
		jint id[size];
		std::vector<jfloat> x; x.resize(size);
		std::vector<jfloat> y; y.resize(size);

		env->GetIntArrayRegion(ids, 0, size, id);
		env->GetFloatArrayRegion(xs, 0, size, &x[0]);
		env->GetFloatArrayRegion(ys, 0, size, &y[0]);

		std::vector<intptr_t> idlong; idlong.resize(size);
		for (int i = 0; i < size; i++)
			idlong[i] = id[i];

		Android_PushEvents([idlong, x, y](){
			ax::Director::getInstance()->getGLView()->handleTouchesMove(idlong.size(), (intptr_t*)&idlong[0], (float*)&x[0], (float*)&y[0]);
		});
	}

	JNIEXPORT void JNICALL Java_org_tvp_kirikiri2_KR2Activity_nativeTouchesCancel(JNIEnv * env, jclass clazz, jintArray ids, jfloatArray xs, jfloatArray ys) {
		int size = env->GetArrayLength(ids);
		if (size == 1) {
			intptr_t idlong;
			jint id;
			jfloat x;
			jfloat y;
			env->GetIntArrayRegion(ids, 0, size, &id);
			env->GetFloatArrayRegion(xs, 0, size, &x);
			env->GetFloatArrayRegion(ys, 0, size, &y);
			idlong = id;
			Android_PushEvents([idlong, x, y](){
				ax::Director::getInstance()->getGLView()->handleTouchesCancel(1, (intptr_t*)&idlong, (float*)&x, (float*)&y);
			});
			return;
		}

		jint id[size];
		std::vector<jfloat> x; x.resize(size);
		std::vector<jfloat> y; y.resize(size);

		env->GetIntArrayRegion(ids, 0, size, id);
		env->GetFloatArrayRegion(xs, 0, size, &x[0]);
		env->GetFloatArrayRegion(ys, 0, size, &y[0]);

		std::vector<intptr_t> idlong; idlong.resize(size);
		for (int i = 0; i < size; i++)
			idlong[i] = id[i];

		Android_PushEvents([idlong, x, y](){
			ax::Director::getInstance()->getGLView()->handleTouchesCancel(idlong.size(), (intptr_t*)&idlong[0], (float*)&x[0], (float*)&y[0]);
		});
	}

#define KEYCODE_BACK 0x04
#define KEYCODE_MENU 0x52
#define KEYCODE_DPAD_UP 0x13
#define KEYCODE_DPAD_DOWN 0x14
#define KEYCODE_DPAD_LEFT 0x15
#define KEYCODE_DPAD_RIGHT 0x16
#define KEYCODE_ENTER 0x42
#define KEYCODE_PLAY  0x7e
#define KEYCODE_DPAD_CENTER  0x17
#define KEYCODE_DEL 0x43

	JNIEXPORT jboolean JNICALL Java_org_tvp_kirikiri2_KR2Activity_nativeKeyAction(JNIEnv * env, jclass cls, jint keyCode, jboolean isPress) {
		ax::EventKeyboard::KeyCode pKeyCode;
		switch (keyCode) {
		case KEYCODE_BACK		: pKeyCode = ax::EventKeyboard::KeyCode::KEY_ESCAPE	; break;
		case KEYCODE_MENU		: pKeyCode = ax::EventKeyboard::KeyCode::KEY_MENU		; break;
		case KEYCODE_DPAD_UP	: pKeyCode = ax::EventKeyboard::KeyCode::KEY_DPAD_UP	; break;
		case KEYCODE_DPAD_DOWN	: pKeyCode = ax::EventKeyboard::KeyCode::KEY_DPAD_DOWN	; break;
		case KEYCODE_DPAD_LEFT	: pKeyCode = ax::EventKeyboard::KeyCode::KEY_DPAD_LEFT	; break;
		case KEYCODE_DPAD_RIGHT	: pKeyCode = ax::EventKeyboard::KeyCode::KEY_DPAD_RIGHT; break;
		case KEYCODE_ENTER		: pKeyCode = ax::EventKeyboard::KeyCode::KEY_ENTER		; break;
		case KEYCODE_PLAY		: pKeyCode = ax::EventKeyboard::KeyCode::KEY_PLAY		; break;
		case KEYCODE_DPAD_CENTER: pKeyCode = ax::EventKeyboard::KeyCode::KEY_DPAD_CENTER; break;
        case KEYCODE_DEL          : pKeyCode = ax::EventKeyboard::KeyCode::KEY_BACKSPACE; break;
		default: return JNI_FALSE;
		}

		Android_PushEvents([pKeyCode, isPress](){
			ax::EventKeyboard event(pKeyCode, isPress);
			ax::Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
		});
		return JNI_TRUE;
	}

	JNIEXPORT void JNICALL Java_org_tvp_kirikiri2_KR2Activity_nativeInsertText(JNIEnv* env, jclass cls, jstring text) {
		const char* pszText = env->GetStringUTFChars(text, NULL);
		if (pszText && *pszText) {
			std::string str = pszText;
			Android_PushEvents([str](){
				ax::IMEDispatcher::sharedDispatcher()->dispatchInsertText(str.c_str(), str.length());
			});
		}
		env->ReleaseStringUTFChars(text, pszText);
	}

	JNIEXPORT void JNICALL Java_org_tvp_kirikiri2_KR2Activity_nativeDeleteBackward(JNIEnv* env, jclass cls) {
		Android_PushEvents([]() {
			ax::IMEDispatcher::sharedDispatcher()->dispatchDeleteBackward(1);
		});
	}

	JNIEXPORT void JNICALL Java_org_tvp_kirikiri2_KR2Activity_nativeCharInput(JNIEnv* env, jclass cls, jint keyCode) {
		TVPMainScene *pScene = TVPMainScene::GetInstance();
		if (!pScene) return;
		pScene->getScheduler()->performFunctionInCocosThread(std::bind(&TVPMainScene::onCharInput, keyCode));
	}

	JNIEXPORT void JNICALL Java_org_tvp_kirikiri2_KR2Activity_nativeCommitText(
		JNIEnv* env, jclass cls,
		jstring text, jint newCursorPosition)
	{
		TVPMainScene *pScene = TVPMainScene::GetInstance();
		if (!pScene) return;
		const char *utftext = env->GetStringUTFChars(text, NULL);
		std::string str(utftext);
		pScene->getScheduler()->performFunctionInCocosThread(std::bind(&TVPMainScene::onTextInput, str));
		env->ReleaseStringUTFChars(text, utftext);
	}

	JNIEXPORT jboolean JNICALL Java_org_tvp_kirikiri2_KR2Activity_nativeGetHideSystemButton(JNIEnv* env, jclass cls)
	{
		return GlobalConfigManager::GetInstance()->GetValue<bool>("hide_android_sys_btn", false);
	}

	static float _mouseX, _mouseY;

	JNIEXPORT jboolean JNICALL Java_org_tvp_kirikiri2_KR2Activity_nativeHoverMoved(JNIEnv* env, jclass cls, jfloat x, jfloat y)
	{
		Android_PushEvents([x, y]() {
			ax::RenderView *glview = ax::Director::getInstance()->getGLView();
			float _scaleX = glview->getScaleX(), _scaleY = glview->getScaleY();
			_mouseX = x; _mouseY = y;
			const ax::Rect _viewPortRect = glview->getViewPortRect();

			float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
			float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;

			ax::EventMouse event(ax::EventMouse::MouseEventType::MOUSE_MOVE);
			event.setMouseInfo(cursorX, cursorY, ax::EventMouse::MouseButton::BUTTON_UNSET);
			ax::Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
		});
		return true;
	}

	JNIEXPORT jboolean JNICALL Java_org_tvp_kirikiri2_KR2Activity_nativeMouseScrolled(JNIEnv* env, jclass cls, jfloat v)
	{
		Android_PushEvents([v]() {
			ax::RenderView *glview = ax::Director::getInstance()->getGLView();
			float _scaleX = glview->getScaleX(), _scaleY = glview->getScaleY();
			const ax::Rect _viewPortRect = glview->getViewPortRect();

			float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
			float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;

			ax::EventMouse event(ax::EventMouse::MouseEventType::MOUSE_SCROLL);
			event.setScrollData(0, v);
			event.setMouseInfo(cursorX, cursorY, ax::EventMouse::MouseButton::BUTTON_UNSET);
			ax::Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
		});
		return true;
	}

	JNIEXPORT void JNICALL Java_org_tvp_kirikiri2_KR2Activity_nativeOnLowMemory(JNIEnv* env, jclass cls)
	{
		Android_PushEvents([]() {
			Application->OnLowMemory();
		});
	}
}
