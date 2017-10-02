//
//  DayCount.hpp
//  XQLib
//
//  Created by Yassine Bounfour on 10/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#ifndef DayCount_hpp
#define DayCount_hpp

#include <XQLib/Utils/Enum.hpp>

namespace XQLib
{
    
    XQLIB_ENUM(Basis,
               Act360,
               Act365,
               Act365L,
               B30360,
               B30E360,
               ActActISDA,
               ActActISMA,
               ActActAFB,
               BD252,
               B30EP360,
               B30I360,
               B30U360,
               B30365,
               B30Act,
               B30UAct);
    
    

    typedef Basis::Enum DayCount;
    
    DayCount const DefaultDayCount = Basis::Act365;

}

#endif /* DayCount_hpp */
