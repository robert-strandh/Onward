/*
 * HW_words.c
 *
 * Created: 2014-02-01 05:57:38
 *  Author: jost1099
 */ 

/* HW_words.h also includes F_defs.h */
#include "HW_words.h"

void Set_fan_out(void)
{
    Set_fan_speed(Cpop().c);
}

void Set_PWM_duty(void)
{
    Stack_item temp =Cpop();
    
    Set_PWM_out(temp.n, Cpop().c);
}

void LCD_status(void)
{
    Stack_item temp;
    
    temp.c =LCD_read_status();
    
    Cpush(temp);
}

void LCD_write(void)
{
    Write_control_code(Cpop().c);
}

void LCD_char(void)
{
    LCD_putc(Cpop().c);
}

void LCD_ini(void)
{
    Init_HD44780_LCD();
}

void LCD_4_bits()
{
    LCD_4_bit_init();
}

void start_fixed(void)
{
    exe_flag =fixed_delay;
}

void shutdown_RF(void)
{
    exe_flag =shutdown;
}

/* HW dependent words */
void p_adc(void)
/* Syntax "ad?" */
{
    print_ADC_channels();
}

void setpot(void)
/* Set wiper position of AD5282 POT A or B dpending on channel */
/* Syntax "<channel> <wiper pos> setpos" */
{
    int channel =Cpop().n;
    Set_AD5282_wiper_pos(channel, Cpop().n);
}

void AD_start(void)
{
    //Start_ADC_meas(Cpop().c);
}

void DAC_out(void)
/* Syntax " <ch> <value> dac_out" where 'ch' =0 or 1 */
/* and 'value' = 0..1023 for SAM4S */
{
    #ifdef SAM4S
    uint32_t temp =Cpop().n;
    
    Write_to_DAC(Cpop().n, temp);
    #endif
}

void Freq_count(void)
{
    Stack_item temp;
    //temp.a_address =&Prescaler_val;
    Cpush(temp);
    //temp.c_address =&Counter_value;
    Cpush(temp);
}

void AT37_I2C_Temperature(void)
/* Read the contents of the temperature register of the AT37xxxx temperature sensor */
/* Syntax "<device no> temp" */
{
    //int device =Cpop().n;
    Stack_item temp;
    
    //temp.n= Read_AT37_temperature((uint8_t)device);
    temp.n= Current_temp;
    Cpush(temp);
}

void AT37_I2C_Pointer(void)
/* Write pointer value to the pointer address of the AT37xxxx temperature sensor */
/* Syntax "<device no> <pointer reg value> point" */
{
    int pointer_data =Cpop().n;
    int device =Cpop().n;
    
    Set_AT37_pointerreg((uint8_t)device, (uint8_t)pointer_data);
}

void AT37_mem_write(void)
/* Do byte write to memory of AT37xxxx */
{
    uint8_t write_data = Cpop().c;
    int address =Cpop().n;
    int device =Cpop().n;
    
    AT37_byte_write((uint8_t)device, (uint16_t)address, &write_data, 1);
}

void AT37_mem_read(void)
/* Do byte write to memory of AT37xxxx */
{
    int address =Cpop().n;
    int device =Cpop().n;
    uint8_t data;
    Stack_item temp;
    
    AT37_random_read((uint8_t)device, (uint16_t)address, &data);
    
    temp.n =data;
    Cpush(temp);
}
