/*
 * HW_words.h
 *
 * Created: 2014-02-01 05:58:03
 *  Author: jost1099
 */ 


#ifndef HW_WORDS_H_
#define HW_WORDS_H_

#include "F_Defs.h"
#include <string.h>
#include "asf.h"
#include "main.h"
#include "Forth_system.h"
#include "BM700_src\generic_i2c.h"
#include "BM700_src\generic_adc.h"
#include "BM700_src\test1.h"
#include "BM700_src\HD44780.h"
#include "BM700_src\AT_37i2c.h"

#define HW_INTO_DICTIONARY {"temp", AT37_I2C_Temperature, 0}, {"point", AT37_I2C_Pointer, 0},\
                            {"byr", AT37_mem_read, 0}, {"byw", AT37_mem_write, 0},\
                            {"setpot", setpot, 0}, {"adstart,", AD_start, 0}, {"ad?", p_adc, 0}, {"dacout", DAC_out, 0},\
                            {"shut", shutdown_RF, 0}, {"fixed", start_fixed, 0}, {"lci", LCD_ini, 0}, {"lc4", LCD_4_bits, 0},\
                            {"lcw", LCD_write, 0}, {"lcc", LCD_char, 0}, {"lcr", LCD_status, 0}, {"pwm", Set_PWM_duty, 0}, \
                            {"fan", Set_fan_out, 0},

extern void AT37_I2C_Temperature(void);
extern void AT37_I2C_Pointer(void);
extern void AT37_mem_write(void);   /* Do byte write to memory of AT37xxxx */
extern void AT37_mem_read(void);
extern void start_fixed(void);
extern void shutdown_RF(void);
extern void setpot(void);
extern void AD_start(void);
extern void DAC_out(void);
extern void Freq_count(void);
extern void p_adc(void);
extern void LCD_ini(void);
extern void LCD_4_bits(void);
extern void LCD_write(void);
extern void LCD_char(void);
extern void LCD_status(void);
extern void Set_PWM_duty(void);
extern void Set_fan_out(void);

#endif /* HW_WORDS_H_ */
