/*
 * IS31FL3197.h
 *
 *  Created on: Aug 13, 2025
 *      Author: apurvaa
 */

#ifndef INC_IS31FL3197_H_
#define INC_IS31FL3197_H_

#include "stm32g0xx_hal.h"



#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// I2C Address based on connection of AD line in hardware
#define IS31FL3197_ADDR_GND 0x50
#define IS31FL3197_ADDR_SCL 0x51
#define IS31FL3197_ADDR_SDA 0x52
#define IS31FL3197_ADDR_VCC 0x53

#define IS31FL3197_CHANNELS 4

#define IS31FL3197_REG_PRODUCT_ID          0x00
#define IS31FL3197_REG_SHUTDOWN            0x01
#define IS31FL3197_REG_CONFIG              0x02
#define IS31FL3197_REG_CP1                 0x03
#define IS31FL3197_REG_CP2                 0x04
#define IS31FL3197_REG_CURRENT_BAND        0x05
#define IS31FL3197_REG_COLOR_HOLD          0x06
#define IS31FL3197_REG_PHASE_DELAY         0x07
#define IS31FL3197_REG_PATTERN_STATE       0x0F

#define IS31FL3197_REG_CL_OUT1             0x10
#define IS31FL3197_REG_CL_OUT2             0x11
#define IS31FL3197_REG_CL_OUT3             0x12
#define IS31FL3197_REG_CL_OUT4             0x13

#define IS31FL3197_REG_COL1_OUT1           0x10
#define IS31FL3197_REG_COL1_OUT2           0x11
#define IS31FL3197_REG_COL1_OUT3           0x12
#define IS31FL3197_REG_COL2_OUT1           0x14
#define IS31FL3197_REG_COL2_OUT2           0x15
#define IS31FL3197_REG_COL2_OUT3           0x16
#define IS31FL3197_REG_COL3_OUT1           0x17
#define IS31FL3197_REG_COL3_OUT2           0x18
#define IS31FL3197_REG_COL3_OUT3           0x19

#define IS31FL3197_REG_PWM_OUT1_L          0x1A
#define IS31FL3197_REG_PWM_OUT1_H          0x1B
#define IS31FL3197_REG_PWM_OUT2_L          0x1C
#define IS31FL3197_REG_PWM_OUT2_H          0x1D
#define IS31FL3197_REG_PWM_OUT3_L          0x1E
#define IS31FL3197_REG_PWM_OUT3_H          0x1F
#define IS31FL3197_REG_PWM_OUT4_L          0x20
#define IS31FL3197_REG_PWM_OUT4_H          0x21

#define IS31FL3197_REG_TS_T1               0x22
#define IS31FL3197_REG_T2_T3               0x23
#define IS31FL3197_REG_TP_T4               0x24

#define IS31FL3197_REG_CROSSFADE_EN        0x25
#define IS31FL3197_REG_CROSSFADE_TIME      0x26
#define IS31FL3197_REG_COLOR_ENABLE        0x27

#define IS31FL3197_REG_COLOR_CYCLE         0x28
#define IS31FL3197_REG_PATTERN             0x29
#define IS31FL3197_REG_PATTERN_LOOPS       0x2A

#define IS31FL3197_REG_UPDATE_COLOR        0x2B
#define IS31FL3197_REG_UPDATE_PWM          0x2C
#define IS31FL3197_REG_UPDATE_TIME_RUN     0x2D
#define IS31FL3197_REG_RESET               0x3F

#define IS31FL3197_SOFTWARE_SHUTDOWN 0x00
#define IS31FL3197_NORMAL 0x01

#define PD_MODE_1   0
#define PD_MODE_2   1

#define UPDATE 0xC5

typedef enum {
  IS31FL3197_SLEEP_DISABLE = 0x00,
  IS31FL3197_SLEEP1        = 0x01,
  IS31FL3197_SLEEP2        = 0x02,
} is31fl3197_sleep_mode_t;

typedef enum {
  IS31FL3197_MODE_PWM_AND_CL = 0x00,
  IS31FL3197_MODE_PATTERN    = 0x01,
  IS31FL3197_MODE_CL_ONLY    = 0x02,
} is31fl3197_led_mode_t;

typedef enum{
	IS31FL3197_MODE_PWM_AND_CL_4 = 0,
	IS31FL3197_MODE_CL_ONLY_4 = 1,
}is31fl3197_led_mode_4_t;

typedef enum{
	IS31FL3197_CPPM_1_X = 	0x00,
	IS31FL3197_CPPM_1_5_X = 0x01,
	IS31FL3197_CPPM_1_X_2 = 0x02,
	IS31FL3197_CPPM_2_X = 0x03,

}is31fl3197_cppm_t;

typedef enum{
	IS31FL3197_AUTO = 0x00,
	IS31FL3197_1_X = 0x01,
	IS31FL3197_CPM_1_5_X = 0x02,
	IS31FL3197_CPM_2_X = 0x03,
}is31fl3197_cpm_t;

typedef enum{
	CPDE_ENABLE = 0,
	CPDE_DISABLE = 1,
}is31fl3197_cpde_t;

typedef enum {
    IS31FL3197_HRT_50MV  = 0b000, // 50 mV
    IS31FL3197_HRT_100MV = 0b001, // 100 mV
    IS31FL3197_HRT_125MV = 0b010, // 125 mV
    IS31FL3197_HRT_150MV = 0b011, // 150 mV (default)
    IS31FL3197_HRT_175MV = 0b100, // 175 mV
    IS31FL3197_HRT_200MV = 0b101, // 200 mV
    IS31FL3197_HRT_250MV = 0b110, // 250 mV
    IS31FL3197_HRT_300MV = 0b111  // 300 mV
} is31fl3197_hrt_t;

typedef enum {
  IS31FL3197_CB_QUARTER = 0,
  IS31FL3197_CB_HALF    = 1,
  IS31FL3197_CB_3Q      = 2,
  IS31FL3197_CB_FULL    = 3,
} is31fl3197_current_band_t;

typedef enum {
    IS31FL3197_PD_MODE_1 = 0,
    IS31FL3197_PD_MODE_2 = 1
} is31fl3197_phase_delay_t;

typedef enum {
    IS31FL3197_TS_RUNNING_AT_TS  = 0x00,
    IS31FL3197_TS_RUNNING_AT_T1  = 0x01,
    IS31FL3197_TS_RUNNING_AT_T2  = 0x02,
    IS31FL3197_TS_RUNNING_AT_T3  = 0x03,
    IS31FL3197_TS_RUNNING_AT_TP  = 0x04,
    IS31FL3197_TS_RUNNING_AT_T4  = 0x05,
    IS31FL3197_TS_RUNNING_AT_TC  = 0x06
} is31fl3197_time_state_t;

typedef enum {
    IS31FL3197_CS_NOT_RUNNING = 0x00,
    IS31FL3197_CS_RUNNING     = 0x01
} is31fl3197_color_state_t;

typedef enum {
    IS31FL3197_PS_NOT_RUNNING = 0x00,
    IS31FL3197_PS_RUNNING     = 0x01
} is31fl3197_pattern_state_t;

typedef enum {
    IS31FL3197_TIME_0P03_S = 0x0,
    IS31FL3197_TIME_0P13_S = 0x1,
    IS31FL3197_TIME_0P26_S = 0x2,
    IS31FL3197_TIME_0P38_S = 0x3,
    IS31FL3197_TIME_0P51_S = 0x4,
    IS31FL3197_TIME_0P77_S = 0x5,
    IS31FL3197_TIME_1P04_S = 0x6,
    IS31FL3197_TIME_1P60_S = 0x7,
    IS31FL3197_TIME_2P10_S = 0x8,
    IS31FL3197_TIME_2P60_S = 0x9,
    IS31FL3197_TIME_3P10_S = 0xA,
    IS31FL3197_TIME_4P20_S = 0xB,
    IS31FL3197_TIME_5P20_S = 0xC,
    IS31FL3197_TIME_6P20_S = 0xD,
    IS31FL3197_TIME_7P30_S = 0xE,
    IS31FL3197_TIME_8P30_S = 0xF
} is31fl3197_time_sel_t;

typedef enum {
    IS31FL3197_TC_0P51_S = 0x0,
    IS31FL3197_TC_0P77_S = 0x5,
    IS31FL3197_TC_1P04_S = 0x6,
    IS31FL3197_TC_1P60_S = 0x7,
    IS31FL3197_TC_2P10_S = 0x8,
    IS31FL3197_TC_2P60_S = 0x9,
    IS31FL3197_TC_3P10_S = 0xA,
    IS31FL3197_TC_4P20_S = 0xB,
    IS31FL3197_TC_5P20_S = 0xC,
    IS31FL3197_TC_6P20_S = 0xD,
    IS31FL3197_TC_7P30_S = 0xE,
    IS31FL3197_TC_8P30_S = 0xF
} is31fl3197_tc_time_t;

typedef enum {
    IS31FL3197_CCT_ENDLESS = 0x0,
    IS31FL3197_CCT_1_TIME  = 0x1,
    IS31FL3197_CCT_2_TIMES = 0x2,
    IS31FL3197_CCT_3_TIMES = 0x3
} is31fl3197_color_cycle_times_t;

typedef enum {
  IS31FL3197_GAMMA_24 = 0,
  IS31FL3197_GAMMA_35 = 1,
  IS31FL3197_GAMMA_LINEAR = 2,
} is31fl3197_gamma_t;

typedef enum {
    IS31FL3197_MTPLT_ENDLESS = 0x0,
    IS31FL3197_MTPLT_1_TIME  = 0x1,
    IS31FL3197_MTPLT_2_TIMES = 0x2,
    IS31FL3197_MTPLT_3_TIMES = 0x3,
    IS31FL3197_MTPLT_4_TIMES = 0x4,
    IS31FL3197_MTPLT_5_TIMES = 0x5,
    IS31FL3197_MTPLT_6_TIMES = 0x6,
    IS31FL3197_MTPLT_7_TIMES = 0x7,
    IS31FL3197_MTPLT_8_TIMES = 0x8,
    IS31FL3197_MTPLT_9_TIMES = 0x9,
    IS31FL3197_MTPLT_10_TIMES = 0xA,
    IS31FL3197_MTPLT_11_TIMES = 0xB,
    IS31FL3197_MTPLT_12_TIMES = 0xC,
    IS31FL3197_MTPLT_13_TIMES = 0xD,
    IS31FL3197_MTPLT_14_TIMES = 0xE,
    IS31FL3197_MTPLT_15_TIMES = 0xF
} is31fl3197_mtpl_time_t;

typedef enum { HOLD_AT_T4 = 0, HOLD_AT_T2 = 1 } is31fl3197_hold_time_t;
typedef enum { HOLD_DISABLE = 0, HOLD_ENABLE = 1 } is31fl3197_hold_en_t;

typedef struct {
  I2C_HandleTypeDef *hi2c;
  uint8_t addr;

  GPIO_TypeDef *sdb_port;
  uint16_t      sdb_pin;

} IS31FL3197_HandleTypeDef;

typedef struct{
	is31fl3197_led_mode_t led[IS31FL3197_CHANNELS];
}leds_handle_t;

typedef struct {
	is31fl3197_current_band_t cb[IS31FL3197_CHANNELS];
}current_band_handle_t;

typedef struct{
	is31fl3197_cpde_t cpde[4];
}charge_pump2_handle_t;

typedef struct {
    is31fl3197_pattern_state_t ps;
    is31fl3197_color_state_t cs1;
    is31fl3197_color_state_t cs2;
    is31fl3197_color_state_t cs3;
    is31fl3197_time_state_t ts;
} is31fl3197_pattern_status_t;

typedef struct{
	uint8_t color1[3],color2[3],color3[3];
}pattern_color_settings_t;

typedef struct{
	is31fl3197_time_sel_t ts,t1,t2,t3,tp,t4;
}pattern_time_set_t;

typedef struct{
	uint8_t cfe[3];
}crossfade_enable_t;

typedef struct{
	is31fl3197_color_cycle_times_t cct[3];
}color_cycle_t;


HAL_StatusTypeDef IS31FL3197_Init(IS31FL3197_HandleTypeDef *d);
HAL_StatusTypeDef IS31FL3197_Reset(IS31FL3197_HandleTypeDef *d);

/*Configuration*/
HAL_StatusTypeDef IS31FL3197_Shutdown_Control(IS31FL3197_HandleTypeDef *d,is31fl3197_sleep_mode_t sleep,uint8_t shutdown);
HAL_StatusTypeDef IS31FL3197_Operating_Config_Register(IS31FL3197_HandleTypeDef *d,const leds_handle_t *leds);
HAL_StatusTypeDef IS31FL3197_Charge_Pump1(IS31FL3197_HandleTypeDef *d,is31fl3197_cppm_t power_mode,is31fl3197_cpm_t working_mode);
HAL_StatusTypeDef IS31FL3197_Charge_Pump2(IS31FL3197_HandleTypeDef *d,const charge_pump2_handle_t *cpde,is31fl3197_hrt_t hrt);
HAL_StatusTypeDef IS31FL3197_Current_Band(IS31FL3197_HandleTypeDef *d, const current_band_handle_t *cb);
HAL_StatusTypeDef IS31FL3197_Phase_Delay_Register(IS31FL3197_HandleTypeDef *d,is31fl3197_phase_delay_t pd);

/*PWM and Current Control Mode*/
HAL_StatusTypeDef IS31FL3197_SetPWMAllChannels(IS31FL3197_HandleTypeDef *d,uint16_t pwm_value);
HAL_StatusTypeDef IS31FL3197_SetPWMSingleChannel(IS31FL3197_HandleTypeDef *d,uint8_t channel, uint16_t pwm_value);

void IS31FL3197_LED_On(IS31FL3197_HandleTypeDef *d, uint8_t channel);
void IS31FL3197_LED_Off(IS31FL3197_HandleTypeDef *d, uint8_t channel);
void IS31FL3197_LED_Blink(IS31FL3197_HandleTypeDef *d, uint8_t channel, uint32_t delay);
void IS31FL3197_LED_All_On(IS31FL3197_HandleTypeDef *d);
void IS31FL3197_LED_All_Off(IS31FL3197_HandleTypeDef *d);
void IS31FL3197_LED_Blink_All(IS31FL3197_HandleTypeDef *d, uint32_t delay);
void IS31FL3197_LED_Group_On(IS31FL3197_HandleTypeDef *d,uint8_t* leds, uint8_t count);
void IS31FL3197_LED_Group_Off(IS31FL3197_HandleTypeDef *d,uint8_t* leds, uint8_t count);
void IS31FL3197_LED_Blink_Multiple_Leds(IS31FL3197_HandleTypeDef *d,uint8_t* leds, uint8_t count,uint32_t delay);


/*Pattern Mode*/
void IS31FL3197_Update_Color(IS31FL3197_HandleTypeDef *d);
void IS31FL3197_Update_PatternTime(IS31FL3197_HandleTypeDef *d);
HAL_StatusTypeDef IS31FL3197_Read_Pattern_State_Register(IS31FL3197_HandleTypeDef *d, is31fl3197_pattern_status_t *status);
HAL_StatusTypeDef IS31FL3197_Color_Hold_Function_Register(IS31FL3197_HandleTypeDef *d,is31fl3197_hold_time_t ht, is31fl3197_hold_en_t chf);
HAL_StatusTypeDef IS31FL3197_PatternSetColor(IS31FL3197_HandleTypeDef *d, pattern_color_settings_t *p);
HAL_StatusTypeDef IS31FL3197_PatternTimeSet(IS31FL3197_HandleTypeDef *d, pattern_time_set_t *t);
HAL_StatusTypeDef IS31FL3197_ColorCrossfadeEnable(IS31FL3197_HandleTypeDef *d, crossfade_enable_t *cfe);
HAL_StatusTypeDef IS31FL3197_CrossfadeTime(IS31FL3197_HandleTypeDef *d, is31fl3197_tc_time_t tc);
HAL_StatusTypeDef IS31FL3197_PatternColorEnable(IS31FL3197_HandleTypeDef *d, crossfade_enable_t *ce);
HAL_StatusTypeDef IS31FL3197_PatternCycleTimes(IS31FL3197_HandleTypeDef *d, color_cycle_t *c);
HAL_StatusTypeDef IS31FL3197_PatternRegisterConfig(IS31FL3197_HandleTypeDef *d, is31fl3197_gamma_t gamma, is31fl3197_mtpl_time_t mtpl);
HAL_StatusTypeDef IS31FL3197_PatternLoopTimesRegister(IS31FL3197_HandleTypeDef *d, uint8_t plt_h, uint8_t plt_l);


#ifdef __cplusplus
}
#endif
#endif /* INC_IS31FL3197_H_ */
