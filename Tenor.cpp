//
//  Tenor.cpp
//  XQLib
//
//  Created by Yassine Bounfour on 09/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#include "Tenor.hpp"
#include <XQLib/Utils/Strings.hpp>
#include <XQLib/Core/Exception.hpp>


namespace XQLib
{
    
    Tenor add_tenor(Tenor const& tenor1, Tenor const& tenor2)
    {
        MY_THROW("Not implemented!");
        return "0D";
        /*
        std::size_t n1 = tenor1.size();
        std::size_t n2 = tenor2.size();
        MY_ASSERT((n1>1) && (n2>1) , "Tenors should have at least 2 characters!");
        // C++11
        // std::string unit1 =  tenor1.back();
        // std::string unit2 =  tenor2.back();
        std::string unit1 =  tenor1.substr(tenor1.size()-1, 1);
        std::string unit2 =  tenor2.substr(tenor2.size()-1, 1);
        MY_ASSERT(unit1 == unit2, "Addition of tenors is only implemented when they have the same unit");
        int value1 = StringToNumber<int>(tenor1.substr(0, tenor1.size()-1));
        int value2 = StringToNumber<int>(tenor2.substr(0, tenor2.size()-1));
        int value = value1 + value2;
        std::string new_tenor = number_to_string<int>(value) + unit1;
        return new_tenor;
         */
    }
    
    std::string Tenor2::to_string() const
    {
        std::ostringstream oss;
        oss << count_;
        switch (unit_)
        {
            case TimeUnit::Day:
                oss << "D";
                break;
            case TimeUnit::BusDay:
                oss << "B";
                break;
            case TimeUnit::Week:
                oss << "W";
                break;
            case TimeUnit::Month:
                oss << "M";
                break;
            case TimeUnit::Year:
                oss << "Y";
                break;
            default:
                XQLIB_THROW("Unknown time unit");
                break;
        }
        return oss.str();
    }
    
    
    Tenor2::operator std::string() const
    {
        return this->to_string();
    }
    
    
    Tenor2::Tenor2(std::string const& tenor)
    {
        std::size_t n1 = tenor.size();
        XQLIB_ASSERT(n1>1, "Tenors should have at least 2 characters!");
        count_ = string_to_number<long>(tenor.substr(0, tenor.size()-1));
        
        std::string unit =  tenor.substr(tenor.size()-1, 1);
        if(unit == "D")
            unit_ = TimeUnit::Day;
        else if (unit == "B")
            unit_ = TimeUnit::BusDay;
        else if (unit == "W")
            unit_ = TimeUnit::Week;
        else if (unit == "M")
            unit_ = TimeUnit::Month;
        else if (unit == "Y")
            unit_ = TimeUnit::Year;
        else
            XQLIB_THROW("Invalid tenor unit.");
    }
    
    
} // namespace XQLib
