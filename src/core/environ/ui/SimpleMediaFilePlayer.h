#pragma once
#include "BaseForm.h"
#include "tjsCommHead.h"
#include "movie/ffmpeg/VideoPlayer.h"

namespace ax {
	class Sprite;
}

class SimplePlayerOverlay;

class SimpleMediaFilePlayer : public iTVPBaseForm {
	typedef ax::Node inherit;

public:
	virtual ~SimpleMediaFilePlayer();
	static SimpleMediaFilePlayer *create();

	void PlayFile(ttstr uri);

	void Play();
	void Pause();
	void TooglePlayOrPause();

private:
	SimpleMediaFilePlayer();
	void onPlayerEvent(KRMovieEvent Msg, void* p);
	void onSliderChanged();
	virtual void rearrangeLayout() override;
	virtual void bindBodyController(const NodeMap &allNodes);
	virtual void bindFooterController(const NodeMap &allNodes);
	virtual void bindHeaderController(const NodeMap &allNodes);

	virtual void update(float dt) override;

	SimplePlayerOverlay *_player;
	int _totalTime; // in sec
	float hideRemain = 0;
	bool _inupdate = false;

	// ui controllers
	ax::ui::Text *Title, *PlayTime, *RemainTime, *OSDText;
	ax::ui::Slider *Timeline;
	ax::Node *NaviBar, *ControlBar, *OSD, *Overlay;
	ax::ui::Widget *PlayBtn;
	ax::Node *PlayBtnNormal, *PlayBtnPress, *PlayIconNormal, *PlayIconPress, *PauseIconNormal, *PauseIconPress;
	void setPlayButtonHighlight(bool highlight);
	void refreshPlayButtonStatus();
};
