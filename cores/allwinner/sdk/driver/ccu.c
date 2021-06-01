#include <ccu.h>

void ccu_set_enable_cpu(bool enable)
{
	if (enable)
		CCU->PLL_CPU_CTRL_REG |= (1 << 31);
	else
		CCU->PLL_CPU_CTRL_REG &= ~(1 << 31);
}

void ccu_set_enable_audio(bool enable)
{
	if (enable)
		CCU->PLL_AUDIO_CTRL_REG |= (1 << 31);
	else
		CCU->PLL_AUDIO_CTRL_REG &= ~(1 << 31);
}

void ccu_set_enable_video(bool enable)
{
	if (enable)
		CCU->PLL_VIDEO_CTRL_REG |= (1 << 31);
	else
		CCU->PLL_VIDEO_CTRL_REG &= ~(1 << 31);
}

void ccu_set_enable_ve(bool enable)
{
	if (enable)
		CCU->PLL_VE_CTRL_REG |= (1 << 31);
	else
		CCU->PLL_VE_CTRL_REG &= ~(1 << 31);
}

void ccu_set_enable_ddr(bool enable)
{
	if (enable)
		CCU->PLL_DDR_CTRL_REG |= (1 << 31);
	else
		CCU->PLL_DDR_CTRL_REG &= ~(1 << 31);
}

void ccu_set_enable_periph(bool enable)
{
	if (enable)
		CCU->PLL_PERIPH_CTRL_REG |= (1 << 31);
	else
		CCU->PLL_PERIPH_CTRL_REG &= ~(1 << 31);
}

uint64_t ccu_get_rate_cpu(uint64_t prate)
{
	uint32_t r, n, k, m, p;
	r = CCU->PLL_CPU_CTRL_REG;
	n = ((r >> 8) & 0x1f) + 1;
	k = ((r >> 4) & 0x3) + 1;
	m = ((r >> 0) & 0x3) + 1;
	p = ((r >> 16) & 0x3) + 1;
	return (uint64_t)((prate * n * k) / (m * p));
}

uint64_t ccu_get_rate_audio(uint64_t prate)
{
	uint32_t r, n, m;
	r = CCU->PLL_AUDIO_CTRL_REG;
	if (r & (1 << 24))
		n = ((r >> 8) & 0xf) + 1;
	else
		n = ((r >> 8) & 0x7f) + 1;
	m = ((r >> 0) & 0x1f) + 1;
	return (uint64_t)((prate * n * 2) / m);
}

uint64_t ccu_get_rate_video(uint64_t prate)
{
	uint32_t r, n, m;
	r = CCU->PLL_VIDEO_CTRL_REG;
	if (r & (1 << 24))
	{
		n = ((r >> 8) & 0x7f) + 1;
		m = ((r >> 0) & 0xf) + 1;
		return (uint64_t)((prate * n) / m);
	}
	else
	{
		if (r & (1 << 25))
			return 297 * 1000 * 1000;
		else
			return 270 * 1000 * 1000;
	}
}

uint64_t ccu_get_rate_ve(uint64_t prate)
{
	uint32_t r, n, m;
	r = CCU->PLL_VE_CTRL_REG;
	if (r & (1 << 24))
	{
		n = ((r >> 8) & 0x7f) + 1;
		m = ((r >> 0) & 0xf) + 1;
		return (uint64_t)((prate * n) / m);
	}
	else
	{
		if (r & (1 << 25))
			return 297 * 1000 * 1000;
		else
			return 270 * 1000 * 1000;
	}
}

uint64_t ccu_get_rate_ddr(uint64_t prate)
{
	uint32_t r, n, k, m;
	r = CCU->PLL_DDR_CTRL_REG;
	n = ((r >> 8) & 0x1f) + 1;
	k = ((r >> 4) & 0x3) + 1;
	m = ((r >> 0) & 0x3) + 1;
	return (uint64_t)((prate * n * k) / m);
}

uint64_t ccu_get_rate_periph(uint64_t prate)
{
	uint32_t r, n, k, m, p;
	r = CCU->PLL_PERIPH_CTRL_REG;
	n = ((r >> 8) & 0x1f) + 1;
	k = ((r >> 4) & 0x3) + 1;
	m = ((r >> 0) & 0x3) + 1;
	p = ((r >> 16) & 0x3) + 1;
	return (uint64_t)((prate * n * k) / p);
}

void ccu_reset(int32_t bit, bool enable)
{
	vuint32_t addr;
	if (bit >= 96)
		return;
	else if (bit >= 64)
	{
		bit -= 64;
		if (enable)
		{
			CCU->BUS_CLK_GATING_REG2 |= (1 << bit);
			CCU->BUS_SOFT_RST_REG2 |= (1 << bit);
		}
		else
		{
			CCU->BUS_CLK_GATING_REG2 &= ~(1 << bit);
			CCU->BUS_SOFT_RST_REG2 &= ~(1 << bit);
		}
	}
	else if (bit >= 32)
	{
		bit -= 32;
		if (enable)
		{
			CCU->BUS_CLK_GATING_REG1 |= (1 << bit);
			CCU->BUS_SOFT_RST_REG1 |= (1 << bit);
		}
		else
		{
			CCU->BUS_CLK_GATING_REG1 &= ~(1 << bit);
			CCU->BUS_SOFT_RST_REG1 &= ~(1 << bit);
		}
	}
	else if (bit >= 0)
	{
		if (enable)
		{
			CCU->BUS_CLK_GATING_REG0 |= (1 << bit);
			CCU->BUS_SOFT_RST_REG0 |= (1 << bit);
		}
		else
		{
			CCU->BUS_CLK_GATING_REG0 &= ~(1 << bit);
			CCU->BUS_SOFT_RST_REG0 &= ~(1 << bit);
		}
	}
}
