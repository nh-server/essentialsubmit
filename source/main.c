#include <curl/curl.h> //curl
#include <3ds.h>       //libctru

// stdlib stuff
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define SOC_ALIGN 0x1000
#define SOC_BUFFERSIZE 0x100000
static u32 *SOC_buffer = NULL;

int main() {
    gfxInitDefault();
    consoleInit(GFX_TOP, GFX_LEFT);
    Result ret;
    SOC_buffer = (u32 *)memalign(SOC_ALIGN, SOC_BUFFERSIZE);
    if (SOC_buffer == NULL) {
        printf("memalign: failed to allocate\n");
    }

    // Now intialise soc:u service
    if ((ret = socInit(SOC_buffer, SOC_BUFFERSIZE)) != 0) {
        printf("socInit: 0x%08X\n", (unsigned int)ret);
    }

    // from now on socket is initialized and we happy use curl

    CURL *curl;
    CURLcode res;
    curl_mime *mime;
    curl_mimepart *part1;
    curl_mimepart *part2;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl) {
        printf("curl did the init stuff\n");
        /* First set the URL that is about to receive our POST. This URL can
           just as well be an https:// URL if that is what should receive the
           data. */
        mime = curl_mime_init(curl);
        part1 = curl_mime_addpart(mime);
        part2 = curl_mime_addpart(mime);
        curl_mime_filedata(part1, "sdmc:/gm9/out/essential.exefs");
        curl_mime_name(part1, "file");
        curl_mime_data(part2, "gruetzig", CURL_ZERO_TERMINATED);
        curl_mime_name(part2, "discordhandle");
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 5000);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_URL, "https://3ds.gruetzig.dev/submit");
        printf("Hehe set all the stuff\n");
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK)
            printf("curl_easy_perform() failed: %s\n",
                   curl_easy_strerror(res));
        printf("sickomatico it worked\n");
        /* always cleanup */
        curl_easy_cleanup(curl);
        curl_mime_free(mime);
    }
    curl_global_cleanup();
    printf("Done!!\n");
    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
            break;
    }

    socExit();
    gfxExit();
}