#ifndef I2C_INTERFACE_H
#define I2C_INTERFACE_H



typedef enum
{
	TWI_OK = 1,
	TWI_NOK = 2,
	TWI_STartCond_Error = 3,
	TWI_ReStartCond_Error = 4,
	TWI_SlaveAddWithWriteError = 5,
	TWI_SlaveAddWithReadError = 6,
	TWI_MasterTxDataError = 7,
	TWI_MasterRxDataError = 8,

}TWI_EnumError_Status;

TWI_EnumError_Status READDATATEST(u8 *Data);

void I2C_INIT(void);

void I2C_START(void);

void I2C_STOP(void);

void I2C_WriteByte(u8 Data);

u8 I2C_ReadByteWith_ACK(void);

u8 I2C_ReadByteWith_NACK(void);

u8 I2C_Status(void);
/***********************************/

TWI_EnumError_Status I2C_MasterInit(u32 cpu_freq , u32 scl_freq);

TWI_EnumError_Status I2C_SlaveInit(u8 slave_address );

TWI_EnumError_Status I2C_SendStartCondition(void);

TWI_EnumError_Status I2C_SendRepeatedStart(void);

TWI_EnumError_Status I2C_SendStopCondition(void);

TWI_EnumError_Status I2C_SendSlaveAddressWrite(u8 slave_address);

TWI_EnumError_Status I2C_SendSlaveAddressRead(u8 slave_address);

TWI_EnumError_Status I2C_SendDataByte(u8 data);

TWI_EnumError_Status I2C_ReadDataByteWithAck(u8 *data);
TWI_EnumError_Status I2C_ReadDataByteWithNack(u8 *data);

#endif
