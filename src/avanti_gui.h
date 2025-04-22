//
// Created by ilias on 8/20/23.
//

#ifndef AVANTI_UI_H
#define AVANTI_UI_H

#include <XPLMDisplay.h>


#ifdef __cplusplus
extern "C" {
#endif

void DrawImGui(XPLMDrawCallback_f inCallback, XPLMDrawingPhase inPhase, int inIsBefore, void *inRefcon);
void StartImGui(void);
void DestroyImGui(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //AVANTI_UI_H
