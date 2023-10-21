#ifndef __tf_h__
#define __tf_h__

#include <stdlib.h>
#include "debug.h"

#define TF_CS(n)   (n ? GPIO_WriteBit(GPIOB, GPIO_Pin_6, SET) : GPIO_WriteBit(GPIOB, GPIO_Pin_6, RESET))





/* ������Ӧ���� define command's response */
#define R1 1
#define R1B 2
#define R2 3
#define R3 4


/******************************** ������� Basic command set **************************/
/* ��λSD �� Reset cards to idle state */
#define CMD0 0
#define CMD0_R R1

/* ��OCR�Ĵ��� Read the OCR (MMC mode, do not use for SD cards) */
#define CMD1 1
#define CMD1_R R1

/* ��CSD�Ĵ��� Card sends the CSD */
#define CMD9 9
#define CMD9_R R1

/* ��CID�Ĵ��� Card sends CID */
#define CMD10 10
#define CMD10_R R1

/* ֹͣ�����ʱ�����ݴ��� Stop a multiple block (stream) read/write operation */
#define CMD12 12
#define CMD12_R R1B

/* �� Card_Status �Ĵ��� Get the addressed card's status register */
#define CMD13 13
#define CMD13_R R2

/***************************** ������ Block read commands **************************/

/* ���ÿ�ĳ��� Set the block length */
#define CMD16 16
#define CMD16_R R1

/* ������ Read a single block */
#define CMD17 17
#define CMD17_R R1

/* �����,ֱ����������CMD12Ϊֹ Read multiple blocks until a CMD12 */
#define CMD18 18
#define CMD18_R R1

/***************************** ��д��� Block write commands *************************/
/* д���� Write a block of the size selected with CMD16 */
#define CMD24 24
#define CMD24_R R1

/* д��� Multiple block write until a CMD12 */
#define CMD25 25
#define CMD25_R R1

/* дCSD�Ĵ��� Program the programmable bits of the CSD */
#define CMD27 27
#define CMD27_R R1

/***************************** д���� Write protection *****************************/
/* Set the write protection bit of the addressed group */
#define CMD28 28
#define CMD28_R R1B

/* Clear the write protection bit of the addressed group */
#define CMD29 29
#define CMD29_R R1B

/* Ask the card for the status of the write protection bits */
#define CMD30 30
#define CMD30_R R1

/***************************** �������� Erase commands *******************************/
/* ���ò��������ʼ��ַ(ֻ����SD��) Set the address of the first write block to be erased(only for SD) */
#define CMD32 32
#define CMD32_R R1

/* ���ò��������ֹ��ַ(ֻ����SD��) Set the address of the last write block to be erased(only for SD) */
#define CMD33 33
#define CMD33_R R1

/* ���ò��������ʼ��ַ(ֻ����MMC��) Set the address of the first write block to be erased(only for MMC) */
#define CMD35 35
#define CMD35_R R1

/* ���ò��������ֹ��ַ(ֻ����MMC��) Set the address of the last write block to be erased(only for MMC) */
#define CMD36 36
#define CMD36_R R1

/* ������ѡ��Ŀ� Erase the selected write blocks */
#define CMD38 38
#define CMD38_R R1B



#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD����Ӧ��־��
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF










#define CMD41   41








/***************************** �������� Lock Card commands ***************************/
/* ����/��λ���������/������ Set/reset the password or lock/unlock the card */
#define CMD42 42
#define CMD42_R        R1B
/* Commands from 42 to 54, not defined here */

/***************************** Ӧ������ Application-specific commands ****************/
/* ��ֹ��һ������ΪӦ������  Flag that the next command is application-specific */
#define CMD55 55
#define CMD55_R R1

/* Ӧ�������ͨ��I/O  General purpose I/O for application-specific commands */
#define CMD56 56
#define CMD56_R R1

/* ��OCR�Ĵ���  Read the OCR (SPI mode only) */
#define CMD58 58
#define CMD58_R R3

/* ʹ�ܻ��ֹ CRC Turn CRC on or off */
#define CMD59 59
#define CMD59_R R1

/***************************** Ӧ������ Application-specific commands ***************/
/* ��ȡ SD Status�Ĵ��� Get the SD card's status */
#define ACMD13 13
#define ACMD13_R R2

/* �õ���д�뿨�еĿ�ĸ��� Get the number of written write blocks (Minus errors ) */
#define ACMD22 22
#define ACMD22_R R1

/* ��д֮ǰ,����Ԥ�Ȳ����Ŀ�ĸ��� Set the number of write blocks to be pre-erased before writing */
#define ACMD23 23
#define ACMD23_R R1

/* ��ȡOCR�Ĵ��� Get the card's OCR (SD mode) */
#define ACMD41 41
#define ACMD41_R R1

/* ����/�Ͽ�CD/DATA[3]�����ϵ��������� Connect or disconnect the 50kOhm internal pull-up on CD/DAT[3] */
#define ACMD42 42
#define ACMD42_R R1

/* ��ȡSCR�Ĵ��� Get the SD configuration register */
#define ACMD51 51
#define ACMD51_R R1
















u32 SD_GetSectorCount(void);
void tf_init(void);
void tf_write_reg(u8 reg);

void tf_read(u8 data);

void tf_write(u8 data);
u8 SD_GetCID(u8 *cid_data);
u8 SD_GetCSD(u8 *csd_data);
#endif
