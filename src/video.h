#ifndef _FCEU_VIDEO_H
#define _FCEU_VIDEO_H

int FCEU_InitVirtualVideo(void);
void FCEU_KillVirtualVideo(void);
int SaveSnapshot(void);
extern uint8 *sp_bgXBuf;
extern uint8 *bgXBuf;
extern uint8 *sp_fgXBuf;
extern uint8 *XDBuf;
extern int show_crosshair;

#endif
