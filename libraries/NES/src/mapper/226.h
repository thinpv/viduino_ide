
/////////////////////////////////////////////////////////////////////
// Mapper 226
class NES_mapper226 : public NES_mapper
{
  friend void adopt_MPRD(SnssMapperBlock *block, NES *nes);
  friend int extract_MPRD(SnssMapperBlock *block, NES *nes);

public:
  NES_mapper226(NES *parent) : NES_mapper(parent) {}
  ~NES_mapper226() {}

  void Reset();
  void MemoryWrite(uint32 addr, uint8 data);

protected:
  uint8 regs[2];

private:
};
/////////////////////////////////////////////////////////////////////
