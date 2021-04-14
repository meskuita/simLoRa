#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "ini.c"
#define MAXCHAR 1000

int checkArg(int argc, char *argv[]);
int rndDevice(int lower, int total_dev);
void delay(int number_seconds);
int readConfigFile(const char *fileName);
int readGwConfig(int gw, const char *filename);
int readDevConfig(int dev, const char *fileName);
// char* gen_payload(int dev);
const char* gen_payload(int dev);
int saveDevPayload(int dev, char fileName[], char payload[]);
void checkOkError(int check, char *argv[], int delayScreen);

void finished(int check, char *argv[]);
void process_field(int field_count, char *value);


int main( int argc, char *argv[] ){

    int done, check = 0;
    int gw, dev, max_time, cache_dev =0;
    int counter, one_dev = 0;
    int delayScreen = 10;
    char configFile[] = "configFile.ini";
    //char gwFile[] = "gwConfig.ini"; y  
    //char devFile[] = "devConfig.ini";
    char payload[255];

    char buf[1024];
    char token[1024];

    int row_count = 0;
    int field_count = 0;
    int in_double_quotes = 0;
    int token_pos = 0;
    int i = 0;


    // emulator information
    printf("\e[1;1H\e[2J");
    printf("Running %s...\n\n", argv[0]);
    printf("LPWAN LoraWAN Network Emulator\n");
    printf("version 1.0\n");
    printf("by Rui Mesquita\n");
    printf("Free Software released under the GNU/GPL License\n\n");
    delay(delayScreen);

    // checking argument
    if (checkArg(argc, argv)==1){;

        srand(time(0));
        gw = atoi(argv[1]); // number of gateways
        dev = atoi(argv[2]); // number of devices
        max_time = atoi(argv[3]); // time between transmissions

        // check for config file
        printf("   Loading %s file...", configFile);
        check = readConfigFile(configFile);
        checkOkError(check, argv, delayScreen);
        delay(delayScreen);
        // load ini file into memory
        ini_t *config = ini_load(configFile);
        const char *gwFile = ini_get(config, "gateways", "file");
        // if (gwFile) printf("gwFile: %s\n\n", gwFile);
        const char *devFile = ini_get(config, "devices", "file");
        // if (devFile) printf("devFile: %s\n\n", devFile);
    
        // unload ini file from memory
        //ini_free(config);

        // show information
        // number of gatewys
        printf("   Number of Gateways: %i\n", gw);
        delay(delayScreen);

        // gateways configuration filename
        printf("   Gateways file configuration: %s\n", gwFile);
        delay(delayScreen);

        // number of devices
        printf("   Number of Devices: %i\n", dev);
        delay(delayScreen);

        // devices configuration filename
        printf("   Device file configuration: %s\n", devFile);
        delay(delayScreen);

        // time between transmissions (minutes)
        printf("   Payload timeframe: %i minutes\n", max_time);
        delay(delayScreen);

        //load x gateway number to memory
        FILE *fp = fopen(gwFile, "r");
        //FILE *fp = fopen("c.csv", "r");
        if (!fp) {
            // check for config file
            printf("   Could not open file %s... ", gwFile);
            check = 1;
            checkOkError(check, argv, delayScreen);
            delay(delayScreen);
            // return 0;
        }
        while (fgets(buf, 1024, fp)) {
            row_count++;
// printf ("%i\n", row_count);
            if (row_count == 1) {

                continue;
            }
       field_count = 0;
        i = 0;
        do {
            token[token_pos++] = buf[i];

            if (!in_double_quotes && (buf[i] == ',' || buf[i] == '\n')) {
                token[token_pos - 1] = 0;
                token_pos = 0;
                process_field(field_count++, token);
            }

            if (buf[i] == '"' && buf[i + 1] != '"') {
                token_pos--;
                in_double_quotes = !in_double_quotes;
            }

            if (buf[i] == '"' && buf[i + 1] == '"')
                i++;

        } while (buf[++i]);

        printf("$$$\n");            
        }
        fclose(fp);






        printf("end\n");

        printf("   Loading %s file... ", gwFile);
        check = readGwConfig(gw, gwFile);        
        checkOkError(check, argv, delayScreen);
        delay(delayScreen);

        printf("   Loading %s file... ", devFile);
        check = readDevConfig(dev, devFile);        
        checkOkError(check, argv, delayScreen);
         delay(delayScreen);



        printf("\033[1;32m");
        printf ("   Start emulation.\n");
        printf("\033[0m");

        done= 0;
        // printf ("%i\n", done);

        while (!done){

 
        }
        finished(0, argv);
    } else {
        finished(0, argv);
    }
}


int checkArg(int argc, char *argv[]){
    if ( argc == 4) {
        printf("\033[1;32m");
        printf ("   Arguments OK!\n");
        printf("\033[0m");
        return 1;
    } else if ( argc > 4 ) {
        printf("\033[1;31m");
        printf("    Too many arguments supplied.\n");
        printf("\033[0m");
        printf("    %s: ", argv[0]); 
        printf ("modo de uso: ./teste1 [gw number] [devices number] [max time].\n");
        printf("\n");
        return 0;
    } else {
        printf("\033[1;31m");
        printf("    Missing arguments.\n");
        printf("\033[0m");
        printf("    %s: ", argv[0]); 
        printf ("modo de uso: ./teste1 [gw number] [devices number] [max time].\n");
        printf("\n");
        return 0;
    }
}

int rndDevice(int lower, int total_dev){
    int out;    
    out = ( rand() % (total_dev -lower +1))+lower;
    return out;
}

void delay(int number_seconds){
    int milli_seconds = 100000 * number_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}
/*
 *
 */


/*
 *
 *
 *
 *
 */
int readConfigFile(const char *fileName){

    FILE *fp;
    char str[MAXCHAR];
    //char* filename = fileName; //"test.txt";
 
    fp = fopen(fileName, "r");
    if (fp == NULL){
        printf("   Could not open file ");
        return 1;
    }
/*    while (fgets(str, MAXCHAR, fp) != NULL)
        // printf("%s", str); */
    fclose(fp);
    return 0;
}


/*
 *
 *
 *
 *
 */
int readGwConfig(int gw, const char *fileName){

    FILE *fp;
    char str[MAXCHAR];
    // char* filename = fileName; //"test.txt";
 
    fp = fopen(fileName, "r");
    if (fp == NULL){
        printf("   Could not open file ");
        return 1;
    }
/*    while (fgets(str, MAXCHAR, fp) != NULL)
        // printf("%s", str); */
    fclose(fp);
    return 0;
}

int readDevConfig(int dev, const char *fileName){

    FILE *fp;
    char str[MAXCHAR];
    // char* filename = fileName; //"test.txt";
 
    fp = fopen(fileName, "r");
    if (fp == NULL){
        printf("   Could not open file ");
        return 1;
    }
/*    while (fgets(str, MAXCHAR, fp) != NULL)
        // printf("%s", str); */
    fclose(fp);
    return 0;
}




const char* gen_payload(int dev){

    static char pt[] = "Id,Mote,Hora,Seq,Port,Radio,Canal,SNR (dB), RSSI (dBm),Freq,Mod,Taxadados,CR,Cargautil\n";
    static char uk[] = "Mote,Time,Seq,Port,Radio,Channel,SNR (dB), RSSI (dBm),Freq,Mod,Datarate,CR,Payload\n";
    static char fr[] = "Grain,Temps,Seq,Port,Radio,Canal,SNR (dB), RSSI (dBm),Freq,Mod,Debit,CR,Chargeutile\n";
    static char l1[255];    
    static char l2[] = "477,70-b3-d5-a9-f0-00-01-5d,2017-07-12 13:38:53,203,1,0,0,10,-91,868.1,LoRa,SF8BW125,4/5,02da02000007";


    // Language
    int out = ( rand() % (3 -1 +1))+1;
    switch (out){
        case 1: // PT
            strcpy (l1, pt);
            break;
        case 2: // UK
            strcpy(l1, uk);
            break;
        case 3: // FR
            strcpy(l1, fr);
            break;
    }
    strcat(l1, l2);

    // printf("%i\n", out);
    // printf("%s\n", l1);

    return l1;

}


int saveDevPayload(int dev, char fileName[], char payload[]){

    FILE *fp;
    time_t timer;
    char ext[] = ".csv";
    char path[] = "ftp/";    
    char name[] = "dev_teste";
    char buffer[26];
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y%m%d%H%M%S", tm_info);
    // puts(buffer);

    strcat(path, name);
    strcat(path, buffer);
    strcat(path, ext);
 
    printf("device #%i in the ", dev);
    printf("file %s ", path);

/*
printf("ext: %s\n", ext);
printf("fileName: %s\n", fileName);
printf("payload: %s\n", payload);
*/
    fp = fopen(path, "w+");
    if (fp == NULL){
        printf("   Could not write file ");
        return 1;
    }
    fputs ("teste\n",fp);

/*    while (fgets(str, MAXCHAR, fp) != NULL)
        // printf("%s", str);
*/
    fclose(fp);
    return 0;

}

void checkOkError(int check, char *argv[], int delayScreen){
    if (check == 0){
        delay(delayScreen);
        printf("\033[1;32m");
        printf("[OK]\n");
        printf("\033[0m");
    } else {
        delay(delayScreen);
        delay(delayScreen);
        printf("\033[1;31m");
        printf("[ERROR]\n");
        printf("\033[0m");
        finished(1, argv);
    }
}


void finished(int check, char *argv[]){
    if (check == 0){
        printf("\n%s finished.\n\n", argv[0]);
    } else {
        printf("\033[1;31m");
        printf("\n%s abnormally finished.\n\n", argv[0]);
        printf("\033[0m");
    }
    exit (1);
}

void process_field(int field_count, char *value) {
    printf("%s\n", value);
    if (field_count == 0) {
        printf("First Name:\t");
    }
    if (field_count == 1) {
        printf("Last Name:\t");
    }
    if (field_count == 2) {
            printf("Phone Number:\t");
    }
    if (field_count == 3) {
        printf("Job Title:\t");
    }
    printf("%s\n", value);
}
