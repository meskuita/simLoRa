/*************************************************************************
	created:	2005/10/19												
	created:	19:10:2005   13:54				
	filename: 	emuLoRa_v1_b1.c													
	file path:	.\IoTdriver													
	file base:	IoTdrive_v1_b1											
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
int tm=1;

// Functions declaration
void IoTdriverInfo(); // IoTdriver information
void postMessage(int msg, char *apName); // present messages
void loadConfig(char configFile[], char *apName); 
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
    char *apName="IoTdriver_v1_b1";

    // IoTdbScript information
    IoTdriverInfo(apName);

    // load config file
    loadConfig(configFile, apName);

    // close 
    shutdown(0, apName);
}
/*************************************************************************
 * end main function                                                     *
 *************************************************************************/

/************************************************************************
 * Function:    IoTdriver                                                *
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
void IoTdriverInfo(char *apName)
{
    // emulator information
    printf("\e[1;1H\e[2J"); // clear screen
    printf("Running %s...\n\n", apName);
    postMessage(0, apName); // emuLoRa infos  
    return;
}
/*************************************************************************
 * end IoTdriver function                                                *


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
            printf("IoTdriver v1.0 build 1.0\n");
            printf("IoT database CPN Tools driver\n");
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
        /*
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
*/
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
