#include	<stdio.h>
#include	<ctype.h>
#include	"sizedefs.h"
#include	"macro.h"
#include	"srec.h"
#include        "port.h"

#define FREAD(x,y) if ((y = fgetc (x)) == EOF) return 0
#define CFREAD(x,y) if ((y = fgetc (x)) == EOF) return 0;\
			icheck = (icheck << 4) + htoc (y)

#define	SREC_MAXCOUNT		64
#define SREC_EOF        1	/* end of file before S9 record */
#define SREC_S9         2	/* S9 record reached */
#define SREC_CHECKSUM   3	/* checksum error in record */
#define SREC_FORMAT     4	/* S-record format error */

typedef struct {
  int rectype;
  int reclen;
  LONG address;
  char bytes[SREC_MAXCOUNT];
} srecord;

static BYTE checksum;
static char DefaultRecordString[] = ".";
char *PrintEachRecord = DefaultRecordString;
LONG ExecuteAddress;

/* htoc returns binary value of hex character */

static int htoc(char c)
{
  c = toupper(c);
  if (c > '9')
    c -= ('A' - 10);
  else
    c -= '0';
  return c;
}

/* collect returns the value of the next (howmany) character pairs *
   counter is decremented for each character pair read */

static long collect(int howmany, int *counter, FILE * infile)
{
  long stuff;
  int icheck, c;

  for (stuff = 0; howmany--;) {
    icheck = 0;
    CFREAD(infile, c);
    stuff = (stuff << 4) + htoc(c);
    CFREAD(infile, c);
    stuff = (stuff << 4) + htoc(c);
    (*counter)--;
    checksum += icheck;
  }
  return (stuff);
}

static BYTE sr_fetch(BYTE ** where, BYTE * ctr)
{
  BYTE temp;

  (*ctr)--;
  temp = **where;
  (*where)++;
  return temp;
}

int do_srec(srecord * where, FILE * infile)
{
  int ctr, asize, c;
  BYTE *put, s9flag = 0;

  do
    c = fgetc(infile);
  while (c != 'S' && c != EOF);
  if (c == EOF)
    return SREC_EOF;
  FREAD(infile, where->rectype);
  switch (where->rectype) {
    case '0':
      asize = 2;
      break;

    case '1':
    case '2':
    case '3':
      asize = 1 + where->rectype - '0';
      break;

    case '7':
    case '8':
    case '9':
      asize = 11 - (where->rectype - '0');
      s9flag = 1;
      break;

    default:
      return SREC_FORMAT;
  }
  checksum = 0;
  where->reclen = collect(1, &c, infile);
  where->address = 0;
  for (ctr = asize; ctr; ctr--) {
    where->address <<= 8;
    where->address |= collect(1, &where->reclen, infile);
  }
  put = where->bytes;
  for (ctr = where->reclen - 1; ctr;)
    *put++ = collect(1, &ctr, infile);
  collect(1, &where->reclen, infile);
  if (checksum != 0xff)
    return SREC_CHECKSUM;
  where->reclen += 4;
  return s9flag ? SREC_S9 : 0;
}

void put_srec(srecord * data, LONG load_offset)
{
  LONG temp, address;
  BYTE reclen, *where;

  if (data->reclen < 5)
    return;
  reclen = data->reclen - 4;
  address = data->address;
  if (data->rectype >= '7' && data->rectype <= '9') {
    ExecuteAddress = address;
    return;
  }
  if (data->rectype == '0')
    return;
  where = data->bytes;
  if (reclen && (load_offset + address) & 1) {
    PutByte(load_offset + address, sr_fetch(&where, &reclen));
    address++;
  }
  else if (reclen >= 2) {
    temp = sr_fetch(&where, &reclen);
    PutWord(load_offset + address, (WORD) ((temp << 8) + sr_fetch(&where, &reclen)));
    address += 2;
  }
  else if (reclen) {
    PutByte(load_offset + address, sr_fetch(&where, &reclen));
    address++;
  }
  while (reclen >= 4) {
    temp = sr_fetch(&where, &reclen);
    temp = (temp << 8) + sr_fetch(&where, &reclen);
    temp = (temp << 8) + sr_fetch(&where, &reclen);
    FillLong(load_offset + address, (LONG) ((temp << 8) + sr_fetch(&where, &reclen)));
    address += 4;
  }
  if (reclen & 2) {
    temp = sr_fetch(&where, &reclen);
    FillWord(address, (WORD) ((temp << 8) + sr_fetch(&where, &reclen)));
    address += 2;
  }
  if (reclen) {
    FillByte(address, sr_fetch(&where, &reclen));
    address++;
  }
}

unsigned long do_load(LONG load_offset, LONG mask, char *infilename)
{
  srecord buff;
  int error, rcount, fcset;
  FILE *infile;
  long MaxAddress;

  infile = fopen(infilename, "r");

  if (!infile) {
    printf("Couldn't open %s for download\r\n",infilename);
    return 0;
  }
  ExecuteAddress = 0;
  MaxAddress = 0;
  StopChip();
  fcset = set_fc();
  for (error = rcount = 0; !error; rcount++) {
    error = do_srec(&buff, infile);
    buff.address = buff.address & mask;
    if (!error || error == SREC_S9) {
      if (buff.address + buff.reclen > MaxAddress)
        MaxAddress = buff.address + buff.reclen;
      put_srec(&buff, load_offset);
    }
    if (PrintEachRecord) {
      printf(PrintEachRecord);
      fflush(NULL);
    }
  }
  putchar('\n');
  switch (error) {
    case SREC_EOF:
      printf("EOF Reached before S9 record on line %d\n", rcount);
      break;

    case SREC_CHECKSUM:
      printf("Checksum error in S-record no. %d\n", rcount);
      break;

    case SREC_S9:
      printf("Download completed OK - %d records read\n", rcount);
      error = 0;
      break;

    case SREC_FORMAT:
      printf("Format error on line %d - file probably not S-records\n",
	     rcount);
      break;

    default:
      printf("Internal error - do_srec returned %d on line %d\n",
	     error, rcount);
  }
  fclose(infile);
  if (fcset)
    restore_fc();
  return MaxAddress;
}
