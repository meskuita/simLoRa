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

#include <sys/stat.h>
#include <sys/types.h>

#include "ini_manage.c"

struct gateways
{
    int id;
    char name[20];
    int coorLat;
    int coorLon;
    int coorAlt;
    float marks;
};

struct devices
{
    int id;    
    char name[20];
    int roll_no;
    float marks;
};

// Variable declaration
struct gateways arr_gw[20];
struct devices arr_dev[30];

int gw, dev, max_gw, max_dev, max_time;
int tm=1;
char configFile[] = "configFile.ini";
char *gwFile;  
char *devFile;
char *type;
char arrayType[4][6] =
    { "ran",
      "raz",
      "geo",
      "qos"
    };

// Functions declaration
int checkArg(int argc, char *argv[]);
void printInfoArgv();
int checkType(char *type, char *argv[]);
void postMessage(int msg, char *argv[]);
void emuLoRa_info(char *argv[]);
void loadConfig(char configFile[], char *argv[]);
void loadGW(char *argv[]);
void loadDev(char *argv[]);
void startEmu(char *argv[]);
const char* payloadFormat(char *format);
int generateRandoms(int lower, int upper, int count);
void shutdown(int check, char *argv[]);


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
int main( int argc, char *argv[] ){

    // emuLoRa information
    emuLoRa_info(argv);

    // checking argument
    if (checkArg(argc, argv)==0){;

        gw = atoi(argv[1]); // number of gateways
        dev = atoi(argv[2]); // number of devices
        max_time = atoi(argv[3]); // time between transmissions
        type = argv[4]; // connection type

        // check valid type
        if (checkType(type, argv)!=0)
        {
            // close 
            shutdown(0, argv);
        }
        // print argv info
        printInfoArgv();
        
        // load config file
        loadConfig(configFile, argv);

        // load gateways list file
        loadGW(argv);

        // load devices list file
        loadDev(argv);

        // start emulation
        startEmu(argv);

    } else {
        // close 
        shutdown(0, argv);
    }
}
/*************************************************************************
 * end main function                                                     *
 *************************************************************************/


/*************************************************************************
 * Function:    emuLoRa_info                                             *
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
void emuLoRa_info(char *argv[])
{
    // emulator information
    printf("\e[1;1H\e[2J"); // clear screen
    printf("Running %s...\n\n", argv[0]);
    postMessage(0, argv); // emuLoRa infos
    /*
    for (int i = 0; i < 4; i++)
    {
        printf("'%s' has length \n", arrayType[i]); //, strlen(arrayType[i]));
    }
    */   
    return;
}
/*************************************************************************
 * end printInfoArgv function                                            *
 *************************************************************************/


/*************************************************************************
 * Function:    checkArg
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
int checkArg(int argc, char *argv[])
{
    if ( argc == 5) { // arguments ok
        postMessage(1, argv); 
        return 0;
    } else if ( argc > 5 ) { // too many arguments
        postMessage(2, argv); 
        return 1;
    } else { // missing arguments
        postMessage(3, argv); 
        return 1;
    }
}
/*************************************************************************
 * end checkArg function                                                 *
 *************************************************************************/


/*************************************************************************
 * Function:    checkType
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
int checkType(char *type, char *argv[])
{
    if (strcmp(type, arrayType[0]) == 0) // any random gateway numbers
    {
        return 0;
    }
    else if (strcmp(type, arrayType[1]) == 0) // don't repeat random gateway numbers
    {
        return 0;
    }
    else if (strcmp(type, arrayType[2]) == 0) // best geographic gateway
    {
        return 0;
    }
    else if (strcmp(type, arrayType[3]) == 0) // best QoS gateway
    {
        return 0;
    }
    else 
    {
        postMessage(4, argv);
        return 1;
    } 
}
/*************************************************************************
 * end checkType function                                                *
 *************************************************************************/


/*************************************************************************
 * Function:    printInfoArgv
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
void printInfoArgv()
{
    // info from argument
    printf("Number of gateways: %i\n", gw);        
    printf("Number of devices: %i\n", dev);
    printf("Time between transmissions: %isec\n", max_time);
    printf("Connection type: [%s] ", type);
    if (strcmp(type, arrayType[0]) == 0) printf("Random method\n"); // ran
    if (strcmp(type, arrayType[1]) == 0)printf("Don't repeat random method\n"); // raz
    if (strcmp(type, arrayType[2]) == 0)printf("Geographical method\n"); // geo
    if (strcmp(type, arrayType[3]) == 0)printf("Quality of the signal method\n"); // qos
    printf("\n");
}
/*************************************************************************
 * end printInfoArgv function                                            *
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
void loadConfig(char configFile[], char *argv[])
{
    char* tmp;

    // open config file
    printf("   Opening %s file...", configFile);    
    ini_start(configFile);
    if (get_last_error())
    {
        postMessage(6, argv); // red error
        // close 
        shutdown(0, argv);
    } else {
        postMessage(5, argv); // green ok
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


/*************************************************************************
 * Function:    loadGW
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
void loadGW(char *argv[])
{
    int ct=1, rand_num=0;
    char* tmp;

    // open gw config file
    printf("   Opening %s file...", gwFile);    
    ini_start(gwFile);
    if (get_last_error())
    {
        postMessage(6, argv); // red error
        // close 
        shutdown(0, argv);
    } else {
        postMessage(5, argv); // green ok
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
            arr_gw[ct].id = ct; // set id GW in the array

            // strcpy(arr[ct-1], tmp);
            // printf("-> %s\n", arr[ct-1]);
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
/*************************************************************************
 * end loadGW function                                                   *
 *************************************************************************/


/*************************************************************************
 * Function:    loadDev                                                  *
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
void loadDev(char *argv[])
{
    int ct=1, rand_num=0;
    char* tmp;

    // open gw config file
    printf("   Opening %s file...", devFile);    
    ini_start(devFile);
    if (get_last_error())
    {
        postMessage(6, argv); // red error
        // close 
        shutdown(0, argv);
    } else {
        postMessage(5, argv); // green ok
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
            arr_dev[ct].id = ct;
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
/*************************************************************************
 * end loadDev function                                                  *
 *************************************************************************/


/*************************************************************************
 * Function:    startEmu
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
void startEmu(char *argv[])
{
    int w, s, i, num;
    int rand_num, rand_dev, rand_tmp;  
    int rand_gw;  
    int ix=1;
    int mx=20;
    int anArray[2][mx]; /* random buffer array
                           [0][1][2][3][4][5]...[20]
                           [1]                       */
    time_t rawtime;
    char fname [50];
    char buffer [50];
    FILE *fptr;
    char *packet;

    // start emulation
    postMessage(7, argv);

    // gateways ready and listening
    printf ("   %i gateways ready and listening...\n", gw);    

    // infinite cycle
    while (true)
    {
        // get time
        time_t rawtime = time(NULL);
        struct tm *ptm = localtime(&rawtime);

        // random buffer array
        s=0;
        w=0;

        for (s=0; s<2; s++)
        {
            //printf("anArray[%i][%i]\n", s, w);
            for (w=0; w<(mx); w++)
            {  
                anArray[s][w] = 0;
                //printf("%i | ", anArray[s][w]);
            }
            //printf("\n");
        }
        //printf("\n\n");

        // cycle time in argv in seconds
        for (i=0; i<(max_time+1); i++)
        { 
            printf("\r      Cycle N.º %i | Time stamp: %02d:%02d:%02d | Seconds: %i", ix, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, i);
            //printf("\r");
            fflush(stdout);
            usleep(1000000); // 1 second
        }
        printf("\n");

        // uses time clock to seed
        srand(time(NULL));

        // random devices from availables half
        num = dev / 2;        
        rand_num = generateRandoms(1, num, 1);
   
     
/* ##########################################################################

     DEVICES  DEVICES  DEVICES  DEVICES  DEVICES  DEVICES  DEVICES  DEVICES

   ########################################################################## */

        /*
            to-do
            adicionar fator de erro de transmissão
        */

        // number of devices transmiting in the cycle
        printf ("      %i/%i device deployed...\n", rand_num, dev);
    
        // variable initialization
        rand_tmp = 0;
        rand_dev = 0;
        i=0;
        s=0;
        w=0;

        // uses time clock to seed
        srand(time(NULL));

        // transmitions cycle from random selected devices 
        while (i<(rand_num)) // number of devices transmiting in the cycle
        { 
            rand_dev = generateRandoms(1, dev, 1); // some device

            while (s<(mx)) // check for repeated devices
            {
                rand_tmp = anArray[0][s];
                // printf ("----  s:%i rand_dev:%i anArray[0][%i] ----\n", s, rand_tmp, rand_dev);
                if (rand_tmp == rand_dev)
                {
                    //printf("repetido ");
                    i--;
                    w=1;
                    break;   
                }
                s++;
            }
            if (w==0){
                // printf ("##############\n");
                anArray[0][i]=rand_dev;
                printf ("         [%i] device n.º%i transmitted...", i+1, anArray[0][i]);
                sleep(1);
                postMessage(5, argv); // green ok;
            }
            s=0;
            w=0;
            // printf ("      loading anArray[0][%i]=%i\n", i, rand_dev);
            i++;
        }



/*
            // print the array buffer
            for (w=0; w<(mx); w++)
            {
                //anArray[0][w] = 0;
                printf("%i | ", anArray[0][w]);
            }
            printf("\n\n");
            for (w=0; w<(mx); w++)
            {
                //anArray[1][w] = 0;
                printf("%i | ", anArray[1][w]);
            }
            printf("\n\n");
*/



        // transmitions available in air array
        printf("\n           ...transmissions on the air\n\n");

        // gateways availables in the network
        printf ("      Gateways [%i] network listening\n", gw);  


/* ##########################################################################

      GATEWAYS  GATEWAYS  GATEWAYS  GATEWAYS  GATEWAYS  GATEWAYS  GATEWAYS

   ########################################################################## */

        /*
            TO DO:
            - adicionar fator de erro
            - tipo de emparalhamento QoS, Distancia, aleatória no argV
              calculo da melhor distancia entre o gw e cada um dos dispositivos
        */

        // variable initialization
        i=0;
        s=0;
        w=0;
        num = gw;
        rand_gw=0;
        rand_tmp=0;

        // uses time clock to seed
        srand(time(NULL));

        // pairing with gateways availables in the network
        while (i<(rand_num))
        { 

            // printf("#############  type: %s   num:%i   randGw:%i\n", type, num, rand_gw);

            // any random gateway numbers
            if (strcmp(type, arrayType[0]) == 0) // ran
            {
                rand_gw = generateRandoms(1, gw, 1);
                anArray[1][i] = rand_gw;
                // printf("$$$$$$$$ RAN %i\n", rand_gw);
                // exit(0);
                printf ("         [%i] gateway n.º%i get device n.º%i...", i+1, anArray[1][i], anArray[0][i]);
                sleep(1);
                postMessage(5, argv); // green ok
            }

           // don't repeat random gateway numbers
            if (strcmp(type, arrayType[1]) == 0) // raz
            {
                // any random gateway 

                rand_gw = generateRandoms(1, gw, 1); // some device

                while (s<(mx)) // check for repeated devices
                {
                    rand_tmp = anArray[1][s];
                    // printf ("----  s:%i rand_dev:%i anArray[1][%i] ----\n", s, rand_tmp, rand_gw);
                    if (rand_tmp == rand_gw)
                    {
                        //printf("repetido ");
                        i--;
                        w=1;
                        break;   
                    }
                    s++;
                }
                if (w==0){
                    // printf ("##############\n");
                    anArray[1][i]=rand_gw;
                    printf ("         [%i] gateway n.º%i get device n.º%i...", i+1, anArray[1][i], anArray[0][i]);
                    sleep(1);
                    postMessage(5, argv); // green ok
                }
                s=0;
                w=0;
            }

           // best geographic gateway
            // printf("------> %s\n", arrayType[2]);
            if (strcmp(type, arrayType[2]) == 0) // geo
            {
                postMessage(8, argv); // stop application
                printf("      [%s] Geographical method connection type not implemented\n", arrayType[2]);
                printf("\n");
                // close 
                shutdown(0, argv);
            }

            // best QoS gateway
            // printf("------> %s\n", arrayType[3]);
            if (strcmp(type, arrayType[3]) == 0) // qos
            {
                postMessage(8, argv); // stop application
                printf("      [%s] Quality of the signal method connection type not implemented\n", arrayType[3]);
                printf("\n");
                // close 
                shutdown(0, argv);
            }
            // printf ("      loading array[1][%i]=%i\n", i, anArray[1][i]);
            i++;
            
        }

/* 
           // print the array buffer
            for (w=0; w<(mx); w++)
            {
                //anArray[0][w] = 0;
                printf("%i | ", anArray[0][w]);
            }
            printf("\n\n");
            for (w=0; w<(mx); w++)
            {
                //anArray[1][w] = 0;
                printf("%i | ", anArray[1][w]);
            }
            printf("\n\n");
*/

        // 
        printf("\n           ...saving payload in FTP\n\n");



/* ##########################################################################

    PACKETS/PAYLOAD

   ########################################################################## */

        /*
            TO DO:
            - adicionar fator de erro
            - criar payload em conformidade com o tipo de device
        */

        // variable initialization
        i=0;

        // check if directory exists
        int check = mkdir("ftp", 0777);
        if (!check) // if not then create
        {
            postMessage(9, argv); // directory not found
            sleep(1);
            postMessage(5, argv); // green ok
        }

        // saving payload files
        while (i<(rand_num))
        { 
            time (&rawtime);
            sprintf(fname,"payload_%s",ctime(&rawtime) );
            // Convert space into _
            char *p = fname;
            for (; *p; ++p)
            {
                if (*p == ' ')
                    *p = '_';
            }
            // remove the \n


            printf (" -------------------> %s", p);
            sprintf(buffer,"ftp/%s",fname);
            // printf ("\r         [%i] save %s file from device n.º%i through gateway n.ºx...", i+1, fname, anArray[i][0]);
            printf ("\r         [%i] saving file from device n.º%i through gateway n.º%i...", i+1, anArray[0][i], anArray[1][i]);
            //printf("%s",buffer);
            fptr = fopen(buffer,"w");

            // error
            if (fptr == NULL) {
                postMessage(6, argv); // red error       
            }


            // build packet payload format
            // strcmp(packet, payloadFormat("json"));

            fprintf(fptr, "%s", payloadFormat("json")); //PAYLOAD
            fclose(fptr);


            postMessage(5, argv); // green ok
            sleep(1);
            i++;        
        }

        printf("\n");
        ix++;    
    }
    return;
}
/*************************************************************************
 * end startEmu function                                                 *
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
void postMessage(int msg, char *argv[])
{
    switch (msg)
    {
        case 0: // emuLoRa infos
            printf("emuLoRa v1.0 build 2.0\n");
            printf("LPWAN LoRaWAN Network Emulator\n");
            printf("by Rui Mesquita\n");
            printf("Free Software released under the GNU/GPL License\n\n");
            break;
        case 1: // arguments ok
            printf("\033[1;32m");
            printf ("   Arguments OK!\n\n");
            printf("\033[0m");
            break;
        case 2: // too many arguments
            printf("\033[1;31m");
            printf("    Too many arguments supplied.\n");
            printf("\033[0m");
            printf("    modo de uso:\n");
            printf("    %s [gw number] [devices number] [max time] [connection type]\n\n", argv[0]);
            printf("        gw number: 1...n\n");
            printf("        device_number: 1...n \n");
            printf("        max time: 0...n in seconds\n");
            printf("        connection type: 'rad' 'raz' 'geo' 'qos'\n");
            printf("\n");
            break;
        case 3: // too many arguments
            printf("\033[1;31m");
            printf("    Missing arguments.\n");
            printf("\033[0m");
            printf("    modo de uso:\n");
            printf("    %s [gw number] [devices number] [max time] [connection type]\n\n", argv[0]);
            printf("        gw number: 1...n\n");
            printf("        device_number: 1...n \n");
            printf("        max time: 0...n in seconds\n");
            printf("        connection type: 'rad' 'raz' 'geo' 'qos'\n");
            printf("\n");
            break;
        case 4: // connection type unknown
            printf("\033[1;31m");
            printf("    Unknown connection type.\n");
            printf("\033[0m");
            printf("    modo de uso:\n");
            printf("    %s [gw number] [devices number] [max time] [connection type]\n\n", argv[0]);
            printf("        gw number: 1...n\n");
            printf("        device_number: 1...n \n");
            printf("        max time: 0...n in seconds\n");
            printf("        connection type: 'rad' 'raz' 'geo' 'qos'\n");
            printf("\n");
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
        case 7: // start emulation
            printf("\033[1;32m");
            printf ("   Start emulation.\n");
            printf("\033[0m");
            break;
        case 8: // stop application
            printf("\n");
            printf("\033[1;31m");
            printf("      %s stopped\n", argv[0]);
            printf("\033[0m");
            break;
         case 9: // directory not found
            printf("\033[1;31m"); // red            
            printf("           Directory not found!");
            printf("\033[0m"); 
            printf(" 'FTP' directory created... ");
            break;
        case 10: // abnormally closed
            printf("\033[1;31m");
            printf("\n%s abnormally closed.\n\n", argv[0]);
            printf("\033[0m");
            break;
    }
}
/*************************************************************************
 * end postMessage function                                              *
 *************************************************************************/


/*************************************************************************
 * Function:    generateRandoms
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
int generateRandoms(int lower, int upper, int count)
{
   int i, randNum;
   for (i = 0; i < count; i++) {
      randNum = (rand() % (upper - lower + 1)) + lower;
      //printf("%d ", randNum);
   }
   return randNum;
}
/*************************************************************************
 * end generateRandoms function                                          *
 *************************************************************************/


/*************************************************************************
 * Function:    payloadFormat  
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
const char* payloadFormat(char *format)
{
    if (strcmp(format, "json") == 0) // json
    {
        return "{"
                "\"gateway_eui\": {";





// "{\"gateway_eui\":\"FFFEB8227EBC09FC2\",\"data_raw\":\"QAnc0QKAEwWBYsjzRvd/9Q==\",\"data_plain\":\"177\",\"data_json\":177,\"Rssi\":-39,\"time\":\"2016-04-01T08:40:39.124Z\",\"datarate\":\"SF9BW125\",\"snr\":9.5,\"node_eui\":\"02D1DC09\",\"data\":\"MTc3\",\"frequency\":868.1}";
    }
    if (strcmp(format, "xml") == 0) // xml
    {
        return "{\"gateway_eui\":\"FFFEB8227EBC09FC2\",\"data_raw\":\"QAnc0QKAEwWBYsjzRvd/9Q==\",\"data_plain\":\"177\",\"data_json\":177,\"Rssi\":-39,\"time\":\"2016-04-01T08:40:39.124Z\",\"datarate\":\"SF9BW125\",\"snr\":9.5,\"node_eui\":\"02D1DC09\",\"data\":\"MTc3\",\"frequency\":868.1}";
    }
    if (strcmp(format, "csv") == 0) // csv
    {
        return "gateway_eui;FFFEB8227EBC09FC2;data_raw;QAnc0QKAEwWBYsjzRvd/9Q==;data_plain;177;data_json;177;Rssi;-39;time;2016-04-01T08:40:39.124Z;datarate;SF9BW125;snr;9.5;node_eui;02D1DC09;data;MTc3;frequency;868.1";
    }
    if (strcmp(format, "txt") == 0) // txt
    {
        return "gateway_eui FFFEB8227EBC09FC2   data_raw    QAnc0QKAEwWBYsjzRvd/9Q==    data_plain  177 data_json   177 Rssi    -39 time    2016-04-01T08:40:39.124Z    datarate    SF9BW125    snr 9.5 node_eui    02D1DC09    data    MTc3    frequency   868.1";
    }
/*

{"gateway_eui":

"FFFEB8227EBC09FC2",
"data_raw":"QAnc0QKAEwWBYsjzRvd/9Q==","data_plain":"177","data_json":
177,"Rssi":-39,"time":"2016-04-01T08:40:39.124Z",
"datarate":"SF9BW125","snr":9.5,"node_eui":"02D1DC09",
"data":"MTc3","frequency":868.1}


*/


}
/*************************************************************************
 * end payloadFormat function                                            *
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
void shutdown(int check, char *argv[]){
    if (check == 0){
        printf("\n%s closed.\n\n", argv[0]);
    } else {
        postMessage(10, argv); // abnormally closed
    }
    exit (1);
}
/*************************************************************************
 * end shutdown function                                                 *
 *************************************************************************/

