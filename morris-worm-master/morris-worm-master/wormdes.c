/* INTERNET WORM CRYPT() ROUTINE
notes from 'Password Cracking: A Game of Wits' by Donn Seeley CACM,
June89 v32n6 pp700-703 : The Worm's crypt algorithm appears to be a
compramise between time	and space: the time needed to encrypt one
password guess verses the substantial extra table space	needed to
squeeze	performance out	of the algorithm...The traditional UNIX
algorithm stores each bit of the password in a byte, while the worms
algorithm packs	packs the bits into into two 32-bit words. This
permits	the worms algorithm to use bitfield and	shift operations on
the password data. (also saves a little	space!)
  Other	speedups include unrolling loops, combining tables,
precomputing shifts and	masks, and eliminating redundant initial and
final permutations when	performing the 25 applications of modified
DES that the password encryption algorithm uses.
  The biggest performance improvement comes from combining
permutations: the worm uses expanded arrays which are indexed by
groups of bits rather than single bits.
  Bishops DESZIP.c does	all these things and also precomputes more
functions yielding twice the performance of the	worms algorithm, but
requiring nearly 200KB of initialized data as opposed to the 6KB
used by	the worm, and the less than 2KB	used by	the normal crypt().
  The worms version of crypt ran 9 times faster	than the normal	crypt
while DESZIP runs about	20 time	faster (FDES + DESZIP are about
equivalent). on	a VAX 6800 encrypting 271 passwords it took the	worms
crypt less than	6 seconds or 45	passwords per sec, and the normal
crypt took 54 seconds or 5 passwords per second. - Tangent */

static	char	e[48] =	{			/* 0x20404 */
    31,	 0,  1,	 2,  3,	 4,  3,	 4,
    5,	 6,  7,	 8,  7,	 8,  9,	10,
    11,	12, 11,	12, 13,	14, 15,	16,
    15,	16, 17,	18, 19,	20, 19,	20,
    21,	22, 23,	24, 23,	24, 25,	26,
    27,	28, 27,	28, 29,	30, 31,	 0,
};
 
int shift[16] =	{					/* 0x20434 */
	1,1,2,2, 2,2,2,2, 1,2,2,2, 2,2,2,1,
};
int ip_L0[] = {
   0x00000008,	 0x00000008,   0x08000808,   0x08000808,
   0x00000008,	 0x00000008,   0x08000808,   0x08000808,};
int ip_L1[] = {
   0x00000000,	 0x00080000,   0x00000000,   0x00080000,
   0x08000000,	 0x08080000,   0x08000000,   0x08080000,
   0x00000000,	 0x00080000,   0x00000000,   0x00080000,
   0x08000000,	 0x08080000,   0x08000000,   0x08080000,};
int ip_L2[] = {
   0x00000004,	 0x00000004,   0x04000404,   0x04000404,
   0x00000004,	 0x00000004,   0x04000404,   0x04000404,};
int ip_L3[] = {
   0x00000000,	 0x00040000,   0x00000000,   0x00040000,
   0x04000000,	 0x04040000,   0x04000000,   0x04040000,
   0x00000000,	 0x00040000,   0x00000000,   0x00040000,
   0x04000000,	 0x04040000,   0x04000000,   0x04040000,};
int ip_L4[] = {
   0x00000002,	 0x00000002,   0x02000202,   0x02000202,
   0x00000002,	 0x00000002,   0x02000202,   0x02000202,};
int ip_L5[] = {
   0x00000000,	 0x00020000,   0x00000000,   0x00020000,
   0x02000000,	 0x02020000,   0x02000000,   0x02020000,
   0x00000000,	 0x00020000,   0x00000000,   0x00020000,
   0x02000000,	 0x02020000,   0x02000000,   0x02020000,};
int ip_L6[] = {
   0x00000001,	 0x00000001,   0x01000101,   0x01000101,
   0x00000001,	 0x00000001,   0x01000101,   0x01000101,};
int ip_L7[] = {
   0x00000000,	 0x00010000,   0x00000000,   0x00010000,
   0x01000000,	 0x01010000,   0x01000000,   0x01010000,
   0x00000000,	 0x00010000,   0x00000000,   0x00010000,
   0x01000000,	 0x01010000,   0x01000000,   0x01010000,};
int ip_L8[] = {
   0x00000080,	 0x00000080,   0x80008080,   0x80008080,
   0x00000080,	 0x00000080,   0x80008080,   0x80008080,};
int ip_L9[] = {
   0x00000000,	 0x00800000,   0x00000000,   0x00800000,
   0x80000000,	 0x80800000,   0x80000000,   0x80800000,
   0x00000000,	 0x00800000,   0x00000000,   0x00800000,
   0x80000000,	 0x80800000,   0x80000000,   0x80800000,};
int ip_La[] = {
   0x00000040,	 0x00000040,   0x40004040,   0x40004040,
   0x00000040,	 0x00000040,   0x40004040,   0x40004040,};
int ip_Lb[] = {
   0x00000000,	 0x00400000,   0x00000000,   0x00400000,
   0x40000000,	 0x40400000,   0x40000000,   0x40400000,
   0x00000000,	 0x00400000,   0x00000000,   0x00400000,
   0x40000000,	 0x40400000,   0x40000000,   0x40400000,};
int ip_Lc[] = {
   0x00000020,	 0x00000020,   0x20002020,   0x20002020,
   0x00000020,	 0x00000020,   0x20002020,   0x20002020,};
int ip_Ld[] = {
   0x00000000,	 0x00200000,   0x00000000,   0x00200000,
   0x20000000,	 0x20200000,   0x20000000,   0x20200000,
   0x00000000,	 0x00200000,   0x00000000,   0x00200000,
   0x20000000,	 0x20200000,   0x20000000,   0x20200000,};
int ip_Le[] = {
   0x00000010,	 0x00000010,   0x10001010,   0x10001010,
   0x00000010,	 0x00000010,   0x10001010,   0x10001010,};
int ip_Lf[] = {
   0x00000000,	 0x00100000,   0x00000000,   0x00100000,
   0x10000000,	 0x10100000,   0x10000000,   0x10100000,
   0x00000000,	 0x00100000,   0x00000000,   0x00100000,
   0x10000000,	 0x10100000,   0x10000000,   0x10100000,};
int ip_H0[] = {
   0x00000000,	 0x00080008,   0x00000000,   0x00080008,
   0x08000800,	 0x08080808,   0x08000800,   0x08080808,};
int ip_H1[] = {
   0x00000000,	 0x00000000,   0x00080000,   0x00080000,
   0x00000000,	 0x00000000,   0x00080000,   0x00080000,
   0x08000000,	 0x08000000,   0x08080000,   0x08080000,
   0x08000000,	 0x08000000,   0x08080000,   0x08080000,};
int ip_H2[] = {
   0x00000000,	 0x00040004,   0x00000000,   0x00040004,
   0x04000400,	 0x04040404,   0x04000400,   0x04040404,};
int ip_H3[] = {
   0x00000000,	 0x00000000,   0x00040000,   0x00040000,
   0x00000000,	 0x00000000,   0x00040000,   0x00040000,
   0x04000000,	 0x04000000,   0x04040000,   0x04040000,
   0x04000000,	 0x04000000,   0x04040000,   0x04040000,};
int ip_H4[] = {
   0x00000000,	 0x00020002,   0x00000000,   0x00020002,
   0x02000200,	 0x02020202,   0x02000200,   0x02020202,};
int ip_H5[] = {
   0x00000000,	 0x00000000,   0x00020000,   0x00020000,
   0x00000000,	 0x00000000,   0x00020000,   0x00020000,
   0x02000000,	 0x02000000,   0x02020000,   0x02020000,
   0x02000000,	 0x02000000,   0x02020000,   0x02020000,};
int ip_H6[] = {
   0x00000000,	 0x00010001,   0x00000000,   0x00010001,
   0x01000100,	 0x01010101,   0x01000100,   0x01010101,};
int ip_H7[] = {
   0x00000000,	 0x00000000,   0x00010000,   0x00010000,
   0x00000000,	 0x00000000,   0x00010000,   0x00010000,
   0x01000000,	 0x01000000,   0x01010000,   0x01010000,
   0x01000000,	 0x01000000,   0x01010000,   0x01010000,};
int ip_H8[] = {
   0x00000000,	 0x00800080,   0x00000000,   0x00800080,
   0x80008000,	 0x80808080,   0x80008000,   0x80808080,};
int ip_H9[] = {
   0x00000000,	 0x00000000,   0x00800000,   0x00800000,
   0x00000000,	 0x00000000,   0x00800000,   0x00800000,
   0x80000000,	 0x80000000,   0x80800000,   0x80800000,
   0x80000000,	 0x80000000,   0x80800000,   0x80800000,};
int ip_Ha[] = {
   0x00000000,	 0x00400040,   0x00000000,   0x00400040,
   0x40004000,	 0x40404040,   0x40004000,   0x40404040,};
int ip_Hb[] = {
   0x00000000,	 0x00000000,   0x00400000,   0x00400000,
   0x00000000,	 0x00000000,   0x00400000,   0x00400000,
   0x40000000,	 0x40000000,   0x40400000,   0x40400000,
   0x40000000,	 0x40000000,   0x40400000,   0x40400000,};
int ip_Hc[] = {
   0x00000000,	 0x00200020,   0x00000000,   0x00200020,
   0x20002000,	 0x20202020,   0x20002000,   0x20202020,};
int ip_Hd[] = {
   0x00000000,	 0x00000000,   0x00200000,   0x00200000,
   0x00000000,	 0x00000000,   0x00200000,   0x00200000,
   0x20000000,	 0x20000000,   0x20200000,   0x20200000,
   0x20000000,	 0x20000000,   0x20200000,   0x20200000,};
int ip_He[] = {
   0x00000000,	 0x00100010,   0x00000000,   0x00100010,
   0x10001000,	 0x10101010,   0x10001000,   0x10101010,};
int ip_Hf[] = {
   0x00000000,	 0x00000000,   0x00100000,   0x00100000,
   0x00000000,	 0x00000000,   0x00100000,   0x00100000,
   0x10000000,	 0x10000000,   0x10100000,   0x10100000,
   0x10000000,	 0x10000000,   0x10100000,   0x10100000,};
int ipi_L0[] = {
   0x00000000,	 0x01000000,   0x00010000,   0x01010000,
   0x00000100,	 0x01000100,   0x00010100,   0x01010100,
   0x00000001,	 0x01000001,   0x00010001,   0x01010001,
   0x00000101,	 0x01000101,   0x00010101,   0x01010101,};
int ipi_L2[] = {
   0x00000000,	 0x04000000,   0x00040000,   0x04040000,
   0x00000400,	 0x04000400,   0x00040400,   0x04040400,
   0x00000004,	 0x04000004,   0x00040004,   0x04040004,
   0x00000404,	 0x04000404,   0x00040404,   0x04040404,};
int ipi_L4[] = {
   0x00000000,	 0x10000000,   0x00100000,   0x10100000,
   0x00001000,	 0x10001000,   0x00101000,   0x10101000,
   0x00000010,	 0x10000010,   0x00100010,   0x10100010,
   0x00001010,	 0x10001010,   0x00101010,   0x10101010,};
int ipi_L6[] = {
   0x00000000,	 0x40000000,   0x00400000,   0x40400000,
   0x00004000,	 0x40004000,   0x00404000,   0x40404000,
   0x00000040,	 0x40000040,   0x00400040,   0x40400040,
   0x00004040,	 0x40004040,   0x00404040,   0x40404040,};
int ipi_L8[] = {
   0x00000000,	 0x02000000,   0x00020000,   0x02020000,
   0x00000200,	 0x02000200,   0x00020200,   0x02020200,
   0x00000002,	 0x02000002,   0x00020002,   0x02020002,
   0x00000202,	 0x02000202,   0x00020202,   0x02020202,};
int ipi_La[] = {
   0x00000000,	 0x08000000,   0x00080000,   0x08080000,
   0x00000800,	 0x08000800,   0x00080800,   0x08080800,
   0x00000008,	 0x08000008,   0x00080008,   0x08080008,
   0x00000808,	 0x08000808,   0x00080808,   0x08080808,};
int ipi_Lc[] = {
   0x00000000,	 0x20000000,   0x00200000,   0x20200000,
   0x00002000,	 0x20002000,   0x00202000,   0x20202000,
   0x00000020,	 0x20000020,   0x00200020,   0x20200020,
   0x00002020,	 0x20002020,   0x00202020,   0x20202020,};
int ipi_Le[] = {
   0x00000000,	 0x80000000,   0x00800000,   0x80800000,
   0x00008000,	 0x80008000,   0x00808000,   0x80808000,
   0x00000080,	 0x80000080,   0x00800080,   0x80800080,
   0x00008080,	 0x80008080,   0x00808080,   0x80808080,};
int ipi_H1[] = {
   0x00000000,	 0x01000000,   0x00010000,   0x01010000,
   0x00000100,	 0x01000100,   0x00010100,   0x01010100,
   0x00000001,	 0x01000001,   0x00010001,   0x01010001,
   0x00000101,	 0x01000101,   0x00010101,   0x01010101,};
int ipi_H3[] = {
   0x00000000,	 0x04000000,   0x00040000,   0x04040000,
   0x00000400,	 0x04000400,   0x00040400,   0x04040400,
   0x00000004,	 0x04000004,   0x00040004,   0x04040004,
   0x00000404,	 0x04000404,   0x00040404,   0x04040404,};
int ipi_H5[] = {
   0x00000000,	 0x10000000,   0x00100000,   0x10100000,
   0x00001000,	 0x10001000,   0x00101000,   0x10101000,
   0x00000010,	 0x10000010,   0x00100010,   0x10100010,
   0x00001010,	 0x10001010,   0x00101010,   0x10101010,};
int ipi_H7[] = {
   0x00000000,	 0x40000000,   0x00400000,   0x40400000,
   0x00004000,	 0x40004000,   0x00404000,   0x40404000,
   0x00000040,	 0x40000040,   0x00400040,   0x40400040,
   0x00004040,	 0x40004040,   0x00404040,   0x40404040,};
int ipi_H9[] = {
   0x00000000,	 0x02000000,   0x00020000,   0x02020000,
   0x00000200,	 0x02000200,   0x00020200,   0x02020200,
   0x00000002,	 0x02000002,   0x00020002,   0x02020002,
   0x00000202,	 0x02000202,   0x00020202,   0x02020202,};
int ipi_Hb[] = {
   0x00000000,	 0x08000000,   0x00080000,   0x08080000,
   0x00000800,	 0x08000800,   0x00080800,   0x08080800,
   0x00000008,	 0x08000008,   0x00080008,   0x08080008,
   0x00000808,	 0x08000808,   0x00080808,   0x08080808,};
int ipi_Hd[] = {
   0x00000000,	 0x20000000,   0x00200000,   0x20200000,
   0x00002000,	 0x20002000,   0x00202000,   0x20202000,
   0x00000020,	 0x20000020,   0x00200020,   0x20200020,
   0x00002020,	 0x20002020,   0x00202020,   0x20202020,};
int ipi_Hf[] = {
   0x00000000,	 0x80000000,   0x00800000,   0x80800000,
   0x00008000,	 0x80008000,   0x00808000,   0x80808000,
   0x00000080,	 0x80000080,   0x00800080,   0x80800080,
   0x00008080,	 0x80008080,   0x00808080,   0x80808080,};
int SP0[] = {
   0x08000820,	 0x00000800,   0x00020000,   0x08020820,
   0x08000000,	 0x08000820,   0x00000020,   0x08000000,
   0x00020020,	 0x08020000,   0x08020820,   0x00020800,
   0x08020800,	 0x00020820,   0x00000800,   0x00000020,
   0x08020000,	 0x08000020,   0x08000800,   0x00000820,
   0x00020800,	 0x00020020,   0x08020020,   0x08020800,
   0x00000820,	 0x00000000,   0x00000000,   0x08020020,
   0x08000020,	 0x08000800,   0x00020820,   0x00020000,
   0x00020820,	 0x00020000,   0x08020800,   0x00000800,
   0x00000020,	 0x08020020,   0x00000800,   0x00020820,
   0x08000800,	 0x00000020,   0x08000020,   0x08020000,
   0x08020020,	 0x08000000,   0x00020000,   0x08000820,
   0x00000000,	 0x08020820,   0x00020020,   0x08000020,
   0x08020000,	 0x08000800,   0x08000820,   0x00000000,
   0x08020820,	 0x00020800,   0x00020800,   0x00000820,
   0x00000820,	 0x00020020,   0x08000000,   0x08020800,};
int SP1[] = {
   0x00100000,	 0x02100001,   0x02000401,   0x00000000,
   0x00000400,	 0x02000401,   0x00100401,   0x02100400,
   0x02100401,	 0x00100000,   0x00000000,   0x02000001,
   0x00000001,	 0x02000000,   0x02100001,   0x00000401,
   0x02000400,	 0x00100401,   0x00100001,   0x02000400,
   0x02000001,	 0x02100000,   0x02100400,   0x00100001,
   0x02100000,	 0x00000400,   0x00000401,   0x02100401,
   0x00100400,	 0x00000001,   0x02000000,   0x00100400,
   0x02000000,	 0x00100400,   0x00100000,   0x02000401,
   0x02000401,	 0x02100001,   0x02100001,   0x00000001,
   0x00100001,	 0x02000000,   0x02000400,   0x00100000,
   0x02100400,	 0x00000401,   0x00100401,   0x02100400,
   0x00000401,	 0x02000001,   0x02100401,   0x02100000,
   0x00100400,	 0x00000000,   0x00000001,   0x02100401,
   0x00000000,	 0x00100401,   0x02100000,   0x00000400,
   0x02000001,	 0x02000400,   0x00000400,   0x00100001,};
int SP2[] = {
   0x10000008,	 0x10200000,   0x00002000,   0x10202008,
   0x10200000,	 0x00000008,   0x10202008,   0x00200000,
   0x10002000,	 0x00202008,   0x00200000,   0x10000008,
   0x00200008,	 0x10002000,   0x10000000,   0x00002008,
   0x00000000,	 0x00200008,   0x10002008,   0x00002000,
   0x00202000,	 0x10002008,   0x00000008,   0x10200008,
   0x10200008,	 0x00000000,   0x00202008,   0x10202000,
   0x00002008,	 0x00202000,   0x10202000,   0x10000000,
   0x10002000,	 0x00000008,   0x10200008,   0x00202000,
   0x10202008,	 0x00200000,   0x00002008,   0x10000008,
   0x00200000,	 0x10002000,   0x10000000,   0x00002008,
   0x10000008,	 0x10202008,   0x00202000,   0x10200000,
   0x00202008,	 0x10202000,   0x00000000,   0x10200008,
   0x00000008,	 0x00002000,   0x10200000,   0x00202008,
   0x00002000,	 0x00200008,   0x10002008,   0x00000000,
   0x10202000,	 0x10000000,   0x00200008,   0x10002008,};
int SP3[] = {
   0x00000080,	 0x01040080,   0x01040000,   0x21000080,
   0x00040000,	 0x00000080,   0x20000000,   0x01040000,
   0x20040080,	 0x00040000,   0x01000080,   0x20040080,
   0x21000080,	 0x21040000,   0x00040080,   0x20000000,
   0x01000000,	 0x20040000,   0x20040000,   0x00000000,
   0x20000080,	 0x21040080,   0x21040080,   0x01000080,
   0x21040000,	 0x20000080,   0x00000000,   0x21000000,
   0x01040080,	 0x01000000,   0x21000000,   0x00040080,
   0x00040000,	 0x21000080,   0x00000080,   0x01000000,
   0x20000000,	 0x01040000,   0x21000080,   0x20040080,
   0x01000080,	 0x20000000,   0x21040000,   0x01040080,
   0x20040080,	 0x00000080,   0x01000000,   0x21040000,
   0x21040080,	 0x00040080,   0x21000000,   0x21040080,
   0x01040000,	 0x00000000,   0x20040000,   0x21000000,
   0x00040080,	 0x01000080,   0x20000080,   0x00040000,
   0x00000000,	 0x20040000,   0x01040080,   0x20000080,};
int SP4[] = {
   0x80401000,	 0x80001040,   0x80001040,   0x00000040,
   0x00401040,	 0x80400040,   0x80400000,   0x80001000,
   0x00000000,	 0x00401000,   0x00401000,   0x80401040,
   0x80000040,	 0x00000000,   0x00400040,   0x80400000,
   0x80000000,	 0x00001000,   0x00400000,   0x80401000,
   0x00000040,	 0x00400000,   0x80001000,   0x00001040,
   0x80400040,	 0x80000000,   0x00001040,   0x00400040,
   0x00001000,	 0x00401040,   0x80401040,   0x80000040,
   0x00400040,	 0x80400000,   0x00401000,   0x80401040,
   0x80000040,	 0x00000000,   0x00000000,   0x00401000,
   0x00001040,	 0x00400040,   0x80400040,   0x80000000,
   0x80401000,	 0x80001040,   0x80001040,   0x00000040,
   0x80401040,	 0x80000040,   0x80000000,   0x00001000,
   0x80400000,	 0x80001000,   0x00401040,   0x80400040,
   0x80001000,	 0x00001040,   0x00400000,   0x80401000,
   0x00000040,	 0x00400000,   0x00001000,   0x00401040,};
int SP5[] = {
   0x00000104,	 0x04010100,   0x00000000,   0x04010004,
   0x04000100,	 0x00000000,   0x00010104,   0x04000100,
   0x00010004,	 0x04000004,   0x04000004,   0x00010000,
   0x04010104,	 0x00010004,   0x04010000,   0x00000104,
   0x04000000,	 0x00000004,   0x04010100,   0x00000100,
   0x00010100,	 0x04010000,   0x04010004,   0x00010104,
   0x04000104,	 0x00010100,   0x00010000,   0x04000104,
   0x00000004,	 0x04010104,   0x00000100,   0x04000000,
   0x04010100,	 0x04000000,   0x00010004,   0x00000104,
   0x00010000,	 0x04010100,   0x04000100,   0x00000000,
   0x00000100,	 0x00010004,   0x04010104,   0x04000100,
   0x04000004,	 0x00000100,   0x00000000,   0x04010004,
   0x04000104,	 0x00010000,   0x04000000,   0x04010104,
   0x00000004,	 0x00010104,   0x00010100,   0x04000004,
   0x04010000,	 0x04000104,   0x00000104,   0x04010000,
   0x00010104,	 0x00000004,   0x04010004,   0x00010100,};
int SP6[] = {
   0x40084010,	 0x40004000,   0x00004000,   0x00084010,
   0x00080000,	 0x00000010,   0x40080010,   0x40004010,
   0x40000010,	 0x40084010,   0x40084000,   0x40000000,
   0x40004000,	 0x00080000,   0x00000010,   0x40080010,
   0x00084000,	 0x00080010,   0x40004010,   0x00000000,
   0x40000000,	 0x00004000,   0x00084010,   0x40080000,
   0x00080010,	 0x40000010,   0x00000000,   0x00084000,
   0x00004010,	 0x40084000,   0x40080000,   0x00004010,
   0x00000000,	 0x00084010,   0x40080010,   0x00080000,
   0x40004010,	 0x40080000,   0x40084000,   0x00004000,
   0x40080000,	 0x40004000,   0x00000010,   0x40084010,
   0x00084010,	 0x00000010,   0x00004000,   0x40000000,
   0x00004010,	 0x40084000,   0x00080000,   0x40000010,
   0x00080010,	 0x40004010,   0x40000010,   0x00080010,
   0x00084000,	 0x00000000,   0x40004000,   0x00004010,
   0x40000000,	 0x40080010,   0x40084010,   0x00084000,};
int SP7[] = {
   0x00808200,	 0x00000000,   0x00008000,   0x00808202,
   0x00808002,	 0x00008202,   0x00000002,   0x00008000,
   0x00000200,	 0x00808200,   0x00808202,   0x00000200,
   0x00800202,	 0x00808002,   0x00800000,   0x00000002,
   0x00000202,	 0x00800200,   0x00800200,   0x00008200,
   0x00008200,	 0x00808000,   0x00808000,   0x00800202,
   0x00008002,	 0x00800002,   0x00800002,   0x00008002,
   0x00000000,	 0x00000202,   0x00008202,   0x00800000,
   0x00008000,	 0x00808202,   0x00000002,   0x00808000,
   0x00808200,	 0x00800000,   0x00800000,   0x00000200,
   0x00808002,	 0x00008000,   0x00008200,   0x00800002,
   0x00000200,	 0x00000002,   0x00800202,   0x00008202,
   0x00808202,	 0x00008002,   0x00808000,   0x00800202,
   0x00800002,	 0x00000202,   0x00008202,   0x00808200,
   0x00000202,	 0x00800200,   0x00800200,   0x00000000,
   0x00008002,	 0x00008200,   0x00000000,   0x00808002,};
int PC1[] = {
   0x10000000,	 0x00000000,   0x00100000,   0x00000000,
   0x00001000,	 0x00000000,   0x00000010,   0x00000000,
   0x00000000,	 0x00010000,   0x00000000,   0x01000000,
   0x00000001,	 0x00000000,   0x00000000,   0x00000000,
   0x20000000,	 0x00000000,   0x00200000,   0x00000000,
   0x00002000,	 0x00000000,   0x00000020,   0x00000000,
   0x00000000,	 0x00020000,   0x00000000,   0x02000000,
   0x00000002,	 0x00000000,   0x00000000,   0x00000000,
   0x40000000,	 0x00000000,   0x00400000,   0x00000000,
   0x00004000,	 0x00000000,   0x00000040,   0x00000000,
   0x00000000,	 0x00040000,   0x00000000,   0x04000000,
   0x00000004,	 0x00000000,   0x00000000,   0x00000000,
   0x80000000,	 0x00000000,   0x00800000,   0x00000000,
   0x00008000,	 0x00000000,   0x00000080,   0x00000000,
   0x00000000,	 0x00080000,   0x00000000,   0x08000000,
   0x00000008,	 0x00000000,   0x00000000,   0x00000000,
   0x01000000,	 0x00000000,   0x00010000,   0x00000000,
   0x00000100,	 0x00000000,   0x00000000,   0x00000100,
   0x00000000,	 0x00001000,   0x00000000,   0x00100000,
   0x00000000,	 0x10000000,   0x00000000,   0x00000000,
   0x02000000,	 0x00000000,   0x00020000,   0x00000000,
   0x00000200,	 0x00000000,   0x00000000,   0x00000200,
   0x00000000,	 0x00002000,   0x00000000,   0x00200000,
   0x00000000,	 0x20000000,   0x00000000,   0x00000000,
   0x04000000,	 0x00000000,   0x00040000,   0x00000000,
   0x00000400,	 0x00000000,   0x00000000,   0x00000400,
   0x00000000,	 0x00004000,   0x00000000,   0x00400000,
   0x00000000,	 0x40000000,   0x00000000,   0x00000000,
   0x08000000,	 0x00000000,   0x00080000,   0x00000000,
   0x00000800,	 0x00000000,   0x00000000,   0x00000800,
   0x00000000,	 0x00008000,   0x00000000,   0x00800000,
   0x00000000,	 0x80000000,   0x00000000,   0x00000000,};
int PC2[] = {
   0x00000000,	 0x20000000,   0x00000000,   0x00800000,
   0x00000000,	 0x00000000,   0x00000000,   0x00040000,
   0x00000010,	 0x00000000,   0x00000000,   0x00000000,
   0x00000000,	 0x02000000,   0x00000001,   0x00000000,
   0x00000080,	 0x00000000,   0x00000000,   0x00100000,
   0x00000000,	 0x00000000,   0x00000000,   0x08000000,
   0x00000000,	 0x40000000,   0x00000000,   0x00200000,
   0x00000008,	 0x00000000,   0x00000000,   0x10000000,
   0x00000000,	 0x04000000,   0x00000000,   0x00080000,
   0x00000000,	 0x80000000,   0x00000040,   0x00000000,
   0x00000000,	 0x00400000,   0x00000000,   0x00000000,
   0x00000004,	 0x00000000,   0x00000000,   0x01000000,
   0x00000000,	 0x00000000,   0x00000000,   0x00000000,
   0x00000000,	 0x00000000,   0x00000000,   0x00000000,
   0x00000000,	 0x00000000,   0x00000000,   0x00000000,
   0x00000000,	 0x00000000,   0x00000000,   0x00000000,
   0x08000000,	 0x00000000,   0x00000100,   0x00000000,
   0x02000000,	 0x00000000,   0x00010000,   0x00000000,
   0x04000000,	 0x00000000,   0x00400000,   0x00000000,
   0x00001000,	 0x00000000,   0x00004000,   0x00000000,
   0x00000000,	 0x00000000,   0x00100000,   0x00000000,
   0x20000000,	 0x00000000,   0x00020000,   0x00000000,
   0x00000200,	 0x00000000,   0x80000000,   0x00000000,
   0x00800000,	 0x00000000,   0x00002000,   0x00000000,
   0x40000000,	 0x00000000,   0x00000000,   0x00000000,
   0x00040000,	 0x00000000,   0x00000400,   0x00000000,
   0x00200000,	 0x00000000,   0x00000000,   0x00000000,
   0x00080000,	 0x00000000,   0x10000000,   0x00000000,
   0x00000000,	 0x00000000,   0x00008000,   0x00000000,
   0x00000800,	 0x00000000,   0x01000000,   0x00000000,
   0x00000000,	 0x00020000,   0x00000002,   0x00000000,
   0x00000020,	 0x00000000,   0x00000000,   0x00010000,};
 
static extra;
char E[48];					/* 0x255c4 */
 
char *crypt(passwd, salt)			/* 0x68f8 */
     char *passwd, *salt;
{
    int	temp, l8;
    register i,	j;
    register  c;				/*d7, d6, d5*/
    static char	iobuf[10];			/* 0x27f34 */
    static unsigned x27f44;
    static unsigned x27f48;
 
    x27f44 = 0;
    x27f48 = 0;
 
    for( i = 0;	i < 48;	i++)
	E[i] = e[i];
 
    for(i = 0; (c = *passwd)  &&  (i < 32); i++, passwd++)
	for(j =	0; j < 7; j++, i++) {
	    l8 = (c >> (6 - j))	& 01;
	    x27f44 |= (l8 << (31 - i));
	}
 
    for	(i = 0;	(c = *passwd)  &&  (i <	32); i++, passwd++)
	for(j =	0; j < 7; j++, i++) {
	    l8 = (c >> (6 - j))	& 01;
	    x27f48 |= (l8 << (31 - i));
	}
 
    compkeys(&x27f44, 0);
 
    for(i=0;i<2;i++){
	c = *salt++;
	iobuf[i] = c;
	if(c>'Z') c -= 6;
	if(c>'9') c -= 7;
	c -= '.';
	for(j=0;j<6;j++){
	    if((c>>j) &	01){
		temp = E[6*i+j];
		E[6*i+j] = E[6*i+j+24];
		E[6*i+j+24] = temp;
	    }
	}
    }
 
    mungE();
    x27f44 = 0;
    x27f48 = 0;
    des(&x27f44, &x27f44);
    ipi(&x27f44, &x27f44);
 
    for(i=0; i<11; i++){
	c = x27f44 >> 26;
	x27f44	= x27f44 << 6;
	x27f44 |= x27f48 >> 26;
	x27f48 = x27f48	<< 6;
	c += '.';
	if(c > '9') c += 7;
	if(c > 'Z') c += 6;
	iobuf[i+2] = c;
    }
    iobuf[i+2] = 0;
    if(iobuf[1]	== 0)
	iobuf[1] = iobuf[0];
    return(iobuf);
}
 
int E_H[8][16];					/* 0x251c4 */
int E_L[8][16];					/* 0x253c4 */
mungE()						/* 0x6b2a */
{
    register i,	j, d5, d4, d3, d2;
    register *a5, *a4;
    int	l28;
 
    for(i = 0; i < 8; i++) {
	a5 = E_L[i];
	a4 = E_H[i];
	for(j =	0; j < 16; j++)	{
	    *a5++ = 0;
	    *a4++ = 0;
	}
    }
    for	(j = 0;	j < 32;	j++) {
	d2 = 1 << (31 -	j);
	d3 = 31	- E[j];
	d4 = 1 << (d3 &	3);
	a5 = E_L[d3 >> 2];
	for (i = 1; i <	16; i++)
	    if (i & d4)
		a5[i] |= d2;
    }
    for	(j = 32; j < 48; j++) {
	d2 = 1 << (63-j);
	d3 = 31	- E[j];
	d4 = 1 << (d3 &	3);
	a5 = E_H[d3 >> 2];
	for (i = 1; i <	16; i++)
	    if (i & d4)
		a5[i] |= d2;
    }
}
 
int keys_H[16],	keys_L[16];			/* 0x255f4,0x25634 */
 
compkeys(iptr, key)					/* 0x6c04 */
     int *iptr;
{
    int	i, l8, l12, l16;
    register d7, d6, d5, d4, d3, d2;
 
    d7 = 0;
    d6 = 0;
    for	(d3 = 0, d2 = iptr[1];	d3 < 64; d2*=2,	d3+=2)
	if (d2 < 0) {
	    d7 |= PC1[d3];
	    d6 |= PC1[d3+1];
	}
 
    for	(d2 = iptr[0];	d3 < 128; d2*=2, d3+=2)
	if (d2 < 0) {
	    d7 |= PC1[d3];
	    d6 |= PC1[d3+1];
	}
 
 
    for	(i = 0;	i < 16;	i++) {
	for (d2	= 0; d2	< shift[i]; d2++) {
	    l16	= l12 =	l8 = 0;
	    if (d7 < 0)
		l8 = 16;
	    if (d7 & 0x08)
		l12 = 256;
	    if (d6 < 0)
		l16 = 1;
	    d7 = ((d7 << 1) & ~0x10) | l8 | l16;
	    d6 = (d6 <<	1) | l12;
	}
 
 
	d5 = 0;
	d4 = 0;
	for (d3=0, d2=d6;  d3 <	64;  d2*=2, d3+=2) {
	    if (d2 < 0)	{
		d5 |= PC2[d3];
		d4 |= PC2[d3+1];
	    }
	}
	for (d2=d7;  d3	< 128;	d2*=2, d3+=2) {
	    if (d2 < 0)	{
		d5 |= PC2[d3];
		d4 |= PC2[d3+1];
	    }
	}
 
	if (key) {
	    keys_L[15-i] = d5;
	    keys_H[15-i] = d4;
	} else {
	    keys_L[i] =	d5;
	    keys_H[i] =	d4;
	}
    }
 
}
 
setupE()
{
    int	i, j, l12;
 
    for(i = 0; i < 8; i++)
	for(j =	0; j < 16; j++)
	    E_H[i][j] =	E_H[i][j] = 0;
 
    for	(j = 0;	j < 32;	j++) {
	l12 = 31 - E[j];
	for (i = 0; i <	16; i++)
	    if ((1 << (l12 % 4)) & i)
		E_L[l12	/ 4][i]	|= (1 << (31 - j));
    }
 
    for	(j = 32; j < 48; j++) {
	l12 = 31 - E[j];
	for (i = 0; i <	16; i++)
	    if ((1 << (l12 % 4)) & i)
		E_H[l12	/ 4][i]	|= (1 << (63 - j));
    }
}
 
des(adr1, adr2)
     int *adr1,	*adr2;
{
    int	l4, *l8, *l12, l16;
    register unsigned d7;
    register unsigned d6, d5;
    register d4, d3, d2;
 
    l4 = adr1[0];
    d2 = adr1[1];
    for	(l16 = 0; l16 <	25; l16++) {
	l8 = keys_L;
	l12 = keys_H;
	for( d3	= 0;  d3 < 16;	d3++) {
	    d5 = d2;
	    d7 = E_L[0][d4 = d5	& 0x0f];
	    d6 = E_H[0][d4];
	    d5 >>= 4;
	    d7 |= E_L[1][d4 = (d5 & 0x0f)];
	    d6 |= E_H[1][d4];
	    d5 >>= 4;
	    d7 |= E_L[2][d4 = (d5 & 0x0f)];
	    d6 |= E_H[2][d4];
	    d5 >>= 4;
	    d7 |= E_L[3][d4 = (d5 & 0x0f)];
	    d6 |= E_H[3][d4];
	    d5 >>= 4;
	    d7 |= E_L[4][d4 = (d5 & 0x0f)];
	    d6 |= E_H[4][d4];
	    d5 >>= 4;
	    d7 |= E_L[5][d4 = (d5 & 0x0f)];
	    d6 |= E_H[5][d4];
	    d5 >>= 4;
            d7 |= E_L[6][d4 = (d5 & 0x0f)];
            d6 |= E_H[6][d4];
            d5 >>= 4;
            d7 |= E_L[7][d4 = (d5 & 0x0f)];
            d6 |= E_H[7][d4];
            d7 ^= *l8++;
            d6 ^= *l12++;

            d5 = SPO[(d6 >> 16) & 0x3f];
            d5 |= SP1[(d6 >> 22) & 00x3f];
            d5 |= SP2[((d7 & 0x03) << 4) | ((d6 >> 28) & 0x0f)];
            d5 |= SP3[(d7 >> 2) & 0x3f];
            d5 |= SP4[(d7 >> 8) & 0x3f];
            d5 |= SP5[(d7 >> 14) & 0x3f];
            d5 |= SP6[(d7 >> 20) & 0x3f];
            d5 |= SP7[(d7 >> 26) & 0x3f];
            {   d6 = 14;
                l4 = d2;
                d2 = d6 ^ d5;
            }
       }
       d5 = l4;
       l4 = d2;
       d2 = d5;
   }
   adr2[0] = l4;
   adr2[1] = d2;
}
ipi(iptr1, iptr2)
            int *iptr1, *iptr2;
{
   register unsigned d7, d6, d5;

   d5 = iptr1[0];
   d7 = ipi_L0[d5 & 0x0f];
   d5 = >>= 4;
   d6 = ipi_H1[d5 & 0x0f];

   d5 >>= 4;
   d7 |= ipi_L2[d5 & 0x0f];
   d5 >>= 4;
   d6 |= ipi_H3[d5 & 0x0f];

   d5 >>= 4;
   d7 |= ipi_L4[d5 & 0x0f];
   d5 >>= 4;
   d6 |= ipi_H5[d5 & 0x0f];

   d5 >>= 4;
   d7 | ipi_L6[d5 & 0x0f];
   d5 >>=4;
   d6 |= ipi_H7[d5 & 0x0f];

   d5 = iptr1[1];
   d7 |= ipi_L8[d5 & 0x0f];
   d5 >>= 4;
   d6 |= ipi_H9[d5 & 0x0f];

   d5 >>= 4;
   d7 |= ipi_La[d5 & 0x0f];
   d5 >>= 4;
   d6 |= ipi_Hb[d5 & 0x0f];

   d5 >>=4;
   d7 |= ipi_Lc[d5 & 0x0f];
   d5 >>= 4;
   d6 |= ipi_Hd[d5 & 0x0f];

   d5 >>= 4;
   d7 |= ipi_Le[d5 & 0x0f];
   d5 >>= 4;
   d6 |= ipi_Hf[d5 & 0x0f];

   iptr2[0] = d7;
   ipyr2[1] = d6;
}

/*
 * Local variables:
 * compile-command: "make"
 * comment-collumn: 48
 * End:
 */
