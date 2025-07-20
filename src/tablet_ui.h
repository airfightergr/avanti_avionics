#ifndef TABLET_UI_H
#define TABLET_UI_H

#ifdef __cplusplus
extern "C" {
#endif

void TabletUIStart(void);
void TabletUIDraw(int x, int y, int  width, int  height , void* refcon);
void TabletUIUpdate(float deltaTime);
void TabletUIStop(void);
int TabletUIMouseClick(int x, int y, int status, void* refcon);
void TabletUIKeyPress(char key, int flags, char virtualKey, void* refcon, int losingFocus);

#ifdef __cplusplus
}
#endif

#endif // TABLET_UI_H
