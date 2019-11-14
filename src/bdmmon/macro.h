extern int set_fc(void);
extern int restore_fc(void);
extern void SetFC(unsigned);
extern void RunChip(LONG where);
extern LONG GetByte(LONG Where);
extern LONG GetWord(LONG Where);
extern LONG GetLong(LONG Where);
extern LONG DumpByte(LONG Where);
extern LONG DumpWord(LONG Where);
extern LONG DumpLong(LONG Where);
extern void PutByte(LONG Where, BYTE Data);
extern void PutWord(LONG Where, WORD Data);
extern void PutLong(LONG Where, LONG Data);
extern void FillByte(LONG Where, BYTE Data);
extern void FillWord(LONG Where, WORD Data);
extern void FillLong(LONG Where, LONG Data);
extern LONG GetReg(unsigned which);
extern void PutReg(unsigned which, LONG Data);
extern void DumpRegisters(void);
void bdm_write(LONG addr, int pcount, WORD cmd,...);




