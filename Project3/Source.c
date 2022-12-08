#include "Func.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
 //   CURL* curl_handle;
 //   CURLcode res;


	//char* startAPIKey = _strdup("https://api.openweathermap.org/data/2.5/weather?q=");
	//char* city = _strdup("Jerusalem");
	//char* endAPIKey = _strdup("&appid=ea07a0dbcc7ad44e8e032453194681b4&units=metric");
	//char* apiKey = _strdup(startAPIKey);
	//apiKey = realloc(apiKey, strlen(apiKey) + strlen(city)+1);
	//apiKey = strcat(apiKey, city);
	//apiKey = realloc(apiKey, strlen(apiKey) + strlen(endAPIKey)+1);
	//apiKey = strcat(apiKey, endAPIKey);

 //   struct MemoryStruct chunk;

 //   chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
 //   chunk.size = 0;    /* no data at this point */

 //   curl_global_init(CURL_GLOBAL_ALL);

 //   /* init the curl session */
 //   curl_handle = curl_easy_init();

 //   /* specify URL to get */
 //   curl_easy_setopt(curl_handle, CURLOPT_URL, apiKey);

 //   /* send all data to this function  */
 //   curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

 //   /* we pass our 'chunk' struct to the callback function */
 //   curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);

 //   /* some servers do not like requests that are made without a user-agent
 //      field, so we provide one */
 //   curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

 //   /* get it! */
 //   res = curl_easy_perform(curl_handle);

 //   /* check for errors */
 //   if (res != CURLE_OK) {
 //       fprintf(stderr, "curl_easy_perform() failed: %s\n",
 //           curl_easy_strerror(res));
 //   }
 //   else {
 //       /*
 //        * Now, our chunk.memory points to a memory block that is chunk.size
 //        * bytes big and contains the remote file.
 //        *
 //        * Do something nice with it!
 //        */

 //       printf("%s bytes retrieved\n", chunk.memory);
 //   }

 //   /* cleanup curl stuff */
 //   curl_easy_cleanup(curl_handle);

 //   free(chunk.memory);

 //   /* we are done with libcurl, so clean it up */
 //   curl_global_cleanup();
	FILE* fpr = NULL, *fpw=NULL;
	fpr = openFile(fpr, "rt");
	fpw = openFile(fpw, "w");
	printToFile(fpr, fpw);
	
    return 0;
}