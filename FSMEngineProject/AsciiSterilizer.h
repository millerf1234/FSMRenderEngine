//                                                 
// File:               AsciiSterilizer.h              
//                                               
// Class:              AsciiSterilizer                       
// QuickInfo:                                            
//       Default Constructor            DISABLED   
//       Copy Operations                ENABLED     
//       Move Operations                ENABLED    
//                                                    
//                                                     
//                                                      
//                                                           
//                                                       
//                                                       
// Description:         Meant to be used to remove all occurrences of a specified sequence of
//                      characters from ASCII files. Object is constructed with a specific 
//                      character sequence and then has member functions for providing it 
//                      targets.
//
// Potentially Thrown in Construction
//                                      -INVALID_ASCII_SEQ
// :                  Purges all occurrences of any desired valid character sequence from a
//                     targeted file.
//                     This types invariant is the character sequence, which must conform to
//                     a few basic rules to be considered 'valid'. Invalid 
//
// Warning:            Will purge all occurrences of the specified valid* character sequence from 
//                     each targeted file. Is very effective. Use with caution. 
//
// Programmer:           Forrest Miller
// Date:                 March 2019
//


#pragma once

#ifndef ASCII_STERILIZER_H_
#define ASCII_STERILIZER_H_ 

#include <string_view>
#include <string>
#include <filesystem>
#include <fstream>

#include "UniversalIncludes.h"
#include "FSMException.h"
#include "AsciiTextFile.h"


// Reference:  
//                                                                                                                
//                 ASCII TABLE                                                                             
//          Source: https://www.cs.cmu.edu/~pattis/15-1XX/common/handouts/ascii.html                     
//     +----------------------------------------------------------------------------------------+              
//    /|                                                                                        |\      
//    ||                                                                                        ||     
//    ||    Dec  Char                           Dec  Char     Dec  Char     Dec  Char           ||     
//    ||    ---------                           ---------     ---------     ----------          ||    
//    ||       0  NUL (null)                      32  SPACE     64  @         96  `             ||      
//    ||       1  SOH (start of heading)          33  !         65  A         97  a             ||    
//    ||       2  STX (start of text)             34  "         66  B         98  b             ||      
//    ||       3  ETX (end of text)               35  #         67  C         99  c             ||    
//    ||       4  EOT (end of transmission)       36  $         68  D        100  d             ||      
//    ||       5  ENQ (enquiry)                   37  %         69  E        101  e             ||    
//    ||       6  ACK (acknowledge)               38  &         70  F        102  f             ||    
//    ||       7  BEL (bell)                      39  '         71  G        103  g             ||    
//    ||       8  BS  (backspace)                 40  (         72  H        104  h             ||      
//    ||       9  TAB (horizontal tab)            41  )         73  I        105  i             ||    
//    ||      10  LF  (NL line feed, new line)    42  *         74  J        106  j             ||       
//    ||      11  VT  (vertical tab)              43  +         75  K        107  k             ||    
//    ||      12  FF  (NP form feed, new page)    44  ,         76  L        108  l             ||      
//    ||      13  CR  (carriage return)           45  -         77  M        109  m             ||     
//    ||      14  SO  (shift out)                 46  .         78  N        110  n             ||      
//    ||      15  SI  (shift in)                  47  /         79  O        111  o             ||    
//    ||      16  DLE (data link escape)          48  0         80  P        112  p             ||      
//    ||      17  DC1 (device control 1)          49  1         81  Q        113  q             ||      
//    ||      18  DC2 (device control 2)          50  2         82  R        114  r             ||        
//    ||      19  DC3 (device control 3)          51  3         83  S        115  s             ||      
//    ||      20  DC4 (device control 4)          52  4         84  T        116  t             ||      
//    ||      21  NAK (negative acknowledge)      53  5         85  U        117  u             ||    
//    ||      22  SYN (synchronous idle)          54  6         86  V        118  v             ||     
//    ||      23  ETB (end of trans. block)       55  7         87  W        119  w             ||    
//    ||      24  CAN (cancel)                    56  8         88  X        120  x             ||    
//    ||      25  EM  (end of medium)             57  9         89  Y        121  y             ||    
//    ||      26  SUB (substitute)                58  :         90  Z        122  z             ||       
//    ||      27  ESC (escape)                    59  ;         91  [        123  {             ||    
//    ||      28  FS  (file separator)            60  <         92  \        124  |             ||     
//    ||      29  GS  (group separator)           61  =         93  ]        125  }             ||    
//    ||      30  RS  (record separator)          62  >         94  ^        126  ~             ||        
//    ||      31  US  (unit separator)            63  ?         95  _        127  DEL           ||     
//    \|                                                                                        |/         
//     `+--------------------------------------------------------------------------------------+'
//
//                              CHARACTER SEQUENCE RULES FOR VALIDITY 
// Rules for determining validity of a potential target character sequence:
//
//    (-) Sequence must be at least 2 characters in length
//
//    (-) 
//   
//    (-) Any string that both begins and ends with whitespace is automatically invalid.
//        Whitespace is defined as any space-occupying character, include ' ' and '\t'.
//        There is an option to enable on valid instances of this object that will allow
//        for an auto-format to be applied which
//                     [(NO) is completely customizable (NO)]
//                  [or]
//                     [Allows for basic pre- and post-location-of-removed-char-sequence
//                      formating to be applied where needed0.
//
//    (-) Any string beginning xor ending with a whitespace (but not at both ends) may be 
//        considered valid only if sequence contains at least 2 non-white-space characters 
//         
//   
// 
//
//





class AsciiSterilizer {
public:
    AsciiSterilizer() = delete;
    //This constructor will throw an 'INVALID_ASCII_SEQ' FSMNamedException  
    //exception if the parameter string does not meet the requirements for
    //being considered valid. 
    AsciiSterilizer(std::string_view targetValidCharSeq);
    ~AsciiSterilizer() noexcept;

    AsciiSterilizer(const AsciiSterilizer&);
    AsciiSterilizer(AsciiSterilizer&&) noexcept;

    AsciiSterilizer& operator=(const AsciiSterilizer&)  = default;
    AsciiSterilizer& operator=(AsciiSterilizer&&) noexcept  = default;


    static bool validateCharSeq(std::string_view seq) noexcept;

    int purgeString(std::string& stringToPurge);
    int purgeFile(AsciiTextFile file) const;     
    int purgeFile(std::filesystem::path theFile) const;


private:
    const std::string mTargetCharSeq_;

    struct PurgeHistoryRecord {
        //What was deleted where and from what
        //filename
        //offsets
        //count_occurances_Purged
    };

    mutable struct PurgeHistoryRecord mHistory_;
};



#endif // ASCII_STERILIZER_H_

