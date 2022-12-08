#define _CRT_SECURE_NO_WARNINGS
#define CURL_STATICLIB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "curl/curl.h"

#ifdef _DEBUG
#pragma comment (lib, "curl/libcurl_a_debug.lib")
#else
#pragma comment (lib, "curl/libcurl_a.lib")
#endif

#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "advapi32.lib")


struct MemoryStruct {
    char* memory;
    size_t size;
};

static size_t
WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

#define CHUNK 10
char* readLine(FILE* read)
{
	char* input = NULL, * tempInput;
	char tempbuf[CHUNK] = { '\0' };
	int inputLength = 0, tempLength = 0;

	do
	{
		fgets(tempbuf, CHUNK, read);//Size ten absorption
		tempLength = (int)strlen(tempbuf);//Checking how much was absorbed

		if ((tempInput = (char*)realloc(input, inputLength + tempLength + 1)) == NULL)//Increasing the main string by the received beat
		{
			return input;
		}
		input = tempInput;

		strcpy(input + inputLength, tempbuf);//String concatenation
		inputLength += tempLength;//A summary of everything that has been absorbed so far

	} while (tempLength == CHUNK - 1 && tempbuf[CHUNK - 2] != '\n');

	if (input[strlen(input) - 1] == '\n')
	{
		input[strlen(input) - 1] = '\0';
	}
	else
	{
		input[strlen(input)] = '\0';
	}

	return input;
}

FILE* openFile(FILE* filePointer)
{
	int CheckingAllocatedDynamicMemory = 0;
	printf("Enter name of file: ");
	char* nameOfFile = readLine(stdin);
	FILE* read = stdin;
	do
	{
		if ((filePointer = fopen(nameOfFile, "rt")) == NULL)//Checking if the opening failed
		{
			if (CheckingAllocatedDynamicMemory)
			{
				free(nameOfFile);
			}

			puts("The file for reading is not opened");
			fputs("Enter a file name to read: ", stdout);

			while ((nameOfFile = readLine(read)) == NULL)//A loop to receive another file name
			{
				printf("try again\n");
			}

			CheckingAllocatedDynamicMemory = 1;
		}
	} while (filePointer == NULL);

	if (CheckingAllocatedDynamicMemory)
	{
		free(nameOfFile);
	}

	return filePointer;//returning the pointer
}


FILE* readingFile(FILE * filePointer)
{
	char* lineFromFile = NULL;
	char endOfFileCheck;
	int customerIndexExists, lineNumber = 0, i;
	int j, m;
	char* city;
	char* temp;
	if ((lineFromFile = readLine(filePointer)) == NULL)
	{
		printf("The system failed to read line %d from the file", lineNumber);
	}
	printf("%s\n", lineFromFile);

	FILE* output_file;
	output_file = fopen("output.csv", "w");
	fprintf(output_file, lineFromFile);
	fprintf(output_file, ", temp, wind_speed, clouds\n");
	//fclose(output_file);

	while ((endOfFileCheck = getc(filePointer)) != EOF)//A loop that ran until the end of the file
	{
		fseek(filePointer, -1, SEEK_CUR);

		if ((lineFromFile = readLine(filePointer)) == NULL)
		{
			printf("The system failed to read line %d from the file", lineNumber);
			continue;
		}
		int i = 0;
		char* str = lineFromFile;
		while (lineFromFile[i] != '\0' && lineFromFile[i + 1] != '\0')
		{
			if (lineFromFile[i] == ',' && lineFromFile[i + 1] == ',') {
				str = realloc(str, strlen(lineFromFile) + 1);
				for (j = 0;j <= i;j++) {
					str[j] = lineFromFile[j];
				}
				str[j] = ' ';
				j++;
				for (;j <= strlen(lineFromFile) + 2;j++) {
					str[j] = lineFromFile[j - 1];					}
				}
				i++;
			}
		str = strtok(str, ",");
		fprintf(output_file, str);
		fprintf(output_file, ",");
		str = strtok(NULL, ",");
		fprintf(output_file, str);
		fprintf(output_file, ",");
		str = strtok(NULL, ",");
		fprintf(output_file, str);
		fprintf(output_file, ",");
		city = _strdup(str); 
		str = strtok(NULL, ",");
		if(str)
			fprintf(output_file, str);
		fprintf(output_file, ",");


		CURL* curl_handle;
		CURLcode res;

		for (j = 0;j < strlen(city);j++) {
			if (city[j] == ' ')
				city[j] = '-';
		}
		char* startAPIKey = _strdup("https://api.openweathermap.org/data/2.5/weather?q=");
		char* endAPIKey = _strdup("&appid=ea07a0dbcc7ad44e8e032453194681b4&units=metric");
		char* apiKey = _strdup(startAPIKey);
		apiKey = realloc(apiKey, strlen(apiKey) + strlen(city) + 1);
		apiKey = strcat(apiKey, city);
		apiKey = realloc(apiKey, strlen(apiKey) + strlen(endAPIKey) + 1);
		apiKey = strcat(apiKey, endAPIKey);

		struct MemoryStruct chunk;

		chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
		chunk.size = 0;    /* no data at this point */

		curl_global_init(CURL_GLOBAL_ALL);

		/* init the curl session */
		curl_handle = curl_easy_init();

		/* specify URL to get */
		curl_easy_setopt(curl_handle, CURLOPT_URL, apiKey);

		/* send all data to this function  */
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

		/* we pass our 'chunk' struct to the callback function */
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);

		/* some servers do not like requests that are made without a user-agent
		   field, so we provide one */
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		/* get it! */
		res = curl_easy_perform(curl_handle);

		/* check for errors */
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		}
		else {
			/*
			 * Now, our chunk.memory points to a memory block that is chunk.size
			 * bytes big and contains the remote file.
			 *
			 * Do something nice with it!
			 */

			printf("%s bytes retrieved\n", chunk.memory);
		}

		/* cleanup curl stuff */
		curl_easy_cleanup(curl_handle);

		//free(chunk.memory);

		/* we are done with libcurl, so clean it up */
		curl_global_cleanup();




		temp = _strdup(chunk.memory);
		temp = strstr(temp, "\"temp\":");
		temp = strstr(temp, ":");
		temp = strtok(temp, ":");
		temp = strtok(NULL, ",");

		fprintf(output_file, temp);
		fprintf(output_file, ",");


		temp = _strdup(chunk.memory);
		temp = strstr(temp, "\"wind\":");
		temp = strstr(temp, "\"speed\"");
		temp = strtok(temp, ":");
		temp = strtok(NULL, ",");

		fprintf(output_file, temp);
		fprintf(output_file, ",");

		temp = _strdup(chunk.memory);
		temp = strstr(temp, "\"description\":");
		temp = strtok(temp, ":");
		temp = strtok(NULL, ",");
		
		fprintf(output_file, temp);
		fprintf(output_file, ",\n");
 		//fclose(output_file);
	}
	return output_file;
}

int main(void)
{

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
	FILE* fpr = NULL;
	fpr = openFile(fpr);
	fclose(readingFile(fpr));
	
    return 0;
}