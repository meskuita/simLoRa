/*************************************************************************
	created:	2005/10/19												
	created:	19:10:2005   13:54				
	filename: 	emuLoRa_v1_b1.c													
	file path:	.\emuLoRa													
	file base:	emuLoRa_v1_b1											
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
#include "ini_manage.c"
//#include "ini.c"
#define MAXCHAR 1000

// Variable declaration
int gw, dev, max_gw, max_dev, max_time;
int tm=1;
char configFile[] = "configFile.ini";
char *gwFile;  
char *devFile;

// Functions declaration
int checkArg(int argc, char *argv[]);
void emuLoRa_info(char *argv[]);
void loadConfig(char configFile[], char *argv[]);
void loadGW(char *argv[]);
void loadDev(char *argv[]);
void startEmu();
int generateRandoms(int lower, int upper, int count);
void shutdown(int check, char *argv[]);


/*
 * Function:    main
 * -----------------
 * date: 09jan2020
 * explicação
 * 
 * argc:
 * argv:
 *
 * return:
 *
 */
int main( int argc, char *argv[] ){

    // emuLoRa information
    emuLoRa_info(argv);

    // checking argument
    if (checkArg(argc, argv)==0){;

        gw = atoi(argv[1]); // number of gateways
        dev = atoi(argv[2]); // number of devices
        max_time = atoi(argv[3]); // time between transmissions
        //printf("dev %i...\n\n", dev);
        
        // load config file
        loadConfig(configFile, argv);

        // load gateways list file
        loadGW(argv);

        // load devices list file
        loadDev(argv);

        // start emulation
        startEmu();

    } else {
        // close 
        shutdown(0, argv);
    }
}


/*
 * Function:    checkArg
 * ---------------------
 * date: 09jan2020
 * explicação
 * 
 * argc:
 * argv:
 *
 * return:
 *
 */
int checkArg(int argc, char *argv[])
{
    if ( argc == 4) { // arguments ok
        printf("\033[1;32m");
        printf ("   Arguments OK!\n\n");
        printf("\033[0m");
        return 0;
    } else if ( argc > 4 ) { // too many arguments
        printf("\033[1;31m");
        printf("    Too many arguments supplied.\n");
        printf("\033[0m");
        printf("    %s: ", argv[0]); 
        printf ("modo de uso: ./teste1 [gw number] [devices number] [max time].\n");
        printf("\n");
        return 1;
    } else { // missing arguments
        printf("\033[1;31m");
        printf("    Missing arguments.\n");
        printf("\033[0m");
        printf("    %s: ", argv[0]); 
        printf ("modo de uso: ./teste1 [gw number] [devices number] [max time].\n");
        printf("\n");
        return 1;
    }
}


/*
 * Function:    emuLoRa_info
 * -------------------------
 * date: 09jan2020
 * explicação
 * 
 * argc:
 * argv:
 *
 * return:
 *
 */
void emuLoRa_info(char *argv[])
{
    // emulator information
    printf("\e[1;1H\e[2J"); // clear screen
    printf("Running %s...\n\n", argv[0]);
    printf("emuLoRa v1.0 build 2.0\n");
    printf("LPWAN LoRaWAN Network Emulator\n");
    printf("by Rui Mesquita\n");
    printf("Free Software released under the GNU/GPL License\n\n");
    return;
}


/*
 * Function:    loadConfig
 * -----------------------
 * date: 09jan2020
 * explicação
 * 
 * argc:
 * argv:
 *
 * return:
 *
 */
void loadConfig(char configFile[], char *argv[])
{
    char* tmp;

    // open config file
    printf("   Opening %s file...", configFile);    
    ini_start(configFile);
    if (get_last_error())
    {
        printf("\033[1;31m"); // red
        printf("[ERROR]\n");
        printf("\033[0m");
        // close 
        shutdown(0, argv);
    } else {
        printf("\033[1;32m"); //green
        printf("[OK]\n");
        printf("\033[0m");
        sleep(tm);
        // get maximum number of gateways
    	tmp = get_value("Main","MAXIMUM GW");
        max_gw = atoi(tmp);
        printf("      Number of Gateways available: %i\n", gw);
        sleep(tm);
        printf("      Number of Gateways selected: %i\n",max_gw);
        sleep(tm);
        // get gateway list file name
    	gwFile = get_value("Main","GW");
    	printf("      Gateways file configuration: %s\n",gwFile);

        sleep(tm);

        // get maximum number of devices
    	tmp = get_value("Main","MAXIMUM DEV");
        max_dev = atoi(tmp);	
        printf("      Number of Devices available: %i\n", max_dev);
        sleep(tm);
        printf("      Number of Devices selected: %i\n",dev);
        sleep(tm);
        // get devices list file name
    	devFile = get_value("Main","DEV");
    	printf("      Devices file configuration: %s\n",devFile);
        sleep(tm);
    	//save_as("configFile.ini");
    	//printf("last error is %s\n",get_last_error());
    	// print_content();	
    	ini_end();
        printf("\033[1;32m"); //green
        printf("   %s file loaded... [OK]\n\n", configFile);
        printf("\033[0m");
        sleep(tm);
    }
    return;
}

/*
 * Function:    loadGW
 * -------------------
 * date: 09jan2020
 * explicação
 * 
 * argc:
 * argv:
 *
 * return:
 *
 */
void loadGW(char *argv[])
{
    int ct=1, rand_num=0;
    char* tmp;

    // open gw config file
    printf("   Opening %s file...", gwFile);    
    ini_start(gwFile);
    if (get_last_error())
    {
        printf("\033[1;31m"); // red
        printf("[ERROR]\n");
        printf("\033[0m");
        // close 
        shutdown(0, argv);
    } else {
        printf("\033[1;32m"); //green
        printf("[OK]\n");
        printf("\033[0m");
        sleep(tm);
        printf("   Loading gateways...\n");
        while (ct != (gw + 1))
        {
            rand_num = generateRandoms(1, gw, 1);
            snprintf(tmp, 10, "%d", rand_num);
            ini_start(gwFile); // open config file
	        tmp = get_value(tmp,"NAME"); // get gateway list file name
	        ini_end();
            printf("\r      N.º%i/%i gateway:%i with name:%s", ct, gw, rand_num, tmp);
            sleep(tm);
            fflush(stdout);
            ct++;
        }
        printf("\n");
        printf("      Loaded %i gateways into the memory.\n", gw); 
        sleep(tm);
        printf("\033[1;32m"); //green
        printf("   %s file loaded... [OK]\n\n", gwFile);
        printf("\033[0m");
        sleep(tm);
    }
    return;
}

/*
 * Function:    loadDev
 * --------------------
 * date: 09jan2020
 * explicação
 * 
 * argc:
 * argv:
 *
 * return:
 *
 */
void loadDev(char *argv[])
{
    int ct=1, rand_num=0;
    char* tmp;

    // open gw config file
    printf("   Opening %s file...", devFile);    
    ini_start(devFile);
    if (get_last_error())
    {
        printf("\033[1;31m"); // red
        printf("[ERROR]\n");
        printf("\033[0m");
        // close 
        shutdown(0, argv);;
    } else {
        printf("\033[1;32m"); //green
        printf("[OK]\n");
        printf("\033[0m");
        sleep(tm);
        printf("   Loading devices...\n");
        while (ct != (dev + 1))
        {
            rand_num = generateRandoms(1, dev, 1);
            snprintf(tmp, 10, "%d", rand_num);       
            ini_start(devFile); // open config file
	        tmp = get_value(tmp,"NAME"); // get gateway list file name
	        ini_end();
            printf("\r      N.º%i/%i device:%i with name:%s", ct, dev, rand_num, tmp);
            sleep(tm);
            fflush(stdout);
            ct++;
        }
        printf("\n");
        printf("      Loaded %i devices into the memory.\n", dev); 
        sleep(tm);
        printf("\033[1;32m"); //green
        printf("   %s file loaded... [OK]\n\n", devFile);
        printf("\033[0m");
        sleep(tm);
    }
    return;
}


/*
 * Function:    startEmu
 * --------------------
 * date: 09jan2020
 * explicação
 * 
 * argc:
 * argv:
 *
 * return:
 *
 */
void startEmu()
{
    printf("\033[1;32m");
    printf ("   Start emulation.\n");
    printf("\033[0m");
    return;
}


/*
 * Function:    generateRandoms
 * --------------------------
 * date: 09jan2020
 * explicação
 * 
 * argc:
 * argv:
 *
 * return:
 *
 */
int generateRandoms(int lower, int upper, int count)
{
   int i, rand_num;
   for (i = 0; i < count; i++) {
      rand_num = (rand() % (upper - lower + 1)) + lower;
      //printf("%d ", rand_num);
   }
   return rand_num;
}

/*
 * Function:    shutdown
 * ---------------------
 * date: 09jan2020
 * explicação
 * 
 * argc:
 * argv:
 *
 * return:
 *
 */
void shutdown(int check, char *argv[]){
    if (check == 0){
        printf("\n%s closed.\n\n", argv[0]);
    } else {
        printf("\033[1;31m");
        printf("\n%s abnormally closed.\n\n", argv[0]);
        printf("\033[0m");
    }
    exit (1);
}

