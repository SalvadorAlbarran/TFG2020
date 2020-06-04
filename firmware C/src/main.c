/* Copyright 2018 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
Modificado por Salvador ALbarrán Tiradas
*/
#include <stdio.h>
#include "kpu.h"
#include <platform.h>
#include <printf.h>
#include <string.h>
#include <stdlib.h>
#include <sysctl.h>
#include "nt35310.h"
#include "lcd.h"
#include "dvp.h"
#include "ov2640.h"
#include "uarths.h"
#include "encoding.h"
#include "gpiohs.h"
#include "sleep.h"
#include "fpioa.h"
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#define INCBIN_PREFIX
#include "incbin.h"
#include "w25qxx.h"
#include "math.h"
#include "iomem.h"
#include <string.h>
#include "inttypes.h"
#define LOAD_FROM_FLASH 1
#define PLL0_OUTPUT_FREQ 800000000UL
#define PLL1_OUTPUT_FREQ 400000000UL
#define PLL2_OUTPUT_FREQ 45158400UL

#define PIC_W 224
#define PIC_H 224
#define IMAGE_DATA_SIZE (224 * 224 * 3)
volatile uint32_t g_ai_done_flag;
volatile uint8_t g_dvp_finish_flag = 0;
uint16_t g_lcd_buf[PIC_W*PIC_H] ;
extern const char* mbnet_label_name[];

uint8_t g_buf28[28*28];
uint8_t g_ai_buf_in[PIC_W * PIC_H *3] __attribute__((aligned(128)));
kpu_model_context_t task1;

#if LOAD_FROM_FLASH
#define KMODEL_SIZE (4220 * 1024)
#define MODEL_ADDRESS (0x200000)
uint8_t model_data[KMODEL_SIZE] __attribute__((aligned(128)));

#else
#endif

float *features;
uint16_t label_index[1000];

char content[50];
unsigned long t0,t1;

static void ai_done(void* userdata)
{
    g_ai_done_flag = 1;

}

static int on_irq_dvp(void* ctx)
{
    if (dvp_get_interrupt(DVP_STS_FRAME_FINISH))
    {
        dvp_clear_interrupt(DVP_STS_FRAME_FINISH);
        g_dvp_finish_flag = 1;
    }
    else
    {
        if(g_dvp_finish_flag == 0)
            dvp_start_convert();
        dvp_clear_interrupt(DVP_STS_FRAME_START);
    }

    return 0;
}

static void io_mux_init(void)
{
    /* Init DVP IO map and function settings */
    fpioa_set_function(42, FUNC_CMOS_RST);
    fpioa_set_function(44, FUNC_CMOS_PWDN);
    fpioa_set_function(46, FUNC_CMOS_XCLK);
    fpioa_set_function(43, FUNC_CMOS_VSYNC);
    fpioa_set_function(45, FUNC_CMOS_HREF);
    fpioa_set_function(47, FUNC_CMOS_PCLK);
    fpioa_set_function(41, FUNC_SCCB_SCLK);
    fpioa_set_function(40, FUNC_SCCB_SDA);

    /* Init SPI IO map and function settings */
    fpioa_set_function(37, FUNC_GPIOHS0 + RST_GPIONUM);
    fpioa_set_function(38, FUNC_GPIOHS0 + DCX_GPIONUM);
    fpioa_set_function(36, FUNC_SPI0_SS3);
    fpioa_set_function(39, FUNC_SPI0_SCLK);
	
    sysctl_set_spi0_dvp_data(1);
}

static void io_set_power(void)
{
    /* Set dvp and spi pin to 1.8V */
    sysctl_set_power_mode(SYSCTL_POWER_BANK6, SYSCTL_POWER_V18);
    sysctl_set_power_mode(SYSCTL_POWER_BANK7, SYSCTL_POWER_V18);
}

volatile uint16_t info_pic[96*92];
static int times=0;
static void show_result(void)
{
	uint64_t time_last = sysctl_get_time_us();
    uint64_t time_now = sysctl_get_time_us();
    int time_count = 0;
	float ms;
    t1=read_cycle();
    float *features;
    size_t count;
    int i;
	float pmax;
	const char* name;
	int i_max;
	
	lcd_draw_picture(0, 0, PIC_W, PIC_H, g_lcd_buf);
	kpu_get_output(&task1, 0, (uint8_t **)&features, &count);
	count /= sizeof(float);
	for (i = 0; i < count; i++)
    {
        if (i % 16 == 0)
            printf("\r\n%04x: ",i);
        printf("%.3f ",features[i]);
    }
	
	
	pmax=0;
	i_max=0;
	for (i = 0; i < count; i++)
    {
        if(features[i]>pmax) 
		{
			pmax=features[i];
			i_max = i;
		}
    }
    time_count ++;
        if(time_count % 100 == 0)
        {
            time_now = sysctl_get_time_us();
            ms = (time_now - time_last)/1000.0/100;
            printf("SPF:%fms\n", (time_now - time_last)/1000.0/100);
            time_last = time_now;
        }
	snprintf(content, 100, "%d: %1.3f", i_max, features[i_max]);
	content[12]=0;
	memset(info_pic,0x00,sizeof(info_pic));
	lcd_ram_draw_string(content, info_pic, WHITE, BLACK,96);
	lcd_draw_picture(224, 0, 96, 16, info_pic);
	
}


int main()
{
	int i;
	fpioa_set_function(8, FUNC_GPIOHS0);
	gpiohs_set_drive_mode(0, GPIO_DM_OUTPUT);
	gpiohs_set_pin(0, GPIO_PV_LOW);
    /* Set CPU and dvp clk */
    sysctl_pll_set_freq(SYSCTL_PLL0, PLL0_OUTPUT_FREQ);
    sysctl_pll_set_freq(SYSCTL_PLL1, PLL1_OUTPUT_FREQ);
    //sysctl_pll_set_freq(SYSCTL_PLL2, PLL2_OUTPUT_FREQ);
    sysctl_clock_enable(SYSCTL_CLOCK_AI);
    uarths_init();
	io_mux_init();
    io_set_power();
    plic_init();

	/* LCD init*/
	printf("LCD init\r\n");
    lcd_init();
	lcd_set_direction(DIR_YX_RLDU);
	lcd_clear(BLACK);
	/* DVP init */
    printf("DVP init\r\n");
	dvp_init(8);
    dvp_set_xclk_rate(24000000);
    dvp_enable_burst();
    dvp_set_output_enable(0, 1);	//enable to ai
    dvp_set_output_enable(1, 1);	//disable to lcd
    dvp_set_image_format(DVP_CFG_RGB_FORMAT);
    dvp_set_image_size(PIC_W, PIC_H);
    ov2640_init();
	
	dvp_set_ai_addr((uint32_t)g_ai_buf_in, (uint32_t)(g_ai_buf_in + PIC_W * PIC_H), (uint32_t)(g_ai_buf_in + PIC_W * PIC_H * 2));
	dvp_set_display_addr((uint32_t)g_lcd_buf);
	dvp_config_interrupt(DVP_CFG_START_INT_ENABLE | DVP_CFG_FINISH_INT_ENABLE, 0);
    dvp_disable_auto();
	/* DVP interrupt config */
    printf("DVP interrupt config\r\n");
    plic_set_priority(IRQN_DVP_INTERRUPT, 1);
    plic_irq_register(IRQN_DVP_INTERRUPT, on_irq_dvp, NULL);
    plic_irq_enable(IRQN_DVP_INTERRUPT);
    
	#if LOAD_FROM_FLASH
	w25qxx_init(3, 0);
    w25qxx_enable_quad_mode();
    w25qxx_read_data(MODEL_ADDRESS, model_data, KMODEL_SIZE, W25QXX_QUAD_FAST);
	printf("load model from flash ok!\r\n");
	#endif
	
    if (kpu_load_kmodel(&task1, model_data) != 0)
    {
        printf("Cannot load kmodel.\n");
        exit(-1);
    }
    /* enable global interrupt */
    sysctl_enable_irq();

    /* system start */
    printf("system start\r\n");
    g_dvp_finish_flag = 0;
    dvp_clear_interrupt(DVP_STS_FRAME_START | DVP_STS_FRAME_FINISH);
    dvp_config_interrupt(DVP_CFG_START_INT_ENABLE | DVP_CFG_FINISH_INT_ENABLE, 1);
	
	while(1)
	{
		while (g_dvp_finish_flag == 0){};
		g_ai_done_flag = 0;
		t0=read_cycle();
		if (kpu_run_kmodel(&task1, g_ai_buf_in, DMAC_CHANNEL5, ai_done, NULL) != 0)
        {
            printf("Cannot run kmodel.\n");
            exit(-1);
        }
		while (!g_ai_done_flag);
		g_ai_done_flag=0;
		g_dvp_finish_flag = 0;
		show_result();
    }  
}
