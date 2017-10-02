//
//  DateTenor.cpp
//  XQLib
//
//  Created by Yassine Bounfour on 18/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#include "DateTenor.hpp"



namespace XQLib
{
    class Calendar;
    
    Date add(Date const& date,
             Tenor2 const& tenor,
             boost::optional<int> count,
             boost::optional<int> rollday,
             boost::optional<RollConvention> roll_conv,
             boost::optional<Calendar> calendar)
    {
        Date new_date(date);
        long n = tenor.count() * count.value_or(1);
        
        switch(tenor.unit())
        {
            case TimeUnit::Day:
                new_date += DateDuration(n);
                break;
            case TimeUnit::Week:
                new_date += DateDuration(7*n);
                break;
            case TimeUnit::BusDay:
            {
                XQLIB_ASSERT(calendar, "Adding business days requires calendar");
                Calendar cal = calendar.get();
                XQLIB_ASSERT(cal.is_business_day(date), "Date " << date << " is not a business day.");

                if(n>0)
                {
                    while(n>0)
                    {
                        do {
                            new_date += DateDuration(1);
                        } while (!cal.is_business_day(new_date));
                        n--;
                    }
                }
                else
                {
                    while(n<0)
                    {
                        do
                        {
                            new_date -= DateDuration(1);
                        }
                        while(!cal.is_business_day(new_date));
                        n++;
                    }
                }
                break;
            }
            case TimeUnit::Month:
                /*
                 new_date = new_date.add(n,rollday);
                 break;*/
            case TimeUnit::Year:
                /*
                 new_date = new_date.add(12 * n,rollday);
                 break;
                 */
            defaut:
                XQLIB_THROW("Not implemented yet!");
        }
        
        /// Adjustment
        XQLIB_ASSERT(roll_conv == none || roll_conv == Adjustment::NoAdjustment,
                     "Adjustment conventions not implemented yet");
        return new_date;
    }
    
    
    long sub(Date const& a,
             Date const& b,
             Tenor2 const& tenor,
             boost::optional<int> rollday,
             boost::optional<RollConvention> roll_conv,
             boost::optional<Calendar> calendar)
    {
        XQLIB_THROW("Not implemented yet!");
        XQLIB_ASSERT(roll_conv == none || roll_conv == Adjustment::NoAdjustment,
                     "Adjustment conventions not implemented yet");
        
        long count = tenor.count();
        
        switch(tenor.unit())
        {
            case TimeUnit::Day:
                XQLIB_ASSERT(!((b-a).days() % count),"Dates are not " << tenor.to_string() << " modulo apart");
                return (b-a).days() % tenor.count();
            case TimeUnit::Week:
                XQLIB_ASSERT(!((b-a).days() % (7 * count)),"Dates are not " << tenor.to_string() << " modulo apart");
                return (b-a).days() % (7 * count);
            case TimeUnit::Month:
                /*

            {
                int n = sub(a,b,rollday.value_or(0));
                XQLIB_ASSERT(!(n%count),  "Dates are not " << tenor.to_string() << " modulo apart");
                return n/count;
            }
                 */
            case TimeUnit::Year:
                /*
            {
                int n = sub(a,b,rollday.value_or(0));
                XQLIB_ASSERT(!(n% (12 *count),  "Dates are not " << tenor.to_string() << " modulo apart");
                return n/(12 * count);
            }
            */
            
            case TimeUnit::BusDay:
                /*
            {
                XQLIB_ASSERT(calendar, "Adding business days requires calendar");
                
                Calendar c = calendar.value();
                XQLIB_ASSERT(c.is_business_day(a), "Date " << a << " is not a business day.");
                XQLIB_ASSERT(c.is_business_day(b), "Date " << b << " is not a business day.");
                long n = 0;
                
                if(b>a)
                {
                    while(b-- > a)
                        if(c.is_business_day(b))
                            n++;
                }
                else
                {
                    while(b++ > a)
                        if(c.is_business_day(b))
                            n--;
                }
                XQLIB_ASSERT(!(n%count),  "Dates are not " << tenor.to_string() << " modulo apart");
                return n/count;
            }
            */
            defaut:
                XQLIB_THROW("Not implemented yet!");
        }
    }

    
}


