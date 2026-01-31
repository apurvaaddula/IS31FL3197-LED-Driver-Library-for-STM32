/*
 * IS31FL3197.c
 *
 *  Created on: Aug 13, 2025
 *      Author: apurvaa
 */

#include "IS31FL3197.h"

static void IS31FL3197_SDB_High(IS31FL3197_HandleTypeDef *d);
static void IS31FL3197_Update_PWM(IS31FL3197_HandleTypeDef *d);

HAL_StatusTypeDef IS31FL3197_Init(IS31FL3197_HandleTypeDef *d) {

	uint8_t id;

    if (d->sdb_port) {
       HAL_GPIO_WritePin(d->sdb_port, d->sdb_pin, GPIO_PIN_RESET);
       HAL_Delay(1);
       HAL_GPIO_WritePin(d->sdb_port, d->sdb_pin, GPIO_PIN_SET);
     }

    IS31FL3197_SDB_High(d);

    HAL_I2C_Mem_Read(d->hi2c, d->addr<<1, IS31FL3197_REG_PRODUCT_ID, 1, &id, 1, 10);

    HAL_Delay(1);

    return HAL_OK;
}

HAL_StatusTypeDef IS31FL3197_Shutdown_Control(IS31FL3197_HandleTypeDef *d,is31fl3197_sleep_mode_t sleep,uint8_t shutdown)
{
	uint8_t data;
	HAL_StatusTypeDef ret;

	data = ((sleep & 0x03U)<<1)|(shutdown & 0x01U);
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_SHUTDOWN , I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
	if (ret != HAL_OK) return ret;

	return HAL_OK;

}

HAL_StatusTypeDef IS31FL3197_Operating_Config_Register(IS31FL3197_HandleTypeDef *d,const leds_handle_t *leds){
	uint8_t data;
	HAL_StatusTypeDef ret;

	data = (uint8_t)((leds->led[3]&0x01U)<<6)|((leds->led[2]&0x03U)<<4)|((leds->led[1]&0x03U)<<2)|(leds->led[0]&0x03U);
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_CONFIG, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
	if (ret != HAL_OK) return ret;

	return HAL_OK;
}

HAL_StatusTypeDef IS31FL3197_Charge_Pump1(IS31FL3197_HandleTypeDef *d,is31fl3197_cppm_t power_mode,is31fl3197_cpm_t working_mode){
	uint8_t data;
	HAL_StatusTypeDef ret;

	data = ((power_mode & 0x03U)<<4)|(working_mode & 0x03U);
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_CP1, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
	if (ret != HAL_OK) return ret;

	return HAL_OK;

}

HAL_StatusTypeDef IS31FL3197_Charge_Pump2(IS31FL3197_HandleTypeDef *d, const charge_pump2_handle_t *cpde , is31fl3197_hrt_t hrt){
	uint8_t data;
	HAL_StatusTypeDef ret;

	data = ((hrt&0x07U)<<4) | ((cpde -> cpde[3] & 0x01U)<<3) | ((cpde -> cpde[2] & 0x01U)<<2) | ((cpde -> cpde[1] & 0x01U)<<1) | ((cpde -> cpde[0] & 0x01U));
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_CP2, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
	if (ret != HAL_OK) return ret;

	return HAL_OK;
}

HAL_StatusTypeDef IS31FL3197_Current_Band(IS31FL3197_HandleTypeDef *d,const current_band_handle_t *cb){
	uint8_t data;
	HAL_StatusTypeDef ret;

	data = ((cb->cb[3]&0x03U)<<6) | ((cb->cb[2] & 0x03)<<4) | ((cb->cb[1] & 0x03)<<2) |(cb->cb[0] & 0x03);
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_CURRENT_BAND, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
	if (ret != HAL_OK) return ret;

	IS31FL3197_Update_PWM(d);

	return HAL_OK;
}

HAL_StatusTypeDef IS31FL3197_Phase_Delay_Register(IS31FL3197_HandleTypeDef *d,is31fl3197_phase_delay_t pd){
	uint8_t data;
	HAL_StatusTypeDef ret;

	data = pd & 0x01U;
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_PHASE_DELAY, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
	if (ret != HAL_OK) return ret;

	return HAL_OK;
}

HAL_StatusTypeDef IS31FL3197_SetPWMAllChannels(IS31FL3197_HandleTypeDef *d,uint16_t pwm_value) {
	HAL_StatusTypeDef ret;

	static const uint8_t pwm_reg_l[] = {IS31FL3197_REG_PWM_OUT1_L, IS31FL3197_REG_PWM_OUT2_L , IS31FL3197_REG_PWM_OUT3_L,IS31FL3197_REG_PWM_OUT4_L};
	static const uint8_t pwm_reg_h[] = {IS31FL3197_REG_PWM_OUT1_H ,IS31FL3197_REG_PWM_OUT2_H,IS31FL3197_REG_PWM_OUT3_H, IS31FL3197_REG_PWM_OUT4_H};

    if (pwm_value > 0x0FFF) pwm_value = 0x0FFF;
    uint8_t pwm_l = pwm_value & 0xFF;
    uint8_t pwm_h = (pwm_value >> 8) & 0x0F;

    for (int i = 0; i < 4; i++) {
        ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, pwm_reg_l[i], I2C_MEMADD_SIZE_8BIT, &pwm_l, 1, 10);
        if (ret!=HAL_OK)break;
        ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, pwm_reg_h[i], I2C_MEMADD_SIZE_8BIT, &pwm_h, 1, 10);
        if (ret!=HAL_OK)break;
    }
    if (ret != HAL_OK) return ret;

    IS31FL3197_Update_PWM(d);
    return HAL_OK;
}

HAL_StatusTypeDef IS31FL3197_SetPWMSingleChannel(IS31FL3197_HandleTypeDef *d,uint8_t channel, uint16_t pwm_value) {
	HAL_StatusTypeDef ret;
	static const uint8_t pwm_reg_l[] = {IS31FL3197_REG_PWM_OUT1_L, IS31FL3197_REG_PWM_OUT2_L , IS31FL3197_REG_PWM_OUT3_L,IS31FL3197_REG_PWM_OUT4_L};
	static const uint8_t pwm_reg_h[] = {IS31FL3197_REG_PWM_OUT1_H ,IS31FL3197_REG_PWM_OUT2_H,IS31FL3197_REG_PWM_OUT3_H, IS31FL3197_REG_PWM_OUT4_H};
    if (channel > 3) return HAL_ERROR;
    if (pwm_value > 0x0FFF) pwm_value = 0x0FFF;

    uint8_t pwm_l = pwm_value & 0xFF;
    uint8_t pwm_h = (pwm_value >> 8) & 0x0F;

    ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, pwm_reg_l[channel], I2C_MEMADD_SIZE_8BIT, &pwm_l, 1, 10);
    if (ret != HAL_OK) return ret;
    ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, pwm_reg_h[channel], I2C_MEMADD_SIZE_8BIT, &pwm_h, 1, 10);
    if (ret != HAL_OK) return ret;

    IS31FL3197_Update_PWM(d);
    return HAL_OK;

}

static void IS31FL3197_SDB_High(IS31FL3197_HandleTypeDef *d) {
    HAL_GPIO_WritePin(d->sdb_port, d->sdb_pin, GPIO_PIN_SET);
}

static void IS31FL3197_Update_PWM(IS31FL3197_HandleTypeDef *d){
	 uint8_t update = UPDATE;
	 HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_UPDATE_PWM , I2C_MEMADD_SIZE_8BIT,&update, 1, 10);
}

HAL_StatusTypeDef IS31FL3197_Reset(IS31FL3197_HandleTypeDef *d) {
  uint8_t update = UPDATE;
  return HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_RESET, I2C_MEMADD_SIZE_8BIT,&update, 1, 10);
}

HAL_StatusTypeDef IS31FL3197_Color_Hold_Function_Register(IS31FL3197_HandleTypeDef *d,is31fl3197_hold_time_t ht, is31fl3197_hold_en_t chf){
	uint8_t data;
	HAL_StatusTypeDef ret;

	data = (ht&0x01U) | ((chf&0x01U)<<1);
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_COLOR_HOLD, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
	if (ret != HAL_OK) return ret;

	return HAL_OK;
}

HAL_StatusTypeDef IS31FL3197_PatternSetColor(IS31FL3197_HandleTypeDef *d,pattern_color_settings_t *p){
	HAL_StatusTypeDef ret;
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_COL1_OUT1, I2C_MEMADD_SIZE_8BIT, p->color1, 3, 10);
	if (ret != HAL_OK) return ret;
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_COL2_OUT1, I2C_MEMADD_SIZE_8BIT, p->color2, 3, 10);
	if (ret != HAL_OK) return ret;
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_COL3_OUT1, I2C_MEMADD_SIZE_8BIT, p->color3, 3, 10);
	if (ret != HAL_OK) return ret;

	return HAL_OK;

}

HAL_StatusTypeDef IS31FL3197_PatternTimeSet(IS31FL3197_HandleTypeDef *d,pattern_time_set_t *t){
	HAL_StatusTypeDef ret;
	uint8_t data;
	data = (t->t1 & 0x0F)<<4 | (t->ts & 0x0F);
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_TS_T1, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
	if (ret != HAL_OK) return ret;
	data = (t->t3 & 0x0F)<<4 | (t->t2 & 0x0F);
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_T2_T3, I2C_MEMADD_SIZE_8BIT,&data, 1, 10);
	if (ret != HAL_OK) return ret;
	data = (t->t4 & 0x0F)<<4 | (t->tp & 0x0F);
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_TP_T4, I2C_MEMADD_SIZE_8BIT,&data, 1, 10);
	if (ret != HAL_OK) return ret;

	return HAL_OK;
}

HAL_StatusTypeDef IS31FL3197_ColorCrossfadeEnable(IS31FL3197_HandleTypeDef *d,crossfade_enable_t *cfe){
	HAL_StatusTypeDef ret;
	uint8_t data;
	data = ((cfe->cfe[2]&0x01)<<2) | ((cfe->cfe[1]&0x01)<<1) | (cfe->cfe[0]&0x01);
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_CROSSFADE_EN , I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
	if (ret != HAL_OK) return ret;

	return HAL_OK;
}

HAL_StatusTypeDef IS31FL3197_CrossfadeTime(IS31FL3197_HandleTypeDef *d,is31fl3197_tc_time_t tc){
	HAL_StatusTypeDef ret;
	uint8_t data;
	data = tc & 0x0F;
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_CROSSFADE_TIME , I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
	if (ret != HAL_OK) return ret;

	return HAL_OK;
}

HAL_StatusTypeDef IS31FL3197_PatternColorEnable(IS31FL3197_HandleTypeDef *d,crossfade_enable_t *ce){
	HAL_StatusTypeDef ret;
	uint8_t data;
	data = ((ce->cfe[2]&0x01)<<2) | ((ce->cfe[1]&0x01)<<1) | (ce->cfe[0]&0x01);
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_COLOR_ENABLE , I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
	if (ret != HAL_OK) return ret;

	return HAL_OK;
}

HAL_StatusTypeDef IS31FL3197_PatternCycleTimes(IS31FL3197_HandleTypeDef *d,color_cycle_t *c){
	HAL_StatusTypeDef ret;
	uint8_t data;
	data = ((c->cct[2]&0x03)<<4) | ((c->cct[1]&0x03)<<2) | (c->cct[0]&0x03);
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_COLOR_CYCLE , I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
	if (ret != HAL_OK) return ret;

	return HAL_OK;
}

HAL_StatusTypeDef IS31FL3197_PatternRegisterConfig(IS31FL3197_HandleTypeDef *d,is31fl3197_gamma_t gamma,is31fl3197_mtpl_time_t mtpl){
	HAL_StatusTypeDef ret;
	uint8_t data;
	data = ((mtpl&0x0F)<<4)|((gamma&0x03)<<2);
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_PATTERN , I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
	if (ret != HAL_OK) return ret;

	return HAL_OK;

}

HAL_StatusTypeDef IS31FL3197_PatternLoopTimesRegister(IS31FL3197_HandleTypeDef *d,uint8_t plt_h,uint8_t plt_l){
	HAL_StatusTypeDef ret;
	uint8_t data;
	if (plt_h) data = 0x00;
	else data = (plt_l & 0x7F) | 0x80;
	ret = HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_PATTERN_LOOPS , I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
	if (ret != HAL_OK) return ret;

	return HAL_OK;
}


HAL_StatusTypeDef IS31FL3197_Read_Pattern_State_Register(IS31FL3197_HandleTypeDef *d, is31fl3197_pattern_status_t *status){
	uint8_t data;
	HAL_StatusTypeDef ret;

	ret = HAL_I2C_Mem_Read(d->hi2c, d->addr<<1, IS31FL3197_REG_PATTERN_STATE, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
	if (ret != HAL_OK) return ret;

	status->ts = data & 0x07U;
	status->cs1 = (data>>4)&0x01;
	status->cs2 = (data>>5)&0x01;
	status->cs3 = (data>>6)&0x01;
	status->ps = (data>>7)&0x01;

	return HAL_OK;
}

void IS31FL3197_Update_Color(IS31FL3197_HandleTypeDef *d){
	uint8_t update = UPDATE;
	HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_UPDATE_COLOR , I2C_MEMADD_SIZE_8BIT,&update, 1, 10);
}

void IS31FL3197_Update_PatternTime(IS31FL3197_HandleTypeDef *d){
	uint8_t update = UPDATE;
	HAL_I2C_Mem_Write(d->hi2c, d->addr<<1, IS31FL3197_REG_UPDATE_TIME_RUN , I2C_MEMADD_SIZE_8BIT,&update, 1, 10);
}

void IS31FL3197_LED_On(IS31FL3197_HandleTypeDef *d,uint8_t channel) {

    IS31FL3197_SetPWMSingleChannel(d,channel, 4095);
}

void IS31FL3197_LED_Off(IS31FL3197_HandleTypeDef *d,uint8_t channel) {

    IS31FL3197_SetPWMSingleChannel(d,channel, 0);
}

void IS31FL3197_LED_Blink(IS31FL3197_HandleTypeDef *d,uint8_t channel,uint32_t delay) {
	IS31FL3197_LED_On(d,channel);
    HAL_Delay(delay);
    IS31FL3197_LED_Off(d,channel);
    HAL_Delay(delay);
}


void IS31FL3197_LED_All_On(IS31FL3197_HandleTypeDef *d) {

	IS31FL3197_SetPWMAllChannels(d,4095);

}

void IS31FL3197_LED_All_Off(IS31FL3197_HandleTypeDef *d) {

	IS31FL3197_SetPWMAllChannels(d,0);

}


void IS31FL3197_LED_Blink_All(IS31FL3197_HandleTypeDef *d,uint32_t delay) {
	IS31FL3197_LED_All_On(d);
    HAL_Delay(delay);
    IS31FL3197_LED_All_Off(d);
    HAL_Delay(delay);
}

void IS31FL3197_LED_Group_On(IS31FL3197_HandleTypeDef *d,uint8_t* leds, uint8_t count) {
    for (uint8_t i = 0; i < count; i++) {
    	IS31FL3197_LED_On(d,leds[i]);
    }
}

void IS31FL3197_LED_Group_Off(IS31FL3197_HandleTypeDef *d,uint8_t* leds, uint8_t count) {
    for (uint8_t i = 0; i < count; i++) {
    	IS31FL3197_LED_Off(d,leds[i]);
    }
}


void IS31FL3197_LED_Blink_Multiple_Leds(IS31FL3197_HandleTypeDef *d,uint8_t* leds, uint8_t count,uint32_t delay){
	IS31FL3197_LED_Group_On(d,leds,count);
	HAL_Delay(delay);
	IS31FL3197_LED_Group_Off(d,leds,count);
	HAL_Delay(delay);
}
