/* ----------------------------------------------------------------------------
 *         SAM Software Package License 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2014, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/**
 * \file
 *
 * Implementation WM8904 driver.
 *
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"
#include "wm8904.h"

/*----------------------------------------------------------------------------
 *        Type
 *----------------------------------------------------------------------------*/
typedef struct {
				uint16_t value;
				uint8_t address;
		}WM8904_PARA;

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/
/**
 * \brief Read data from WM8904 Register.
 *
 * \param pTwid   Pointer to twi driver structure
 * \param device  Twi slave address.
 * \param regAddr Register address to read.
 * \return value in the given register.
 */
uint16_t WM8904_Read(Twid *pTwid,
		uint32_t device,
		uint32_t regAddr)
{
	uint16_t bitsDataRegister;
	uint8_t Tdata[2]={0,0};

	TWID_Read(pTwid, device, regAddr, 1, Tdata, 2, 0);
	bitsDataRegister = (Tdata[0] << 8) | Tdata[1];
	return bitsDataRegister;
}

/**
 * \brief  Write data to WM8904 Register.
 *
 * \param pTwid   Pointer to twi driver structure
 * \param device  Twi slave address.
 * \param regAddr Register address to read.
 * \param data    Data to write
 */
void WM8904_Write(Twid *pTwid,
		uint32_t device,
		uint32_t regAddr,
		uint16_t data)
{
	uint8_t tmpData[2];

	tmpData[0] = (data & 0xff00) >> 8;
	tmpData[1] = data & 0xff;
	TWID_Write(pTwid, device, regAddr, 1, tmpData, 2, 0);
}

static WM8904_PARA wm8904_access_slow[]=
{ 
	{ 0x0000, 0},         /** R0   - SW Reset and ID */ 
	{ 0x001A, 4},         /** R4   - Bias Control 0 */ 
	{ 0x0047, 5},         /** R5   - VMID Control 0 */     /*insert_delay_ms 5*/

	{ 0x0043, 5},         /** R5   - VMID Control 0 */ 
	{ 0x000B, 4},         /** R4   - Bias Control 0 */ 

	{ 0x0003, 0x0C},      /** R12  - Power Management 0 CC */ 

	{ 0x0003, 0x0E},      /** R14  - Power Management 2 */ 
	{ 0x000C, 0x12},      /** R18  - Power Management 6 */
	{ 0x0000, 0x21},      /** R33  - DAC Digital 1 */ 
	{ 0x0000, 0x3D},      /** R61  - Analogue OUT12 ZC */ 
	{ 0x0001, 0x62},      /** R98  - Charge Pump 0 */ 
	{ 0x0005, 0x68},     /** R104 - Class W 0 */ 

	//FLL setting,32.768KHZ MCLK input,12.288M output.
	{ 0x0000, 0x74},     /** R116 - FLL Control 1 */ 
	{ 0x0704, 0x75},     /** R117 - FLL Control 2 */ 
	{ 0x8000, 0x76},     /** R118 - FLL Control 3 */ 
	{ 0x1760, 0x77},     /** R119 - FLL Control 4 */ 
	{ 0x0005, 0x74},     /** R116 - FLL Control 1 */     /*insert_delay_ms 5*/

	{ 0x0C05, 0x15},      /** R21  - Clock Rates 1 */ 
	{ 0x845E, 0x14},      /** R20  - Clock Rates 0 */     
	{ 0x4006, 0x16},      /** R22  - Clock Rates 2 */

	//WM8904 IIS master
	//BCLK=12.288MHz/8=1.536MHz
	//LRCK=1.536MHz/32=48KHz
	//{ 0x0042, 0x18},    /** R24  - Audio Interface 0 */ 
	{ 0x0042, 0x19},      /** R25  - Audio Interface 1 */ 
	{ 0x00E8, 0x1A},      /** R26  - Audio Interface 2 */ 
	{ 0x0820, 0x1B},      /** R27  - Audio Interface 3 */ 
	////////////////ADC

	{ 0x0003, 0x0C},      /** R12  - Power Management 0 */ 
	{ 0x000F, 0x12},      /** R18  - Power Management 6 */     /*insert_delay_ms 5*/

	{ 0x0010, 0x2C},      /** R44  - Analogue Left Input 0 */ 
	{ 0x0010, 0x2D},      /** R45  - Analogue Right Input 0 */ 
	{ 0x0044, 0x2E},      /** R46  - Analogue Left Input 1 */ 
	{ 0x0044, 0x2F},      /** R47  - Analogue Right Input 1 */

	{ 0x0011, 0x5A},      /** R90  - Analogue HP 0 */ 
	{ 0x0033, 0x5A},      /** R90  - Analogue HP 0 */ 

	{ 0x000F, 0x43},      /** R67  - DC Servo 0 */ 
	{ 0x00F0, 0x44},      /** R68  - DC Servo 1 */     /*insert_delay_ms 100*/

	{ 0x0077, 0x5A},      /** R90  - Analogue HP 0 */ 
	{ 0x00FF, 0x5A},      /** R90  - Analogue HP 0 */ 
	{ 0x00B9, 0x39},      /** R57  - Analogue OUT1 Left */ 
	{ 0x00B9, 0x3A},      /** R58  - Analogue OUT1 Right */  
};

static WM8904_PARA wm8904_access_main[] = 
{ 
//  { 0x8904, 0x00}, /** R0 - SW Reset and ID */
//  { 0x0000, 0x01}, /** R1 - Revision */
//  { 0x0000, 0x02}, /** R2 */
//  { 0x0000, 0x03}, /** R3 */
    { 0x0019, 0x04}, /** R4 - Bias Control 0 */
    { 0x0043, 0x05}, /** R5 - VMID Control 0 */
//  { 0x0003, 0x06}, /** R6 - Mic Bias Control 0 */
//  { 0xC000, 0x07}, /** R7 - Mic Bias Control 1 */
//  { 0x001E, 0x08}, /** R8 - Analogue DAC 0 */
//  { 0xFFFF, 0x09}, /** R9 - mic Filter Control */
//  { 0x0001, 0x0A}, /** R10 - Analogue ADC 0 */
//  { 0x0000, 0x0B}, /** R11 */
    { 0x0003, 0x0C}, /** R12 - Power Management 0 */
//  { 0x0000, 0x0D}, /** R13 */
    { 0x0003, 0x0E}, /** R14 - Power Management 2 */
//  { 0x0003, 0x0F}, /** R15 - Power Management 3 */
//  { 0x0000, 0x10}, /** R16 */
//  { 0x0000, 0x11}, /** R17 */
    { 0x000F, 0x12}, /** R18 - Power Management 6 */
//  { 0x0000, 0x13}, /** R19 */
    { 0x845E, 0x14}, /** R20 - Clock Rates 0 */
//  { 0x3C07, 0x15}, /** R21 - Clock Rates 1 */
    { 0x0006, 0x16}, /** R22 - Clock Rates 2 */
//  { 0x0000, 0x17}, /** R23 */
//  { 0x1FFF, 0x18}, /** R24 - Audio Interface 0 */
    { 0x404A, 0x19}, /** R25 - Audio Interface 1 */
//  { 0x0004, 0x1A}, /** R26 - Audio Interface 2 */
    { 0x0840, 0x1B}, /** R27 - Audio Interface 3 */
//  { 0x0000, 0x1C}, /** R28 */
//  { 0x0000, 0x1D}, /** R29 */
//  { 0x00FF, 0x1E}, /** R30 - DAC Digital Volume Left */
//  { 0x00FF, 0x1F}, /** R31 - DAC Digital Volume Right */
//  { 0x0FFF, 0x20}, /** R32 - DAC Digital 0 */
    { 0x0000, 0x21}, /** R33 - DAC Digital 1 */
//  { 0x0000, 0x22}, /** R34 */
//  { 0x0000, 0x23}, /** R35 */
//  { 0x00FF, 0x24}, /** R36 - ADC Digital Volume Left */
//  { 0x00FF, 0x25}, /** R37 - ADC Digital Volume Right */
//  { 0x0073, 0x26}, /** R38 - ADC Digital 0 */
//  { 0x1800, 0x27}, /** R39 - Digital Microphone 0 */
//  { 0xDFEF, 0x28}, /** R40 - DRC 0 */
//  { 0xFFFF, 0x29}, /** R41 - DRC 1 */
//  { 0x003F, 0x2A}, /** R42 - DRC 2 */
//  { 0x07FF, 0x2B}, /** R43 - DRC 3 */
    { 0x0005, 0x2C}, /** R44 - Analogue Left Input 0 */
    { 0x0005, 0x2D}, /** R45 - Analogue Right Input 0 */
    { 0x0000, 0x2E}, /** R46 - Analogue Left Input 1 */
    { 0x0000, 0x2F}, /** R47 - Analogue Right Input 1 */
//  { 0x0000, 0x30}, /** R48 */
//  { 0x0000, 0x31}, /** R49 */
//  { 0x0000, 0x32}, /** R50 */
//  { 0x0000, 0x33}, /** R51 */
//  { 0x0000, 0x34}, /** R52 */
//  { 0x0000, 0x35}, /** R53 */
//  { 0x0000, 0x36}, /** R54 */
//  { 0x0000, 0x37}, /** R55 */
//  { 0x0000, 0x38}, /** R56 */
//  { 0x017F, 0x39}, /** R57 - Analogue OUT1 Left */
    { 0x00AD, 0x3A}, /** R58 - Analogue OUT1 Right */
//  { 0x017F, 0x3B}, /** R59 - Analogue OUT2 Left */
//  { 0x017F, 0x3C}, /** R60 - Analogue OUT2 Right */
//  { 0x000F, 0x3D}, /** R61 - Analogue OUT12 ZC */
//  { 0x0000, 0x3E}, /** R62 */
//  { 0x0000, 0x3F}, /** R63 */
//  { 0x0000, 0x40}, /** R64 */
//  { 0x0000, 0x41}, /** R65 */
//  { 0x0000, 0x42}, /** R66 */
    { 0x0003, 0x43}, /** R67 - DC Servo 0 */
//  { 0xFFFF, 0x44}, /** R68 - DC Servo 1 */
//  { 0x0F0F, 0x45}, /** R69 - DC Servo 2 */
//  { 0x0000, 0x46}, /** R70 */
//  { 0x007F, 0x47}, /** R71 - DC Servo 4 */
//  { 0x007F, 0x48}, /** R72 - DC Servo 5 */
//  { 0x00FF, 0x49}, /** R73 - DC Servo 6 */
//  { 0x00FF, 0x4A}, /** R74 - DC Servo 7 */
//  { 0x00FF, 0x4B}, /** R75 - DC Servo 8 */
//  { 0x00FF, 0x4C}, /** R76 - DC Servo 9 */
//  { 0x0FFF, 0x4D}, /** R77 - DC Servo Readback 0 */
//  { 0x0000, 0x4E}, /** R78 */
//  { 0x0000, 0x4F}, /** R79 */
//  { 0x0000, 0x50}, /** R80 */
//  { 0x0000, 0x51}, /** R81 */
//  { 0x0000, 0x52}, /** R82 */
//  { 0x0000, 0x53}, /** R83 */
//  { 0x0000, 0x54}, /** R84 */
//  { 0x0000, 0x55}, /** R85 */
//  { 0x0000, 0x56}, /** R86 */
//  { 0x0000, 0x57}, /** R87 */
//  { 0x0000, 0x58}, /** R88 */
//  { 0x0000, 0x59}, /** R89 */
    { 0x00FF, 0x5A}, /** R90 - Analogue HP 0 */
//  { 0x0000, 0x5B}, /** R91 */
//  { 0x0000, 0x5C}, /** R92 */
//  { 0x0000, 0x5D}, /** R93 */
//  { 0x00FF, 0x5E}, /** R94 - Analogue Lineout 0 */
//  { 0x0000, 0x5F}, /** R95 */
//  { 0x0000, 0x60}, /** R96 */
//  { 0x0000, 0x61}, /** R97 */
    { 0x0001, 0x62}, /** R98 - Charge Pump 0 */
//  { 0x0000, 0x63}, /** R99 */
//  { 0x0000, 0x64}, /** R100 */
//  { 0x0000, 0x65}, /** R101 */
//  { 0x0000, 0x66}, /** R102 */
//  { 0x0000, 0x67}, /** R103 */
    { 0x0005, 0x68}, /** R104 - Class W 0 */
//  { 0x0000, 0x69}, /** R105 */
//  { 0x0000, 0x6A}, /** R106 */
//  { 0x0000, 0x6B}, /** R107 */
//  { 0x011F, 0x6C}, /** R108 - Write Sequencer 0 */
//  { 0x7FFF, 0x6D}, /** R109 - Write Sequencer 1 */
//  { 0x4FFF, 0x6E}, /** R110 - Write Sequencer 2 */
//  { 0x003F, 0x6F}, /** R111 - Write Sequencer 3 */
//  { 0x03F1, 0x70}, /** R112 - Write Sequencer 4 */
//  { 0x0000, 0x71}, /** R113 */
//  { 0x0000, 0x72}, /** R114 */
//  { 0x0000, 0x73}, /** R115 */
    { 0x0004, 0x74}, /** R116 - FLL Control 1 */
    { 0x0704, 0x75}, /** R117 - FLL Control 2 */
    { 0x8000, 0x76}, /** R118 - FLL Control 3 */
    { 0x1760, 0x77}, /** R119 - FLL Control 4 */
//  { 0x001B, 0x78}, /** R120 - FLL Control 5 */
//  { 0x0014, 0x79}, /** R121 - GPIO Control 1 */
//  { 0x0010, 0x7A}, /** R122 - GPIO Control 2 */
//  { 0x0010, 0x7B}, /** R123 - GPIO Control 3 */
//  { 0x0000, 0x7C}, /** R124 - GPIO Control 4 */
//  { 0x0000, 0x7D}, /** R125 */
//  { 0x000A, 0x7E}, /** R126 - Digital Pulls */
//  { 0x07FF, 0x7F}, /** R127 - Interrupt Status */
//  { 0x03FF, 0x80}, /** R128 - Interrupt Status Mask */
//  { 0x03FF, 0x81}, /** R129 - Interrupt Polarity */
//  { 0x03FF, 0x82}, /** R130 - Interrupt Debounce */
//  { 0x0000, 0x83}, /** R131 */
//  { 0x0000, 0x84}, /** R132 */
//  { 0x0000, 0x85}, /** R133 */
//  { 0x0001, 0x86}, /** R134 - EQ1 */
//  { 0x001F, 0x87}, /** R135 - EQ2 */
//  { 0x001F, 0x88}, /** R136 - EQ3 */
//  { 0x001F, 0x89}, /** R137 - EQ4 */
//  { 0x001F, 0x8A}, /** R138 - EQ5 */
//  { 0x001F, 0x8B}, /** R139 - EQ6 */
//  { 0xFFFF, 0x8C}, /** R140 - EQ7 */
//  { 0xFFFF, 0x8D}, /** R141 - EQ8 */
//  { 0xFFFF, 0x8E}, /** R142 - EQ9 */
//  { 0xFFFF, 0x8F}, /** R143 - EQ10 */
//  { 0xFFFF, 0x90}, /** R144 - EQ11 */
//  { 0xFFFF, 0x91}, /** R145 - EQ12 */
//  { 0xFFFF, 0x92}, /** R146 - EQ13 */
//  { 0xFFFF, 0x93}, /** R147 - EQ14 */
//  { 0xFFFF, 0x94}, /** R148 - EQ15 */
//  { 0xFFFF, 0x95}, /** R149 - EQ16 */
//  { 0xFFFF, 0x96}, /** R150 - EQ17 */
//  { 0xFFFF, 0x97}, /** R151wm8523_dai - EQ18 */
//  { 0xFFFF, 0x98}, /** R152 - EQ19 */
//  { 0xFFFF, 0x99}, /** R153 - EQ20 */
//  { 0xFFFF, 0x9A}, /** R154 - EQ21 */
//  { 0xFFFF, 0x9B}, /** R155 - EQ22 */
//  { 0xFFFF, 0x9C}, /** R156 - EQ23 */
//  { 0xFFFF, 0x9D}, /** R157 - EQ24 */
//  { 0x0000, 0x9E}, /** R158 */
//  { 0x0000, 0x9F}, /** R159 */
//  { 0x0000, 0xA0}, /** R160 */
//  { 0x0002, 0xA1}, /** R161 - Control Interface Test 1 */
//  { 0x0000, 0xA2}, /** R162 */
//  { 0x0000, 0xA3}, /** R163 */
//  { 0x0000, 0xA4}, /** R164 */
//  { 0x0000, 0xA5}, /** R165 */
//  { 0x0000, 0xA6}, /** R166 */
//  { 0x0000, 0xA7}, /** R167 */
//  { 0x0000, 0xA8}, /** R168 */
//  { 0x0000, 0xA9}, /** R169 */
//  { 0x0000, 0xAA}, /** R170 */
//  { 0x0000, 0xAB}, /** R171 */
//  { 0x0000, 0xAC}, /** R172 */
//  { 0x0000, 0xAD}, /** R173 */
//  { 0x0000, 0xAE}, /** R174 */
//  { 0x0000, 0xAF}, /** R175 */
//  { 0x0000, 0xB0}, /** R176 */
//  { 0x0000, 0xB1}, /** R177 */
//  { 0x0000, 0xB2}, /** R178 */
//  { 0x0000, 0xB3}, /** R179 */
//  { 0x0000, 0xB4}, /** R180 */
//  { 0x0000, 0xB5}, /** R181 */
//  { 0x0000, 0xB6}, /** R182 */
//  { 0x0000, 0xB7}, /** R183 */
//  { 0x0000, 0xB8}, /** R184 */
//  { 0x0000, 0xB9}, /** R185 */
//  { 0x0000, 0xBA}, /** R186 */
//  { 0x0000, 0xBB}, /** R187 */
//  { 0x0000, 0xBC}, /** R188 */
//  { 0x0000, 0xBD}, /** R189 */
//  { 0x0000, 0xBE}, /** R190 */
//  { 0x0000, 0xBF}, /** R191 */
//  { 0x0000, 0xC0}, /** R192 */
//  { 0x0000, 0xC1}, /** R193 */
//  { 0x0000, 0xC2}, /** R194 */
//  { 0x0000, 0xC3}, /** R195 */
//  { 0x0000, 0xC4}, /** R196 */
//  { 0x0000, 0xC5}, /** R197 */
//  { 0x0000, 0xC6}, /** R198 */
//  { 0x0000, 0xC7}, /** R199 */
//  { 0x0000, 0xC8}, /** R200 */
//  { 0x0000, 0xC9}, /** R201 */
//  { 0x0000, 0xCA}, /** R202 */
//  { 0x0000, 0xCB}, /** R203 */
//  { 0x0070, 0xCC}, /** R204 - Analogue Output Bias 0 */
//  { 0x0000, 0xCD}, /** R205 */
//  { 0x0000, 0xCE}, /** R206 */
//  { 0x0000, 0xCF}, /** R207 */
//  { 0x0000, 0xD0}, /** R208 */
//  { 0x0000, 0xD1}, /** R209 */
//  { 0x0000, 0xD2}, /** R210 */
//  { 0x0000, 0xD3}, /** R211 */
//  { 0x0000, 0xD4}, /** R212 */
//  { 0x0000, 0xD5}, /** R213 */
//  { 0x0000, 0xD6}, /** R214 */
//  { 0x0000, 0xD7}, /** R215 */
//  { 0x0000, 0xD8}, /** R216 */
//  { 0x0000, 0xD9}, /** R217 */
//  { 0x0000, 0xDA}, /** R218 */
//  { 0x0000, 0xDB}, /** R219 */
//  { 0x0000, 0xDC}, /** R220 */
//  { 0x0000, 0xDD}, /** R221 */
//  { 0x0000, 0xDE}, /** R222 */
//  { 0x0000, 0xDF}, /** R223 */
//  { 0x0000, 0xE0}, /** R224 */
//  { 0x0000, 0xE1}, /** R225 */
//  { 0x0000, 0xE2}, /** R226 */
//  { 0x0000, 0xE3}, /** R227 */
//  { 0x0000, 0xE4}, /** R228 */
//  { 0x0000, 0xE5}, /** R229 */
//  { 0x0000, 0xE6}, /** R230 */
//  { 0x0000, 0xE7}, /** R231 */
//  { 0x0000, 0xE8}, /** R232 */
//  { 0x0000, 0xE9}, /** R233 */
//  { 0x0000, 0xEA}, /** R234 */
//  { 0x0000, 0xEB}, /** R235 */
//  { 0x0000, 0xEC}, /** R236 */
//  { 0x0000, 0xED}, /** R237 */
//  { 0x0000, 0xEE}, /** R238 */
//  { 0x0000, 0xEF}, /** R239 */
//  { 0x0000, 0xF0}, /** R240 */
//  { 0x0000, 0xF1}, /** R241 */
//  { 0x0000, 0xF2}, /** R242 */
//  { 0x0000, 0xF3}, /** R243 */
//  { 0x0000, 0xF4}, /** R244 */
//  { 0x0000, 0xF5}, /** R245 */
//  { 0x0000, 0xF6}, /** R246 */
//  { 0x0000, 0xF7}, /** R247 - FLL NCO Test 0 */
//  { 0x0019, 0xF8}, /** R248 - FLL NCO Test 1 */
	{ 0x55AA, 255}  /** end */ 
};

uint8_t WM8904_Init(Twid *pTwid, uint32_t device,  uint32_t PCK)
{
	uint8_t count, size;
	uint16_t data = 0;

	// Reset (write Reg@0x0 to reset)
	WM8904_Write(pTwid, device, 0, 0xFFFF);

	for(data=0;data<1000;data++);
	//wait ready    
	while(data!=0x8904)
		data=WM8904_Read(pTwid, device, 0);

	if (PMC_MCKR_CSS_SLOW_CLK == PCK) {
		size = sizeof(wm8904_access_slow) / 4 + 1;
		for(count=0; count<size; count++) {
			WM8904_Write(pTwid, device, wm8904_access_slow[count].address,
							wm8904_access_slow[count].value);
			if(((wm8904_access_slow[count].address==0x05)
							&&(wm8904_access_slow[count].value==0x0047))
					||((wm8904_access_slow[count].address==0x74)
							&&(wm8904_access_slow[count].value==0x0005))
					||((wm8904_access_slow[count].address==0x12)
							&&(wm8904_access_slow[count].value==0x000F))) {
				Wait(5);
			}
			if (((wm8904_access_slow[count].address==0x44)
							&&(wm8904_access_slow[count].value==0x00F0))
					||((wm8904_access_slow[count].address==0x3A)
							&&(wm8904_access_slow[count].value==0x00B9))) {
				Wait(100);
			}
		} 
	}
	else if (PMC_MCKR_CSS_MAIN_CLK == PCK) {
		for(count = 0; count < 255; count++) {
			if(wm8904_access_main[count].address < 255) {
				WM8904_Write(pTwid, device, wm8904_access_main[count].address, 
								wm8904_access_main[count].value);
			} else {
				break;
			}
		}
	} else {
		printf("W: PCK not supported! \n\r");
		while(1);
	}
	return 0;
}

void WM8904_IN2R_IN1L(Twid *pTwid, uint32_t device)
{
	//{ 0x0005, 44},  /** R44  - Analogue Left Input 0 */ 
	//{ 0x0005, 45},  /** R45  - Analogue Right Input 0 */ 
	//{ 0x0000, 46},  /** R46  - Analogue Left Input 1 */ 
	//{ 0x0010, 47},  /** R47  - Analogue Right Input 1 */
	WM8904_Write(pTwid, device, 0x2C, 0x0008);
	WM8904_Write(pTwid, device, 0x2D, 0x0005);
	WM8904_Write(pTwid, device, 0x2E, 0x0000);
	WM8904_Write(pTwid, device, 0x2F, 0x0010);
}
