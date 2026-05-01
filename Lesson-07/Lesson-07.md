# Lesson 7 - Parsing the NMEA 0183 output

## Step 1 - Create the NMEA 0183 library

In the pico-gps-oled/thirdparty/ directory, create a directory named nmea0183/, and within it, create nmea0183.h and nmea0183.c files.  We're creating these outside of our lessons, so they can be used from multiple lessons as needed.

The nmea0183.h file should consist of the following lines:
```
#pragma once


enum NMEA0183_SentenceIdEnum
{
    NMEA0183_SentenceId_Error = -1,
    NMEA0183_SentenceId_Unknown,
    NMEA0183_SentenceId_GGA,
    NMEA0183_SentenceId_GLL,
    NMEA0183_SentenceId_GSA,
    NMEA0183_SentenceId_GSV,
    NMEA0183_SentenceId_RMC,
    NMEA0183_SentenceId_VTG
};


enum NMEA0183_SentenceIdEnum nmea0183_getSentenceId(const char *sentence, int sentenceLength);

int nmea0183_tokenizeSentence(const char *sentence, char tokens[][20]);
void nmea0183_parseSentenceRMC(const char *sentence, char *date, char *time, double *latitude, double *longitude, float *speed, float *course);
```

Because we're using C, which doesn't have namespaces, I've prefixed the functions and enum with nmea0183 to identify them as part of this library.  The enumeration, NMEA0183_SentenceIdEnum, represents all of the NMEA 0183 sentences that we might reasonably expect to see.

nmea0183_getSentenceId() will take the current sentence and sentence length, and it will return the enumeration value that represents the sentence we gave it.  In this way, we can identify the sentence, and we can process it correctly.

nmea0183_tokenizeSentence() will be called by the parse function for each sentence type.  It generically breaks the sentence into tokens using the comma as a delimiter.  However, the final delimiter is an asterix, which separates the fields from the checksum.  All of the tokens will be placed in the provided tokens[] array, and the number of tokens will be returned.

nmea0183_parseSentenceRMC() is the only parsing function that I'll implement for you.  The RMC sentence is the recommended minimum GPS data, which provides you a position, speed, and course.  We'll write these into the provided prointer variables for the calling program to use.

The nmea0183.c file should consist of the following lines:
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nmea0183.h"


enum NMEA0183_SentenceIdEnum nmea0183_getSentenceId(const char *sentence, int sentenceLength)
{
    if(sentenceLength < 10  ||  sentenceLength > 82)
        return NMEA0183_SentenceId_Error;
    
    if(sentence[0] != '$')
        return NMEA0183_SentenceId_Error;
    
    if(sentence[sentenceLength - 2] != '\r'  ||  sentence[sentenceLength - 1] != '\n')
        return NMEA0183_SentenceId_Error;
    
    char sentenceName[4] = { 0, 0, 0, 0 };
    strncpy(sentenceName, sentence + 3, 3);
    
    if(strcmp(sentenceName, "GGA") == 0)
        return NMEA0183_SentenceId_GGA;
    else if(strcmp(sentenceName, "GLL") == 0)
        return NMEA0183_SentenceId_GLL;
    else if(strcmp(sentenceName, "GSA") == 0)
        return NMEA0183_SentenceId_GSA;
    else if(strcmp(sentenceName, "GSV") == 0)
        return NMEA0183_SentenceId_GSV;
    else if(strcmp(sentenceName, "RMC") == 0)
        return NMEA0183_SentenceId_RMC;
    else if(strcmp(sentenceName, "VTG") == 0)
        return NMEA0183_SentenceId_VTG;
    else
        return NMEA0183_SentenceId_Unknown;
}


int nmea0183_tokenizeSentence(const char *sentence, char tokens[][20])
{
    int tokenIndex = 0;
    const char *start = sentence + 7;
    const char *ptr;

    do
    {
        ptr = start;

        while(*ptr  &&  *ptr != ',')
            ++ptr;
        
        if(*ptr)
        {
            int size = ptr - start;
            strncpy(tokens[tokenIndex], start, size);
            tokens[tokenIndex++][size] = '\0';
            
            start = ptr + 1;
        }

    } while(*ptr);
    
    
    ptr = start;

    while(*ptr  &&  *ptr != '*')
        ++ptr;
    
    if(*ptr)
    {
        int size = ptr - start;
        strncpy(tokens[tokenIndex], start, size);
        tokens[tokenIndex++][size] = '\0';

        strcpy(tokens[tokenIndex++], ptr);
    }
    else
    {
        strcpy(tokens[tokenIndex++], start);
    }

    return tokenIndex;
}


void nmea0183_parseSentenceRMC(const char *sentence, char *date, char *time, double *latitude, double *longitude, float *speed, float *course)
{
    char tokens[20][20];
    int tokenCount = nmea0183_tokenizeSentence(sentence, tokens);
    
    sprintf(date, "%c%c/%c%c/%c%c", tokens[8][2], tokens[8][3], tokens[8][0], tokens[8][1], tokens[8][4], tokens[8][5]);
    sprintf(time, "%c%c:%c%c:%c%c", tokens[0][0], tokens[0][1], tokens[0][2], tokens[0][3], tokens[0][4], tokens[0][5]);
    
    *latitude = (tokens[2][0] - '0') * 10.0 +
                (tokens[2][1] - '0') +
                atof(&tokens[2][2]) / 60.0;
    
    if(tokens[3][0] == 'S')
        *latitude *= -1.0;
    
    if(tokens[4][4] == '.')
    {
        *longitude = (tokens[4][0] - '0') * 10.0 +
                     (tokens[4][1] - '0') +
                     atof(&tokens[4][2]) / 60.0;
    }
    else
    {
        *longitude = (tokens[4][0] - '0') * 100.0 +
                     (tokens[4][1] - '0') * 10.0 +
                     (tokens[4][2] - '0') +
                     atof(&tokens[4][3]) / 60.0;
    }

    if(tokens[5][0] == 'W')
        *longitude *= -1.0;
    
    *speed = atof(&tokens[6][0]);

    *course = atof(&tokens[7][0]);
}
```

I've provided the implementation for the three library functions, and I'll describe how each works.  Make sure you understand the pointer indirection and indexing that is occuring, especially in the tokenization function.

nmea0183_getSentenceId() ensures the sentence is of legal length, between 10 and 82 charactes.  Then it ensures the sentence begins with a dollar sign and ends with a crriage return and linefeed pair.  Finally, it jumps over the talker ID to read the three character sentence type, and it returns the associated enumeration value.  If anything goes wrong, it returns the error value, and if it doesn't recognize the sentence type, it returns the unknown value.

nmea0183_tokenizeSentence() initializes its read pointer to the eighth the character in the sentence.  This bypasses the talker ID and sentence type.  It moves the pointer forward until it finds a comma, which is the field delimiter.  Then it copies the characters from start to the pointer into the current token array element.  The start is moved forward to one character after the comma, and the process is repeated.  This continues until the pointer reaches the NUL character at the end of the string.  Then it rescans the final token looking for the asterix character.  If it finds it, then the token is copied and so is the checksum.  If it doesn't find it, then just the token is copied.  The number of valid tokens is returned to the calling function.

nmea0183_parseSentenceRMC() calls the tokenizer function to populate its token array.  Since each sentence has a *mostly* fixed number of fields, we know which variables each field represents.  We convert each field to the type of the variable provided for it by the caller.  Date and time are turned into strings with the common U.S. representations.  Latitude and longitude are provided as degrees and minutes in base sixty, or sexagesimal.  If latitude is south of the equator or longitude is west of the prime meridian, then we need to negate them.  Finally, speed is provided in knots, and course is in degrees.


## Step 2 - Modify the CMake

Add the following line to the add_executable() for pico-gps-oled:
```
../thirdparty/nmea0183/nmea0183.c
```

This will include the NMEA 0183 library in your application.  To ensure the application can find the header file for the library, add this line to your target_include_directories():
```
${CMAKE_SOURCE_DIR}/../thirdparty/nmea0183
```


## Step 3 - Modify the main.c

Add the following lines to the top of you main.c, so you can include the NMEA 0183 library:
```
#include "nmea0183.h"
```

Just before your main loop, add these lines to declare the variables we will use.
```
char date[10];
char time[10];
double latitude = 99.0;
double longitude = 999.0;
float speed = 999.0;
float course = 999.0;
```

Add the following lines between printing the gpsLine and zeroing gpsLineOffset:
```
switch(nmea0183_getSentenceId(gpsLine, gpsLineOffset))
{
    case NMEA0183_SentenceId_RMC:
        nmea0183_parseSentenceRMC(gpsLine, date, time, &latitude, &longitude, &speed, &course);
        break;
    
    default:
        // unknown or unhandled sentence
        break;
}
```

Between clear'ing the OLED display and show'ing it, we should have only these lines:
```
char buf[10];
ssd1306_draw_string(&disp, 0, 0, 1, date);
ssd1306_draw_string(&disp, 0, 8, 1, time);
snprintf(buf, sizeof(buf), "%.3lf %c", fabs(latitude), latitude >= 0.0 ? 'N' : 'S');
ssd1306_draw_string(&disp, 0, 16, 1, buf);
snprintf(buf, sizeof(buf), "%.3lf %c", fabs(longitude), longitude >= 0.0 ? 'E' : 'W');
ssd1306_draw_string(&disp, 0, 24, 1, buf);
snprintf(buf, sizeof(buf), "%.1f kts", speed);
ssd1306_draw_string(&disp, 0, 32, 1, buf);
snprintf(buf, sizeof(buf), "%.1f", course);
ssd1306_draw_string(&disp, 0, 40, 1, buf);
```


## Conclusion

After building, loading, and running this, you should have a complete program that reads the time, date, position, speed, and course from GNSS and displays it on your OLED screen.  However, this doesn't look great, and its only providing parameters from a single message.  Your task is to implement the other messages, and design good-looking displays for the information contained within.  I would expect to see a clock screen (bonus internet points for an analog clock), a position screen (bonus internet points for a map of the world centered on you), and a course and speed page (bonus internet points for a compass with mph).  For now, you can just switch between the screens every few seconds, but we will be adding a button for this purpose in a future chapter.
