//#include "main.h"
//
//
//void I2C2_clock_high(void)
//{
////  gpio_write_pin(PIOD, 28, HIGH);
//    PIOD_REGS->PIO_SODR |= (PIO_PD28);
//}
//
//void I2C2_clock_low(void)
//{
////  gpio_write_pin(PIOD, 28, LOW);
//    PIOD_REGS->PIO_CODR |= (PIO_PD28);
//}
//
//void I2C2_data_high(void)
//{
////  gpio_write_pin(PIOD, 27, HIGH);
//  PIOD_REGS->PIO_SODR |= (PIO_PD27);
//}
//
//void I2C2_data_low(void)
//{
////  gpio_write_pin(PIOD, 27, LOW);
//    PIOD_REGS->PIO_CODR |= (PIO_PD27);
//}
//
//void I2C2_clock_output(void)
//{
////  gpio_set_mode(PIOD, 28, OUTPUT); 
//    PIOD_REGS->PIO_OER |= (PIO_PD28);
//}
//
//void I2C2_clock_input(void)
//{
////  gpio_set_mode(PIOD, 28, INPUT); 
//  PIOD_REGS->PIO_ODR |= (PIO_PD28);
//}
//
//void I2C2_data_output(void)
//{
////  gpio_set_mode(PIOD, 27, OUTPUT);
//    PIOD_REGS->PIO_OER |= (PIO_PD27);
//}
//
//void I2C2_data_input(void)
//{
////  gpio_set_mode(PIOD, 27, INPUT);
//    PIOD_REGS->PIO_ODR |= (PIO_PD27);
//}
//
//uint8_t I2C2_clock_read(void)
//{
//  return (((PIOD_REGS->PIO_PDSR)>>28) & 0x01);
//}
//
//uint8_t I2C2_data_read(void)
//{
//  return (((PIOD_REGS->PIO_PDSR)>>27) & 0x01);
//}
//
//
//void I2C2_init(void)
//{
////  Peripheral_init(ID_PIOD);
////  
////  gpio_peripheral_enable(PIOD,27);      //data
////  gpio_peripheral_enable(PIOD,28);      //clock
//    
//    PIOD_REGS->PIO_PER |= (PIO_PD27);
//    PIOD_REGS->PIO_PER |= (PIO_PD28);
//    
//    I2C2_clock_output(); 
//    I2C2_data_output(); 
//}
//
//void I2C2_start(void)
//{
//  I2C2_clock_output(); 
//  I2C2_data_output();
//  
//  I2C2_data_high();
//  I2C2_clock_high();
//  I2C2_data_low();
//  I2C2_clock_low();
//}
//
//void I2C2_stop(void)
//{
//  I2C2_clock_output(); 
//  I2C2_data_output();
//  
//  I2C2_clock_low();
//  I2C2_data_low();
//  I2C2_clock_high();
//  I2C2_data_high(); 
//}
//
//void I2C2_write_byte(uint8_t data)
//{
//  I2C2_clock_output(); 
//  I2C2_data_output();
//  
//  unsigned char outBits;
//  unsigned char inBit;
//  
//  /* 8 bits */
//  for(outBits = 0; outBits < 8; outBits++) 
//  {
//    if(data & 0x80)
//    {
//      I2C2_data_high();
//      SYSTICK_DelayUs(1);
//    }
//    else
//    {
//      I2C2_data_low();
//      SYSTICK_DelayUs(1);
//    }
//    data  <<= 1;
//    /* Generate clock for 8 data bits */
//    I2C2_clock_high();
//    SYSTICK_DelayUs(1);
//    I2C2_clock_low();
//    SYSTICK_DelayUs(1);
//  }
//  
//  /* Generate clock for ACK */
//  I2C2_clock_high();
//  SYSTICK_DelayUs(1);
//  
//  I2C2_clock_input();
//  SYSTICK_DelayUs(1);
//  /* Wait for clock to go high, clock stretching */
//  while(I2C2_clock_read());
//  /* Clock high, valid ACK */
//  inBit = I2C2_data_read();
//  I2C2_clock_low();
//  SYSTICK_DelayUs(1);
//}
//
//void  writeI2CSlave (uint8_t slave_address, uint8_t *data, uint32_t len)	
//{
//  /* Start */
//  I2C2_start();
//  /* Slave address */
//  I2C2_write_byte((slave_address<<1) & 0xFE);
//  /* Slave data */
//  while(len)
//  {
//    I2C2_write_byte(*data++);
//    len--;
//  }
//  /* Stop */
//  I2C2_stop();
//}
//
//void I2C2_read_byte(uint8_t data)
//{
////  static uint8_t inData = 0;
//  
//  I2C2_clock_output(); 
//  I2C2_data_input();
//  
//  for(int i=0;i<8;i++)
//  {
//    data <<= 1;
//    I2C2_clock_high();
//    SYSTICK_DelayUs(1);
//    
//    data |= I2C2_data_read();
//    
//    I2C2_clock_low();
//    SYSTICK_DelayUs(1);
//  }
//  
//  //send ack to the i2c slave
//  I2C2_data_output();
//  
//  
//  I2C2_data_low();
//  SYSTICK_DelayUs(1);
//  I2C2_clock_high();
//  SYSTICK_DelayUs(1);
//  I2C2_clock_low();
//  SYSTICK_DelayUs(1);
//  I2C2_data_high();
//  SYSTICK_DelayUs(1);
//  I2C2_clock_high();
//  SYSTICK_DelayUs(1);
//  //added for clock stretching to be tested
//    /* Wait for clock to go high, clock stretching */
//  while(I2C2_clock_read());
//  
////  return inData;
//}
//
//void readI2CSlave (uint8_t slave_address, uint8_t *data, uint32_t len)	
//{
//  static uint8_t temp = 0;
//  /* Start */
//  I2C2_start();
//  /* Slave address */
//  I2C2_write_byte((slave_address<<1) | 0x01);
//  
//  while(len)
//  {
//    temp = 0;
//    
//    I2C2_clock_output(); 
//    I2C2_data_input();
//    
//    for(int i=0;i<8;i++)
//    {
//      temp <<= 1;
//      I2C2_clock_high();
//      SYSTICK_DelayUs(1);
//      
//      temp |= (((PIOD_REGS->PIO_PDSR)>>27) & 0x01);
//      I2C2_clock_low();
//    //  delay_us(1);
//    }
//    
//    //send ack to the i2c slave
//    I2C2_data_output();
//    
//    I2C2_data_low();
//    SYSTICK_DelayUs(1);
//    I2C2_clock_high();
//    SYSTICK_DelayUs(1);
//    I2C2_clock_low();
//    SYSTICK_DelayUs(1);
//    I2C2_data_high();
//    SYSTICK_DelayUs(1);
//    I2C2_clock_high();
//    SYSTICK_DelayUs(1);
//    
//    len = len--;//len = 0;
//  }
//  /* Stop */
//  I2C2_stop();
//}