/*************************************************************************
	created:	2005/10/19												
	created:	19:10:2005   13:54				
	filename: 	emuLoRa_v1_b1.c													
	file path:	.\IoTdbScript													
	file base:	IoTdbScript_v1_b1											
	file ext:	c											
	author:		Rui Mesquita													
																				
	purpose:	test program for ini_management														
*************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <dirent.h> 

#include <sys/stat.h>
#include <sys/types.h>

#include "ini_manage.c"

// Variable declaration
char configFile[] = "configFile.ini";
char *old;
char *error;
char *unknown;
char *unrec;
char *path;
int tm=1;


// Functions declaration
void IoTdbScriptInfo(); // IoTdbScript information
void postMessage(int msg, char *apName); // present message 
void loadConfig(char configFile[], char *apName);
void checkDirStructure(char *apName); // check directory structure
void startScripting(char *apName, char *fileName); // start scripting 
void checkOlderFile(); // check older file
void checkFileIntegrity(char *fileName); // check file integrity
void readFile(char *fileName); // read file
void shutdown(int check, char *apName); // close


/*************************************************************************
 * Function:    main                                                     *
 * --------------------------------------------------------------------- *
 * date: 09jan2020                                                       *
 * explicação                                                            *
 *                                                                       *
 * argc:                                                                 *
 * argv:                                                                 *
 *                                                                       *
 * return:                                                               *
 *                                                                       *
 *************************************************************************/
int main(void)
{
    char *apName="IoTdbScript_v1_b1";
    char *fileName="teste.ini";

    // IoTdbScript information
    IoTdbScriptInfo(apName);

    // load config file
    loadConfig(configFile, apName);

    // check directory structure
    checkDirStructure(apName);

    // start scripting    
    startScripting(apName, fileName);

    // close 
    shutdown(0, apName);
}
/*************************************************************************
 * end main function                                                     *
 *************************************************************************/


/************************************************************************
 * Function:    IoTdbScriotInfo                                          *
 * --------------------------------------------------------------------- *
 * date: 09jan2020                                                       *
 * explicação                                                            *
 *                                                                       *
 * argc:                                                                 *
 * argv:                                                                 *
 *                                                                       *
 * return:                                                               *
 *                                                                       *
 *************************************************************************/
void IoTdbScriptInfo(char *apName)
{
    // emulator information
    printf("\e[1;1H\e[2J"); // clear screen
    printf("Running %s...\n\n", apName);
    postMessage(0, apName); // emuLoRa infos  
    return;
}
/*************************************************************************
 * end IoTdbScriptInfo function                                          *
 *************************************************************************/


/*************************************************************************
 * Function:    loadConfig
 * --------------------------------------------------------------------- *
 * date: 09jan2020                                                       *
 * explicação                                                            *
 *                                                                       *
 * argc:                                                                 *
 * argv:                                                                 *
 *                                                                       *
 * return:                                                               *
 *                                                                       *
 *************************************************************************/
void loadConfig(char configFile[], char *apName)
{
    char* tmp;

    // open config file
    printf("   Opening %s file...", configFile);    
    ini_start(configFile);
    if (get_last_error())
    {
        postMessage(6, apName); // red error
        // close 
        shutdown(0, apName);
    } else {
        postMessage(5, apName); // green ok
        // 
    	tmp = get_value("Main","OLD");
        old = tmp;
        printf("      Read files directory: /%s\n", old);
        sleep(tm);

    	tmp = get_value("Main","ERROR");
        error = tmp;
        printf("      Error file directory: /%s\n", error);
        sleep(tm);

    	tmp = get_value("Main","UNKNOWN");
        unknown = tmp;
        printf("      Unknown files directory: /%s\n", unknown);
        sleep(tm);

    	tmp = get_value("Main","UNREC");
        unrec = tmp;
        printf("      Unrecognized files directory: /%s\n", unrec);
        sleep(tm);

    	tmp = get_value("Main","PATH");
        path = tmp;
        printf("      Package directory path: %s\n", path);
        sleep(tm);

    	// save_as("configFile.ini");
    	// printf("last error is %s\n",get_last_error());
    	// print_content();	
    	ini_end();
        printf("\033[1;32m"); //green
        printf("   %s file loaded... [OK]\n\n", configFile);
        printf("\033[0m");
        sleep(tm);
    }
    return;
}
/*************************************************************************
 * end loadConfig                                                        *
 *************************************************************************/


/************************************************************************
 * Function:    checkDirStructure                                        *
 * --------------------------------------------------------------------- *
 * date: 09jan2020                                                       *
 * explicação                                                            *
 *                                                                       *
 * argc:                                                                 *
 * argv:                                                                 *
 *                                                                       *
 * return:                                                               *
 *                                                                       *
 *************************************************************************/
void checkDirStructure(char *apName)
{
    int check;
    char *str;

    postMessage(1, apName); // check directorie structure
    str=path;
    printf("-str---> %s\n", str);
    printf("-path---> %s\n", path);
    // check for archive
    check = mkdir(strcat(str,"/archive")      , 0777);
    printf("----> %s\n", str);    
    if (!check) // if not then create
    {
        postMessage(9, apName); // directory not found
        sleep(1);
        postMessage(5, apName); // green ok
    }
    str=path;
    printf("-str---> %s\n", str);
    printf("-path---> %s\n", path);
    // check for error
    check = mkdir("../emuLoRa/ftp/error", 0777);
    printf("----> %s\n", str);
    if (!check) // if not then create
    {
        postMessage(8, apName); // directory not found
        sleep(1);
        postMessage(5, apName); // green ok
    }  
    // check for unknown
    check = mkdir("../emuLoRa/ftp/unknown", 0777);
    if (!check) // if not then create
    {
        postMessage(7, apName); // directory not found
        sleep(1);
        postMessage(5, apName); // green ok
    }   
    // check for unrecognized
    check = mkdir("../emuLoRa/ftp/unrecognized", 0777);
    if (!check) // if not then create
    {
        postMessage(11, apName); // directory not found
        sleep(1);
        postMessage(5, apName); // green ok
    }
    postMessage(2, apName); // directorie structure checked
    postMessage(5, apName); // green ok
    printf("\n");
    return;
}
/*************************************************************************
 * end checkDirStructure function                                        *
 *************************************************************************/


/************************************************************************
 * Function:    startScripting                                           *
 * --------------------------------------------------------------------- *
 * date: 09jan2020                                                       *
 * explicação                                                            *
 *                                                                       *
 * argc:                                                                 *
 * argv:                                                                 *
 *                                                                       *
 * return:                                                               *
 *                                                                       *
 *************************************************************************/
void startScripting(char *apName, char *fileName)
{

    DIR *d;
    char *str;
    struct stat filestat;

    struct dirent *dir;
    d = opendir("../emuLoRa/ftp");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
            {
                char *txt = dir->d_name;
                str = strstr(txt, "error");
                if (!str)
                {
                    stat(txt, &filestat);
                    printf("----> %s - %s\n", txt, ctime(&filestat.st_ctime));
                }    
            }
        }
    }    
    closedir(d);

    // check older file 
    checkOlderFile();

    // check file integrity
    checkFileIntegrity(fileName);

    // read file
    readFile(fileName);
}
/*************************************************************************
 * end startScripting function                                           *
 *************************************************************************/


/************************************************************************
 * Function:    checkOlderFile                                           *
 * --------------------------------------------------------------------- *
 * date: 09jan2020                                                       *
 * explicação                                                            *
 *                                                                       *
 * argc:                                                                 *
 * argv:                                                                 *
 *                                                                       *
 * return:                                                               *
 *                                                                       *
 *************************************************************************/
void checkOlderFile()
{
    return;
}
/*************************************************************************
 * end checkOlderFile function                                           *
 *************************************************************************/

/************************************************************************
 * Function:    checkFileIntegrity                                       *
 * --------------------------------------------------------------------- *
 * date: 09jan2020                                                       *
 * explicação                                                            *
 *                                                                       *
 * argc:                                                                 *
 * argv:                                                                 *
 *                                                                       *
 * return:                                                               *
 *                                                                       *
 *************************************************************************/
void checkFileIntegrity(char *fileName)
{
    return;
}
/*************************************************************************
 * end checkFileIntegrity function                                       *
 *************************************************************************/

/************************************************************************
 * Function:    readFile                                                 *
 * --------------------------------------------------------------------- *
 * date: 09jan2020                                                       *
 * explicação                                                            *
 *                                                                       *
 * argc:                                                                 *
 * argv:                                                                 *
 *                                                                       *
 * return:                                                               *
 *                                                                       *
 *************************************************************************/
void readFile(char *fileName)
{
    return;
}
/*************************************************************************
 * end readFile function                                                 *
 *************************************************************************/













/*************************************************************************
 * Function:    postMessage                                              *
 * --------------------------------------------------------------------- *
 * date: 09jan2020                                                       *
 * explicação                                                            *
 *                                                                       *
 * argc:                                                                 *
 * argv:                                                                 *
 *                                                                       *
 * return:                                                               *
 *                                                                       *
 *************************************************************************/
void postMessage(int msg, char *apName)
{
    switch (msg)
    {
        case 0: // emuLoRa infos
            printf("IoTdbScript v1.0 build 1.0\n");
            printf("LPWAN LoRaWAN IoT Database Script\n");
            printf("by Rui Mesquita\n");
            printf("Free Software released under the GNU/GPL License\n\n");
            break;
        case 1: // check directorie structure
            printf ("   Checking the directorie structure\n");
            break;
        case 2: // directorie structure checked
            printf ("   Directorie structure... "); 
            break;
        case 3: // 
            break;
        case 4: // 
            break;
        case 5: // green ok
            printf("\033[1;32m"); //green
            printf("[OK]\n");
            printf("\033[0m");
            break;
        case 6: // red error
            printf("\033[1;31m"); // red
            printf("[ERROR]\n");
            printf("\033[0m");
            break;
         case 7: // unknown directory not found
            printf("\033[1;31m"); // red            
            printf("           Directory not found!");
            printf("\033[0m"); 
            printf(" 'UNKNOWN' directory created... ");
            break;
         case 8: // error directory not found
            printf("\033[1;31m"); // red            
            printf("           Directory not found!");
            printf("\033[0m"); 
            printf(" 'ERROR' directory created... ");
            break;
         case 9: // old directory not found
            printf("\033[1;31m"); // red            
            printf("           Directory not found!");
            printf("\033[0m"); 
            printf(" 'ARCHIVE' directory created... ");
            break;
        case 10: // abnormally closed
            printf("\033[1;31m");
            printf("\n%s abnormally closed.\n\n", apName);
            printf("\033[0m");
            break;
         case 11: // old directory not found
            printf("\033[1;31m"); // red            
            printf("           Directory not found!");
            printf("\033[0m"); 
            printf(" 'UNRECOGNIZED' directory created... ");
            break;
    }
}
/*************************************************************************
 * end postMessage function                                              *
 *************************************************************************


/*************************************************************************
 * Function:    shutdown
 * --------------------------------------------------------------------- *
 * date: 09jan2020                                                       *
 * explicação                                                            *
 *                                                                       *
 * argc:                                                                 *
 * argv:                                                                 *
 *                                                                       *
 * return:                                                               *
 *                                                                       *
 *************************************************************************/
void shutdown(int check, char *apName){
    if (check == 0){
        printf("\n%s closed.\n\n", apName);
    } else {
        postMessage(10, apName); // abnormally closed
    }
    exit (1);
}
/*************************************************************************
 * end shutdown function                                                 *
 *************************************************************************/
