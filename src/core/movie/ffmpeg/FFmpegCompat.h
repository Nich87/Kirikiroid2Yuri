#pragma once
/**
 * FFmpegCompat.h — minimal FFmpeg 3.x ↔ 7.x compatibility shim
 *
 * Android ships FFmpeg 3.x (libavcodec 57, libavutil 55).
 * The host system uses FFmpeg 7.x (libavcodec 62, libavutil 60).
 *
 * This header normalises the most pervasive API changes.
 *
 * Usage: #include "FFmpegCompat.h" AFTER the standard FFmpeg headers.
 */

/* ------------------------------------------------------------------ */
/*  AV_CODEC_CONTEXT channel helpers                                   */
/*  FFmpeg 7.x removed channels / channel_layout members; use ch_layout */
/* ------------------------------------------------------------------ */

#if LIBAVCODEC_VERSION_MAJOR >= 61
#  define FFMPEG_CCTX_CHANNELS(ctx)        ((ctx)->ch_layout.nb_channels)
#  define FFMPEG_CCTX_CH_LAYOUT(ctx)       ((ctx)->ch_layout.u.mask)
#else
#  define FFMPEG_CCTX_CHANNELS(ctx)        ((ctx)->channels)
#  define FFMPEG_CCTX_CH_LAYOUT(ctx)       ((ctx)->channel_layout)
#endif

/* ------------------------------------------------------------------ */
/*  AVCodecParameters channel helpers (used in DemuxFFmpeg.cpp)        */
/* ------------------------------------------------------------------ */

#if LIBAVCODEC_VERSION_MAJOR >= 61
#  define FFMPEG_CPAR_CHANNELS(p)        ((p)->ch_layout.nb_channels)
#  define FFMPEG_CPAR_CH_LAYOUT(p)       ((p)->ch_layout.u.mask)
#else
#  define FFMPEG_CPAR_CHANNELS(p)        ((p)->channels)
#  define FFMPEG_CPAR_CH_LAYOUT(p)       ((p)->channel_layout)
#endif

/* ------------------------------------------------------------------ */
/*  AVFrame channel helpers                                            */
/*  AVFrame.channels removed in FFmpeg 7.x (libavutil 59+)             */
/* ------------------------------------------------------------------ */

#if LIBAVUTIL_VERSION_MAJOR >= 59
#  define FFMPEG_FRAME_CHANNELS(fr)        ((fr)->ch_layout.nb_channels)
#else
#  define FFMPEG_FRAME_CHANNELS(fr)        ((fr)->channels)
#endif

/* ------------------------------------------------------------------ */
/*  best_effort_timestamp                                              */
/*  Accessor function removed in FFmpeg 5.x, use field directly        */
/* ------------------------------------------------------------------ */

#if LIBAVUTIL_VERSION_MAJOR >= 57
#  define FFMPEG_FRAME_PTS(fr)             ((fr)->best_effort_timestamp)
#else
#  define FFMPEG_FRAME_PTS(fr)             (av_frame_get_best_effort_timestamp(fr))
#endif

/* ------------------------------------------------------------------ */
/*  avcodec_send_packet / avcodec_receive_frame compat                  */
/*  Old FFmpeg uses avcodec_decode_audio4 / avcodec_decode_video2.      */
/*  We provide a wrapper that returns true (got_frame) / false.         */
/*  iBytesUsed is written back via the last argument.                   */
/* ------------------------------------------------------------------ */

#if LIBAVCODEC_VERSION_MAJOR >= 61

static inline int ffmpeg_decode_audio(AVCodecContext *ctx, AVFrame *frame,
                                       int *got_frame, AVPacket *pkt) {
    int ret = avcodec_send_packet(ctx, pkt);
    if (ret < 0) return ret;
    ret = avcodec_receive_frame(ctx, frame);
    if (ret >= 0) *got_frame = 1;
    else *got_frame = 0;
    return pkt->size;  /* all bytes consumed */
}

#else

static inline int ffmpeg_decode_audio(AVCodecContext *ctx, AVFrame *frame,
                                       int *got_frame, AVPacket *pkt) {
    return avcodec_decode_audio4(ctx, frame, got_frame, pkt);
}

#endif
