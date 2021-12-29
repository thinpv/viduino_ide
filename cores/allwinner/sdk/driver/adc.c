#include "adc.h"
#include <F1C100S.h>

void Init_TP_ADC(/* int mode */)
{
	/*配置IO*/
	// Init_TPADC_IO();

	/*24M / 6 = 4Mhz(CLK_IN)/ 2^13(8192) = 488.28125 hz*/
	/*Conversion Time = 1 / (4MHz/13Cycles) = 3.25us*/
	/*触摸ADC获取时间T_ACQ  = CLK_IN /(16*(1+63)) = 3906.25hz 左右*/
	// write32(TP_CTRL_REG0, (0x2 << 20) | (0x1f << 23) | (0x7 << 16) | 63);
  TP->TP_CTRL_REG0 = (0x2 << 20) | (0x1f << 23) | (0x7 << 16) | 63;

	/*Mode setting*/
	// write32(TP_CTRL_REG1, (5 << 12) | (1 << 9));
  TP->TP_CTRL_REG1 = (5 << 12) | (1 << 9);
	// if (mode == ADC_MODE)
	// 	S_BIT(TP_CTRL_REG1, 4); //ADC
	// else
	// 	C_BIT(TP_CTRL_REG1, 4); //TP
  S_Bit(TP->TP_CTRL_REG1, 4);

	// write32(TP_CTRL_REG2, ((u64_t)0x08 << 28) | (0x0 << 26) | (0 << 24) | 0xFFF);
  TP->TP_CTRL_REG2 = ((u64_t)0x08 << 28) | (0x0 << 26) | (0 << 24) | 0xFFF;

	/*Filter settings */
	// write32(TP_CTRL_REG3, read32(TP_CTRL_REG3) | (0x1) << 2 | (0x3) << 0);
  TP->TP_CTRL_REG3 |= (0x1) << 2 | (0x3) << 0;

	/*Enable ADC */
	S_Bit(TP->TP_CTRL_REG1, 5);
}

/*Set ADC channel */
void Set_ADC_Channel(int Channel)
{
	C_Value(TP->TP_CTRL_REG1, 0, 0xf);
	S_Value(TP->TP_CTRL_REG1, 0, Channel);
}

/*Read AD value */
int TP_ADC_Read_Value(void)
{
	return (TP->TP_DATA_REG & 0xfff);
}