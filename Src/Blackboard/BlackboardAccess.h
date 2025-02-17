/*
 * Copyright 2023 ZF Friedrichshafen AG
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef BLACKBOARDACCESS_H
#define BLACKBOARDACCESS_H

#include "Blackboard.h"

/*************************************************************************
**    constants and macros
*************************************************************************/

#define ASYNC_READ_BYTE(p, i)   read_bbvari_byte (p[i])
#define ASYNC_READ_UBYTE(p, i)  read_bbvari_ubyte (p[i])
#define ASYNC_READ_WORD(p, i)   read_bbvari_word (p[i])
#define ASYNC_READ_UWORD(p, i)  read_bbvari_uword (p[i])
#define ASYNC_READ_DWORD(p, i)  read_bbvari_dword (p[i])
#define ASYNC_READ_UDWORD(p, i) read_bbvari_udword (p[i])
#define ASYNC_READ_FLOAT(p, i)  read_bbvari_float (p[i])
#define ASYNC_READ_DOUBLE(p, i) read_bbvari_double (p[i])
#define ASYNC_READ_UNION(p, i)  read_bbvari_union (p[i])
#define ASYNC_READ_CONVERT_DOUBLE(p, i) read_bbvari_convert_double (p[i])

#define ASYNC_WRITE_BYTE(p, i, v)   write_bbvari_byte (p[i], v)
#define ASYNC_WRITE_UBYTE(p, i, v)  write_bbvari_ubyte (p[i], v)
#define ASYNC_WRITE_WORD(p, i, v)   write_bbvari_word (p[i], v)
#define ASYNC_WRITE_UWORD(p, i, v)  write_bbvari_uword (p[i], v)
#define ASYNC_WRITE_DWORD(p, i, v)  write_bbvari_dword (p[i], v)
#define ASYNC_WRITE_UDWORD(p, i, v) write_bbvari_udword (p[i], v)
#define ASYNC_WRITE_FLOAT(p, i, v)  write_bbvari_float (p[i], v)
#define ASYNC_WRITE_DOUBLE(p, i, v) write_bbvari_double (p[i], v)
#define ASYNC_WRITE_UNION(p, i, v)  write_bbvari_union (p[i], v)
#define ASYNC_WRITE_MINMAX_CHECK(p,i,v) write_bbvari_minmax_check (p[i],v)

/*************************************************************************
**    data types
*************************************************************************/

/*************************************************************************
**    global variables
*************************************************************************/

/*************************************************************************
**    function prototypes
*************************************************************************/

/*************************************************************************
**
**    Function    : write_bbvari_byte
**
**    Description : einer Variablen einen Wert zuweisen (byte)
**    Parameter   : VID vid - Variablen-ID
**                  char v - Wert
**    Returnvalues:
**
*************************************************************************/
void write_bbvari_byte(VID vid, int8_t v);
void write_bbvari_byte_x(PID pid, VID vid, int8_t v);

/*************************************************************************
**
**    Function    : write_bbvari_ubyte
**
**    Description : einer Variablen einen Wert zuweisen (ubyte)
**    Parameter   : VID vid - Variablen-ID
**                  unsigned char v - Wert
**    Returnvalues:
**
*************************************************************************/
void write_bbvari_ubyte (VID vid, uint8_t v);
void write_bbvari_ubyte_x (PID pid, VID vid, uint8_t v);

/*************************************************************************
**
**    Function    : write_bbvari_word
**
**    Description : einer Variablen einen Wert zuweisen (word)
**    Parameter   : VID vid - Variablen-ID
**                  short v - Wert
**    Returnvalues:
**
*************************************************************************/
void write_bbvari_word (VID vid, int16_t v);
void write_bbvari_word_x (PID pid, VID vid, int16_t v);

/*************************************************************************
**
**    Function    : write_bbvari_uword
**
**    Description : einer Variablen einen Wert zuweisen (uword)
**    Parameter   : VID vid - Variablen-ID
**                  unsigned short v - Wert
**    Returnvalues:
**
*************************************************************************/
void write_bbvari_uword (VID vid, uint16_t v);
void write_bbvari_uword_x (PID pid, VID vid, uint16_t v);

/*************************************************************************
**
**    Function    : write_bbvari_dword
**
**    Description : einer Variablen einen Wert zuweisen (dword)
**    Parameter   : VID vid - Variablen-ID
**                  long v - Wert
**    Returnvalues:
**
*************************************************************************/
void write_bbvari_dword (VID vid, int32_t v);
void write_bbvari_dword_x (PID pid, VID vid, int32_t v);

/*************************************************************************
**
**    Function    : write_bbvari_udword
**
**    Description : einer Variablen einen Wert zuweisen (udword)
**    Parameter   : VID vid - Variablen-ID
**                  unsigned long v - Wert
**    Returnvalues:
**
*************************************************************************/
void write_bbvari_udword (VID vid, uint32_t v);
void write_bbvari_udword_x (PID pid, VID vid, uint32_t v);

#ifdef REMOTE_MASTER
void write_bbvari_udword_without_check(VID vid, uint32_t v);
#endif

/*************************************************************************
**
**    Function    : write_bbvari_qword
**
**    Description : einer Variablen einen Wert zuweisen (dword)
**    Parameter   : VID vid - Variablen-ID
**                  long v - Wert
**    Returnvalues:
**
*************************************************************************/
void write_bbvari_qword (VID vid, int64_t v);
void write_bbvari_qword_x (PID pid, VID vid, int64_t v);

/*************************************************************************
**
**    Function    : write_bbvari_uqword
**
**    Description : einer Variablen einen Wert zuweisen (udword)
**    Parameter   : VID vid - Variablen-ID
**                  unsigned long v - Wert
**    Returnvalues:
**
*************************************************************************/
void write_bbvari_uqword (VID vid, uint64_t v);
void write_bbvari_uqword_x (PID pid, VID vid, uint64_t v);

/*************************************************************************
**
**    Function    : write_bbvari_float
**
**    Description : einer Variablen einen Wert zuweisen (float)
**    Parameter   : VID vid - Variablen-ID
**                  float v - Wert
**    Returnvalues:
**
*************************************************************************/
void write_bbvari_float (VID vid, float v);
void write_bbvari_float_x (PID pid, VID vid, float v);

/*************************************************************************
**
**    Function    : write_bbvari_double
**
**    Description : einer Variablen einen Wert zuweisen (double)
**    Parameter   : VID vid - Variablen-ID
**                  double v - Wert
**    Returnvalues:
**
*************************************************************************/
void write_bbvari_double (VID vid, double v);
void write_bbvari_double_x (PID pid, VID vid, double v);

#ifdef REMOTE_MASTER
void write_bbvari_double_without_check(VID vid, double v);
#endif

/*************************************************************************
**
**    Function    : write_bbvari_union
**
**    Description : einer Variablen einen Wert zuweisen (BB_VARI)
**    Parameter   : VID vid - Variablen-ID
**                  union BB_VARI v - Wert
**    Returnvalues:
**
*************************************************************************/
void write_bbvari_union (VID vid, union BB_VARI v);
void write_bbvari_union_x (PID pid, VID vid, union BB_VARI v);

/*************************************************************************
**
**    Function    : write_bbvari_union_pid
**
**    Description : einer Variablen einen Wert zuweisen (BB_VARI)
**    Parameter   : int Pid - Prozess ID
**                  VID vid - Variablen-ID
**                  int DataType - Datentyp
**                  union BB_VARI v - Wert
**    Returnvalues:
**
*************************************************************************/
void write_bbvari_union_pid (int Pid, VID vid, int DataType, union BB_VARI v);

/*************************************************************************
**
**    Function    : write_bbvari_minmax_check
**
**    Description : einer Variablen einen Wert zuweisen mit ueberpruefung
**                  des Min- und Maxwertes
**    Parameter   : VID vid - Variablen-ID
**                  union BB_VARI v - Wert
**    Returnvalues:
**
*************************************************************************/
void write_bbvari_minmax_check (VID vid, double v);
void write_bbvari_minmax_check_pid (PID pid, VID vid, double v);

/*************************************************************************
**
**    Function    : write_bbvari_phys_minmax_check
**
**    Description : einer Variablen einen phys. Wert zuweisen mit
**                  Umrechnung und Ueberpruefung der Min- und Maxwerte
**    Parameter   : VID vid - Variablen-ID
**                  union BB_VARI v - Wert
**    Returnvalues:
**
*************************************************************************/
int write_bbvari_phys_minmax_check (VID vid, double new_phys_value);
int write_bbvari_phys_minmax_check_cs (VID vid, double new_phys_value, int cs);
int write_bbvari_phys_minmax_check_pid (PID pid, VID vid, double new_phys_value);

/*************************************************************************
**
**    Function    : write_bbvari_convert_to
**
**    Description : den durch 'vid' adressierten Wert in einen bestimmten Datentyp konvertieren
**                  und auf die Min.- Max. Grenzen des Datentypes begrenzen und zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**    Returnvalues: Anzahl Bytes des Datentypes oder -1 falls Datentyp unbekannt
**
*************************************************************************/
int write_bbvari_convert_to (PID pid, VID vid, int convert_from_type, void *ret_Ptr);

/*************************************************************************
**
**    Function    : read_bbvari_byte
**
**    Description : den durch 'vid' adressierten Wert zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**    Returnvalues: Wert der Variablen (byte)
**
*************************************************************************/
int8_t read_bbvari_byte (VID vid);
/*************************************************************************
**
**    Function    : read_bbvari_ubyte
**
**    Description : den durch 'vid' adressierten Wert zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**    Returnvalues: Wert der Variablen (ubyte)
**
*************************************************************************/
uint8_t read_bbvari_ubyte (VID vid);
/*************************************************************************
**
**    Function    : read_bbvari_word
**
**    Description : den durch 'vid' adressierten Wert zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**    Returnvalues: Wert der Variablen (word)
**
*************************************************************************/
int16_t read_bbvari_word (VID vid);
/*************************************************************************
**
**    Function    : read_bbvari_uword
**
**    Description : den durch 'vid' adressierten Wert zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**    Returnvalues: Wert der Variablen (uword)
**
*************************************************************************/
uint16_t read_bbvari_uword (VID vid);
/*************************************************************************
**
**    Function    : read_bbvari_dword
**
**    Description : den durch 'vid' adressierten Wert zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**    Returnvalues: Wert der Variablen (dword)
**
*************************************************************************/
int32_t read_bbvari_dword (VID vid);
/*************************************************************************
**
**    Function    : read_bbvari_udword
**
**    Description : den durch 'vid' adressierten Wert zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**    Returnvalues: Wert der Variablen (udword)
**
*************************************************************************/
uint32_t read_bbvari_udword (VID vid);
/*************************************************************************
**
**    Function    : read_bbvari_qword
**
**    Description : den durch 'vid' adressierten Wert zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**    Returnvalues: Wert der Variablen (dword)
**
*************************************************************************/
int64_t read_bbvari_qword (VID vid);
/*************************************************************************
**
**    Function    : read_bbvari_uqword
**
**    Description : den durch 'vid' adressierten Wert zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**    Returnvalues: Wert der Variablen (udword)
**
*************************************************************************/
uint64_t read_bbvari_uqword (VID vid);
/*************************************************************************
**
**    Function    : read_bbvari_float
**
**    Description : den durch 'vid' adressierten Wert zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**    Returnvalues: Wert der Variablen (float)
**
*************************************************************************/
float read_bbvari_float (VID vid);
/*************************************************************************
**
**    Function    : read_bbvari_double
**
**    Description : den durch 'vid' adressierten Wert zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**    Returnvalues: Wert der Variablen (double)
**
*************************************************************************/
double read_bbvari_double (VID vid);
/*************************************************************************
**
**    Function    : read_bbvari_union
**
**    Description : den durch 'vid' adressierten Wert zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**    Returnvalues: Wert der Variablen (BB_VARI)
**
*************************************************************************/
union BB_VARI read_bbvari_union (VID vid);
enum BB_DATA_TYPES read_bbvari_union_type(VID vid, union BB_VARI *ret_Value);
int read_bbvari_union_type_frame (int Number, VID *Vids, enum BB_DATA_TYPES *ret_Types, union BB_VARI *ret_Values);

/*************************************************************************
**
**    Function    : read_bbvari_equ
**
**    Description : den durch 'vid' adressierten Wert zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**    Returnvalues: Wert der Variablen (als double)
**
*************************************************************************/
double read_bbvari_equ (VID vid);
/*************************************************************************
**
**    Function    : read_bbvari_convert_double
**
**    Description : den durch 'vid' adressierten Wert zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**    Returnvalues: Wert der Variablen (als double)
**
*************************************************************************/
double read_bbvari_convert_double (VID vid);
/*************************************************************************
**
**    Function    : read_bbvari_convert_to
**
**    Description : den durch 'vid' adressierten Wert in einen bestimmten Datentyp konvertieren
**                  und auf die Min.- Max. Grenzen des Datentypes begrenzen und zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**    Returnvalues: Anzahl Bytes des Datentypes oder -1 falls Datentyp unbekannt
**
*************************************************************************/
int read_bbvari_convert_to (VID vid, int convert_to_type, union BB_VARI *ret_Ptr);
/*************************************************************************
**
**    Function    : read_bbvari_textreplace
**
**    Description : Textersatz fuer die entsprechende Variable zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**                  char *txt - Ziel fuer Textersatz
**                  int maxc - Groesse des Zielstrings
**    Returnvalues:
**
*************************************************************************/
int read_bbvari_textreplace(VID vid, char *txt, int maxc, int *pcolor);
/*************************************************************************
**
**    Function    : SET_SEM_WR
**
**    Description : Dummyfunktion (Kompatibilitaetsgruende)
**    Parameter   : BLACKBOARD_ELEM **f - Adresse eines Zeigers auf ein
**                                        Blackboardelement
**    Returnvalues:
**
*************************************************************************/
void SET_SEM_WR(BLACKBOARD_ELEM **f);
/*************************************************************************
**
**    Function    : SET_SEM_RD
**
**    Description : Dummyfunktion (Kompatibilitaetsgruende)
**    Parameter   : BLACKBOARD_ELEM **f - Adresse eines Zeigers auf ein
**                                        Blackboardelement
**    Returnvalues:
**
*************************************************************************/
void SET_SEM_RD(BLACKBOARD_ELEM **f);
/*************************************************************************
**
**    Function    : RESET_SEMSI
**
**    Description : Dummyfunktion (Kompatibilitaetsgruende)
**    Parameter   : BLACKBOARD_ELEM **f - Adresse eines Zeigers auf ein
**                                        Blackboardelement
**                  int p - ????
**    Returnvalues:
**
*************************************************************************/
void RESET_SEMSI(BLACKBOARD_ELEM **f, int p);
/*************************************************************************
**
**    Function    : convert_value_textreplace
**
**    Description : Textersatz fuer die entsprechende Variable zurueckgeben
**    Parameter   : VID vid - Variablen-ID
**                  long value - Wert fuer den TExtersatz gelten soll
**                  char *txt - Ziel fuer Textersatz
**                  int maxc - Groesse des Zielstrings
**    Returnvalues:
**
*************************************************************************/
int convert_value_textreplace(VID vid, int32_t value, char *txt, int maxc, int *pcolor);

int convert_textreplace_value (VID vid, char *txt, int64_t *pfrom, int64_t *pto);

/*************************************************************************
**
**    Function    : read_bbvari_frame
**
**    Description :
**    Parameter   : VID Vids - Array der zu lesenden Variablen-ID
**                  double *RetFrameValues - Rueckgabe des Werte-Arrays
**                  int Size - Anzahl der Variable
**    Returnvalues:
**
*************************************************************************/

int read_bbvari_frame (VID *Vids, double *RetFrameValues, int Size);
int write_bbvari_frame (VID *Vids, double *FrameValues, int Size);
int write_bbvari_frame_pid (PID pid, VID *Vids, double *FrameValues, int Size);


int get_phys_value_for_raw_value (VID vid, double raw_value, double *ret_phys_value);
int get_raw_value_for_phys_value (VID vid, double phys_value, double *ret_raw_value, double *ret_phys_value);

void ConvertDoubleToUnion(enum BB_DATA_TYPES Type,
                          double Value,
                          union BB_VARI *ret_Value);
void ConvertRawMemoryToUnion(enum BB_DATA_TYPES Type,
                             void *RawMemValue,
                             union BB_VARI *ret_Value);
int IsMinOrMaxLimit (enum BB_DATA_TYPES Type,union BB_VARI Value);


int read_bbvari_convert_to_FloatAndInt64(VID vid, union FloatOrInt64 *ret_Value);
int mul_FloatOrInt64(union FloatOrInt64 value1, int type1, union FloatOrInt64 value2, int type2, union FloatOrInt64 *ret_value);
int add_FloatOrInt64(union FloatOrInt64 value1, int type1, union FloatOrInt64 value2, int type2, union FloatOrInt64 *ret_value);
int sub_FloatOrInt64(union FloatOrInt64 value1, int type1, union FloatOrInt64 value2, int type2, union FloatOrInt64 *ret_value);
int string_to_FloatOrInt64(char *src_string, union FloatOrInt64 *ret_value);
int FloatOrInt64_to_string(union FloatOrInt64 value, int type, int base, char *dst_string, int maxc);
int is_equal_FloatOrInt64(double ref_value, union FloatOrInt64 value, int type);
double to_double_FloatOrInt64(union FloatOrInt64 value, int type);
int to_bool_FloatOrInt64(union FloatOrInt64 value, int type);
int to_int_FloatOrInt64(union FloatOrInt64 value, int type);

void write_bbvari_convert_with_FloatAndInt64 (VID vid, union FloatOrInt64 value, int type);
void write_bbvari_convert_with_FloatAndInt64_pid(PID pid, VID vid, union FloatOrInt64 value, int type);
void write_bbvari_convert_with_FloatAndInt64_cs (VID vid, union FloatOrInt64 value, int type, int cs);

#define READ_TYPE_RAW_VALUE      0
#define READ_TYPE_PHYS           1
#define READ_TYPE_ONS_COMPLEMENT 2
#define READ_TYPE_RAW_BINARY     3
int read_bbvari_by_name(const char *name, union FloatOrInt64 *ret_value, int *ret_byte_width, int read_type);
int read_bbvari_by_id(int vid, union FloatOrInt64 *ret_value, int *ret_byte_width, int read_type);
#define WRITE_TYPE_RAW_VALUE      0
#define WRITE_TYPE_PHYS           1
//#define WRITE_TYPE_ONS_COMPLEMENT 2
#define WRITE_TYPE_RAW_BINARY     3
int write_bbvari_by_name(PID pid, const char *name, union FloatOrInt64 value, int type, int bb_type, int add_if_not_exist, int read_type, int cs);
int write_bbvari_by_id(int vid, union FloatOrInt64 value, int type, int bb_type, int add_if_not_exist, int read_type);


#endif

