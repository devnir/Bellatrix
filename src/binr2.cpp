/******************************************************************************
* FILE NAME :
* PURPOSE   :
* AUTHOR    :
* $Rev::                                                                       $
* $Date::                                                                      $
* $Author::                                                                    $
* COMMENT   :
******************************************************************************/
/******************************************************************************
* Includes
******************************************************************************/
#include "Binr2.h"
#include "string.h"
#include "crc16.h"
#include "buffFunctions.h"
/******************************************************************************
* Macro Definitions
******************************************************************************/

/******************************************************************************
* Type Definitions
******************************************************************************/

/******************************************************************************
* Local Data
******************************************************************************/
static INT8U preamble[4] = {0xA5, 0x5A, 0xB5, 0x5B};
static INT8U inBuff[4096];
static INT32U bytes_expected = 0;
static INT32U bytes_received = 0;

enum
{
  BINR2_MAX_DATA_SIZE =  2047,
  BINR2_MAX_SIZE      =  (4 + 2 + BINR2_MAX_DATA_SIZE + 2)
};

enum
{
  BINR2_MORE,
  BINR2_EPREAMBLE,
  BINR2_ELENGTH,
  BINR2_EID,
  BINR2_ECHECKSUM,
  BINR2_DONE
};

static BINR2_POINTERS pLib;
/******************************************************************************
* Local Function Prototypes
******************************************************************************/
void receiveStart();
INT8U extract(INT32U Len);
void updateX80(INT32U len);
void updateX90(INT32U len);
void updateX91(INT32U len);
void updateX92(INT32U len);
void updateX93(INT32U len);
void updateX94(INT32U len);
void updateX95(INT32U len);
void updateX96(INT32U len);
void updateX9A(INT32U len);
void updateXA0(INT32U len);
void updateXA1(INT32U len);
void updateXB0(INT32U len);
void updateXB1(INT32U len);
void updateXB2(INT32U len);
void updateXB3(INT32U len);
void updateXB4(INT32U len);
void updateXB5(INT32U len);
void updateXBA(INT32U len);
void updateXC0(INT32U len);
void updateXC1(INT32U len);
void updateXC2(INT32U len);
void updateXC3(INT32U len);
void updateXD0(INT32U len);
void updatexF9(INT32U Len);
/******************************************************************************
* Function Definitions
******************************************************************************/
/******************************************************************************
* OVERVIEW :
* INPUT    :
* OUTPUT   :
* RETURN   :
******************************************************************************/
void Binr2Init(BINR2_POINTERS pointers)
{
  receiveStart();
  memset(&pLib, NULL, sizeof(BINR2_POINTERS));
  pLib.dataBuff = pointers.dataBuff;
  pLib.dataLen = pointers.dataLen;

  pLib.p80 = pointers.p80;
  pLib.p90 = pointers.p90;
  pLib.p91 = pointers.p91;
  pLib.p92 = pointers.p92;
  pLib.p93 = pointers.p93;
  pLib.p94 = pointers.p94;
  pLib.p95 = pointers.p95;
  pLib.p96 = pointers.p96;
  pLib.p9A = pointers.p9A;
  pLib.pA0 = pointers.pA0;
  pLib.pA1 = pointers.pA1;
  pLib.pB0 = pointers.pB0;
  pLib.pB1 = pointers.pB1;
  pLib.pB2 = pointers.pB2;
  pLib.pB3 = pointers.pB3;
  pLib.pB4 = pointers.pB4;
  pLib.pB5 = pointers.pB5;
  pLib.pBA = pointers.pBA;
  pLib.pC0 = pointers.pC0;
  pLib.pC1 = pointers.pC1;
  pLib.pC2 = pointers.pC2;
  pLib.pC3 = pointers.pC3;
  pLib.pD0 = pointers.pD0;
  pLib.pF9 = pointers.pF9;  
}

/******************************************************************************
* OVERVIEW :
* INPUT    :
* OUTPUT   :
* RETURN   :
******************************************************************************/
INT16U Binr2Encode(INT8U *destination, INT8U *source, INT16U num)
{
  INT32U i = 0;
  INT16U crc =0;

  if(num > BINR2_MAX_DATA_SIZE)
    return 0;
  crc = crc16CalcRaw(source, num);
  memmove(destination + 6, source, num);
  for(i=0; i < 4; i++)
  {
    *destination = preamble[i];
    destination++;
  }
  *destination = (INT8U)(num >>  0);
  destination++;
  *destination = (INT8U)(num >>  8);
  destination++;
  destination+=num;
  *destination = (INT8U)(crc >>  0);
  destination++;
  *destination = (INT8U)(crc >>  8);
  destination++;
  return num + 8;
}
/******************************************************************************
* OVERVIEW :
* INPUT    :
* OUTPUT   :
* RETURN   :
******************************************************************************/
INT16U Binr2ReqEncode(INT8U *out,INT8U id,
                      BINR2_INTERVAL iType, INT16U interval)
{
  INT16U num =0;
  out[num++]=0x01;
  out[num++]=id;
  out[num++]=(iType&0xFF);
  out[num++]=(INT8U)(interval >>  0);
  out[num++]=(INT8U)(interval >>  8);
  return Binr2Encode(out, out, num);
}

/******************************************************************************
* OVERVIEW :
* INPUT    :
* OUTPUT   :
* RETURN   :
******************************************************************************/
void receiveStart()
{
  bytes_expected = 0;
  bytes_received = 0;
  return;
}
/******************************************************************************
* OVERVIEW :
* INPUT    :
* OUTPUT   :
* RETURN   :
******************************************************************************/
INT8U Binr2Unpack(INT8U c)
{
  INT32U  retval;
  INT32U  i = bytes_received;
  inBuff[bytes_received++] = c;
  do
  {
    retval = BINR2_MORE;
    c = inBuff[i];
    switch (i) {
    /* preamble */
    case 0:
    case 1:
    case 2:
    case 3:
      if ((c&0xFF) != (preamble[i]&0xFF))
      {
        retval = BINR2_EPREAMBLE;
      }
      break;
      /* length */
    case 4:
      break;
    case 5:
      if (c > 7)
      {
        retval = BINR2_ELENGTH;
      }
      else
        bytes_expected = c * 256u + inBuff[4] + 8;
      break;

      /* id */
    case 6:
      if (!c)
      {
        retval = BINR2_EID;
      }
      break;

      /* crc */
    default:
      if (i == bytes_expected - 1) {
        INT16U  checksum_expected = c * 256u + inBuff[i - 1];
        INT16U  checksum_received =
            crc16CalcRaw(inBuff + 6, bytes_expected - 8u);
        if (checksum_expected != checksum_received)
          retval = BINR2_ECHECKSUM;
        else
          retval = BINR2_DONE;
      }
      break;
    }
    ++i;
    if (retval != BINR2_MORE)
    {
      void *p;
      if (retval == BINR2_DONE)
      { //finished parsing pkg in buff
        i-=8;
        memmove(inBuff, inBuff + 6, i); //muve in buff BINR2_DATA
        INT8U id = extract(i);
        if((pLib.dataBuff != NULL)&&
           (pLib.dataLen != NULL))
        {
          memcpy(pLib.dataBuff, inBuff,  i);
          *pLib.dataLen  = i;
        }
        receiveStart();
        return id;
      }
      else
        i = 4;
      p = memchr(inBuff + i, preamble[0], bytes_received - i);
      if (bytes_received > i && (p != NULL))
      {
        bytes_received = (INT32U)(inBuff + bytes_received - (INT8U *)p);
        memmove(inBuff, p, bytes_received );
      }
      else
        bytes_received = 0;
      i = 0;
    }
  } while (i < bytes_received);
  return 0;
}
/******************************************************************************
* OVERVIEW :
* INPUT    :
* OUTPUT   :
* RETURN   :
******************************************************************************/
INT8U extract(INT32U Len)
{
  switch(inBuff[0])
  {
  case 0x80:
    updateX80(Len);
    break;
  case 0x90:
    updateX90(Len);
    break;
  case 0x91:
    updateX91(Len);
    break;
  case 0x92:
    updateX92(Len);
    break;
  case 0x93:
    updateX93(Len);
    break;
  case 0x94:
    updateX94(Len);
    break;
  case 0x95:
    updateX95(Len);
    break;
  case 0x96:
    updateX96(Len);
    break;
  case 0x9A:
    updateX9A(Len);
    break;
  case 0xA0:
    updateXA0(Len);
    break;
  case 0xA1:
    updateXA1(Len);
    break;
  case 0xB0:
    updateXB0(Len);
    break;
  case 0xB1:
    updateXB1(Len);
    break;
  case 0xB2:
    updateXB2(Len);
    break;
  case 0xB3:
    updateXB3(Len);
    break;
  case 0xB4:
    updateXB4(Len);
    break;
  case 0xB5:
    updateXB5(Len);
    break;
  case 0xBA:
    updateXBA(Len);
    break;
  case 0xC0:
    updateXC0(Len);
    break;
  case 0xC1:
    updateXC1(Len);
    break;
  case 0xC2:
    updateXC2(Len);
    break;
  case 0xC3:
    updateXC3(Len);
    break;
  case 0xD0:
    updateXD0(Len);
    break;
  case 0xF9:
//    updateXF9(Len);
    break;
  default:
    return 0;
  }
  return inBuff[0];
}
/******************************************************************************
* OVERVIEW : функция разбора пакета x80
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateX80(INT32U len)
{
  if(len < 3)
    return;
  if(pLib.p80 == NULL)
    return;
  pLib.p80->id  = load8lu(inBuff);
  pLib.p80->pId = load8lu(inBuff + 1);
  pLib.p80->result = load8lu(inBuff + 2);
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета x90
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateX90(INT32U len)
{
  if(len < 59)
    return;
  if(pLib.p90 == NULL)
    return;
  pLib.p90->id = load8lu(inBuff);
  pLib.p90->timeInt = load32lu(inBuff + 1);
  pLib.p90->timeFrac = load32lu(inBuff + 5);
  pLib.p90->week = load16lu(inBuff + 9);
  pLib.p90->timeScale = load8lu(inBuff + 11);
  pLib.p90->dT = load32ls(inBuff + 12);
  pLib.p90->b = load32ls(inBuff + 16);
  pLib.p90->l = load32ls(inBuff + 20);
  pLib.p90->h = load32ls(inBuff + 24);
  pLib.p90->GeoidHeight = load16ls(inBuff + 28);
  pLib.p90->bRms = load16lu(inBuff + 30);
  pLib.p90->lRms = load16lu(inBuff + 32);
  pLib.p90->hRms = load16lu(inBuff + 34);
  pLib.p90->Vb = load32ls(inBuff + 36);
  pLib.p90->Vl = load32ls(inBuff + 40);
  pLib.p90->Vh = load32ls(inBuff + 44);
  pLib.p90->HDOP = load8lu(inBuff + 48);
  pLib.p90->VDOP = load8lu(inBuff + 49);
  pLib.p90->TDOP = load8lu(inBuff + 50);
  pLib.p90->PVTStatus = load32lu(inBuff + 51);
  pLib.p90->nGPS = load8lu(inBuff + 55);
  pLib.p90->nGLN = load8lu(inBuff + 56);
  pLib.p90->nGLL = load8lu(inBuff + 57);
  pLib.p90->nSBAS = load8lu(inBuff + 58);
  return;
}

/******************************************************************************
* OVERVIEW : функция разбора пакета x91
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateX91(INT32U len)
{
  INT32U i;
  if(len < 8)
    return;
  if(pLib.p91 == NULL)
    return;
  pLib.p91->nChnl = len / 23;
  if(pLib.p91->nChnl > MAX_CHANNELS)
    pLib.p91->nChnl = MAX_CHANNELS;
  pLib.p91->id = load8lu(inBuff);
  pLib.p91->weekTime = load32lu(inBuff + 1);
  pLib.p91->week = load16lu(inBuff + 5);
  pLib.p91->timeScale = load8lu(inBuff + 7);
  for(i = 0; i< pLib.p91->nChnl; i++)
  {
    pLib.p91->channel[i].sys = load8lu(inBuff + (i * 23) + 0 + 8);
    pLib.p91->channel[i].satN = load8lu(inBuff + (i * 23) + 1+ 8);
    pLib.p91->channel[i].FLiter = load8ls(inBuff + (i * 23) + 2+ 8);
    pLib.p91->channel[i].state = load8lu(inBuff + (i * 23) + 3+ 8);
    pLib.p91->channel[i].SN = load16lu(inBuff + (i * 23) + 4+ 8);
    pLib.p91->channel[i].range = load32lu(inBuff + (i * 23) + 6+ 8);
    pLib.p91->channel[i].dopler = load32ls(inBuff + (i * 23) + 10+ 8);
    pLib.p91->channel[i].boardTime = load32lu(inBuff + (i * 23) + 14+ 8);
    pLib.p91->channel[i].sign = load16lu(inBuff + (i * 23) + 18+ 8);
    pLib.p91->channel[i].rangeDispersion = load16lu(inBuff + (i * 23) + 20+ 8);
    pLib.p91->channel[i].status = load8lu(inBuff + (i * 23) + 22+ 8);
  }
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета x92
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateX92(INT32U len)
{
  INT32U i;
  if(len < 13)
    return;
  if(pLib.p92 == NULL)
    return;
  pLib.p92->id = load8lu(inBuff + 0);
  pLib.p92->timeFrac = load32lu(inBuff + 1);
  pLib.p92->timeInt = load32lu(inBuff + 5);
  pLib.p92->week = load16lu(inBuff + 9);
  pLib.p92->timeScale = load8lu(inBuff + 11);
  pLib.p92->timeCorrect = load8ls(inBuff + 12);
  pLib.p92->satCount = (len - 13)/22;
  for(i=0; i<pLib.p92->satCount; i++)
  {
    pLib.p92->sat[i].sys =        load8lu(inBuff + 13 + (i*22)+0);
    pLib.p92->sat[i].PRN =        load8lu(inBuff + 13 + (i*22)+1);
    pLib.p92->sat[i].fLiter =     load8ls(inBuff + 13 + (i*22)+2);
    pLib.p92->sat[i].SN =         load16lu(inBuff + 13 + (i*22)+3);
    pLib.p92->sat[i].pseudoRangeInt = load8lu(inBuff + 13 + (i*22)+5);
    pLib.p92->sat[i].pseudoRangeFeac = load32lu(inBuff + 13 + (i*22)+6);
    pLib.p92->sat[i].dopler =     load32ls(inBuff + 13 + (i*22)+10);
    pLib.p92->sat[i].phaseInt =   load32ls(inBuff + 13 + (i*22)+14);
    pLib.p92->sat[i].phaseFrac =  load16ls(inBuff + 13 + (i*22)+18);
    pLib.p92->sat[i].status =     load8lu(inBuff + 13 + (i*22)+20);
    pLib.p92->sat[i].null =       load8lu(inBuff + 13 + (i*22)+21);
  }
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета x93
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateX93(INT32U len)
{
  if(len < 3)
    return;
  if(pLib.p93 == NULL)
    return;
  pLib.p93->id = load8lu(inBuff + 0);
  pLib.p93->sys = load8lu(inBuff + 1);
  pLib.p93->PRN = load8lu(inBuff + 2);
  if((pLib.p93->sys & 0x07)==1) /*GLN*/
  {
    if(pLib.p93->PRN > (NUM_GLN))
      return;
    pLib.p93->eGln[pLib.p93->PRN].fLiter = load8ls(inBuff + 3);
    pLib.p93->eGln[pLib.p93->PRN].Bn = load8lu(inBuff + 4);
    pLib.p93->eGln[pLib.p93->PRN].Tb = load8lu(inBuff + 5);
    pLib.p93->eGln[pLib.p93->PRN].P1 = load8lu(inBuff + 6);
    pLib.p93->eGln[pLib.p93->PRN].Nt = load16lu(inBuff + 7);
    pLib.p93->eGln[pLib.p93->PRN].Ft = load8lu(inBuff + 9);
    pLib.p93->eGln[pLib.p93->PRN].deltaTau = load8ls(inBuff + 10);
    pLib.p93->eGln[pLib.p93->PRN].Gamma = load16ls(inBuff + 11);
    pLib.p93->eGln[pLib.p93->PRN].Tau = load32ls(inBuff + 13);
    pLib.p93->eGln[pLib.p93->PRN].Px = load32ls(inBuff + 17);
    pLib.p93->eGln[pLib.p93->PRN].Py = load32ls(inBuff + 21);
    pLib.p93->eGln[pLib.p93->PRN].Pz = load32ls(inBuff + 25);
    pLib.p93->eGln[pLib.p93->PRN].Vx = load32ls(inBuff + 29);
    pLib.p93->eGln[pLib.p93->PRN].Vy = load32ls(inBuff + 33);
    pLib.p93->eGln[pLib.p93->PRN].Vz = load32ls(inBuff + 37);
    pLib.p93->eGln[pLib.p93->PRN].Ax = load8ls(inBuff + 41);
    pLib.p93->eGln[pLib.p93->PRN].Ay = load8ls(inBuff + 42);
    pLib.p93->eGln[pLib.p93->PRN].Az = load8ls(inBuff + 43);
    pLib.p93->eGln[pLib.p93->PRN].En = load8lu(inBuff + 44);
    pLib.p93->eGln[pLib.p93->PRN].valid = 1;
  }
  else if((pLib.p93->sys & 0x07)==2) /*GPS*/
  {
    if(pLib.p93->PRN > (NUM_GPS))
      return;
    pLib.p93->eGps[pLib.p93->PRN].URA = load8lu(inBuff + 3);
    pLib.p93->eGps[pLib.p93->PRN].IODE = load8lu(inBuff + 4);
    pLib.p93->eGps[pLib.p93->PRN].Health = load8lu(inBuff + 5);
    pLib.p93->eGps[pLib.p93->PRN].CodeL2 = load8lu(inBuff + 6);
    pLib.p93->eGps[pLib.p93->PRN].WeekN = load16ls(inBuff + 7);
    pLib.p93->eGps[pLib.p93->PRN].IODC = load16lu(inBuff + 9);
    pLib.p93->eGps[pLib.p93->PRN].Toe = load16lu(inBuff + 11);
    pLib.p93->eGps[pLib.p93->PRN].M0 = load32ls(inBuff + 13);
    pLib.p93->eGps[pLib.p93->PRN].e = load32lu(inBuff + 17);
    pLib.p93->eGps[pLib.p93->PRN].sqrtA = load32lu(inBuff + 21);
    pLib.p93->eGps[pLib.p93->PRN].Omega0 = load32ls(inBuff + 25);
    pLib.p93->eGps[pLib.p93->PRN].I0 = load32ls(inBuff + 29);
    pLib.p93->eGps[pLib.p93->PRN].w = load32ls(inBuff + 33);
    pLib.p93->eGps[pLib.p93->PRN].Ir = load16ls(inBuff + 37);
    pLib.p93->eGps[pLib.p93->PRN].Dn = load16ls(inBuff + 39);
    pLib.p93->eGps[pLib.p93->PRN].OmegaR = load32ls(inBuff + 41);
    pLib.p93->eGps[pLib.p93->PRN].Cuc = load16ls(inBuff + 45);
    pLib.p93->eGps[pLib.p93->PRN].Cus = load16ls(inBuff + 47);
    pLib.p93->eGps[pLib.p93->PRN].Crc = load16ls(inBuff + 49);
    pLib.p93->eGps[pLib.p93->PRN].Crs = load16ls(inBuff + 51);
    pLib.p93->eGps[pLib.p93->PRN].Cic = load16ls(inBuff + 53);
    pLib.p93->eGps[pLib.p93->PRN].Cis = load16ls(inBuff + 55);
    pLib.p93->eGps[pLib.p93->PRN].Toc = load16lu(inBuff + 57);
    pLib.p93->eGps[pLib.p93->PRN].Af0 = load32ls(inBuff + 59);
    pLib.p93->eGps[pLib.p93->PRN].Af1 = load16ls(inBuff + 63);
    pLib.p93->eGps[pLib.p93->PRN].Af2 = load8ls(inBuff + 65);
    pLib.p93->eGps[pLib.p93->PRN].Tgd = load8ls(inBuff + 66);
    pLib.p93->eGps[pLib.p93->PRN].valid = 1;
  }
  else if((pLib.p93->sys & 0x07)==4) /*GLL*/
  {
    if(pLib.p93->PRN > (NUM_GLL))
      return;
    pLib.p93->eGll[pLib.p93->PRN].valid = 1;
    pLib.p93->eGll[pLib.p93->PRN].IOD = load8lu(inBuff + 3);
    pLib.p93->eGll[pLib.p93->PRN].Health = load8lu(inBuff + 4);
    pLib.p93->eGll[pLib.p93->PRN].WeekN = load16ls(inBuff + 5);
    pLib.p93->eGll[pLib.p93->PRN].Toe = load16lu(inBuff + 7);
    pLib.p93->eGll[pLib.p93->PRN].M0 = load32ls(inBuff + 9);
    pLib.p93->eGll[pLib.p93->PRN].e = load32lu(inBuff + 13);
    pLib.p93->eGll[pLib.p93->PRN].sqrtA = load32lu(inBuff + 17);
    pLib.p93->eGll[pLib.p93->PRN].Omega0 = load32ls(inBuff + 21);
    pLib.p93->eGll[pLib.p93->PRN].I0 = load32ls(inBuff + 25);
    pLib.p93->eGll[pLib.p93->PRN].w = load32ls(inBuff + 29);
    pLib.p93->eGll[pLib.p93->PRN].Ir = load16ls(inBuff + 33);
    pLib.p93->eGll[pLib.p93->PRN].Dn = load16ls(inBuff + 35);
    pLib.p93->eGll[pLib.p93->PRN].OmegaR = load32ls(inBuff + 37);
    pLib.p93->eGll[pLib.p93->PRN].Cuc = load16ls(inBuff + 41);
    pLib.p93->eGll[pLib.p93->PRN].Cus = load16ls(inBuff + 43);
    pLib.p93->eGll[pLib.p93->PRN].Crc = load16ls(inBuff + 45);
    pLib.p93->eGll[pLib.p93->PRN].Crs = load16ls(inBuff + 47);
    pLib.p93->eGll[pLib.p93->PRN].Cic = load16ls(inBuff + 49);
    pLib.p93->eGll[pLib.p93->PRN].Cis = load16ls(inBuff + 51);
    pLib.p93->eGll[pLib.p93->PRN].Toc = load16lu(inBuff + 53);
    pLib.p93->eGll[pLib.p93->PRN].Af0 = load32ls(inBuff + 55);
    pLib.p93->eGll[pLib.p93->PRN].Af1 = load32ls(inBuff + 59);
    pLib.p93->eGll[pLib.p93->PRN].Af2 = load8ls(inBuff + 63);
  }
  else
    return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета x94
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateX94(INT32U len)
{
  INT32U i=0;
  INT32U numAlm =0;
  INT32U prn;
  if(pLib.p94==NULL)
    return;
  pLib.p94->id = load8lu(inBuff + 0);
  numAlm = (len - 1)/28;
  for(i =0; i< numAlm; i++)
  {
    prn = load8lu(inBuff + 1 + (i*28));
    if(prn > NUM_GLN)
      return;
    pLib.p94->alm[prn].PRN = load8lu(inBuff + 1 + (i*28));
    pLib.p94->alm[prn].fLiter = load8ls(inBuff + 2 + (i*28));
    pLib.p94->alm[prn].health = load8lu(inBuff + 3 + (i*28));
    pLib.p94->alm[prn].Na = load16lu(inBuff + 4 + (i*28));
    pLib.p94->alm[prn].tlambda = load32lu(inBuff + 6 + (i*28));
    pLib.p94->alm[prn].Lambda = load32ls(inBuff + 10 + (i*28));
    pLib.p94->alm[prn].deltai = load32ls(inBuff + 14 + (i*28));
    pLib.p94->alm[prn].deltaT = load32ls(inBuff + 18 + (i*28));
    pLib.p94->alm[prn].RdeltaT = load8ls(inBuff + 22 + (i*28));
    pLib.p94->alm[prn].e = load16lu(inBuff + 23 + (i*28));
    pLib.p94->alm[prn].w = load16ls(inBuff + 25 + (i*28));
    pLib.p94->alm[prn].Tau = load16ls(inBuff + 27 + (i*28));
    pLib.p94->alm[prn].valid = 1;
  }
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета x95
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateX95(INT32U len)
{
  INT32U i=0;
  INT32U numAlm =0;
  INT32U prn;
  if(pLib.p95==NULL)
    return;
  pLib.p95->id = load8lu(inBuff + 0);
  numAlm = (len - 1)/31;
  for(i =0; i< numAlm; i++)
  {
    prn = load8lu(inBuff + 1 + (i*31));
    if(prn > NUM_GPS)
      return;
    pLib.p95->alm[prn].PRN = load8lu(inBuff + 1 + (i*31));
    pLib.p95->alm[prn].Health = load8lu(inBuff + 2 + (i*31));
    pLib.p95->alm[prn].Week = load16ls(inBuff + 3 + (i*31));
    pLib.p95->alm[prn].Toa = load8lu(inBuff + 5 + (i*31));
    pLib.p95->alm[prn].e = load16lu(inBuff + 6 + (i*31));
    pLib.p95->alm[prn].M0 = load32ls(inBuff + 8 + (i*31));
    pLib.p95->alm[prn].sqrtA = load32lu(inBuff + 12 + (i*31));
    pLib.p95->alm[prn].Omega0 = load32ls(inBuff + 16 + (i*31));
    pLib.p95->alm[prn].w = load32ls(inBuff + 20 + (i*31));
    pLib.p95->alm[prn].Ir = load16ls(inBuff + 24 + (i*31));
    pLib.p95->alm[prn].OmegaR = load16ls(inBuff + 26 + (i*31));
    pLib.p95->alm[prn].Af0 = load16ls(inBuff + 28 + (i*31));
    pLib.p95->alm[prn].Af1 = load16ls(inBuff + 30 + (i*31));
    pLib.p95->alm[prn].valid = 1;
  }
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета x96
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateX96(INT32U len)
{
  INT32U i=0;
  INT32U numAlm =0;
  INT32U prn;
  if(pLib.p96==NULL)
    return;
  pLib.p96->id = load8lu(inBuff + 0);
  numAlm = (len - 1)/24;
  for(i =0; i< numAlm; i++)
  {
    prn = load8lu(inBuff + 1 + (i*24));
    if(prn > NUM_GLL)
      return;
    pLib.p96->alm[prn].PRN = load8lu(inBuff + 1 + (i*24));
    pLib.p96->alm[prn].Health = load8lu(inBuff + 2 + (i*24));
    pLib.p96->alm[prn].Week = load16ls(inBuff + 3 + (i*24));
    pLib.p96->alm[prn].Toa = load16lu(inBuff + 5 + (i*24));
    pLib.p96->alm[prn].e = load16lu(inBuff + 7 + (i*24));
    pLib.p96->alm[prn].M0 = load16ls(inBuff + 9 + (i*24));
    pLib.p96->alm[prn].dSqrtA = load16ls(inBuff + 11 + (i*24));
    pLib.p96->alm[prn].Omega0 = load16ls(inBuff + 13 + (i*24));
    pLib.p96->alm[prn].w = load16ls(inBuff + 15 + (i*24));
    pLib.p96->alm[prn].Ir = load16ls(inBuff + 17 + (i*24));
    pLib.p96->alm[prn].OmegaR = load16ls(inBuff + 19 + (i*24));
    pLib.p96->alm[prn].Af0 = load16ls(inBuff + 21 + (i*24));
    pLib.p96->alm[prn].Af1 = load16ls(inBuff + 23 + (i*24));
    pLib.p96->alm[prn].valid = 1;
  }
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета x9A
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateX9A(INT32U len)
{
  if(pLib.p9A==NULL)
    return;
  if(len < 36)
    return;
  pLib.p9A->id = load8lu(inBuff + 0);
  pLib.p9A->X = load32ls(inBuff + 5);
  pLib.p9A->Y = load32ls(inBuff + 9);
  pLib.p9A->Z = load32ls(inBuff + 13);
  pLib.p9A->XRms = load16lu(inBuff + 15);
  pLib.p9A->YRms = load16lu(inBuff + 17);
  pLib.p9A->ZRms = load16lu(inBuff + 19);
  pLib.p9A->vX = load32ls(inBuff + 23);
  pLib.p9A->vY = load32ls(inBuff + 27);
  pLib.p9A->vZ = load32ls(inBuff + 31);
  pLib.p9A->status = load32lu(inBuff + 35);
  pLib.p9A->kinematics = load8lu(inBuff + 36);
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета xA0
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateXA0(INT32U len)
{
  if(pLib.pA0 == NULL)
    return;
  INT32U i=0;
  pLib.pA0->satN = (len - 1)/9;
  pLib.pA0->id = load8lu(inBuff + 0);
  for(i =0; i< pLib.pA0->satN; i++)
  {
    pLib.pA0->sat[i].sys = load8lu(inBuff + 1 + (i * 9));
    pLib.pA0->sat[i].PRN = load8lu(inBuff + 2 + (i * 9));
    pLib.pA0->sat[i].fLiter = load8ls(inBuff + 3 + (i * 9));
    pLib.pA0->sat[i].elev = load16lu(inBuff + 4 + (i * 9));
    pLib.pA0->sat[i].az = load8lu(inBuff + 6 + (i * 9));
    pLib.pA0->sat[i].SN = load8lu(inBuff + 8 + (i * 9));
  }
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета xA1
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateXA1(INT32U len)
{
  INT32U offset =0;
  if(pLib.pA1==NULL)
    return;
  if(len < 3)
    return;
  pLib.pA1->id = load8lu(inBuff);
  pLib.pA1->data = load16lu(inBuff + 1);
  offset = 3;
  if(pLib.pA1->data & 0x0001)
  {
    pLib.pA1->uGln = load32lu(inBuff + offset);
    offset += 4;
  }
  if(pLib.pA1->data & 0x0002)
  {
    pLib.pA1-> eGln = load32lu(inBuff + offset);
    offset += 4;
  }
  if(pLib.pA1->data & 0x0004)
  {
    pLib.pA1-> uGps = load32lu(inBuff + offset);
    offset += 4;
  }
  if(pLib.pA1->data & 0x0008)
  {
    pLib.pA1-> eGps = load32lu(inBuff + offset);
    offset += 4;
  }
  if(pLib.pA1->data & 0x0010)
  {
    pLib.pA1-> uSbas = load32lu(inBuff + offset);
    offset += 4;
  }
  if(pLib.pA1->data & 0x0020)
  {
    pLib.pA1-> eSbas = load32lu(inBuff + offset);
    offset += 4;
  }
  if(pLib.pA1->data & 0x0040)
  {
    pLib.pA1-> uGll0 = load32lu(inBuff + offset);
    offset += 4;
  }
  if(pLib.pA1->data & 0x0080)
  {
    pLib.pA1-> eGll0 = load32lu(inBuff + offset);
    offset += 4;
  }
  if(pLib.pA1->data & 0x0100)
  {
    pLib.pA1-> uGll1 = load32lu(inBuff + offset);
    offset += 4;
  }
  if(pLib.pA1->data & 0x0200)
  {
    pLib.pA1-> eGll1 = load32lu(inBuff + offset);
    offset += 4;
  }
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета xB0
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateXB0(INT32U len)
{
  if(pLib.pB0==NULL)
    return;
  if(len < 8)
    return;
  pLib.pB0->id = load8lu(inBuff + 0);
  pLib.pB0->sys = load8lu(inBuff + 1);
  pLib.pB0->b = load16lu(inBuff + 2);
  pLib.pB0->l = load16lu(inBuff + 4);
  pLib.pB0->h = load16lu(inBuff + 6);
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета xB1
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateXB1(INT32U len)
{
  if(pLib.pB1==NULL)
    return;
  if(len < 10)
    return;
  pLib.pB1->id = load8lu(inBuff + 0);
  pLib.pB1->valid = load8lu(inBuff + 1);
  pLib.pB1->Alfa0 = load8ls(inBuff + 2);
  pLib.pB1->Alfa1 = load8ls(inBuff + 3);
  pLib.pB1->Alfa2 = load8ls(inBuff + 4);
  pLib.pB1->Alfa3 = load8ls(inBuff + 5);
  pLib.pB1->Beta0 = load8ls(inBuff + 6);
  pLib.pB1->Beta1 = load8ls(inBuff + 7);
  pLib.pB1->Beta2 = load8ls(inBuff + 8);
  pLib.pB1->Beta3 = load8ls(inBuff + 9);
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета xB2
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateXB2(INT32U len)
{
  if(pLib.pB2==NULL)
    return;
  if(len < 29)
    return;
  pLib.pB2->id = load8lu(inBuff + 0);
  pLib.pB2->valid = load16lu(inBuff + 1);
  pLib.pB2->GPS_GLN = load32ls(inBuff + 3);
  pLib.pB2->GPS_GST = load32ls(inBuff + 7);
  pLib.pB2->GPS_UTC = load32ls(inBuff + 11);
  pLib.pB2->GST_UTC = load32ls(inBuff + 15);
  pLib.pB2->SNT_UTC = load32ls(inBuff + 19);
  pLib.pB2->GLB_UTC = load32ls(inBuff + 23);
  pLib.pB2->Ls = load8lu(inBuff + 27);
  pLib.pB2->timeScale = load8lu(inBuff + 28);
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета xB3
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateXB3(INT32U len)
{
  if(pLib.pB3==NULL)
    return;
  if(len < 16)
    return;
  pLib.pB3->id = load8lu(inBuff + 0);
  pLib.pB3->valid = load8lu(inBuff + 1);
  pLib.pB3->Tot = load8ls(inBuff + 2);
  pLib.pB3->WNt = load8lu(inBuff + 3);
  pLib.pB3->A1 = load32ls(inBuff + 4);
  pLib.pB3->A0 = load32ls(inBuff + 8);
  pLib.pB3->dtLS = load8ls(inBuff + 12);
  pLib.pB3->dtLSF = load8ls(inBuff + 13);
  pLib.pB3->DN = load8lu(inBuff + 14);
  pLib.pB3->WNlsf = load8lu(inBuff + 15);
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета xB4
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateXB4(INT32U len)
{
  if(pLib.pB4==NULL)
    return;
  if(len < 18)
    return;
  pLib.pB4->id = load8lu(inBuff + 0);
  pLib.pB4->valid = load8lu(inBuff + 1);
  pLib.pB4->B1 = load16ls(inBuff + 2);
  pLib.pB4->B2 = load16ls(inBuff + 4);
  pLib.pB4->KP = load8lu(inBuff + 6);
  pLib.pB4->N4 = load8lu(inBuff + 7);
  pLib.pB4->Na = load16lu(inBuff + 8);
  pLib.pB4->Tauc = load32ls(inBuff + 10);
  pLib.pB4->TauGPS = load32ls(inBuff + 14);
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета xB5
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateXB5(INT32U len)
{
  if(len < 11)
    return;
  if(pLib.pB5 == NULL)
    return;
  pLib.pB5->id = load8lu(inBuff + 0);
  pLib.pB5->validity = load8lu(inBuff + 1);
  pLib.pB5->year = load16lu(inBuff + 2);
  pLib.pB5->month = load8lu(inBuff + 4);
  pLib.pB5->day = load8lu(inBuff + 5);
  pLib.pB5->hour = load8lu(inBuff + 6);
  pLib.pB5->min = load8lu(inBuff + 7);
  pLib.pB5->sec = load8lu(inBuff + 8);
  pLib.pB5->mSec = load8lu(inBuff + 9);
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета xBA
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateXBA(INT32U len)
{
  INT32U num = 0;
  INT32U i=0;
  if(len < 11)
    return;
  if(pLib.pBA == NULL)
    return;
  num = (len - 4)/9;
  pLib.pBA->id = load8lu(inBuff + 0);
  pLib.pBA->type = load8lu(inBuff + 1);
  pLib.pBA->time = load16lu(inBuff + 2);
  for(i=0;i<num; i++)
  {
    pLib.pBA->sat[i].PRN = load16lu(inBuff + 4 + (i * 9));
    pLib.pBA->sat[i].IOD = load16lu(inBuff + 5 + (i * 9));
    pLib.pBA->sat[i].UDRE = load16lu(inBuff + 7 + (i * 9));
    pLib.pBA->sat[i].PRC = load16lu(inBuff + 9 + (i * 9));
    pLib.pBA->sat[i].RRC = load16lu(inBuff + 11 + (i * 9));
    pLib.pBA->sat[i].valid=1;
  }
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета xC0
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateXC0(INT32U len)
{
  if(len < 65)
    return;
  if(pLib.pC0 == NULL)
    return;
  INT32U i=0;
  pLib.pC0->id = load8lu(inBuff);
  pLib.pC0->chn = load8lu(inBuff + 1);
  for(i =0; i<30; i++)
  {
    if(i < 6)
      pLib.pC0->module[i] = load8lu(inBuff + 2 + i);
    if(i < 3)
    {
      pLib.pC0->version[i] = load8lu(inBuff + 8 + i);
      pLib.pC0->subVersion[i] = load8lu(inBuff + 11 + i);
    }
    if(i < 12)
      pLib.pC0->creationDate[i] = load8lu(inBuff + 14 + i);
    if(i < 9)
      pLib.pC0->creationTime[i] = load8lu(inBuff + 26 + i);
    pLib.pC0->coment[i] = load8lu(inBuff + 35 + i);
  }
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета xC1
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateXC1(INT32U len)
{
  if(len < 11)
    return;
  if(pLib.pC1 == NULL)
    return;
  pLib.pC1->id = load8lu(inBuff + 0);
  pLib.pC1->prot1 = load8lu(inBuff + 1);
  pLib.pC1->baud1 = load32lu(inBuff + 2);
  pLib.pC1->prot2 = load8lu(inBuff + 6);
  pLib.pC1->baud2 = load32lu(inBuff + 7);
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета xC2
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateXC2(INT32U len)
{
  if(len < 6)
    return;
  if(pLib.pC2 == NULL)
    return;
  pLib.pC2->id = load8lu(inBuff + 0);
  pLib.pC2->satSys = load8lu(inBuff + 1);
  pLib.pC2->minElev = load8lu(inBuff + 2);
  pLib.pC2->maxSKO = load16lu(inBuff + 3);
  pLib.pC2->coordinateSys = load8lu(inBuff + 5);
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета xC3
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateXC3(INT32U len)
{
  if(len < 14)
    return;
  if(pLib.pC3 == NULL)
    return;
  pLib.pC3->id = load8lu(inBuff + 0);
  pLib.pC3->filter = load8lu(inBuff + 1);
  pLib.pC3->maxAccel = load16lu(inBuff + 2);
  pLib.pC3->navInterval = load16lu(inBuff + 4);
  pLib.pC3->pseudorInterval = load16lu(inBuff + 6);
  pLib.pC3->navMode = load16lu(inBuff + 8);
  pLib.pC3->diffCor = load8lu(inBuff + 10);
  pLib.pC3->zeroSBAS = load8lu(inBuff + 11);
  pLib.pC3->asistedOnline = load8lu(inBuff + 12);
  pLib.pC3->difCor = load8lu(inBuff + 13);
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета xD0
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateXD0(INT32U len)
{
  if(len < 8)
    return;
  if(pLib.pD0 == NULL)
    return;
  pLib.pD0->id = load8lu(inBuff + 0);
  pLib.pD0->mode = load8lu(inBuff + 1);
  pLib.pD0->data = load16lu(inBuff + 2);
  pLib.pD0->sats = load32lu(inBuff + 4);
  return;
}
/******************************************************************************
* OVERVIEW : функция разбора пакета xF9
* INPUT    : len - длинна данных во входящем буфере
******************************************************************************/
void updateXF9(INT32U len)
{
  INT32U n=0;
  INT32U i=0;
  if(pLib.pF9 == NULL)
    return;
  n = (len -1) / 32;
  pLib.pF9->id = load8lu(inBuff + 0);
  pLib.pF9->n = n;
  for(i = 0; i < n; i++)
  {
    pLib.pF9->val[i].s32_0 = load32ls(inBuff + 1 + i*32); //range
    pLib.pF9->val[i].s32_1 = load32ls(inBuff + 5 + i*32); //dopler
    pLib.pF9->val[i].s32_2 = load32ls(inBuff + 9 + i*32);
    pLib.pF9->val[i].s32_3 = load32ls(inBuff + 13 + i*32);

    pLib.pF9->val[i].u32_0 = load32lu(inBuff + 17 + i*32); //prn
    pLib.pF9->val[i].u32_1 = load32lu(inBuff + 21 + i*32);
    pLib.pF9->val[i].u32_2 = load32lu(inBuff + 25 + i*32);
    pLib.pF9->val[i].u32_3 = load32lu(inBuff + 29 + i*32);
  }
  return;
}


/*EOF*/


