//
//  ScheduleGenerator.hpp
//  XQLib
//
//  Created by Yassine Bounfour on 03/07/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#ifndef ScheduleGenerator_hpp
#define ScheduleGenerator_hpp

#include <XQLib/Core/Maybe.hpp>
#include <XQLib/DateTime/Date.hpp>

namespace XQLib
{

    class RollRule;
    
    
    static void GenSchedule(std::shared_ptr<RollRule> const&		rule,		// rolling rule
                            Date const&					start,		// first date in schedule
                            Date const&					finish,		// last date in schedule
                            maybe_t<size_t> const&			fs,			// front stub control
                            maybe_t<size_t> const&			bs,			// back stub control
                            std::vector<Date>&			dates		// result
);

}

#endif /* ScheduleGenerator_hpp */
