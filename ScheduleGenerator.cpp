//
//  ScheduleGenerator.cpp
//  XQLib
//
//  Created by Yassine Bounfour on 03/07/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#include "ScheduleGenerator.hpp"


/*
 static void GenSchedule(
 const object_t<IRollRule>&		rule,		// rolling rule
 date_t							start,		// first date in schedule
 date_t							finish,		// last date in schedule
 const maybe_t<size_t>&			fs,			// front stub control
 const maybe_t<size_t>&			bs,			// back stub control
 std::vector<date_t>&			dates		// result
 )
 {
 if (start > finish)
 THROW(invalid_argument, "reversed or equal date range: " << start.to_string() << ", " << finish.to_string());
 
 // n.b. this is poor guess of capacity under mixed freq but it won't affect correctness
 
 size_t guess = 1 + static_cast<size_t>(rule->Freq(rule->Ceil(start)) * (finish - start) / 365.);
 
 dates.clear();
 dates.reserve(guess);
 
 dates.push_back(start);
 
 date_t next = rule->Ceil(start);
 date_t last = rule->Floor(finish);
 
 if (next != start && fs.nothing())
 THROW(invalid_argument, start.to_string() << " is not regular, next = " << next.to_string() << " (stub control?)");
 
 if (last != finish && bs.nothing())
 THROW(invalid_argument, finish.to_string() << " is not regular, prev = " << last.to_string() << " (stub control?)");
 
 next = rule->Add(next, std::max<int>(next == start ? 1 : 0, maybe<size_t>(fs, 0)));
 last = rule->Add(last, -std::max<int>(last == finish ? 1 : 0, maybe<size_t>(bs, 0)));
 
 if (next > last)
 {
 if(maybe<size_t>(fs, 0) != maybe<size_t>(bs, 0))
 THROW(invalid_argument, "schedule has no interior periods, front and back stub controls must be equal for consistency");
 }
 else
 {
 while (next < last)
 {
 dates.push_back(next);
 next = rule->Add(next, 1, true);
 }
 
 if (last > start && last < finish)
 dates.push_back(last);
 }
 
 if ( finish != start )
 dates.push_back(finish);
 }

*/