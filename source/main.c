#include <curl/curl.h> //curl
#include <3ds.h>       //libctru

#include <stdio.h>
#include "network.h"
#include "ui.h"

DrawContext ctx;

void enter(char* inout) {
    SwkbdState swkbd;
    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 1, 50);
    swkbdInputText(&swkbd, inout, 51);
    return;
}

int main() {
    gfxInitDefault();
    if (!initSocket()) {
        goto fail;
    }
    int result;
    result = initUI();
    if (result) {
        printf("initUI returned %d", result);
        goto fail;
    }
    char discordtag[33] = "";
    char address[52] = "";
    initContext(&ctx);
    initColors(&ctx);
    int menustate = 0;
    char finaltext[200];

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(ctx.top, ctx.clrBgDark);
        C2D_SceneBegin(ctx.top);
        drawText(SCREEN_WIDTH_TOP/2, 0, 0, 0.7, ctx.clrWhite, C2D_AlignCenter, "Nintendo Homebrew essential.exefs Submitter");
        switch(menustate) {
            case 0:
                drawText(20, 40, 0, 0.5, ctx.clrWhite, 0, "Your Discord tag: %s", discordtag);
                drawText(20, 70, 0, 0.5, ctx.clrWhite, 0, "Address entered: %s", address);
                if (strlen(address) > 0 && strlen(discordtag) > 0) {
                    drawText(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT*3/4, 0, 0.7, ctx.clrWhite, C2D_AlignCenter, "Press A to submit");
                } else {
                    drawText(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT*3/4, 0, 0.7, ctx.clrWhite, C2D_AlignCenter, "Press X to enter the server address\nPress Y to enter your Discord tag");
                }
                if (kDown & KEY_X) {
                    enter(address);
                }
                if (kDown & KEY_Y) {
                    enter(discordtag);
                }
                if (strlen(address) > 0 && strlen(discordtag) > 0 && kDown & KEY_A) {
                    menustate++;
                } 
                break;
            case 1:
                drawText(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT*3/4, 0, 0.7, ctx.clrWhite, C2D_AlignCenter, "Submitting...");
                break;
            case 2: 
                drawText(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT*3/4, 0, 0.7, ctx.clrWhite, C2D_AlignCenter, finaltext);
                break;

        }
        C3D_FrameEnd(0);
        switch(menustate) {
            case 0:
            case 2:
                if (kDown & KEY_START)
                    goto exit;
                break;
            case 1:
                initcurl();
                initform(); 
                discordhandleentry(discordtag);
                fileentry("sdmc:/gm9/out/essential.exefs");
                CURLcode res = submittourl(address);
                if(res != CURLE_OK) {
                    sprintf(finaltext, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));
                } else {
                    sprintf(finaltext, "Information submitted.");
                }
                exiteverything();
                menustate++;
                break;


        }
        
        
       
    }

fail:
    consoleInit(GFX_TOP, GFX_LEFT);
    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
            break;
    }
exit:
    exitUI();
    socExit();
    gfxExit();
}