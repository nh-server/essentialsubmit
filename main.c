#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    CURL *curl;
  CURLcode res;
  curl_mime *mime;
  curl_mimepart *part1;
  curl_mimepart *part2;
 
  /* In windows, this will init the winsock stuff */
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be an https:// URL if that is what should receive the
       data. */
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5000/submit");
    /* Now specify the POST data */
    int array[] = {1,2,3,4,5,6,7,8};

    mime = curl_mime_init(curl);
    part1 = curl_mime_addpart(mime);
    part2 = curl_mime_addpart(mime);
    curl_mime_filedata(part1, "sdmc:/gm9/out/essential.exefs");
    curl_mime_name(part1, "file");
    curl_mime_data(part2, "gruetzig", CURL_ZERO_TERMINATED);
    curl_mime_name(part2, "discordhandle");
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
 
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    
    /* Check for errors */
    if(res != CURLE_OK)
      printf("curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */
    curl_easy_cleanup(curl);
    curl_mime_free(mime);
  }
  curl_global_cleanup();
    printf("Hello World\n");
    return 0;
}