#pragma once


#include <string>
#include <ostream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/date_time/gregorian/greg_month.hpp>
#include <boost/date_time/date_formatting.hpp>
#include <XQLib/Core/Maybe.hpp>
#include <XQLib/DateTime/DayCount.hpp>
#include <XQLib/DateTime/Tenor.hpp>
#include <boost/date_time/date_defs.hpp>

namespace XQLib
{
    /// XQLib date class
    typedef boost::gregorian::date Date;
    
    /// XQLib date/time class
    typedef boost::posix_time::ptime DateTime;
    
    /// XQLib time duration class
    typedef boost::posix_time::time_duration TimeDuration;
    
    /// XQLib date duration class
    typedef boost::gregorian::date_duration DateDuration;
    
    using boost::gregorian::days;
    using boost::gregorian::months;
    using boost::gregorian::years;
    
    using boost::gregorian::Monday;
    using boost::gregorian::Tuesday;
    using boost::gregorian::Wednesday;
    using boost::gregorian::Thursday;
    using boost::gregorian::Friday;
    using boost::gregorian::Saturday;
    using boost::gregorian::Sunday;
    
    typedef boost::date_time::weekdays Weekdays;
    
    class Calendar; // forward declaration
    
    /// date conversion
    Date datetime_to_date(DateTime const& d);
    DateTime date_to_datetime(Date const& d);
    std::vector<DateTime> date_to_datetime(std::vector<Date> const& dates);
    std::vector<Date> datetime_to_date(std::vector<DateTime> const& dates);
    double time_duration_to_days(const TimeDuration& t);
    
    /// date add
    Date add_tenor(Date const& d, const Tenor& tenor, int count = 1);
    Date add_tenor(Date const& d, const Tenor& tenor, Calendar const& calendar, int count = 1);
    DateTime DateTimeAddDiff(double delta, DateTime const& d);
    Date DateTimeAddDiff(double delta, Date const& d);
    
    /// date difference
    long  nb_days(Date const& d1 , Date const& d2, DayCount const& basis = DefaultDayCount);
    long  nb_days(const DateTime& d1 , const DateTime& d2, DayCount const& basis = DefaultDayCount);
    //double  DateTimeDiffInDays(const DateTime& d, const DateTime& d1);
    // Simply returns the number of days, no rounding up or change of unit to Months or Years
    //Tenor toTenor(Date const& d1 , Date const& d2, DayCount const& basis = DefaultDayCount);
    
    /// serialization
    /*
    std::string to_string(const DateTime& d);
    std::string to_string(Date const& d);
    
    Date date_from_string(std::string const& str);
    DateTime datetime_from_string(std::string const& str);
    */
    
    // Get the minimum possible date
    Date min_date();
    
    Date eom(Date const& date, maybe_t<std::shared_ptr<Calendar> > const& cal = none);
    
} // namespace XQLib



