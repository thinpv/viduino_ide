
/////////////////////////////////////////////////////////////////////
// Mapper 77
class NES_mapper77 : public NES_mapper
{
  friend void adopt_MPRD(SnssMapperBlock *block, NES *nes);
  friend int extract_MPRD(SnssMapperBlock *block, NES *nes);

public:
  NES_mapper77(NES *parent) : NES_mapper(parent) {}
  ~NES_mapper77() {}

  void Reset();

  void MemoryWrite(uint32 addr, uint8 data);

protected:
private:
};
/////////////////////////////////////////////////////////////////////
