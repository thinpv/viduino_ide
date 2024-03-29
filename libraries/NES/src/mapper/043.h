
/////////////////////////////////////////////////////////////////////
// Mapper 43
class NES_mapper43 : public NES_mapper
{
  friend void adopt_MPRD(SnssMapperBlock *block, NES *nes);
  friend int extract_MPRD(SnssMapperBlock *block, NES *nes);

public:
  NES_mapper43(NES *parent) : NES_mapper(parent) {}
  ~NES_mapper43() {}

  void Reset();
  uint8 MemoryReadLow(uint32 addr);
  void MemoryWriteLow(uint32 addr, uint8 data);
  void MemoryWrite(uint32 addr, uint8 data);
  void HSync(uint32 scanline);

protected:
  uint8 irq_enabled;
  uint32 irq_counter;

private:
};
/////////////////////////////////////////////////////////////////////
