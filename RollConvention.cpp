//
//  RangeAccrualCouponSchedule.cpp
//  XQLib
//
//  Created by Yassine Bounfour on 09/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#include "RollConvention.hpp"

#include <XQLib/Core/Exception.hpp>
#include <XQLib/DateTime/Tenor.hpp>
#include <XQLib/DateTime/Calendar.hpp>
#include <XQLib/DateTime/ShifterType.hpp>

#include <boost/lexical_cast.hpp>
#include <XQLib/Core/Memory.hpp>
#include <boost/make_shared.hpp>



namespace XQLib
{
    
    
    void populate_korea_tags(std::vector<std::string> & v)
    {
        v.resize(11);
        
        v[0] = "XKFB_S";
        v[1] = "XKFB_T";
        v[2] = "XKOS_S";
        v[3] = "XKOS_T";
        v[4] = "XKFE_S";
        v[5] = "XKFE_T";
        v[6] = "XKRX_S";
        v[7] = "XKRX_T";
        v[8] = "KR_SEL";
        v[9] = "XKON_S";
        v[10] = "XKON_T";
    }
    
    bool korea_tag_present(std::string const& str)
    {
        std::vector<std::string> korea_tags;
        populate_korea_tags(korea_tags);
        
        for(size_t i=0; i<korea_tags.size(); ++i)
        {
            if(str.find(korea_tags[i]) != std::string::npos)
                return true;
        }
        
        return false;
    }
    

    Date DateAdjust::adjust(Date const&d, const Calendar &cal) const 
    {
        Date new_date(d); /// FIXME
        switch(roll_convention_)
        {
            case Adjustment::NoAdjustment:
            {
                break;
            }
            case Adjustment::Following:
            {
                while(!cal.is_business_day(new_date))
                    new_date += DateDuration(1);
                break;
            }
            case Adjustment::Preceding:
            {
                while(!cal.is_business_day(new_date))
                    new_date -= DateDuration(1);
                break;
            }
            case Adjustment::ModifiedFollowing:
            {
                Date start_date = new_date;
                while(!cal.is_business_day(new_date))
                    new_date += DateDuration(1);
                if(new_date > start_date.end_of_month())
                {
                    new_date = DateAdjust(Adjustment::Preceding).adjust(start_date,cal);
                }
                break;
            }
            case Adjustment::ModifiedPreceding:
            {
                Date start_date = new_date;
                while(!cal.is_business_day(new_date))
                    new_date -= DateDuration(1);
                if(new_date < Date(start_date.year(),start_date.month(),1))
                {
                    new_date = DateAdjust(Adjustment::Following).adjust(start_date,cal);
                }
                break;
            }
            case Adjustment::Nearest:
            {
                if(new_date.day_of_week() == Sunday || new_date.day_of_week() == Monday)
                {
                    new_date = DateAdjust(Adjustment::Following).adjust(new_date,cal);
                }
                else
                {
                    new_date = DateAdjust(Adjustment::Preceding).adjust(new_date,cal);
                }
                break;
            }
            case Adjustment::KoreaFollowing:
            {
                XQLIB_ASSERT(korea_tag_present(cal.get_tag()),"Korean calendar tag required for KoreaFollowing adjustment.");
                if(!(cal.is_business_day(new_date)))
                {
                    new_date = DateAdjust(Adjustment::Following).adjust(new_date,cal);
                }
                else
                {
                    new_date = DateAdjust(Adjustment::Preceding).adjust(new_date,cal);
                }
                break;
            }
            case Adjustment::ModifiedFollowingBiMon:
            {
                XQLIB_THROW("Not implemented yet.")
                break;
            }
            default :
                XQLIB_THROW("Not implemented yet.")
                break;
        }
        
        return new_date;
    }
    
    
    
    Date roll_to_previous_eom(Date const& d, Calendar const& cal, RollConvention roll)
    {
        boost::gregorian::month_iterator one_month(d);
        --one_month;
        
        return DateAdjust(roll).adjust(one_month->end_of_month(), cal);
    }
    
    Date roll_to_next_som(Date const& d, Calendar const& cal, RollConvention roll)
    {
        boost::gregorian::month_iterator one_month(d);
        ++one_month;
        
        Date start_of_month = Date(one_month->year(),one_month->month(),1);
        
        return DateAdjust(roll).adjust(start_of_month, cal);
    }

    
} //namespace XQLib

