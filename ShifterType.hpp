//
//  ShifterType.hpp
//  XQLib
//
//  Created by Yassine Bounfour on 16/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#ifndef ShifterType_hpp
#define ShifterType_hpp

#include <XQLib/Utils/Enum.hpp>

namespace XQLib
{
    
    XQLIB_ENUM(ShifterTypes,
               NoShifter);
    
    typedef ShifterTypes::Enum ShifterType;
    
}


#endif /* ShifterType_hpp */
