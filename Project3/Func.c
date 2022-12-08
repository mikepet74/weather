#include "Func.h"

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

FILE* openFile(FILE* filePointer, char* readingOrWriting)
{
	static int numOfFile = 0;
	int CheckingAllocatedDynamicMemory = 0;
	if (numOfFile == 0)
	{
		fputs("Enter a file name to read: ", stdout);
	}
	else if (numOfFile == 1)
	{
		fputs("Enter a file name to write: ", stdout);
	}
	char* nameOfFile = readLine(stdin);
	FILE* read = stdin;
	do
	{
		if ((filePointer = fopen(nameOfFile, readingOrWriting)) == NULL)//Checking if the opening failed
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
	numOfFile++;
	return filePointer;//returning the pointer
}

char* returnAPI(char* city)
{
	char* startAPIKey = _strdup("https://api.openweathermap.org/data/2.5/weather?q=");
	char* endAPIKey = "&appid=ea07a0dbcc7ad44e8e032453194681b4&units=metric";
	char* apiKey = NULL;
	apiKey = (char*)realloc(startAPIKey, strlen(startAPIKey) + strlen(city) + 1 + strlen(endAPIKey) + 1);

	apiKey = strcat(apiKey, city);
	apiKey = strcat(apiKey, endAPIKey);

	return apiKey;
}

char* serverRequest(char* city)
{
	CURL* curl_handle;
	CURLcode res;
	char* apiKey = returnAPI(city);

	struct MemoryStruct chunk;

	chunk.memory = (char*)malloc(sizeof(char));  /* will be grown as needed by the realloc above */
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
	return chunk.memory;
}

void printToFile(FILE* filePointerReading, FILE* filePointerWriting)
{
	char* lineFromFile = NULL;
	char endOfFileCheck;
	int i, j, k, lineNumber = 1;
	char* city, * temp, * apiKey, * data, *saving;
	if ((lineFromFile = readLine(filePointerReading)) == NULL)
	{
		printf("The system failed to read line %d from the file", lineNumber);
	}

	//Print title
	fprintf(filePointerWriting, lineFromFile);
	fprintf(filePointerWriting, ",temp,wind_speed,clouds\n");


	while ((endOfFileCheck = getc(filePointerReading)) != EOF)//A loop that ran until the end of the file
	{
		i = 0;
		fseek(filePointerReading, -1, SEEK_CUR);

		if ((lineFromFile = readLine(filePointerReading)) == NULL)
		{
			printf("The system failed to read line %d from the file", lineNumber);
			continue;
		}

		//char* str = lineFromFile;
		while (lineFromFile[i] != '\0' && lineFromFile[i + 1] != '\0')
		{
			if (lineFromFile[i] == ',' && lineFromFile[i + 1] == ',')
			{
				lineFromFile = (char*)realloc(lineFromFile, strlen(lineFromFile) + 1);
				for (j = strlen(lineFromFile) + 1;j > i;j--)
				{
					lineFromFile[j + 1] = lineFromFile[j];
				}
				lineFromFile[i + 1] = ' ';
			}
			i++;
		}

		lineFromFile = strtok(lineFromFile, ",");
		fprintf(filePointerWriting, lineFromFile);
		fprintf(filePointerWriting, ",");
		lineFromFile = strtok(NULL, ",");
		fprintf(filePointerWriting, lineFromFile);
		fprintf(filePointerWriting, ",");
		lineFromFile = strtok(NULL, ",");
		fprintf(filePointerWriting, lineFromFile);
		fprintf(filePointerWriting, ",");
		city = _strdup(lineFromFile);
		lineFromFile = strtok(NULL, ",");
		if (lineFromFile)
		{
			fprintf(filePointerWriting, lineFromFile);
		}
		fprintf(filePointerWriting, ",");

		for (j = 0;j < strlen(city);j++) {
			if (city[j] == ' ')
			{
				city = (char*)realloc(city, strlen(city) + 3);
				for (k = strlen(city);k >= j;k--) {
					city[k + 2] = city[k];
				}
				city[j] = '%';
				j++;
				city[j] = '2';
				j++;
				city[j] = '0';
			}
		}

		data = serverRequest(city);

		saving=temp = _strdup(data);
		temp = strstr(temp, "\"temp\":");
		temp = strstr(temp, ":");
		temp = strtok(temp, ":");
		temp = strtok(NULL, ",");

		fprintf(filePointerWriting, temp);
		fprintf(filePointerWriting, ",");
		free(saving);

		saving=temp = _strdup(data);
		temp = strstr(temp, "\"wind\":");
		temp = strstr(temp, "\"speed\"");
		temp = strtok(temp, ":");
		temp = strtok(NULL, ",");

		fprintf(filePointerWriting, temp);
		fprintf(filePointerWriting, ",");
		free(saving);

		saving=temp = _strdup(data);
		temp = strstr(temp, "\"description\":");
		temp = strtok(temp, ":");
		temp = strtok(NULL, ",");

		fprintf(filePointerWriting, temp);
		fprintf(filePointerWriting, ",\n");
		free(saving);


	}
	fclose(filePointerWriting);
	fclose(filePointerReading);

}