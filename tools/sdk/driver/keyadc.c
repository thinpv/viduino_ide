#include <keyadc.h>
#include <io.h>

#define DEBUG( ... ) // printf(##__VA_ARGS__)

#define KEYADC_UNKNOW 0
#define KEYADC_DOWN 1
#define KEYADC_UP 2

static int count = 0;
static int key_func = KEYADC_UNKNOW;
static int adc_value = 0;

void keyadc_interrupt_handle(int arg)
{
	uint32_t data = KEYADC->KEYADC_INTS_REG;
	KEYADC->KEYADC_INTS_REG = data;
	if ((data & 0x3) && (count < 100))
		count++;

	//down
	if (count == 3)
	{
		key_func = KEYADC_DOWN;
		adc_value = (KEYADC->KEYADC_DATA_REG & 0x3f) * 200 / 63;
		DEBUG("adc_value=%d\r\n", adc_value);
	}

	// up
	if (data & 0x10)
	{
		count = 0;
		key_func = KEYADC_UP;
	}
}

void keyadc_init()
{
	/* Transformation mode */
	C_Value(KEYADC->KEYADC_CTRL_REG, 12, 0x3);
	S_Value(KEYADC->KEYADC_CTRL_REG, 12, 0); //ADC_NormalMode

	/*ADC HOLD DIS*/
	//	C_BIT(KEYADC_CTRL_REG,6);
	/* Set UP DOWN value */
	C_Value(KEYADC->KEYADC_CTRL_REG, 4, 0x3);
	S_Value(KEYADC->KEYADC_CTRL_REG, 4, 0); //ADC_1_9_V
	//
	/* Enable interrupt UP DOWN DATA */
	S_Bit(KEYADC->KEYADC_INTC_REG, 0); //Data interruption
	S_Bit(KEYADC->KEYADC_INTC_REG, 1); //down interrupt
	S_Bit(KEYADC->KEYADC_INTC_REG, 4); //up interrupt

	irq_register(IRQ_LEVEL_1, F1C100S_IRQ_KEYADC, keyadc_interrupt_handle, 3);
	S_Bit(KEYADC->KEYADC_CTRL_REG, 0);
}

static int keyad[] = {12, 57, 85, 101, 114, 123};

int keyadc_getkey()
{
	int i;
	switch (key_func)
	{
	case KEYADC_DOWN:
		for (i = 0; i < 6; i++)
		{
			if ((adc_value > (keyad[i] - 5)) && (adc_value < (keyad[i] + 5)))
			{
				// KEY_v |= (1 << (i));
				DEBUG("key %d\r\n", i);
				break;
			}
		}
		break;
	case KEYADC_UP:
		DEBUG("KEYADC_UP\r\n");
		break;

	default:
		break;
	}
	key_func = KEYADC_UNKNOW;
	return 0;
}