/******************************************************************************
* FILE NAME :
* PURPOSE   :
* AUTHOR    :
* $Rev::                                                                       $
* $Date::                                                                      $
* $Author::                                                                    $
* COMMENT   :
******************************************************************************/
#ifndef BINR2_H
#define BINR2_H
/******************************************************************************
* Includes
******************************************************************************/
#include "typedef.h"
#include "buffFunctions.h"
/******************************************************************************
* Macro Definitions
******************************************************************************/
#define MAX_CHANNELS     128
#define MAX_X92_SAT      128
#define MAX_XA0_SAT      128
#define MAX_XBA_SAT      64
#define NUM_GPS          33
#define NUM_GLN          33
#define NUM_GLL          33
/******************************************************************************
* Type Definitions
******************************************************************************/
typedef enum
{
  Binr2Cancel = 0,
  Binr2Single = 1,
  Binr2PVT =2,
  Binr2Second = 3,
  Binr2Meas = 4,
  Binr2CancelAll = 0xFF
}BINR2_INTERVAL;
//-----------------------------x80----------------------------------------------
typedef struct
{
  INT8U id;
  INT8U pId;
  INT8U result;
}BINR2x80;
//-----------------------------x90----------------------------------------------
typedef struct
{
  INT8U  id;
  INT32U timeInt;     //integral time
  INT32U timeFrac;    //fractional time
  INT16U week;
  INT8U  timeScale;
  INT32S dT;          //deviation of the generator
  INT32S b;
  INT32S l;
  INT32S h;
  INT16S GeoidHeight;
  INT16U bRms;
  INT16U lRms;
  INT16U hRms;
  INT32S Vb;
  INT32S Vl;
  INT32S Vh;
  INT8U  HDOP;
  INT8U  VDOP;
  INT8U  TDOP;
  INT32U PVTStatus;
  INT8U  nGPS;
  INT8U  nGLN;
  INT8U  nGLL;
  INT8U  nSBAS;
}BINR2x90;
//-----------------------------x91----------------------------------------------
typedef struct
{
  INT8U  sys;
  INT8U  satN;
  INT8S  FLiter;
  INT8U  state;
  INT16U SN;
  INT32U range;
  INT32S dopler;
  INT32U boardTime;
  INT16U sign;
  INT16U rangeDispersion;
  INT8U  status;
}Tx91Chnl;

typedef struct
{
  INT8U   id;
  INT32U  weekTime;
  INT16U  week;
  INT8U   timeScale;
  Tx91Chnl channel[MAX_CHANNELS];
  INT32U  nChnl;
}BINR2x91;
//-----------------------------x92----------------------------------------------
typedef struct
{
  INT8U  sys;
  INT8U  PRN;
  INT8S  fLiter;
  INT16U SN;
  INT8U  pseudoRangeInt;  //integral
  INT32U pseudoRangeFeac; //Fractional pseudorange
  INT32S dopler;
  INT32S phaseInt;
  INT16S phaseFrac;
  INT8U  status;
  INT8U  null;
}Tx92Sat;

typedef struct
{
  INT8U  id;
  INT32U timeInt;  //integral time
  INT32U timeFrac;    //fractional time
  INT16U week;
  INT8U  timeScale;
  INT8S  timeCorrect;
  Tx92Sat sat[MAX_X92_SAT];
  INT32U  satCount;
}BINR2x92;
//-----------------------------x93----------------------------------------------
typedef struct
{
  INT8U valid;
  INT8S  fLiter;
  INT8U  Bn;
  INT8U  Tb;
  INT8U  P1;
  INT16U Nt;
  INT8U  Ft;
  INT8S  deltaTau;
  INT16S Gamma;
  INT32S Tau;
  INT32S Px;
  INT32S Py;
  INT32S Pz;
  INT32S Vx;
  INT32S Vy;
  INT32S Vz;
  INT8S  Ax;
  INT8S  Ay;
  INT8S  Az;
  INT8U  En;
}TEPHGLN;

typedef struct
{
  INT8U  valid;
  INT8U  URA;
  INT8U  IODE;
  INT8U  Health;
  INT8U  CodeL2;
  INT16S WeekN;
  INT16U IODC;
  INT16U Toe;
  INT32S M0;
  INT32U e;
  INT32U sqrtA;
  INT32S Omega0;
  INT32S I0;
  INT32S w;
  INT16S Ir;
  INT16S Dn;
  INT32S OmegaR;
  INT16S Cuc;
  INT16S Cus;
  INT16S Crc;
  INT16S Crs;
  INT16S Cic;
  INT16S Cis;
  INT16U Toc;
  INT32S Af0;
  INT16S Af1;
  INT8S  Af2;
  INT8S  Tgd;
}TEPHGPS;

typedef struct
{
  INT8U  valid;
  INT8U IOD;
  INT8U Health;
  INT16S WeekN;
  INT16U Toe;
  INT32S M0;
  INT32U e;
  INT32U sqrtA;
  INT32S Omega0;
  INT32S I0;
  INT32S w;
  INT16S Ir;
  INT16S Dn;
  INT32S OmegaR;
  INT16S Cuc;
  INT16S Cus;
  INT16S Crc;
  INT16S Crs;
  INT16S Cic;
  INT16S Cis;
  INT16U Toc;
  INT32S Af0;
  INT32S Af1;
  INT8S Af2;
}TEPHGLL;

typedef struct
{
  INT8U  id;
  INT8U  sys;
  INT8U  PRN;
  TEPHGLN eGln[NUM_GLN];
  TEPHGPS eGps[NUM_GPS];
  TEPHGLL eGll[NUM_GLL];
}BINR2x93;
//-----------------------------x94----------------------------------------------
typedef struct
{
  INT8U  PRN;
  INT8S  fLiter;
  INT8U  health;
  INT16U Na;
  INT32U tlambda;
  INT32S Lambda;
  INT32S deltai;
  INT32S deltaT;
  INT8S  RdeltaT;
  INT16U e;
  INT16S w;
  INT16S Tau;
  INT8U  valid;
}TALMGLN;

typedef struct
{
  INT8U id;
  TALMGLN alm[NUM_GLN];
}BINR2x94;
//-----------------------------x95----------------------------------------------
typedef struct
{
  INT8U  PRN;
  INT8U  Health;
  INT16S Week;
  INT8U  Toa;
  INT16U e;
  INT32S M0;
  INT32U sqrtA;
  INT32S Omega0;
  INT32S w;
  INT16S Ir;
  INT16S OmegaR;
  INT16S Af0;
  INT16S Af1;
  INT8U valid;
}TALMGPS;

typedef struct
{
  INT8U id;
  TALMGPS alm[NUM_GPS];
}BINR2x95;
//-----------------------------x96----------------------------------------------
typedef struct
{
  INT8U  PRN;
  INT8U  Health;
  INT16S Week;
  INT16U Toa;
  INT16U e;
  INT16S M0;
  INT16S dSqrtA;
  INT16S Omega0;
  INT16S w;
  INT16S Ir;
  INT16S OmegaR;
  INT16S Af0;
  INT16S Af1;
  INT8U  valid;
}TALMGLL;

typedef struct
{
  INT8U id;
  TALMGLL alm[NUM_GLL];
}BINR2x96;
//-----------------------------x9A----------------------------------------------
typedef struct
{
  INT8U  id;
  INT32S X;
  INT32S Y;
  INT32S Z;
  INT16U XRms;
  INT16U YRms;
  INT16U ZRms;
  INT32S vX;
  INT32S vY;
  INT32S vZ;
  INT32U status;
  INT8U  kinematics;
}BINR2x9A;
//-----------------------------xA0----------------------------------------------
typedef struct
{
  INT8U  sys;
  INT8U  PRN;
  INT8S  fLiter;
  INT16U elev;
  INT16U az;
  INT16U SN;
}TxA0Sat;

typedef struct
{
  INT8U id;
  TxA0Sat sat[MAX_XA0_SAT];
  INT32U  satN;
}BINR2xA0;
//-----------------------------xA1----------------------------------------------
typedef struct
{
  INT8U  id;
  INT16U data;
  INT32U uGln;
  INT32U eGln;
  INT32U uGps;
  INT32U eGps;
  INT32U uSbas;
  INT32U eSbas;
  INT32U uGll0;
  INT32U eGll0;
  INT32U uGll1;
  INT32U eGll1;
}BINR2xA1;
//-----------------------------xB0----------------------------------------------
typedef struct
{
  INT8U id;
  INT8U  sys;
  INT16S b;
  INT16S l;
  INT16S h;

}BINR2xB0;
//-----------------------------xB1----------------------------------------------
typedef struct
{
  INT8U id;
  INT8U valid;
  INT8S Alfa0;
  INT8S Alfa1;
  INT8S Alfa2;
  INT8S Alfa3;
  INT8S Beta0;
  INT8S Beta1;
  INT8S Beta2;
  INT8S Beta3;
}BINR2xB1;
//-----------------------------xB2----------------------------------------------
typedef struct
{
  INT8U   id;
  INT16U  valid;
  INT32S  GPS_GLN;
  INT32S  GPS_GST;
  INT32S  GPS_UTC;
  INT32S  GST_UTC;
  INT32S  SNT_UTC;
  INT32S  GLB_UTC;
  INT8U   Ls;
  INT8U   timeScale;
}BINR2xB2;
//-----------------------------xB3----------------------------------------------
typedef struct
{
  INT8U  id;
  INT8U  valid;
  INT8S  Tot ;
  INT8U  WNt;
  INT32S A1;
  INT32S A0;
  INT8S  dtLS;
  INT8S  dtLSF;
  INT8U  DN;
  INT8U  WNlsf;
}BINR2xB3;
//-----------------------------xB4----------------------------------------------
typedef struct
{
  INT8U  id;
  INT8U  valid;
  INT16S  B1;
  INT16S B2;
  INT8U  KP;
  INT8U  N4;
  INT16U Na;
  INT32S Tauc;
  INT32S TauGPS;
}BINR2xB4;
//-----------------------------xB5----------------------------------------------
typedef struct
{
  INT8U  id;
  INT8U  validity;
  INT16U year;
  INT8U  month;
  INT8U  day;
  INT8U  hour;
  INT8U  min;
  INT8U  sec;
  INT16U mSec;
}BINR2xB5;
//-----------------------------xBA----------------------------------------------
typedef struct
{
  INT8U PRN;
  INT16U IOD;
  INT16U UDRE;
  INT16S PRC;
  INT16S RRC;
  INT8U  valid;
}TxBASat;
typedef struct
{
  INT8U  id;
  INT8U  type;
  INT16U time;
  TxBASat sat[MAX_XBA_SAT];
}BINR2xBA;
//-----------------------------xC0----------------------------------------------
typedef struct
{
  INT8U id;
  INT8U chn;
  INT8U module[6];
  INT8U version[3];
  INT8U subVersion[3];
  INT8U creationDate[12];
  INT8U creationTime[9];
  INT8U coment[30];
}BINR2xC0;
//-----------------------------xC1----------------------------------------------
typedef struct
{
  INT8U id;
  INT8U prot1;
  INT32U baud1;
  INT8U prot2;
  INT32U baud2;
}BINR2xC1;
//-----------------------------xC2----------------------------------------------
typedef struct
{
  INT8U id;
  INT8U satSys;
  INT8U minElev;
  INT16U maxSKO;
  INT8U  coordinateSys;
}BINR2xC2;
//-----------------------------xC3----------------------------------------------
typedef struct
{
  INT8U  id;
  INT8U  filter;
  INT16U maxAccel;
  INT16U navInterval;
  INT16U pseudorInterval;
  INT16U navMode;
  INT8U  diffCor;
  INT8U  zeroSBAS;
  INT8U  asistedOnline;
  INT8U  difCor;
}BINR2xC3;
//-----------------------------xD0----------------------------------------------
typedef struct
{
  INT8U id;
  INT8U mode;
  INT16U data;
  INT32U sats;
}BINR2xD0;
//-----------------------------x79----------------------------------------------
typedef struct
{
  INT32S s32_0;
  INT32S s32_1;
  INT32S s32_2;
  INT32S s32_3;
  INT32U u32_0;
  INT32U u32_1;
  INT32U u32_2;
  INT32U u32_3;
}TDebugVal;
typedef struct
{
  INT8U id;
  INT32U n;
  TDebugVal val[32];
}BINR2xF9;
//-----------------------------Pointers-----------------------------------------
typedef struct
{
  INT8U    *dataBuff;
  INT32U   *dataLen;
  BINR2x80 *p80;
  BINR2x90 *p90;
  BINR2x91 *p91;
  BINR2x92 *p92;
  BINR2x93 *p93;
  BINR2x94 *p94;
  BINR2x95 *p95;
  BINR2x96 *p96;
  BINR2x9A *p9A;
  BINR2xA0 *pA0;
  BINR2xA1 *pA1;
  BINR2xB0 *pB0;
  BINR2xB1 *pB1;
  BINR2xB2 *pB2;
  BINR2xB3 *pB3;
  BINR2xB4 *pB4;
  BINR2xB5 *pB5;
  BINR2xBA *pBA;
  BINR2xC0 *pC0;
  BINR2xC1 *pC1;
  BINR2xC2 *pC2;
  BINR2xC3 *pC3;
  BINR2xD0 *pD0;
  BINR2xF9 *pF9;
}BINR2_POINTERS;
/******************************************************************************
* Extern Functions
******************************************************************************/
extern C_CPP void   Binr2Init(BINR2_POINTERS pointers);
extern C_CPP INT8U  Binr2Unpack(INT8U c);
extern C_CPP INT16U Binr2Encode(INT8U *destination, INT8U *source, INT16U num);
extern C_CPP INT16U Binr2ReqEncode(INT8U *out, INT8U id, BINR2_INTERVAL iType,
                                   INT16U interval);
#endif // BINR2_H
