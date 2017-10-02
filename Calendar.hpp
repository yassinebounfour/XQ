//
//  Calendar.hpp
//  XQLib
//
//  Created by Yassine Bounfour on 09/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#ifndef Calendar_hpp
#define Calendar_hpp

#include <map>
#include <set>
#include <string>
#include <ostream>
#include <XQLib/DateTime/Date.hpp>
#include <XQLib/Core/Exception.hpp>
#include <XQLib/Core/Variant.hpp>
#include <XQLib/Utils/Strings.hpp>


namespace XQLib
{
    
    ///! \class Calendar
    class Calendar
    {
    public:
        
        /// Default constructor
        Calendar();
        
        /// Construct Calendar from user inputted Weekends and Holidays.
        /// Give the calendar a name via the tag input. This tag should agree with the RDS tag.
        Calendar(std::set<Weekdays> const& weekend,
                 std::set<Date> const& holidays,
                 std::string const& tag )
        : weekend_(weekend),
        holidays_(holidays),
        tag_(tag)
        {}
        
        /// Construct Calendar from user inputed Weekends and Holidays.
        /// Give the calendar a name via the tag input. This tag should agree with the RDS tag.
        Calendar(std::map<std::string,Variant> const& data)
        {
            XQLIB_THROW("Parsing of calendar data, not implemented yet!");
        }
        
        virtual ~Calendar(){};
        
        bool is_business_day(Date const& d) const;
        
        void add_holidays(std::vector<Date> const& v);
        
        Calendar& add(Calendar const& other);
        
        
        std::set<Weekdays> const& get_weekend() const;
        std::set<Date> const& get_holidays() const;
        std::string const& get_tag() const;
        
    private:
        std::set<Weekdays>   weekend_;
        std::set<Date>       holidays_;
        std::string          tag_;
    };
    
    //Some useful functions of Calendar
    
    //Take the union of two XQLib calendars:
    Calendar calendar_union(Calendar const& c1, Calendar const& c2);
    
    //Take the union of n XQLib Calendars. Input is a vector of Calendar
    Calendar calendar_union(std::vector<Calendar> const& v);
    
    
    
    
    //typedef Cortex::Stem::Core::object_t<Cortex::Temporal::Date::ICalendar> DateCalendar;
    /*
     // XQLib Calendar Class
     class Calendar
     {
     public:
     Calendar() {}
     virtual ~Calendar() {}
     
     virtual bool IsBusinessDay(XQLib::Date d) const = 0;
     
     // For Cortex only
     virtual bool IsCortex() const = 0;
     virtual DateCalendar GetCalendar() const = 0;
     };
     
     
     // Wrapping Cortex Calendar Class
     class CalendarCortex : public Calendar
     {
     public:
     CalendarCortex(DateCalendar cldr)
     : calendar_(cldr) {}
     virtual ~CalendarCortex(){}
     
     bool IsBusinessDay(XQLib::Date d) const
     {
     return Cortex::Temporal::Date::isBusDay(calendar_, to_cortex(d));
     }
     
     
     bool IsCortex() const { return true; }
     
     DateCalendar GetCalendar() const
     {
     return calendar_;
     }
     
     private:
     DateCalendar calendar_;
     };
     */
    
} // namespace XQLib



#endif /* Calendar_hpp */
