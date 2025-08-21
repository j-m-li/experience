                               B9
                          public domain

  three registers, 27 instructions, CPU stack based.
  2 spaces introduce a comment till the end of line
  1 space introduce an instruction
  0 space is a label
  all characters except 01234abcd, <space> and <newline> are ignored.
  a character is 2 trit
  jump labels begin with a 0.
  data labels begin with 00.
  a number after a label sets the label's address.
  instruction size is 1 tryte (3 trits, 6 bits)
  word size is 18 trit (6 trytes, 34 bits)
  labels are tryte aligned.
  constant instruction begining with a 0 is a label, otherwise it's a number.

  in balanced ternary, a trit has 3 states: -1 0 1
  we represent a trit in binary using two bits.
  two trits have 3 * 3 = 9 states: -4 -3 -2 -1 0 1 2 3 4
  three trits are a tryte which has 3 * 3 * 3 = 27 states (-13 .. 0 .. 13).

  we use a single ASCII or Braille character to represent two trits :
  4  4  ⠎_
  3  3  ⠌_
  2  2  ⠕_
  1  1  ⠜_
  0  0  ⠀_
  a -1  ⠣_
  b -2  ⠪_
  c -3  ⠡_
  d -4  ⠱_

  power of 9:
        7      6     5    4   3  2 1 0
  4782969 531441 59049 6561 729 81 9 1

  power of 3:
      11    10     9    8    7   6   5  4  3 2 1 0
  177147 59049 19683 6561 2187 729 243 81 27 9 3 1

  ---------------------------------------------------- 
   +-
   ---
  |   | 
  |** |  trit 0  1      FONT : 3x4 pixels. 
  |** |  trit 1  3
  |** |  trit 2  9
   ---
  RULES: get the most readable font with only 6 pixels in ternary context.
         1 blank column between glyphs, one blank row at the top of each glyph.
         trit 0 and trit 2 can't have the value 0, there must be a
         least one + trit and one - trit, except for empty glyph (0). 
  -- -11 d     -- 11 4
  *             *
   *           *
   *           *
  --  -8 c     --  8 3 
  *             *
  
   *           *
  --  -7 b     --  7 2
   *           *
  *             *
   *           *
  --  -5 a     --  5 1
  *             *
  *             *
   *           *
  --   0 0     --

  

  -- ---------------------------------------


  This is the entry point :
01 0
 14  div3       a = a / 3
 13  mul        a = a * b
 12  sub        a = a - b
 11  add        a = a + b
 10 001  const  a = 0x0xxx 
 1a  swapac     tmp = a; a = c; c = tmp
 1b  jal        tmp = c; c = pc; goto tmp
 1c  lst        a = get_trit_0(a)  least significant trit
 1d  div2       a = a / 2

 04  bgt    if (a > b) goto c
 03  bge    if (a >= b) goto c
 02  beq    if (a == b) goto c
 01  beqz   if (a == 0) goto c
 00  nop        no operation
 0a  bnez   if (a != 0) goto c
 0b  bne    if (a != b) goto c
 0c  ble    if (a <= b) goto c
 0d  blt    if (a < b) goto c

 a4  loada    a = mem[b]
 a3  storea   mem[b] = a
 a2  popa     a = mem[sp++]
 a1  pusha    mem[--sp] = a
 a0  swapab   tmp = a; a = b; b = tmp
 aa  pushb    mem[--sp] = b 
 ab  popb     b = mem[sp++]
 ac  storeb   mem[c] = b
 ad  loadb    b = mem[c]

  Load the word starting at the second tryte of data label into register A:
02
 a0
 10 001
 a0
 10 1
 11
 ad

   Some data (7 trytes):
001
 33 4 ac 12 40 1b c 
002
 01
004

