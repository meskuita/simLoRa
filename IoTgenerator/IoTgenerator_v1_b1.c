/*************************************************************************
	created:	02/04/2021	
	created:	19:10:2005   13:54				
	filename: 	emuLoRa_v1_b1.c					
	file path:	.\IoTdriver						
	file base:	IoTdrive_v1_b1						
	file ext:	c						
	author:		Rui Mesquita					
																				
	purpose:	test program for ini_managemen

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
#include <string.h>

// Variable declaration
char configFile[] = "configFile.ini";
char *path;
int tm=1;

// IoTgenerator -gw|-dev 500 all -w|-a
char *device; // device type -gw | -dev
int number; // devices number
char *type; // type of device all | water | light
char *fname; // output namefile
char *fformat; // output file -w | -a




// functions declaration
void IoTgenerator_info(char *argv[]); //
void postMessage(int msg, char *argv[]); // IoTgenerator information
void loadConfig(char configFile[], char *argv[]); // load config file
int checkArg(int argc, char *argv[]);
void makeFiles(); //	
int generateRandoms(int lower, int upper, int count);



const char* iotName(char str[]); //
const char* iotLatitude(char str[]); //
const char* iotLongitude(char str[]); //
const char* iotAltitude(char str[]); //
const char* iotPlacement(char str[]); //
const char* iotBrand(char str[]); //
const char* iotModel(char str[]); //
const char* iotAntenna(char str[]); //
const char* iotSerial(char str[]); //
const char* iotPIB(char str[]); //
const char* iotCN(char str[]); //
const char* iotPN(char str[]); //
const char* iotSKU(); //

void shutdown(int check, char *argv[]); // close


/*************************************************************************
 * Function: main                                                        *
 * --------------------------------------------------------------------- *
 * date: 03abr2021                                                       *
 * explicação                                                            *
 *                                                                       *
 * argc:                                                                 *
 * argv:                                                                 *
 *                                                                       *
 * return:                                                               *
 *                                                                       *
 *************************************************************************/
int main( int argc, char *argv[] )
{

    char *apName="IoTgenerator_v1_b1";
    srand((unsigned int)time(NULL));
    
	// IoTgenerator information
	IoTgenerator_info(argv);

	// IoTgenerator -gw|-dev 500 all -w|-a

    // checking argument
    if (checkArg(argc, argv)==0){

		// show generation information
		printf ("Generating: ");
		if (strcmp(device, "-gw") == 0) printf("gateways\n");
		if (strcmp(device, "-dev") == 0) printf("devices\n");
		printf ("Number of devices: %i\n", number);
		if (strcmp(device, "-dev") == 0) printf ("Type of devices: %s\n", type);
		printf ("Output name file: %s\n", fname);
		printf ("File Type: ");
		if (strcmp(fformat, "-w") == 0) printf ("new\n");
		if (strcmp(fformat, "-a") == 0) printf ("appedings\n");
		printf("\n");

	    // load config file
	    loadConfig(configFile, argv);

	    makeFiles();
	    
	}
    // close
	shutdown(0, argv);
}
/************************************************************************
 * end main function                                                    *
 ************************************************************************/
 
 
 
 
 
 
/************************************************************************
* Function: IoTgenrator_info                                            *
* --------------------------------------------------------------------- *
* date: 03abr2021                                                       *
* explicação                                                            *
*                                                                       *
* argc:                                                                 *
* argv:                                                                 *
*                                                                       *
* return:                                                               *
*                                                                       *
*************************************************************************/
void IoTgenerator_info(char *argv[])
{
	// generator information
	printf("\e[1;1H\e[2J"); // clear screen
	printf("Running %s...\n\n", argv[0]);
	postMessage(0, argv); // IoTgenerator infos
	return;
}
/************************************************************************
 * end IoTgenerator_info function                                       *
 ************************************************************************/


/************************************************************************
* Function: makeFile                                                    *
* --------------------------------------------------------------------- *
* date: 03abr2021                                                       *
* explicação                                                            *
*                                                                       *
* argc:                                                                 *
* argv:                                                                 *
*                                                                       *
* return:                                                               *
*                                                                       *
*************************************************************************/
void makeFiles()
{
	int st = 1;
	char tmpName[20];
	char tmpBrand[20];
	FILE *f;
	char ch[256];
	int chr = '[';

	if (strcmp(fformat, "-a") == 0) { // append -a

	    printf("\033[0m");
    	printf("   Opening %s file... ", fname);
    	printf("\033[0m");
    	sleep(tm);
    	
    	f = fopen(fname, "r");

     	printf("\033[1;32m"); //green
    	printf("[OK]\n\n");
    	printf("\033[0m");
    	sleep(tm);	

	    printf("\033[0m");
    	printf("      Reading %s file:\n", fname);
    	printf("\033[0m");
    	sleep(tm);

    	while (fgets(ch, sizeof(ch), f)) {
	        //printf("%s", ch); 
	        if (strchr( ch, chr )) {
	        	st++;
	        	printf("\r         found %i.º device...", st);
	        	fflush(stdout);
				sleep(tm);
       			// printf("       ST:%i \n",st);
	        }
	    }

	    printf("\033[0m");
    	printf("\n      %s file ended. ", fname);
    	printf("\033[0m");
    	sleep(tm);
    	
	    fclose(f);

     	printf("\033[1;32m"); //green
    	printf("[OK]\n");
    	printf("\033[0m");
    	sleep(tm);	
	
	    f = fopen(fname, "a"); /* should check the result */

	} else if (strcmp(fformat, "-w") == 0) { // write -w
	    printf("\033[0m");
    	printf("   Creating %s file... ", fname);
    	printf("\033[0m");
    	sleep(tm);
    
		f = fopen(fname, "w");
		
    	printf("\033[1;32m"); //green
    	printf("[OK]\n\n");
    	printf("\033[0m");
    	sleep(tm);	
		
	} else {
		printf("ERRO no formato: %s\n", fformat);
		exit(1);
	}

	// printf("#5 ST:%i \n",st);


;
	
	for(int x = st; x < number + st; x++){
		if (f == NULL){
    		printf("Error opening file!\n");
    		exit(1);
		}

		/* print some text */
		const char *text = "#";


		//printf("device: %s\n", device);

		fprintf(f, "[%i]\n", x);

		if (type == NULL){
			fprintf(f, "NAME=gateway\n");
			strcpy(tmpName, "gateway");
		} else{
			strcpy(tmpName, iotName(type));
			if (strcmp(type, "all") == 0) {
				fprintf(f, "NAME=%s\n", tmpName);
			} else {
				fprintf(f, "NAME=%s\n", type);
			}
		}
		fprintf(f, "LATITUDE=%s\n", iotLatitude(device));
		fprintf(f, "LONGITUDE=%s\n", iotLongitude(device));
		//printf("############### %s\n", tmpName);
		fprintf(f, "ALTITUDE=%s\n", iotAltitude(tmpName));
		fprintf(f, "PLACEMENT=%s\n", iotPlacement(device));
		strcpy(tmpBrand, iotBrand(device));
		fprintf(f, "BRAND=%s\n", tmpBrand);
		fprintf(f, "MODEL=%s\n", iotModel(tmpBrand));
		fprintf(f, "PIB=%s\n", iotPIB(device));
		fprintf(f, "SN=%s\n", iotSerial(device));
		fprintf(f, "PN=%s\n", iotPN(device));
		fprintf(f, "CN=%s\n", iotCN(device));
		fprintf(f, "SKU=%s\n", iotSKU());
		fprintf(f, "ANTENNA=%s\n", iotModel(tmpBrand));
		fprintf(f, "ANTENNA_SERIAL=%s\n", iotSerial(device));
	
		printf("\r      Adding ");
		if (strcmp(device, "-gw") == 0) printf("gateway");
		if (strcmp(device, "-dev") == 0) printf("device");
		printf(" n.º%i/%i to the %s file...", x, number + st-1, fname);
		fflush(stdout);
		sleep(tm);
	}
	
	printf("\n\n");

    printf("\033[0m");
   	printf("   Closing %s file... ", fname);
   	printf("\033[0m");
   	sleep(tm);
    
	fclose(f);		

   	printf("\033[1;32m"); //green
   	printf("[OK]\n\n");
   	printf("\033[0m");
   	sleep(tm);	

   	printf("\033[1;32m"); //green
   	printf("   File %s successfully created [OK]\n\n", fname);
   	printf("\033[0m");
   	sleep(tm);	
   	
	return;
}
/************************************************************************
 * end buildFile function                                               *
 ************************************************************************/



const char* iotName(char str[]){

/*
[1] waste
[2] environment
[3] particle
[4] meter
[5] water
[6] buoy
[7] light
[8] watering
[9] people
[10] vehicle
[11] button
[12] level
[13] pet
[14] object
[15] park
[16] meteo
[17] uvs
[18] all
*/

	// printf("-------- %s\n\n", str);

    if (strcmp(str, "all") == 0) 
    {
		int num = generateRandoms(1, 17, 1);
    	if (num == 1) str = "waste";
    	if (num == 2) str = "environment";
    	if (num == 3) str = "particle";
    	if (num == 4) str = "meter";
    	if (num == 5) str = "water";
    	if (num == 6) str = "buoy";
    	if (num == 7) str = "light";
    	if (num == 8) str = "watering";
    	if (num == 9) str = "people";
    	if (num == 10) str = "vehicle";
    	if (num == 11) str = "button";
    	if (num == 12) str = "level";
    	if (num == 13) str = "pet";
    	if (num == 14) str = "object";
    	if (num == 15) str = "park";
    	if (num == 16) str = "meteo";
    	if (num == 17) str = "uvs";
    }
	return str;
}
/************************************************************************
 * end iotName function                                                 *
 ************************************************************************/
 
 
const char* iotLatitude(char str[]){
/*	char tmp[20];
	int num = generateRandoms(380138, 531369, 1);
	sprintf(tmp,"%d", num); // int to string
	return "kjk"; */
	
 
 	int  num, i;
 	char tmp[20];
	char lt[12]="";

	lt[0] = '3';
	lt[1] = '7';
	lt[2] = '.';
	
	num = generateRandoms(380138, 531369, 1);
	sprintf(tmp,"%d", num); // int to string
	strcat (lt, tmp);
 	str = lt;
	return str;
	// return "37.120497";
}
/************************************************************************
 * end iotLatitude function                                             *
 ************************************************************************/
 
 
const char* iotLongitude(char str[]){
 
 	int  num, i;
 	char tmp[20];
	char lo[12]="";

	lo[0] = '-';
	lo[1] = '8';
	lo[2] = '.';
	
	num = generateRandoms(375234, 531590, 1);
	sprintf(tmp,"%d", num); // int to string
	strcat (lo, tmp);
 	str = lo;
	return str;
	// return "37.120497";
}
/************************************************************************
 * end iotLongitude function                                            *
 ************************************************************************/
 
 
const char* iotAltitude(char str[]){
	int a1, a2;
	//printf("-iotAltitude-----------> %s\n", str);
	if (strcmp(str, "gateway") == 0) {
		a1 = 15;
		a2 = 50;
	} else if (strcmp(str, "waste") == 0) {
		a1 = -1;
		a2 = 3;	
	} else if (strcmp(str, "environment") == 0) {
		a1 = 3;
		a2 = 15;	
	} else if (strcmp(str, "particle") == 0) {
		a1 = 3;
		a2 = 25;	
	} else if (strcmp(str, "meter") == 0) {
		a1 = 3;
		a2 = 25;	
	} else if (strcmp(str, "water") == 0) {
		a1 = -5;
		a2 = 20;	
	} else if (strcmp(str, "buoy") == 0) {
		a1 = -5;
		a2 = 5;	
	} else if (strcmp(str, "watering") == 0) {
		a1 = -1;
		a2 = 2;	
	} else if (strcmp(str, "people") == 0) {
		a1 = 15;
		a2 = 50;	
	} else if (strcmp(str, "vehicle") == 0) {
		a1 = 15;
		a2 = 50;	
	} else if (strcmp(str, "button") == 0) {
		a1 = 15;
		a2 = 50;	
	} else if (strcmp(str, "level") == 0) {
		a1 = 15;
		a2 = 50;	
	} else if (strcmp(str, "pet") == 0) {
		a1 = 15;
		a2 = 50;	
	} else if (strcmp(str, "park") == 0) {
		a1 = 15;
		a2 = 50;	
	} else if (strcmp(str, "meteo") == 0) {
		a1 = 15;
		a2 = 50;	
	} else if (strcmp(str, "uvs") == 0) {
		a1 = 15;
		a2 = 50;	
	}
	int num = generateRandoms(a1, a2, 1);
	sprintf(str,"%d", num); // int to string
	return str;
}
/************************************************************************
 * end iotAltitude function                                             *
 ************************************************************************/
 
 
const char* iotPlacement(char str[]){
	if (strcmp(str, "-gw") == 0) {
		str = "outdoor";
	} else {
		int num = generateRandoms(1, 2, 1);
    	if (num == 1) str = "indoor";
    	if (num == 2) str = "outdoor";
	}
	return str;
}
/************************************************************************
 * end iotPlacement function                                            *
 ************************************************************************/
 
 
const char* iotBrand(char str[]){

/*
[1] Laird
[2] Kerlink
[3] Multitech
[4] Lorrier
[5] Lorix
[6] Lorank
[7] Aaeon
[8] Cisco
[9] Gemtek
[10] Tektelic
[11] LinkLabs
[12] Ursalink
[12] Robustel
[13] MikroTik
[14] RAK
[15] Seeed

[1] SensingLABS
[2] MCCI
[3] DecentLabs
[4] Ineo-sense
[5] RAK
[6] Seeed
[7] Strega
[8] netvox
[9] vaxcore
[10] Wvld
[11] xignal
[12] Yandel
[12] PI Suply
[13] digital matter
[14] AcSip
[15] SafeHouse
[16] Skysens
[17] Barani Design
[18] ELSYS.se
[19] Com-t-ac
[20] ellenex

*/ 
	
	//printf("iotBrand: %s\n", str);
	if (strcmp(str, "-gw") == 0) {
		int num = generateRandoms(1, 16, 1);
    	if (num == 1) str = "Laird";
    	if (num == 2) str = "Kerlink";
    	if (num == 3) str = "Multitech";
    	if (num == 4) str = "Lorrier";
    	if (num == 5) str = "Lorix";
    	if (num == 6) str = "Lorank";
    	if (num == 7) str = "Aaeon";
    	if (num == 8) str = "Cisco"; // cisco.com
    	if (num == 9) str = "Gemtek";
    	if (num == 10) str = "Tektelic";
    	if (num == 11) str = "LinkLabs";
    	if (num == 12) str = "Ursalink";
    	if (num == 13) str = "Robustel";
    	if (num == 14) str = "MikroTik"; // mikrotik.com
    	if (num == 15) str = "RAK"; // store.rakwireless.com
    	if (num == 16) str = "Seeed"; //
	} else {
		int num = generateRandoms(1, 20, 1);
    	if (num == 1) str = "SensingLABS";
    	if (num == 2) str = "MCCI";
    	if (num == 3) str = "DecentLabs";
    	if (num == 4) str = "Ineo-sense";
    	if (num == 5) str = "Seeed";
    	if (num == 6) str = "Strega";
    	if (num == 7) str = "netvox";
    	if (num == 8) str = "vaxcore";
    	if (num == 9) str = "Wvld";
    	if (num == 10) str = "xignal";
    	if (num == 11) str = "Yandel";
    	if (num == 12) str = "PI Suply";
    	if (num == 13) str = "digital matter";
    	if (num == 14) str = "AcSip";
    	if (num == 15) str = "SafeHouse";
    	if (num == 16) str = "Skysens";
    	if (num == 17) str = "Barani Design";
    	if (num == 18) str = "ELSYS.se";
    	if (num == 19) str = "Com-t-ac";
    	if (num == 20) str = "ellenex";
    	if (num == 20) str = "Cisco"; // cisco.com | insight.com
	}
	return str;
}
/************************************************************************
 * end iotBrand fuction                                                 *
 ************************************************************************/
 
 
const char* iotModel(char str[]){
	// printf("############# %s\n", str);
	if (strcmp(device, "-gw") == 0) { // GATEWAYS	
		if (strcmp(str, "Cisco") == 0) { // CISCO
    		str = "IXM-LPWA-800-16-K9";
		} else if (strcmp(str, "RAK") == 0) {// RAK
			int num = generateRandoms(1, 10, 1);
    		if (num == 1) str = "RAK7240";
    		if (num == 2) str = "RAK7249";
    		if (num == 3) str = "RAK7258";
    		if (num == 4) str = "RAK7243";
    		if (num == 5) str = "RAK7243C";
    		if (num == 6) str = "RAK7246";
    		if (num == 7) str = "RAK7246G";
    		if (num == 8) str = "RAK7244";
    		if (num == 9) str = "RAK7244C";
    		if (num == 10) str = "RAK7244P";
		} else if (strcmp(str, "MikroTik") == 0) { // MikroTik
			int num = generateRandoms(1, 4, 1);
    		if (num == 1) str = "R11e-LR8";
    		if (num == 2) str = "KNOT";
    		if (num == 3) str = "wAP LR8";
    		if (num == 4) str = "LtAP LR8 LTE";
		} else if (strcmp(str, "Seeed") == 0) { // Seeed
			int num = generateRandoms(1, 5, 1);
    		if (num == 1) str = "SenseCAP";
    		if (num == 2) str = "LPS8";
    		if (num == 3) str = "LG01-N";
    		if (num == 4) str = "OLG01-N";
	   		if (num == 5) str = "raspPI3";
		} else {
			str = "unknow";
		}
	} else { // DEVICES
		if (strcmp(str, "Cisco") == 0) { //CISCO
			int num = generateRandoms(1, 10, 1);
    		if (num == 1) str = "AV250";
    		if (num == 2) str = "AV202";
    		if (num == 3) str = "AV201";
    		if (num == 4) str = "AV207";
    		if (num == 5) str = "AV205";
    		if (num == 6) str = "AV206";
    		if (num == 7) str = "AV203";
    		if (num == 8) str = "AV204";
    		if (num == 9) str = "AV200";
    		if (num == 10) str = "AV300";
		} else if (strcmp(str, "Seeed") == 0) { // Seeed
			int num = generateRandoms(1, 9, 1);
    		if (num == 1) str = "Door Sensor";
    		if (num == 2) str = "GPS Traker";
    		if (num == 3) str = "Water Leak";
    		if (num == 4) str = "SenseCAP Soil";
	   		if (num == 5) str = "SenseCAP Air Temp";
    		if (num == 6) str = "SenseCAP Light";
    		if (num == 7) str = "SenseCAP CO2";
    		if (num == 8) str = "SenseCAP Pressure";
    		if (num == 9) str = "LHT65 temp";
		} else {
			str = "unknow";
		}
	}
	return str;
}
/************************************************************************
 * end iotModel fuction                                                 *
 ************************************************************************/


const char* iotAntenna(char str[]){
	int num;
	// printf("############# %s\n", str);
	if (strcmp(device, "-gw") == 0) { // GATEWAYS	
		if (strcmp(str, "Cisco") == 0) { // CISCO
    		str = "ANT-LPWA-DB-O-N-5";
		} else if (strcmp(str, "RAK") == 0) {// RAK
			int num = generateRandoms(1, 4, 1);
    		if (num == 1) str = "3dBi SubG";
    		if (num == 2) str = "2dBi SubG";
    		if (num == 3) str = "5.8dBi Fiberglass";
    		if (num == 4) str = "8dBi Fiberglass";
		} else if (strcmp(str, "MikroTik") == 0) { // MikroTik
    		if (num == 1) str = "TOF-0809-7V-S1";
		} else if (strcmp(str, "Seeed") == 0) { // Seeed
			int num = generateRandoms(1, 5, 1);
    		if (num == 1) str = "SenseCAP";
    		if (num == 2) str = "LPS8";
    		if (num == 3) str = "LG01-N";
    		if (num == 4) str = "OLG01-N";
	   		if (num == 5) str = "raspPI3";
		} else {
			str = "unknow";
		}
	} else { // DEVICES
		if (strcmp(str, "RAK") == 0) {// RAK
			int num = generateRandoms(1, 2, 1);
    		if (num == 1) str = "pcb";
    		if (num == 2) str = "3dBi Fiberglass";
		} else {
			str = "unknow";
		}
	}
	return str;
}
/************************************************************************
 * end iotAntenna fuction                                               *
 ************************************************************************/
 
 
 
 
/*************************************************************************
 * Function: iotSerial                                                   *
 * --------------------------------------------------------------------- *
 * date: 08abr2021                                                       *
 * explicação                                                            *
 *                                                                       *
 * argc:                                                                 *
 * argv:                                                                 *
 *                                                                       *
 * return:                                                               *
 *                                                                       *
 *************************************************************************/
 const char* iotSerial(char str[]){
/*
SN FTX1525849K
PN 341-0402-01
MAC ACA016236082
PID SRW2016-K9 V01
*/

	int i;
	char sn[10]="";

	for (i = 0; i < 3; i++) {
		sn[i] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" [random() % 26];
	}
	for (i = 3; i < 10; i++) {
		sn[i] = "1234567890" [random() % 10];
	}
	sn[9] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" [random() % 26];
	// printf("SN [%s]\n", sn);
 	str = sn;
	return str;
}
/************************************************************************
 * end iotSerial function                                               *
 ************************************************************************/


/*************************************************************************
 * Function: iotPIB                                                      *
 * --------------------------------------------------------------------- *
 * date: 08abr2021                                                       *
 * explicação                                                            *
 *                                                                       *
 * argc:                                                                 *
 * argv:                                                                 *
 *                                                                       *
 * return:                                                               *
 *                                                                       *
 *************************************************************************/
 const char* iotPIB(char str[]){
 
 	int i;
	char pib[15]="";

	for (i = 0; i < 2; i++) {
		pib[i] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" [random() % 26];
	}
	for (i = 2; i < 7; i++) {
		pib[i] = "1234567890" [random() % 10];
	}
	for (i = 7; i < 9; i++) {
		pib[i] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" [random() % 26];
	}
	for (i = 9; i < 15; i++) {
		pib[i] = "1234567890" [random() % 10];
	}
	
	// printf("PIB [%s]\n", pib);
 	str = pib;
	return str;
	
	// return "SG 675762FF232736";
}
/************************************************************************
 * end iotPIB function                                                  *
 ************************************************************************/
 
/*************************************************************************
 * Function: iotCN                                                       *
 * --------------------------------------------------------------------- *
 * date: 08abr2021                                                       *
 * explicação                                                            *
 *                                                                       *
 * argc:                                                                 *
 * argv:                                                                 *
 *                                                                       *
 * return:                                                               *
 *                                                                       *
 *************************************************************************/
 const char* iotCN(char str[]){
  
 	int i;
	char cn[18]="";

	for (i = 0; i < 3; i++) {
		cn[i] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" [random() % 26];
	}
	cn[3] = '-';
	for (i = 4; i < 7; i++) {
		cn[i] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" [random() % 26];
	}
	cn[7] = '-';
	for (i = 8; i < 11; i++) {
		cn[i] = "1234567890" [random() % 10];
	}
	cn[11] = '-';
	for (i = 12; i < 14; i++) {
		cn[i] = "1234567890" [random() % 10];
	}
	cn[14] = '-';
	cn[15] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" [random() % 26];
	cn[16] = "1234567890" [random() % 10];
	// printf("PIB [%s]\n", cn);
 	str = cn;
	return str;
	// return "IXM-LPWA-900-16-K9";
}
/************************************************************************
 * end iotCN function                                                   *
 ************************************************************************/


/*************************************************************************
 * Function: iotPN                                                       *
 * --------------------------------------------------------------------- *
 * date: 08abr2021                                                       *
 * explicação                                                            *
 *                                                                       *
 * argc:                                                                 *
 * argv:                                                                 *
 *                                                                       *
 * return:                                                               *
 *                                                                       *
 *************************************************************************/
 const char* iotPN(char str[]){
  
 	int i;
	char pn[12]="";

	for (i = 0; i < 2; i++) {
		pn[i] = "1234567890" [random() % 10];
	}
	pn[2] = '.';
	for (i = 3; i < 7; i++) {
		pn[i] = "1234567890" [random() % 10];
	}
	pn[7] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" [random() % 26];
	for (i = 8; i < 10; i++) {
		pn[i] = "1234567890" [random() % 10];
	}

	// printf("PIB [%s]\n", cn);
 	str = pn;
	return str;
	// 	return "95.1602T00";
}
/************************************************************************
 * end iotPN function                                                   *
 ************************************************************************/
 

/*************************************************************************
 * Function: iotSKU                                                      *
 * --------------------------------------------------------------------- *
 * date: 08abr2021                                                       *
 * explicação                                                            *
 *                                                                       *
 * argc:                                                                 *
 * argv:                                                                 *
 *                                                                       *
 * return:                                                               *
 *                                                                       *
 *************************************************************************/
const char* iotSKU(){
	return "868";
}
/************************************************************************
 * end iotSKU fuction                                                   *
 ************************************************************************/




/*************************************************************************
 * Function: generateRandoms                                             *
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
 * Function: checkArg    
 * --------------------------------------------------------------------- *
 * date: 03abr2021                                                       *
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
	int max = 0; // number of arguments

	if (argc > 1) { // have arguments

		device = argv[1]; // device type -gw | -dev


		// check device type
	    if (strcmp(device, "-gw") == 0) 
	    {
			max = 5;
	    } else if (strcmp(device, "-dev") == 0){
			max = 6;
	    } else {
	    	postMessage(3, argv); 
        	return 1;
	    }


		if ( argc == max) { // arguments ok
			postMessage(1, argv); 
		} else if ( argc > max ) { // too many arguments
			postMessage(2, argv); 
		} else if ( argc < max ) { // too missing arguments
			postMessage(3, argv); 
		}


		// IoTgenerator -gw|-dev 500 all -w|-a
        number = atoi(argv[2]); // number of devices
	    if (strcmp(device, "-gw") == 0) 
	    {
	        fname = argv[3]; // output namefile
	        fformat = argv[4]; // output file -w | -a
	    } else if (strcmp(device, "-dev") == 0){
	        type = argv[3]; // number of devices
	        fname = argv[4]; // output namefile
	        fformat = argv[5]; // output file -w | -a
	    }
		return 0;
		
	} else {
		printf("\nempty!\n");
    	postMessage(3, argv); 
       	return 1;
	}

}
/*************************************************************************
 * end checkArg function                                                 *
 *************************************************************************/
 
 
 
 
/*************************************************************************
 * Function: loadConfig   
 * --------------------------------------------------------------------- *
 * date: 03abr2021                                                       *
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
	char* apName;

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

/*    	tmp = get_value("Main","PATH");
        path = tmp;
        printf("      Package directory path: %s\n", path);
        sleep(tm);

    	// save_as("configFile.ini");
    	// printf("last error is %s\n",get_last_error());
    	// print_content();	
*/
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
* Function: postMessage                                                 *
* --------------------------------------------------------------------- *
* date: 03abr2021                                                       *
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
            printf("IoTgenerator v1.0 build 1.0\n");
            printf("LPWAN LoRaWAN IoT file generator\n");
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
        case 7: // 
            break;
        case 8: // 
            break;
         case 9: // 
            break;
        case 10: // 
            break;
    }
}
/************************************************************************
 * end postMessage function                                             *
 ************************************************************************/


/*************************************************************************
 * Function: shutdown                                                    *
 * --------------------------------------------------------------------- *
 * date: 03abr2021                                                       *
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
        // postMessage(10, apName); // abnormally closed
    }
    exit (1);
}
/*************************************************************************
 * end shutdown function                                                 *
 *************************************************************************/
