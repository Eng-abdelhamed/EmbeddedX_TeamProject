#include"STD_TYPEs.h"
#include"BIT_MATH.h"
#include <avr/io.h>
#include "I2C_Interface.h"

/* ========== CONFIGURATION MACROS ========== */
#define I2C_TIMEOUT_MAX     1000    // Maximum timeout iterations
#define I2C_SCL_FREQ_400K   400000  // 400kHz frequency
#define I2C_SCL_FREQ_100K   100000  // 100kHz frequency

/* ========== STATUS CODE DEFINITIONS ========== */
#define TWI_START_TRANSMITTED       0x08
#define TWI_REPEATED_START          0x10
#define TWI_SLA_W_ACK               0x18
#define TWI_SLA_R_ACK               0x40
#define TWI_DATA_TRANSMITTED_ACK    0x28
#define TWI_DATA_RECEIVED_ACK       0x50
#define TWI_DATA_RECEIVED_NACK      0x58
#define TWI_TIMEOUT_ERROR 1

/* ========== IMPROVED INITIALIZATION FUNCTIONS ========== */

/**
 * @brief Initialize I2C as Master with configurable frequency
 * @param cpu_freq: CPU frequency in Hz
 * @param scl_freq: Desired SCL frequency in Hz
 * @return TWI_EnumError_Status
 */
TWI_EnumError_Status I2C_MasterInit(u32 cpu_freq, u32 scl_freq)
{
    TWI_EnumError_Status status = TWI_OK;

    // Calculate TWBR value: SCL_freq = CPU_freq / (16 + 2*TWBR*prescaler)
    // Assuming prescaler = 1 (TWPS = 00)
    uint8_t twbr_value = ((cpu_freq / scl_freq) - 16) / 2;

    // Validate TWBR range (must be >= 10 for stable operation)
    if(twbr_value < 10) {
        status = TWI_NOK;
        twbr_value = 10; // Set minimum safe value
    }

    // Clear prescaler bits (set to 1)
    TWSR &= ~((1<<TWPS1) | (1<<TWPS0));

    // Set bit rate register
    TWBR = twbr_value;

    // Enable I2C and ACK
    TWCR = (1<<TWEN) | (1<<TWEA);

    return status;
}

/**
 * @brief Initialize I2C as Slave with proper address handling
 * @param slave_address: 7-bit slave address (0x01 to 0x7F)
 * @return TWI_EnumError_Status
 */
TWI_EnumError_Status I2C_SlaveInit(u8 slave_address)
{
    TWI_EnumError_Status status = TWI_OK;

    // Validate address range (7-bit addresses: 0x01-0x7F)
    if(slave_address == 0x00 || slave_address > 0x7F) {
        return TWI_NOK;
    }

    // Set slave address (left-shifted, bit 0 = 0 for general call recognition)
    TWAR = (slave_address << 1);

    // Enable I2C, ACK, and interrupt (if needed)
    TWCR = (1<<TWEN) | (1<<TWEA);

    return status;
}

/* ========== IMPROVED COMMUNICATION FUNCTIONS ========== */

/**
 * @brief Send start condition with timeout protection
 * @return TWI_EnumError_Status
 */
TWI_EnumError_Status I2C_SendStartCondition(void)
{
    TWI_EnumError_Status status = TWI_OK;
    uint16_t timeout = 0;

    // Send start condition
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

    // Wait for transmission complete with timeout
    while(!(TWCR & (1<<TWINT)) && (timeout < I2C_TIMEOUT_MAX)) {
        timeout++;
    }

    if(timeout >= I2C_TIMEOUT_MAX) {
        return TWI_TIMEOUT_ERROR;
    }

    // Check status code
    if((TWSR & 0xF8) != TWI_START_TRANSMITTED) {
        status = TWI_STartCond_Error;
    }

    return status;
}

/**
 * @brief Send repeated start condition
 * @return TWI_EnumError_Status
 */
TWI_EnumError_Status I2C_SendRepeatedStart(void)
{
    TWI_EnumError_Status status = TWI_OK;
    uint16_t timeout = 0;

    // Send repeated start condition
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

    // Wait with timeout
    while(!(TWCR & (1<<TWINT)) && (timeout < I2C_TIMEOUT_MAX)) {
        timeout++;
    }

    if(timeout >= I2C_TIMEOUT_MAX) {
        return TWI_TIMEOUT_ERROR;
    }

    // Check status code
    if((TWSR & 0xF8) != TWI_REPEATED_START) {
        status = TWI_ReStartCond_Error;
    }

    return status;
}

/**
 * @brief Send stop condition
 * @return TWI_EnumError_Status
 */
TWI_EnumError_Status I2C_SendStopCondition(void)
{
    // Send stop condition
    TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);

    // Note: Stop condition doesn't set TWINT flag, so no waiting required
    // But we can add a small delay for safety
    for(volatile uint8_t i = 0; i < 10; i++);

    return TWI_OK;
}

/**
 * @brief Send slave address with write bit
 * @param slave_address: 7-bit slave address
 * @return TWI_EnumError_Status
 */
TWI_EnumError_Status I2C_SendSlaveAddressWrite(u8 slave_address)
{
    TWI_EnumError_Status status = TWI_OK;
    uint16_t timeout = 0;

    // Load slave address with write bit (LSB = 0)
    TWDR = (slave_address << 1) | 0x00;

    // Start transmission
    TWCR = (1<<TWINT) | (1<<TWEN);

    // Wait with timeout
    while(!(TWCR & (1<<TWINT)) && (timeout < I2C_TIMEOUT_MAX)) {
        timeout++;
    }

    if(timeout >= I2C_TIMEOUT_MAX) {
        return TWI_TIMEOUT_ERROR;
    }

    // Check if slave acknowledged
    if((TWSR & 0xF8) != TWI_SLA_W_ACK) {
        status = TWI_SlaveAddWithWriteError;
    }

    return status;
}

/**
 * @brief Send slave address with read bit
 * @param slave_address: 7-bit slave address
 * @return TWI_EnumError_Status
 */
TWI_EnumError_Status I2C_SendSlaveAddressRead(u8 slave_address)
{
    TWI_EnumError_Status status = TWI_OK;
    uint16_t timeout = 0;

    // Load slave address with read bit (LSB = 1)
    TWDR = (slave_address << 1) | 0x01;

    // Start transmission
    TWCR = (1<<TWINT) | (1<<TWEN);

    // Wait with timeout
    while(!(TWCR & (1<<TWINT)) && (timeout < I2C_TIMEOUT_MAX)) {
        timeout++;
    }

    if(timeout >= I2C_TIMEOUT_MAX) {
        return TWI_TIMEOUT_ERROR;
    }

    // Check if slave acknowledged
    if((TWSR & 0xF8) != TWI_SLA_R_ACK) {
        status = TWI_SlaveAddWithReadError;
    }

    return status;
}

/**
 * @brief Send data byte
 * @param data: Data byte to send
 * @return TWI_EnumError_Status
 */
TWI_EnumError_Status I2C_SendDataByte(u8 data)
{
    TWI_EnumError_Status status = TWI_OK;
    uint16_t timeout = 0;

    // Load data into register
    TWDR = data;

    // Start transmission
    TWCR = (1<<TWINT) | (1<<TWEN);

    // Wait with timeout
    while(!(TWCR & (1<<TWINT)) && (timeout < I2C_TIMEOUT_MAX)) {
        timeout++;
    }

    if(timeout >= I2C_TIMEOUT_MAX) {
        return TWI_TIMEOUT_ERROR;
    }

    // Check if data was acknowledged
    if((TWSR & 0xF8) != TWI_DATA_TRANSMITTED_ACK) {
        status = TWI_MasterTxDataError;
    }

    return status;
}

/**
 * @brief Read data byte with ACK
 * @param data: Pointer to store received data
 * @return TWI_EnumError_Status
 */
TWI_EnumError_Status I2C_ReadDataByteWithAck(uint8_t *data)
{
    TWI_EnumError_Status status = TWI_OK;


     // Your existing code...
     TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
     while (BIT_IS_CLEAR(TWCR , TWINT));
     while ((TWSR & 0xf8) != 0x60);

     TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
     while (BIT_IS_CLEAR(TWCR , TWINT));
     while ((TWSR & 0xf8) != 0x80);

     *data = TWDR;

     // ADD THESE LINES:
     TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);  // Acknowledge data
     while (BIT_IS_CLEAR(TWCR, TWINT));          // Wait for stop
     while ((TWSR & 0xf8) != 0xA0);              // Wait for 0xA0 (STOP)
     TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);  // Acknowledge stop

     return status;
	return status;
}


/**
 * @brief Read data byte with NACK (last byte)
 * @param data: Pointer to store received data
 * @return TWI_EnumError_Status
 */
TWI_EnumError_Status I2C_ReadDataByteWithNack(u8 *data)
{
    TWI_EnumError_Status status = TWI_OK;
	TWCR = (1<<TWEN) | (1<<TWINT)  ;
	while (BIT_IS_CLEAR(TWCR , TWINT)) ;


	TWCR = (1<<TWEN) | (1<<TWINT)  ;
	while (BIT_IS_CLEAR(TWCR , TWINT) == 0) ;
	while ((TWSR & 0xf8) != 0x88) ;

	*data =  TWDR ;
	return status;
}
/* ========== HIGH-LEVEL UTILITY FUNCTIONS ========== */

/**
 * @brief Write multiple bytes to I2C slave
 * @param slave_addr: 7-bit slave address
 * @param data: Pointer to data array
 * @param length: Number of bytes to write
 * @return TWI_EnumError_Status
 */
TWI_EnumError_Status I2C_WriteBytes(uint8_t slave_addr, uint8_t *data, uint8_t length)
{
    TWI_EnumError_Status status;



    // Start condition
    status = I2C_SendStartCondition();
    if(status != TWI_OK) return status;

    // Send slave address with write
    status = I2C_SendSlaveAddressWrite(slave_addr);
    if(status != TWI_OK) {
        I2C_SendStopCondition();
        return status;
    }

    // Send data bytes
    for(uint8_t i = 0; i < length; i++) {
        status = I2C_SendDataByte(data[i]);
        if(status != TWI_OK) {
            I2C_SendStopCondition();
            return status;
        }
    }

    // Stop condition
    I2C_SendStopCondition();
    return TWI_OK;
}

/**
 * @brief Read multiple bytes from I2C slave
 * @param slave_addr: 7-bit slave address
 * @param data: Pointer to data buffer
 * @param length: Number of bytes to read
 * @return TWI_EnumError_Status
 */
TWI_EnumError_Status I2C_ReadBytes(uint8_t slave_addr, uint8_t *data, uint8_t length)
{
    TWI_EnumError_Status status;


    // Start condition
    status = I2C_SendStartCondition();
    if(status != TWI_OK) return status;

    // Send slave address with read
    status = I2C_SendSlaveAddressRead(slave_addr);
    if(status != TWI_OK) {
        I2C_SendStopCondition();
        return status;
    }

    // Read data bytes
    for(uint8_t i = 0; i < length; i++) {
        if(i == length - 1) {
            // Last byte - send NACK
            status = I2C_ReadDataByteWithNack(&data[i]);
        } else {
            // Not last byte - send ACK
        }

        if(status != TWI_OK) {
            I2C_SendStopCondition();
            return status;
        }
    }

    // Stop condition
    I2C_SendStopCondition();
    return TWI_OK;
}

/* ========== LEGACY FUNCTIONS (for backwards compatibility) ========== */

void I2C_INIT(void)
{
    I2C_MasterInit(8000000, 400000); // 8MHz CPU, 400kHz I2C
}

void I2C_START(void)
{
    I2C_SendStartCondition();
}

void I2C_STOP(void)
{
    I2C_SendStopCondition();
}

void I2C_WriteByte(u8 data)
{
    I2C_SendDataByte(data);
}

u8 I2C_ReadByteWith_ACK(void)
{
    u8 data = 0;
    I2C_ReadDataByteWithAck(&data);
    return data;
}

u8 I2C_ReadByteWith_NACK(void)
{
    u8 data = 0;
    I2C_ReadDataByteWithNack(&data);
    return data;
}

u8 I2C_Status(void)
{
    return (TWSR & 0xF8);
}
TWI_EnumError_Status READDATATEST(u8 *Data)
{
	TWI_EnumError_Status Errors = TWI_OK;
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA) ;
	while (BIT_IS_CLEAR(TWCR , TWINT)) ;
	while ((TWSR & 0xf8) != 0x60) ;

	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA) ;
	while (BIT_IS_CLEAR(TWCR , TWINT) == 0) ;
	while ((TWSR & 0xf8) != 0x80) ;

	*Data =  TWDR ;
	return Errors;
}
