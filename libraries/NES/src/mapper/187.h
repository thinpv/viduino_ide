
/////////////////////////////////////////////////////////////////////
// Mapper 187
class NES_mapper187 : public NES_mapper
{
  friend void adopt_MPRD(SnssMapperBlock *block, NES *nes);
  friend int extract_MPRD(SnssMapperBlock *block, NES *nes);

public:
  NES_mapper187(NES *parent) : NES_mapper(parent) {}
  ~NES_mapper187() {}

  void Reset();
  void MemoryWriteLow(uint32 addr, uint8 data);
  void MemoryWrite(uint32 addr, uint8 data);
  void HSync(uint32 scanline);

protected:
  uint8 regs[8];
  uint32 ex_bank_enabled, ex_bank_mode;
  uint32 prg0, prg1, ex_prg0, ex_prg1;
  uint32 chr01, chr23, chr4, chr5, chr6, chr7;
  uint32 chr_swap() { return regs[0] & 0x80; }
  uint32 prg_swap() { return regs[0] & 0x40; }
  uint8 irq_enabled; // IRQs enabled
  uint8 irq_counter; // IRQ scanline counter, decreasing
  uint8 irq_latch;   // IRQ scanline counter latch
  void MMC3_set_CPU_banks();
  void MMC3_set_PPU_banks();

private:
};
/////////////////////////////////////////////////////////////////////
