//
//  Calendar.cpp
//  XQLib
//
//  Created by Yassine Bounfour on 09/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#include "Calendar.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <XQLib/Utils/VectorUnion.hpp>
#include <XQLib/Utils/Strings.hpp>
#include <XQLib/Core/Exception.hpp>

namespace XQLib
{

    Calendar::Calendar()
    {
        weekend_.insert(Sunday);
        weekend_.insert(Saturday);
        holidays_ = std::set<Date>();
        tag_ = "STANDARD";
        
    }
    
    bool Calendar::is_business_day(Date const& d) const
    {
        if(!holidays_.empty())
        {
            XQLIB_ASSERT( d >= *holidays_.begin() , "Date is earlier than earliest holiday date ");
            //XQLIB_ASSERT((d >= holidays_[0]) , "Date " +to_simple_string(d) +  " is earlier than earliest holiday date " + to_simple_string(holidays_[0]) );
        }
        
        bool result = true;
        
        // loop through weekend_:
        for(auto w : weekend_)
        {
            if(d.day_of_week() == w)
            {
                result = false;
                break;
            }
        }
        
        //loop through holidays_:
        for(auto h : holidays_)
        {
            if(d == h)
            {
                result = false;
                break;
            }
            
        }
        
        return result;
    }
    
    std::set<Weekdays> const& Calendar::get_weekend() const
    {
        return weekend_;
    }
    
    std::set<Date> const& Calendar::get_holidays() const
    {
        return holidays_;
    }
    
    std::string const& Calendar::get_tag() const
    {
        return tag_;
    }
    
    void Calendar::add_holidays(std::vector<Date> const& v)
    {
        for(std::size_t j = 0; j < v.size(); ++j)
        {
            holidays_.insert(v[j]);
        }
    }
    
    Calendar& Calendar::add(Calendar const& other)
    {
        weekend_.insert(other.weekend_.begin(),other.weekend_.end()); // = vector_union(weekend_, other.get_weekend());
        holidays_.insert(other.holidays_.begin(),other.holidays_.end()); // = vector_union(holidays_, other.get_holidays());
        tag_ = tag_ == "STANDARD" ? other.get_tag() : tag_ + "." + other.get_tag();
        
        return *this;
    }
    
    //Calendar unions:
    Calendar calendar_union(const Calendar& c1, const Calendar& c2)
    {
        Calendar c(c1);
        c.add(c2);
        return c;
    }
    
    Calendar calendar_union(std::vector<Calendar> const& v)
    {
        Calendar cal;
        for(auto c : v)
            cal.add(c);
        return cal;
    }
    
}//namespace XQLib
