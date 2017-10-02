//
//  RollRule.hpp
//  XQLib
//
//  Created by Yassine Bounfour on 03/07/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#ifndef RollRule_hpp
#define RollRule_hpp

#include <XQLib/DateTime/Date.hpp>

namespace XQLib
{
    
    XQLIB_ENUM(Rolltype,
               Default,       ///< Roll on the day with the same number.
               EOM,           ///< Roll on the last day in the month.
               IMM,           ///< Roll on the third occurrence of wednesday in the month.
               NthDay,        ///< Roll on the n-th occurrence of a weekday in the month (specified by the roll date).
               Adjusted,      ///< Roll date falling on holiday is adjusted with ModifiedFollowing, and the subsequential dates will roll on this new date. Currently it is used by some FRN.
               NMonthEnd,     ///< Roll on the last day of the month is the rolldate is the last calendar day of the month. Otherwise, rolls like Default.
               IMMAUD,        ///< Roll on the day preceding the second occurrence of friday in the month.
               IMMCAD,        ///< Roll with expiry two days preceding the third wednesday in the month.  (Same as IMM?)
               IMMNZD         ///< Roll on the first wednesday after the ninth day of the month.
    );

}

#endif /* RollRule_hpp */
