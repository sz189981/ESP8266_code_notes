/************************************************************************************************************************
									果云科技                                              

							     ESP8266   开发板例程 

							     SDK Verson: 2.0   IOT_SDK
							
		               		             CLASS 1 :       GPIO  篇
		               		   
						            
 主要 内容:

GPIO 的初始化和输出(包含特殊GPIO16)

系统微秒级延时os_delay_us

注意 对应的拨码开关打到ON ,才有效果。
/***********************************************************************************************************************

果云开发板购买地址: https://item.taobao.com/item.htm?spm=a230r.1.14.136.sTa8ie&id=42115449685&ns=1&abbucket=20#detail

***********************************************************************************************************************/
#include "ets_sys.h"
#include "osapi.h"

#include "user_interface.h"

#include "user_devicefind.h"
#include "user_webserver.h"
#include "driver/gpio16.h"//GPIO16头文件
#include "Gpio.h"//GPIO头文件
#define delay_us   os_delay_us  //系统微妙级

#if ESP_PLATFORM
#include "user_esp_platform.h"
#endif


/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void ICACHE_FLASH_ATTR
user_rf_pre_init(void)
{
}

void My_JDQ_Init(void)   //继电器初始化
{
      gpio16_output_conf();//GPIO16设置成输出模式
      gpio16_output_set(0);//默认低电平
      
}

void My_BEEP_Init(void)
{
      //配置 GPIO5  为 普通IO口
      PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U,FUNC_GPIO5);//蜂鸣器
      GPIO_OUTPUT_SET(GPIO_ID_PIN(5), 0);//默认低电平
}


void My_LED_Init(void)
{
    //配置 GPIO12 13 14 16  为 普通IO口
 	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO12);//蓝灯
 	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U,FUNC_GPIO15);//红灯
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U,FUNC_GPIO13);//绿灯
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U,FUNC_GPIO14);//白灯
		
}

void delay_ms(unsigned int t)
{
	 for(;t>0;t--)
	 	{
	   	delay_us(1000);
	       system_soft_wdt_feed();//这里我们喂下看门狗  ，不让看门狗复位，实际开发上不会用那么长的阻塞延时
	 	}
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
user_init(void)
{
    os_printf("SDK version:%s\n", system_get_sdk_version());
 
   My_JDQ_Init();
    My_BEEP_Init();
    My_LED_Init();
   while(1)
   {
       gpio16_output_set(1);	
	GPIO_OUTPUT_SET(GPIO_ID_PIN(5), 1);
	//gpio_output_set(BIT5, 0, BIT5, 0);  //输出高电平的第二种方式
       delay_ms(1000);
       gpio16_output_set(0);  
	GPIO_OUTPUT_SET(GPIO_ID_PIN(5), 0); 
	//gpio_output_set(0, BIT5, BIT5, 0);//输出低电平的第二种方式
       delay_ms(1000);

       GPIO_OUTPUT_SET(GPIO_ID_PIN(12), 1);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 0);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 0);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(15), 0);
    	delay_ms(1000);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(12), 0);
	delay_ms(1000);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(12), 0);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 1);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 0);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(15), 0);
	delay_ms(1000);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 0);
	delay_ms(1000);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(12), 0);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 0);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 1);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(15), 0);
	delay_ms(1000);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 0);
	delay_ms(1000);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(12), 0);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 0);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 0);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(15), 1);
	delay_ms(1000);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(15), 0);
       delay_ms(1000);
   }
#if ESP_PLATFORM
    /*Initialization of the peripheral drivers*/
    /*For light demo , it is user_light_init();*/
    /* Also check whether assigned ip addr by the router.If so, connect to ESP-server  */
    user_esp_platform_init();
#endif
    /*Establish a udp socket to receive local device detect info.*/
    /*Listen to the port 1025, as well as udp broadcast.
    /*If receive a string of device_find_request, it rely its IP address and MAC.*/
    user_devicefind_init();

    /*Establish a TCP server for http(with JSON) POST or GET command to communicate with the device.*/
    /*You can find the command in "2B-SDK-Espressif IoT Demo.pdf" to see the details.*/
    /*the JSON command for curl is like:*/
    /*3 Channel mode: curl -X POST -H "Content-Type:application/json" -d "{\"period\":1000,\"rgb\":{\"red\":16000,\"green\":16000,\"blue\":16000}}" http://192.168.4.1/config?command=light      */
    /*5 Channel mode: curl -X POST -H "Content-Type:application/json" -d "{\"period\":1000,\"rgb\":{\"red\":16000,\"green\":16000,\"blue\":16000,\"cwhite\":3000,\"wwhite\",3000}}" http://192.168.4.1/config?command=light      */
#ifdef SERVER_SSL_ENABLE
    user_webserver_init(SERVER_SSL_PORT);
#else
    user_webserver_init(SERVER_PORT);
#endif
}

