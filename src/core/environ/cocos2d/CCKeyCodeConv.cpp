#include "CCKeyCodeConv.h"
#include "base/Controller.h"
#include "vkdefine.h"

int TVPConvertMouseBtnToVKCode(tTVPMouseButton _mouseBtn)
{
	int btncode;
	switch (_mouseBtn) {
	case mbLeft:    btncode = VK_LBUTTON;	break;
	case mbMiddle:  btncode = VK_MBUTTON;	break;
	case mbRight:   btncode = VK_RBUTTON;	break;
	default:		btncode = 0;			break;
	}
	return btncode;
}

int TVPConvertKeyCodeToVKCode(ax::EventKeyboard::KeyCode keyCode)
{
#define CASE(x) case ax::EventKeyboard::KeyCode::KEY_##x:	return VK_##x
	switch (keyCode) {
		CASE(0);
		CASE(1);
		CASE(2);
		CASE(3);
		CASE(4);
		CASE(5);
		CASE(6);
		CASE(7);
		CASE(8);
		CASE(9);
		CASE(A);
		CASE(B);
		CASE(C);
		CASE(D);
		CASE(E);
		CASE(F);
		CASE(G);
		CASE(H);
		CASE(I);
		CASE(J);
		CASE(K);
		CASE(L);
		CASE(M);
		CASE(N);
		CASE(O);
		CASE(P);
		CASE(Q);
		CASE(R);
		CASE(S);
		CASE(T);
		CASE(U);
		CASE(V);
		CASE(W);
		CASE(X);
		CASE(Y);
		CASE(Z);
		CASE(F1);
		CASE(F2);
		CASE(F3);
		CASE(F4);
		CASE(F5);
		CASE(F6);
		CASE(F7);
		CASE(F8);
		CASE(F9);
		CASE(F10);
		CASE(F11);
		CASE(F12);
		CASE(PAUSE);
		CASE(PRINT);
		CASE(ESCAPE);
	case ax::EventKeyboard::KeyCode::KEY_BACK_TAB:
		CASE(TAB);
		CASE(RETURN);
	case ax::EventKeyboard::KeyCode::KEY_SCROLL_LOCK:	return VK_SCROLL;
	case ax::EventKeyboard::KeyCode::KEY_SYSREQ:	return VK_SNAPSHOT;
	case ax::EventKeyboard::KeyCode::KEY_BREAK:	return VK_CANCEL;
	case ax::EventKeyboard::KeyCode::KEY_BACKSPACE:	return VK_BACK;
	case ax::EventKeyboard::KeyCode::KEY_CAPS_LOCK:	return VK_CAPITAL;
	case ax::EventKeyboard::KeyCode::KEY_LEFT_SHIFT:	return VK_SHIFT; // LR the same
	case ax::EventKeyboard::KeyCode::KEY_RIGHT_SHIFT:	return VK_SHIFT;
	case ax::EventKeyboard::KeyCode::KEY_LEFT_CTRL:	return VK_CONTROL; // LR the same
	case ax::EventKeyboard::KeyCode::KEY_RIGHT_CTRL:	return VK_CONTROL;
	case ax::EventKeyboard::KeyCode::KEY_LEFT_ALT:	return VK_MENU;
	case ax::EventKeyboard::KeyCode::KEY_RIGHT_ALT:	return VK_MENU;
	case ax::EventKeyboard::KeyCode::KEY_MENU:	return VK_APPS;
	case ax::EventKeyboard::KeyCode::KEY_HYPER:	return VK_LWIN;
		CASE(INSERT);
		CASE(HOME);
		CASE(DELETE);
		CASE(END);
	case ax::EventKeyboard::KeyCode::KEY_KP_PG_UP:
	case ax::EventKeyboard::KeyCode::KEY_PG_UP:	return VK_PRIOR;
	case ax::EventKeyboard::KeyCode::KEY_KP_PG_DOWN:
	case ax::EventKeyboard::KeyCode::KEY_PG_DOWN:	return VK_NEXT;
	case ax::EventKeyboard::KeyCode::KEY_KP_LEFT:
	case ax::EventKeyboard::KeyCode::KEY_LEFT_ARROW:	return VK_LEFT;
	case ax::EventKeyboard::KeyCode::KEY_KP_RIGHT:
	case ax::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:	return VK_RIGHT;
	case ax::EventKeyboard::KeyCode::KEY_KP_UP:
	case ax::EventKeyboard::KeyCode::KEY_UP_ARROW:	return VK_UP;
	case ax::EventKeyboard::KeyCode::KEY_KP_DOWN:
	case ax::EventKeyboard::KeyCode::KEY_DOWN_ARROW:	return VK_DOWN;
	case ax::EventKeyboard::KeyCode::KEY_NUM_LOCK:	return VK_NUMLOCK;
	case ax::EventKeyboard::KeyCode::KEY_KP_PLUS:	return VK_ADD;
	case ax::EventKeyboard::KeyCode::KEY_KP_MINUS:	return VK_SUBTRACT;
	case ax::EventKeyboard::KeyCode::KEY_KP_MULTIPLY:	return VK_MULTIPLY;
	case ax::EventKeyboard::KeyCode::KEY_KP_DIVIDE:	return VK_DIVIDE;
	case ax::EventKeyboard::KeyCode::KEY_KP_ENTER:	return VK_RETURN;
	case ax::EventKeyboard::KeyCode::KEY_KP_HOME:	return VK_HOME;
	case ax::EventKeyboard::KeyCode::KEY_KP_FIVE:	return VK_NUMPAD5;
	case ax::EventKeyboard::KeyCode::KEY_KP_END:	return VK_END;
	case ax::EventKeyboard::KeyCode::KEY_KP_INSERT:	return VK_INSERT;
	case ax::EventKeyboard::KeyCode::KEY_KP_DELETE:	return VK_DELETE;
		CASE(SPACE);
	case ax::EventKeyboard::KeyCode::KEY_EXCLAM:	return VK_SPACE;
	case ax::EventKeyboard::KeyCode::KEY_QUOTE:	return VK_OEM_7;
	case ax::EventKeyboard::KeyCode::KEY_COMMA:	return VK_OEM_COMMA;
	case ax::EventKeyboard::KeyCode::KEY_MINUS:	return VK_OEM_MINUS;
	case ax::EventKeyboard::KeyCode::KEY_PERIOD:	return VK_OEM_PERIOD;
	case ax::EventKeyboard::KeyCode::KEY_EQUAL: return VK_OEM_PLUS;
	case ax::EventKeyboard::KeyCode::KEY_SLASH:	return VK_OEM_2;
	case ax::EventKeyboard::KeyCode::KEY_SEMICOLON:	return VK_OEM_1;
	case ax::EventKeyboard::KeyCode::KEY_BACK_SLASH:	return VK_OEM_5;
	case ax::EventKeyboard::KeyCode::KEY_LEFT_BRACE:	return VK_OEM_4;
	case ax::EventKeyboard::KeyCode::KEY_RIGHT_BRACE:	return VK_OEM_6;
	case ax::EventKeyboard::KeyCode::KEY_PLAY:	return VK_PLAY;
	case ax::EventKeyboard::KeyCode::KEY_NUMBER:
	case ax::EventKeyboard::KeyCode::KEY_DOLLAR:
	case ax::EventKeyboard::KeyCode::KEY_PERCENT:
	case ax::EventKeyboard::KeyCode::KEY_CIRCUMFLEX:
	case ax::EventKeyboard::KeyCode::KEY_AMPERSAND:
	case ax::EventKeyboard::KeyCode::KEY_APOSTROPHE:
	case ax::EventKeyboard::KeyCode::KEY_LEFT_PARENTHESIS:
	case ax::EventKeyboard::KeyCode::KEY_RIGHT_PARENTHESIS:
	case ax::EventKeyboard::KeyCode::KEY_ASTERISK:
	case ax::EventKeyboard::KeyCode::KEY_COLON:
	case ax::EventKeyboard::KeyCode::KEY_LESS_THAN:
	case ax::EventKeyboard::KeyCode::KEY_PLUS:
	case ax::EventKeyboard::KeyCode::KEY_GREATER_THAN:
	case ax::EventKeyboard::KeyCode::KEY_QUESTION:
	case ax::EventKeyboard::KeyCode::KEY_AT:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_B:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_C:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_E:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_F:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_G:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_H:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_I:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_J:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_K:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_L:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_M:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_N:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_O:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_P:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_Q:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_R:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_T:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_U:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_V:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_X:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_Y:
	case ax::EventKeyboard::KeyCode::KEY_CAPITAL_Z:
	case ax::EventKeyboard::KeyCode::KEY_LEFT_BRACKET:
	case ax::EventKeyboard::KeyCode::KEY_RIGHT_BRACKET:
	case ax::EventKeyboard::KeyCode::KEY_UNDERSCORE:
	case ax::EventKeyboard::KeyCode::KEY_GRAVE:
	case ax::EventKeyboard::KeyCode::KEY_BAR:
	case ax::EventKeyboard::KeyCode::KEY_TILDE:
	case ax::EventKeyboard::KeyCode::KEY_EURO:
	case ax::EventKeyboard::KeyCode::KEY_POUND:
	case ax::EventKeyboard::KeyCode::KEY_YEN:
	case ax::EventKeyboard::KeyCode::KEY_MIDDLE_DOT:
	case ax::EventKeyboard::KeyCode::KEY_SEARCH:
	case ax::EventKeyboard::KeyCode::KEY_DPAD_LEFT:
	case ax::EventKeyboard::KeyCode::KEY_DPAD_RIGHT:
	case ax::EventKeyboard::KeyCode::KEY_DPAD_UP:
	case ax::EventKeyboard::KeyCode::KEY_DPAD_DOWN:
	case ax::EventKeyboard::KeyCode::KEY_DPAD_CENTER:
	case ax::EventKeyboard::KeyCode::KEY_ENTER:
	default: return 0;
	}
#undef CASE
}

int TVPConvertPadKeyCodeToVKCode(int keyCode)
{
	switch (keyCode) {
	case ax::Controller::BUTTON_A: return VK_PAD1;
	case ax::Controller::BUTTON_B: return VK_PAD2;
	case ax::Controller::BUTTON_C: return VK_PAD3;
	case ax::Controller::BUTTON_X: return VK_PAD4;
	case ax::Controller::BUTTON_Y: return VK_PAD5;
	case ax::Controller::BUTTON_Z: return VK_PAD6;
	case ax::Controller::BUTTON_LEFT_SHOULDER: return VK_PAD7;
	case ax::Controller::BUTTON_RIGHT_SHOULDER: return VK_PAD8;
	case ax::Controller::BUTTON_LEFT_THUMBSTICK: return VK_PAD9;
	case ax::Controller::BUTTON_RIGHT_THUMBSTICK: return VK_PAD10;
	case ax::Controller::BUTTON_START: return VK_PAD9;
	case ax::Controller::BUTTON_SELECT: return VK_PAD10;
	case ax::Controller::AXIS_LEFT_TRIGGER: return VK_PAD5;
	case ax::Controller::AXIS_RIGHT_TRIGGER: return VK_PAD6;
	case ax::Controller::BUTTON_PAUSE: return VK_PAD7;
	case ax::Controller::BUTTON_DPAD_UP: return VK_PADUP;
	case ax::Controller::BUTTON_DPAD_DOWN: return VK_PADDOWN;
	case ax::Controller::BUTTON_DPAD_LEFT: return VK_PADLEFT;
	case ax::Controller::BUTTON_DPAD_RIGHT: return VK_PADRIGHT;
	case ax::Controller::BUTTON_DPAD_CENTER:
	default: return 0;
	}
}


const std::unordered_map<std::string, int> & TVPGetVKCodeNameMap()
{
	static std::unordered_map<std::string, int> ret({
#define CASE(x) { #x, VK_##x }
		CASE(0),
		CASE(1),
		CASE(2),
		CASE(3),
		CASE(4),
		CASE(5),
		CASE(6),
		CASE(7),
		CASE(8),
		CASE(9),
		CASE(A),
		CASE(B),
		CASE(C),
		CASE(D),
		CASE(E),
		CASE(F),
		CASE(G),
		CASE(H),
		CASE(I),
		CASE(J),
		CASE(K),
		CASE(L),
		CASE(M),
		CASE(N),
		CASE(O),
		CASE(P),
		CASE(Q),
		CASE(R),
		CASE(S),
		CASE(T),
		CASE(U),
		CASE(V),
		CASE(W),
		CASE(X),
		CASE(Y),
		CASE(Z),
		CASE(F1),
		CASE(F2),
		CASE(F3),
		CASE(F4),
		CASE(F5),
		CASE(F6),
		CASE(F7),
		CASE(F8),
		CASE(F9),
		CASE(F10),
		CASE(F11),
		CASE(F12),
		CASE(PAUSE),
		CASE(PRINT),
		CASE(ESCAPE),
		CASE(TAB),
		CASE(RETURN),
		CASE(SCROLL),
		CASE(SNAPSHOT),
		CASE(CANCEL),
		CASE(BACK),
		CASE(CAPITAL),
		CASE(SHIFT),
		CASE(CONTROL),
		CASE(MENU),
		CASE(APPS),
		CASE(LWIN),
		CASE(INSERT),
		CASE(HOME),
		CASE(DELETE),
		CASE(END),
		CASE(PRIOR),
		CASE(NEXT),
		CASE(LEFT),
		CASE(RIGHT),
		CASE(UP),
		CASE(DOWN),
		CASE(NUMLOCK),
		CASE(ADD),
		CASE(SUBTRACT),
		CASE(MULTIPLY),
		CASE(DIVIDE),
		CASE(HOME),
		CASE(NUMPAD5),
		CASE(END),
		CASE(INSERT),
		CASE(DELETE),
		CASE(SPACE),
		CASE(OEM_COMMA),
		CASE(OEM_MINUS),
		CASE(OEM_PERIOD),
		CASE(OEM_PLUS),
		CASE(OEM_1),
		CASE(OEM_2),
		CASE(OEM_4),
		CASE(OEM_5),
		CASE(OEM_6),
		CASE(OEM_7),
		CASE(PLAY),
#undef CASE
	});
	return ret;
}

std::string TVPGetVKCodeName(int keyCode)
{
	return "";
}
