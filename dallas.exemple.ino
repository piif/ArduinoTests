///////////////////////////////////////////////////////////////////////////
//  Name   : ds12887.c                                                     //
//  Author : Timothy Reitmeyer                                           //
//  Notice:
//         :                                       //
//  Date   : 05-05-2005                                                  //
//  Version: 1.00                       //
/////////////////////////////////////////////////////////////////////////////

#define CLK_SECS     0
#define CLK_SECS_ALM 1
#define CLK_MINS     2
#define CLK_MINS_ALM 3
#define CLK_HRS      4
#define CLK_HRS_ALM  5
#define CLK_DOW      6
#define CLK_DOM      7
#define CLK_MON      8
#define CLK_YR       9
#define REGA         10
#define REGB         11
#define REGC         12
#define REGD         13
#define nvram_min    14
#define nvram_max    127

const char dow_text[8][4] =
{
	"---",
	"Mon",
	"Tue",
	"Wed",
	"Thr",
	"Fri",
	"Sat",
	"Sun"
};

const char month_text[13][4] = { "---", "Jan", "Feb", "Mar", "Apr", "May",
		"Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

struct rtc_pin_def {
	BOOLEAN cs_bar;
	BOOLEAN as;
	BOOLEAN rw_bar;
	BOOLEAN ds;
	BOOLEAN reset_bar;
	BOOLEAN irq_bar;
	BOOLEAN rclr_bar;
	BOOLEAN swq;
	int8 ad;
};

struct rtc_rega_struc {
	BOOLEAN RS0;
	BOOLEAN RS1;
	BOOLEAN RS2;
	BOOLEAN RS3;
	BOOLEAN DV0;
	BOOLEAN DV1;
	BOOLEAN DV2;
	BOOLEAN UIP;
} rega_var;
struct rtc_regb_struc {
	BOOLEAN DSE;
	BOOLEAN MIL;
	BOOLEAN DM;
	BOOLEAN SQWE;
	BOOLEAN UIE;
	BOOLEAN AIE;
	BOOLEAN PIE;
	BOOLEAN SET;
} regb_var; //0b00001100
struct rtc_regc_struc {
	BOOLEAN bit0;
	BOOLEAN bit1;
	BOOLEAN bit2;
	BOOLEAN bit3;
	BOOLEAN UF;
	BOOLEAN AF; //1=current time has matched the alarm time.
	BOOLEAN PF;
	BOOLEAN IRQF;
} regc_var;
struct rtc_regd_struc {
	BOOLEAN bit0;
	BOOLEAN bit1;
	BOOLEAN bit2;
	BOOLEAN bit3;
	BOOLEAN bit4;
	BOOLEAN bit5;
	BOOLEAN bit6;
	BOOLEAN VRT;
} regd_var;

struct rtc_pin_def rtc; //pins for the rtc
struct rtc_pin_def rtc_tris; //tris for the rtc
#byte rtc  = 0xF82      //note: no semicolin 0x82=C0 on a 18F452
#byte rtc_tris = 0xF94  //tris location for port C pin 0
#define rtc_tris_r() rtc_tris=0;rtc_tris.ad=0xFF//read data is input
#define rtc_tris_w() rtc_tris=0;rtc_tris.ad=0//write data is output

void init_rtc(void);
char read_rtc(char addr);
void write_rtc(char addr, char data);
void set_time(void);

void write_rtc(char addr, char data) {
	//C7=swq,6=rclr_bar,5=irq_bar,4=reset_bar,3=ds,2=rw_bar,1=as,0=cs_bar
	rtc_tris_w()
	;
	rtc.cs_bar = 0; //chip active
	rtc.ad = addr; //addr is on bus
	rtc.rw_bar = 0; //write mode
	rtc.ds = 0;    //data strob idle
	rtc.as = 1;    //addr strob
	delay_cycles(1); // pause
	rtc.as = 0;    //latch address
	rtc.ds = 1;    //data strob idle
	rtc.ad = data; //data is on bus
	delay_cycles(1); // pause
	rtc.ds = 0;    //latch data
	rtc_tris_r()
	; //set the tris of C  and D to ALL INPUTS
}

char read_rtc(char addr) {
	//C7=swq,6=rclr_bar,5=irq_bar,4=reset_bar,3=ds,2=rw_bar,1=as,0=cs_bar
	char data;
	rtc_tris = 0b11100000; //set the tris of C for setting address
	rtc_tris.ad = 0x00; //set the tris of D for setting address
	rtc = 0b00011110; //set C for for setting address
	rtc.ad = addr;      //put address on bus
#asm nop #endasm    //pause
	rtc.as = 0;           //latch
	delay_cycles(1); // pause
	rtc_tris.ad = 0xFF; //set the tris of D for reading data
	rtc.ds = 0;           //release
#asm nop #endasm    //pause
	data = rtc.ad;      //read the data from the bus
	rtc_tris_r()
	; //set the tris of B  and D to ALL INPUTS
	return (data);
}

void init_rtc() {
	rtc_tris.reset_bar = 0; //set the tris of C
	rtc.reset_bar = 0;     //reset
	delay_ms(200);     // delay the required time for reset
	rtc.reset_bar = 1;  //release
	rtc_tris_r()
	; //set the tris of C & D to ALL INPUTS
	while (rega_var.UIP)
		; //wait for update to finish
	regd_var = read_rtc(REGD);
	if (regd_var.VRT)
		fprintf(DEBUG, "Lithium battery OK\n\r");
	else
		fprintf(DEBUG, "Lithium battery dead\n\r");

	rega_var = 0;
	write_rtc(REGA, 0b11110100); //set reg A
	write_rtc(REGB, 0b11111100); //(msb) SET,PIE,AIE,UIE,SQWE,DM,MIL,DSE (lsb)
}

void set_time() {
	char yr, mn, dt, dy, hr, min, sec;
	char input[10];
	regb_var = read_rtc(REGB);
	if (regb_var.DM = 1) /* Binary data */
	{

		fprintf(DEBUG, "\nEnter the year (0-99): ");
		fgets(input, DEBUG); //scanf("%bd", &yr);
		yr = atoi(input);
		fprintf(DEBUG, "\n\rread %u\n\r", yr);

		fprintf(DEBUG, "Enter the month (1-12): ");
		fgets(input, DEBUG); // scanf("%bd", &mn);
		mn = atoi(input);
		fprintf(DEBUG, "\n\rread %u\n\r", mn);

		fprintf(DEBUG, "Enter the date (1-31): ");
		fgets(input, DEBUG); // scanf("%bd", &dt);
		dt = atoi(input);
		fprintf(DEBUG, "\n\rread %u\n\r", dt);

		fprintf(DEBUG, "Enter the day (1-7): ");
		fgets(input, DEBUG); // scanf("%bd", &dy);
		dy = atoi(input);
		fprintf(DEBUG, "\n\rread %u\n\r", dy);

		if (regb_var.MIL = 1) /* if 24 hour mode */
		{
			fprintf(DEBUG, "Enter the hour (1-23): ");
			fgets(input, DEBUG); //scanf("%bd", &hr);
			hr = atoi(input);
			fprintf(DEBUG, "\n\rread %u\n\r", hr);
		} else {
			fprintf(DEBUG, "Enter the hour (1-11): ");
			fgets(input, DEBUG); //scanf("%bd", &hr);
			hr = atoi(input);
			fprintf(DEBUG, "\n\rread %u\n\r", hr);
			fprintf(DEBUG, "A)M or P)M (A/P) ");
			fgets(input, DEBUG); //scanf("%1bs", &min);
			if (input == 'P' || input == 'p')
				hr |= 0x80; /* add PM indicator */
		}
		fprintf(DEBUG, "Enter the minute (0-59): ");
		fgets(input, DEBUG); //scanf("%bd", &min);
		min = atoi(input);
		fprintf(DEBUG, "\n\rread %u\n\r", min);
		fprintf(DEBUG, "Enter the second (0-59): ");
		fgets(input, DEBUG); //scanf("%bd", &sec);
		sec = atoi(input);
		fprintf(DEBUG, "\n\rread %u\n\r", sec);
	}

	regb_var.SET = 1; //  REGB |= 0x80; /* inhibit update while writing to clock */
	write_rtc(REGB, (int8) regb_var); //set reg B
	write_rtc(CLK_SECS, sec);
	write_rtc(CLK_MINS, min);
	write_rtc(CLK_HRS, hr);
	write_rtc(CLK_DOW, dy);
	write_rtc(CLK_DOM, dt);
	write_rtc(CLK_MON, mn);
	write_rtc(CLK_YR, yr);
	write_rtc(CLK_HRS_ALM, 0xFF);
	write_rtc(CLK_MINS_ALM, 0xFF);
	write_rtc(CLK_SECS_ALM, 0xFF);

	regb_var.SET = 0;
	write_rtc(REGB, (int8) regb_var); //set reg B
}
