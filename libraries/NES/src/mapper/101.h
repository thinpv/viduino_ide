
/////////////////////////////////////////////////////////////////////
// Mapper 101
class NES_mapper101 : public NES_mapper
{
  friend void adopt_MPRD(SnssMapperBlock *block, NES *nes);
  friend int extract_MPRD(SnssMapperBlock *block, NES *nes);

public:
  NES_mapper101(NES *parent) : NES_mapper(parent) {}
  ~NES_mapper101() {}

  void Reset();
  void MemoryWriteSaveRAM(uint32 addr, uint8 data);
  void MemoryWrite(uint32 addr, uint8 data);

protected:
private:
};
/////////////////////////////////////////////////////////////////////
