#include "Date.hpp"
#include <XQLib/Core/Maybe.hpp>
#include <XQLib/Core/Exception.hpp>
#include <XQLib/DateTime/DateTenor.hpp>

namespace XQLib
{
    
    
    
    /// Conversions
    std::vector<DateTime> date_to_datetime(std::vector<Date> const& dates)
    {
        std::vector<DateTime> v(dates.size());
        for (std::size_t i = 0; i < dates.size(); ++i)
        {
            v[i] = date_to_datetime(dates[i]);
        }
        return v;
    }
    
    std::vector<Date> datetime_to_date(const std::vector<DateTime>& dates)
    {
        std::vector<Date> v(dates.size());
        for (std::size_t i = 0; i < dates.size(); ++i)
        {
            v[i] = datetime_to_date(dates[i]);
        }
        return v;
    }
    
    DateTime date_to_datetime(Date const& d)
    {
        TimeDuration t(0,0,0);
        return DateTime(d, t);
    }
    
    Date datetime_to_date(const DateTime & d)
    {
        return d.date();
    }
    
    
    
    double time_duration_to_days(const TimeDuration& t)
    {
        return t.hours() / (24.) + t.minutes() / (24.*60.) + t.seconds() / (24.* 60.* 60.);
        
    }
    
    
    
    
    /// Date arithmetic
    long nb_days(Date const& d1, Date const& d2, DayCount const& basis)
    {
        if(d2<d1)
            return -nb_days(d2,d1,basis);
        
        switch(basis)
        {
            case Basis::Act360:
            case Basis::Act365:
            case Basis::Act365L:
            case Basis::ActActAFB:
            case Basis::ActActISDA:
            case Basis::ActActISMA:
                return (d2 -d1).days();
            default:
                XQLIB_THROW("Daycount basis not recognised!");
        }
    }
    
    long nb_days(const DateTime & d1, const DateTime & d2, DayCount const& basis)
    {
        return nb_days(datetime_to_date(d1), datetime_to_date(d2), basis);
    }
    
    /*
     Tenor toTenor(Date const& d1, Date const& d2, DayCount const& basis)
     {
     int n = nDays(to_cortex(d1), to_cortex(d2), to_cortex(basis));
     return number_to_string<int>(n) + "D";
     }
     */
    
    
    
    
    
    /// Date/Tenor arithmetic
    Date add_tenor(Date const& d, const Tenor& tenor, int count)
    {
        return add(d, Tenor2(tenor), count, none, none, none);
    }
    
    Date add_tenor(Date const& d, Tenor const& tenor, Calendar const& calendar, int count)
    {
        return add(d, Tenor2(tenor), count, none, none, calendar);
    }
    
    
    /*
    Date add_tenor(Date const& d, const Tenor& tenor, int count)
    {
        using namespace Cortex::Stem::Core;
        using namespace Cortex::Temporal::Date;
        
        // Use default calendar: Sat and Sun weekend days
        return from_cortex(add(to_cortex(d), tenor, count, none, none, Cortex::Temporal::Date::calendar(array_t<date_t>(), none)));
    }
    
    Date add_tenor(Date const& d, const Tenor& tenor, int count)
    {
        Date d2 = add_tenor(datetime_to_date(d), tenor, count);
        
        return date_to_datetime(d2);
    }
    */
    
    
    DateTime DateTimeAddDiff(double delta, const DateTime & d)
    {
        return  date_to_datetime(DateTimeAddDiff(delta,datetime_to_date(d)));
    }
     
    
    Date DateTimeAddDiff(double delta, Date const& d)
    {
        long days = std::ceil(365* delta); //FIXME
        return  add(d, Tenor2(TimeUnit::Day,days), none, none);
    }
    
    /*
    double DateTimeDiffInDays(Date const& d1, Date const& d2)
    {
        if(d1>d2)
            return DateTimeDiffInDays(d2, d1);
        else
            return time_duration_to_days(d2-d1);
    }
     */
    
    
    /*
    /// Conversions to/from string
    std::string to_string(Date const& d)
    {
        return boost::gregorian::to_iso_extended_string(d);
    }
    
    std::string to_string(const DateTime& d)
    {
        return boost::posix_time::to_iso_extended_string(d);
    }
    
    Date date_from_string(std::string const& str)
    {
        return boost::gregorian::from_undelimited_string(str);
    }
    
    DateTime datetime_from_string(std::string const& str)
    {
        return boost::posix_time::from_iso_string(str);
    }
    */
    
    
    Date min_date()
    {
        return Date(boost::gregorian::min_date_time);
    }
    
    
    Date eom(Date const& date,
             maybe_t<std::shared_ptr<Calendar> > const&  cal)
    {
        /*
        int d = date.day();
        int m = date.month();
        int y = date.year();
        
        if ( m == 12 )
        {
            ++y;
            m = 1;
        }
        else
            ++m;
        Date end( y, m, 1);
        
        end -= DateDuration(1);
         */
        Date end = date.end_of_month();
        
        if (!!cal)
            end = DateAdjust(Adjustment::Preceding).adjust( end, *(cal.get()) );
        
        return end;
    }

    
} // namespace XQLib


