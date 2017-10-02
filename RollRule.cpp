//
//  RollRule.cpp
//  XQLib
//
//  Created by Yassine Bounfour on 03/07/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#include "RollRule.hpp"


/*
class RollRuleRegular: public Implements<RollRuleRegular, IRollRule, IScheduleGenerator, IRollRuleRegularW, IPretty>
{
public:
    // IRollRule
    
    virtual date_t Floor(date_t d) const
    {
        switch (m_tenor.unit())
        {
            case TimeUnit::Day:
            {
                int n = d - m_anchor;
                
                n = n / m_tenor.count() - (n < 0 && n % m_tenor.count() ? 1 : 0);
                
                return m_anchor + m_tenor.count() * n;
            }
            case TimeUnit::BusDay:
            {
                d = adjust(d, Adjustment::Preceding, m_calendar.just());
                
                int n = sub(d, m_anchor, tenor_t(TimeUnit::BusDay, 1), m_rollday, none, m_calendar);
                
                n = n / m_tenor.count() - (n < 0 && n % m_tenor.count() ? 1 : 0);
                
                return add(m_anchor, tenor_t(TimeUnit::BusDay, 1), m_tenor.count() * n, m_rollday, none, m_calendar);
            }
                break;
            case TimeUnit::Week:
            {
                int n = d - m_anchor;
                
                n = n / (7 * m_tenor.count()) - (n < 0 && n % (7 * m_tenor.count()) ? 1 : 0);
                
                return m_anchor + 7 * m_tenor.count() * n;
            }
            case TimeUnit::Month:
            {
                date_t::components_t ca = m_anchor.components(), cd = d.components();
                int n = ((cd.yyyy * 12 + cd.mm) - (ca.yyyy * 12 + ca.mm));
                
                n = n / m_tenor.count() - (n < 0 && n % m_tenor.count() ? 1 : 0);
                
                date_t r = add(m_anchor, m_tenor, n, m_rollday, none, m_calendar);
                
                return r > d ? add(r, m_tenor, -1, m_rollday, none, m_calendar) : r;
            }
            case TimeUnit::Year:
            {
                date_t::components_t ca = m_anchor.components(), cd = d.components();
                int n = cd.yyyy - ca.yyyy;
                
                n = n / m_tenor.count() - (n < 0 && n % m_tenor.count() ? 1 : 0);
                
                date_t r = add(m_anchor, m_tenor, n, m_rollday, none, m_calendar);
                
                return r > d ? add(r, m_tenor, -1, m_rollday, none, m_calendar) : r;
            }
            default:
                THROW(invalid_argument, "floor not supported for tenor: " << m_tenor.to_string());
        }
    }
    
    virtual date_t Ceil(date_t d) const
    {
        date_t d2 = Floor(d);
        
        return d2 < d ? Add(d2, 1, true) : d2;
    }
    
    virtual date_t Add(date_t d, int n, bool unchecked) const
    {
        if (!unchecked && Floor(d) != d)
            THROW(invalid_argument, d.to_string() << " not in roll sequence, floor = " << Floor(d).to_string());
        else
            DEBUG_ASSERT(Floor(d) == d);	// check anyway in debug mode - this is really important!
        
        return add(d, m_tenor, n, m_rollday, none, m_calendar);
    }
    
    virtual int Sub(date_t d1, date_t d2, bool unchecked) const
    {
        if (!unchecked && Floor(d1) != d1)
            THROW(invalid_argument, d1.to_string() << " not in roll sequence, floor = " << Floor(d1).to_string());
        else
            DEBUG_ASSERT(Floor(d1) == d1);	// check anyway in debug mode - this is really important!
        
        if (!unchecked && Floor(d2) != d2)
            THROW(invalid_argument, d2.to_string() << " not in roll sequence, floor = " << Floor(d2).to_string());
        else
            DEBUG_ASSERT(Floor(d2) == d2);	// check anyway in debug mode - this is really important!
        
        return sub(d1, d2, m_tenor, m_rollday, none, m_calendar);
    }
    
    virtual double Freq(date_t d, bool unchecked) const
    {
        if (!unchecked && Floor(d) != d)
            THROW(invalid_argument, d.to_string() << " not in roll sequence, floor = " << Floor(d).to_string());
        else
            DEBUG_ASSERT(Floor(d) == d);	// check anyway in debug mode - this is really important!
        
        switch (m_tenor.unit())
        {
            case TimeUnit::Day:
                if(m_tenor.count() == 7)
                    return 52.0;
                else if(m_tenor.count() == 28)
                    return 13.0;
                else if(m_tenor.count() == 91)
                    return 4.0;
                else if(m_tenor.count() == 182)
                    return 2.0;
                else if(m_tenor.count() == 364)
                    return 1.0;
                else
                    return 365. / m_tenor.count();
            case TimeUnit::BusDay:
                return 252. / m_tenor.count();
            case TimeUnit::Week:
                return 52. / m_tenor.count();
            case TimeUnit::Month:
                return 12. / m_tenor.count();
            case TimeUnit::Year:
                return 1. / m_tenor.count();
            default: THROW(invalid_argument, "frequency not supported for tenor: " << m_tenor.to_string());
        }
    }
    
    // IScheduleGenerator
    
    virtual void MakeSchedule(
                              date_t							start,
                              date_t							finish,
                              const maybe_t<size_t>&			fs,
                              const maybe_t<size_t>&			bs,
                              std::vector<date_t>&			dates
                              ) const
    {
        GenSchedule(ThisObject<IRollRule>(this), start, finish, fs, bs, dates);
    }
    
    // IRollRuleRegularW
    
    virtual void Define(
                        const tenor_t&							tenor,
                        date_t									anchor,
                        const maybe_t<int>&						rollday,
                        const maybe_t<object_t<ICalendar> >&	calendar
                        )
    {
        if (add(anchor, tenor, 1, rollday, none, calendar) <= anchor)
            THROW(invalid_argument, "tenor fails to generate increasing dates: " << tenor.to_string());
        
        m_tenor = tenor;
        m_anchor = anchor;
        m_rollday = maybe_t<int>(maybe<int>(rollday, anchor.components().dd));	// default to day of anchor
        m_calendar = calendar;
    }
    
    // IPretty
    
    virtual void PrettyPrint(const object_t<IPrettyFormatterW>& fmt) const
    {
        const size_t n = 16;
        date_t d = date_t::today();
        
        fmt->OpenGrid(n, 2);
        
        for (size_t i = 0; i < n; ++i)
        {
            d = i ? Add(d, 1, false) : Floor(d);
            
            fmt->OpenCell();
            pretty<int>::print(fmt, i);
            fmt->CloseCell();
            fmt->OpenCell();
            pretty<date_t>::print(fmt, d);
            fmt->CloseCell();
        }
        
        fmt->CloseGrid();
    }
    
protected:
    void Save(const object_t<IOStreamW>& strm) const
    {
        const int32_t ver = 0;
        
        switch (ver)
        {
            case 0:
                version_stream(strm, ver) << m_tenor << m_anchor << m_rollday << m_calendar;
                break;
            default:
                XQLIB_THROW("unhandled stream version: " << ver);
        }
    }
    
    void Load(const object_t<IIStreamW>& strm)
    {
        const int32_t ver = stream_version(strm);
        
        switch (ver)
        {
            case 0:
                strm >> m_tenor >> m_anchor >> m_rollday >> m_calendar;
                break;
            default:
                XQLIB_THROW("unhandled stream version: " << ver);
        }
    }
    
private:
    tenor_t							m_tenor;
    date_t							m_anchor;
    maybe_t<int>					m_rollday;
    maybe_t<object_t<ICalendar> >	m_calendar;
};

class RollRuleNthDay: public Implements<RollRuleNthDay, IRollRule, IScheduleGenerator, IRollRuleNthDayW, IPretty>
{
public:
    // IRollRule
    
    virtual date_t Floor(date_t d) const
    {
        switch (m_tenor.unit())
        {
            case TimeUnit::Month:
            {
                date_t dnthday = nthDay(d, m_occurrence, m_weekday);
                bool inferior = d < dnthday;
                date_t::components_t ca = m_anchor.components(), cd = dnthday.components();
                int n = ((cd.yyyy * 12 + cd.mm) - (ca.yyyy * 12 + ca.mm)) - (inferior ? 1 : 0);
                n = n / m_tenor.count() - (n < 0 && n % m_tenor.count() ? 1 : 0);
                
                return (nthDay(add(m_anchor, m_tenor, n, none, none, none), m_occurrence, m_weekday));
            }
                
            case TimeUnit::Year:
            {
                date_t dnthday = nthDay(d, m_occurrence, m_weekday);
                bool inferior = d < dnthday;
                date_t::components_t ca = m_anchor.components(), cd = dnthday.components();
                int n = cd.yyyy - ca.yyyy - (inferior ? 1 : 0);
                n = n / m_tenor.count() - (n < 0 && n % m_tenor.count() ? 1 : 0);
                
                return (nthDay(add(m_anchor, m_tenor, n, none, none, none), m_occurrence, m_weekday));
            }
                
            default:
                THROW(invalid_argument, "floor not supported for tenor: " << m_tenor.to_string());
        }
    }
    
    virtual date_t Ceil(date_t d) const
    {
        date_t d2 = Floor(d);
        
        return d2 < d ? Add(d2, 1, true) : d2;
    }
    
    virtual date_t Add(date_t d, int n, bool unchecked) const
    {
        if (!unchecked && Floor(d) != d)
            THROW(invalid_argument, d.to_string() << " not in roll sequence, floor = " << Floor(d).to_string());
        else
            DEBUG_ASSERT(Floor(d) == d);    // check anyway in debug mode - this is really important!
        
        return nthDay(add(d, m_tenor, n, none, none, none), m_occurrence, m_weekday);
    }
    
    virtual int Sub(date_t d1, date_t d2, bool unchecked) const
    {
        if (!unchecked && Floor(d1) != d1)
            THROW(invalid_argument, d1.to_string() << " not in roll sequence, floor = " << Floor(d1).to_string());
        else
            DEBUG_ASSERT(Floor(d1) == d1);  // check anyway in debug mode - this is really important!
        
        if (!unchecked && Floor(d2) != d2)
            THROW(invalid_argument, d2.to_string() << " not in roll sequence, floor = " << Floor(d2).to_string());
        else
            DEBUG_ASSERT(Floor(d2) == d2);  // check anyway in debug mode - this is really important!
        
        int dd1, m1, y1;
        d1.yyyymmdd(y1, m1, dd1);
        date_t r1(y1, m1, 1);
        
        int dd2, m2, y2;
        d2.yyyymmdd(y2, m2, dd2);
        date_t r2(y2, m2, 1);
        
        return sub(r1, r2, m_tenor, none, none, none);
    }
    
    virtual double Freq(date_t d, bool unchecked) const
    {
        if (!unchecked && Floor(d) != d)
            THROW(invalid_argument, d.to_string() << " not in roll sequence, floor = " << Floor(d).to_string());
        else
            DEBUG_ASSERT(Floor(d) == d);    // check anyway in debug mode - this is really important!
        
        switch (m_tenor.unit())
        {
            case TimeUnit::Month:
                return 12. / m_tenor.count();
            case TimeUnit::Year:
                return 1. / m_tenor.count();
            default: THROW(invalid_argument, "frequency not supported for tenor: " << m_tenor.to_string());
        }
    }
    
    // IScheduleGenerator
    
    virtual void MakeSchedule(
                              date_t                          start,
                              date_t                          finish,
                              const maybe_t<size_t>&          fs,
                              const maybe_t<size_t>&          bs,
                              std::vector<date_t>&            dates
                              ) const
    {
        GenSchedule(ThisObject<IRollRule>(this), start, finish, fs, bs, dates);
    }
    
    // IRollRuleNthDayW
    
    virtual void Define(
                        const tenor_t&                          tenor,
                        date_t                                  anchor
                        )
    {
        m_tenor = tenor;
        m_anchor = anchor;
        timeunit_t tenorunit = tenor.unit();
        if(tenorunit != TimeUnit::Month && tenorunit != TimeUnit::Year)
            THROW(invalid_argument, "RollRuleNthDay requires the tenor unit to be either Month or Year: " << tenor.to_string());
        
        if (add(anchor, tenor, 1, none, none, none) <= anchor)
            THROW(invalid_argument, "tenor fails to generate increasing dates: " << tenor.to_string());
        
        m_weekday = weekday(anchor);
        m_occurrence = occurrence(anchor);
    }
    
    // IPretty
    
    virtual void PrettyPrint(const object_t<IPrettyFormatterW>& fmt) const
    {
        const size_t n = 16;
        date_t d = date_t::today();
        
        fmt->OpenGrid(n + 2, 2);
        fmt->OpenCell(); pretty<string_t>::print(fmt, "Weekday"); fmt->CloseCell();
        fmt->OpenCell(); pretty<weekday_t>::print(fmt, m_weekday); fmt->CloseCell();
        
        fmt->OpenCell();pretty<string_t>::print(fmt, "Occurence");fmt->CloseCell();
        fmt->OpenCell();pretty<int>::print(fmt, m_occurrence);fmt->CloseCell();
        
        for (size_t i = 0; i < n; ++i)
        {
            d = i ? Add(d, 1, false) : Floor(d);
            
            fmt->OpenCell();
            pretty<int>::print(fmt, i);
            fmt->CloseCell();
            fmt->OpenCell();
            pretty<date_t>::print(fmt, d);
            fmt->CloseCell();
        }
        
        fmt->CloseGrid();
    }
    
protected:
    void Save(const object_t<IOStreamW>& strm) const
    {
        const int32_t ver = 0;
        
        switch (ver)
        {
            case 0:
                version_stream(strm, ver) << m_tenor << m_anchor << m_weekday << m_occurrence;
                break;
            default:
                XQLIB_THROW("unhandled stream version: " << ver);
        }
    }
    
    void Load(const object_t<IIStreamW>& strm)
    {
        const int32_t ver = stream_version(strm);
        
        switch (ver)
        {
            case 0:
                strm >> m_tenor >> m_anchor >> m_weekday >> m_occurrence;
                break;
            default:
                XQLIB_THROW("unhandled stream version: " << ver);
        }
    }
    
private:
    tenor_t                         m_tenor;
    date_t                          m_anchor;
    weekday_t                       m_weekday;
    int                             m_occurrence;
};

class RollRuleShifted: public Implements<RollRuleShifted, IRollRule, IScheduleGenerator, IRollRuleShiftedW, IPretty>
{
public:
    // IRollRule
    
    virtual date_t Floor(date_t d) const
    {
        return m_rr->Floor(d - m_shift) + m_shift;
    }
    
    virtual date_t Ceil(date_t d) const
    {
        date_t d2 = Floor(d);
        
        return d2 < d ? Add(d2, 1, true) : d2;
    }
    
    virtual date_t Add(date_t d, int n, bool unchecked) const
    {
        if (!unchecked && Floor(d) != d)
            THROW(invalid_argument, d.to_string() << " not in roll sequence, floor = " << Floor(d).to_string());
        else
            DEBUG_ASSERT(Floor(d) == d);    // check anyway in debug mode - this is really important!
        
        return m_rr->Add(d - m_shift, n, unchecked) + m_shift;
    }
    
    virtual int Sub(date_t d1, date_t d2, bool unchecked) const
    {
        if (!unchecked && Floor(d1) != d1)
            THROW(invalid_argument, d1.to_string() << " not in roll sequence, floor = " << Floor(d1).to_string());
        else
            DEBUG_ASSERT(Floor(d1) == d1);  // check anyway in debug mode - this is really important!
        
        if (!unchecked && Floor(d2) != d2)
            THROW(invalid_argument, d2.to_string() << " not in roll sequence, floor = " << Floor(d2).to_string());
        else
            DEBUG_ASSERT(Floor(d2) == d2);  // check anyway in debug mode - this is really important!
        
        return m_rr->Sub(d1 - m_shift, d2 - m_shift, unchecked);
    }
    
    virtual double Freq(date_t d, bool unchecked) const
    {
        if (!unchecked && Floor(d) != d)
            THROW(invalid_argument, d.to_string() << " not in roll sequence, floor = " << Floor(d).to_string());
        else
            DEBUG_ASSERT(Floor(d) == d);    // check anyway in debug mode - this is really important!
        
        return m_rr->Freq(d - m_shift, unchecked);
    }
    
    // IScheduleGenerator
    
    virtual void MakeSchedule(
                              date_t                          start,
                              date_t                          finish,
                              const maybe_t<size_t>&          fs,
                              const maybe_t<size_t>&          bs,
                              std::vector<date_t>&            dates
                              ) const
    {
        GenSchedule(ThisObject<IRollRule>(this), start, finish, fs, bs, dates);
    }
    
    // IRollRuleShiftedW
    
    virtual void Define(
                        const object_t<IRollRule>&  rr,
                        int shift
                        )
    {
        m_rr = rr;
        m_shift = shift;
    }
    
    // IPretty
    
    virtual void PrettyPrint(const object_t<IPrettyFormatterW>& fmt) const
    {
        const size_t n = 16;
        date_t d = date_t::today();
        
        fmt->OpenGrid(n, 2);
        
        for (size_t i = 0; i < n; ++i)
        {
            d = i ? Add(d, 1, false) : Floor(d);
            
            fmt->OpenCell();
            pretty<int>::print(fmt, i);
            fmt->CloseCell();
            fmt->OpenCell();
            pretty<date_t>::print(fmt, d);
            fmt->CloseCell();
        }
        
        fmt->CloseGrid();
    }
    
protected:
    void Save(const object_t<IOStreamW>& strm) const
    {
        const int32_t ver = 0;
        
        switch (ver)
        {
            case 0:
                version_stream(strm, ver) << m_rr << m_shift;
                break;
            default:
                XQLIB_THROW("unhandled stream version: " << ver);
        }
    }
    
    void Load(const object_t<IIStreamW>& strm)
    {
        const int32_t ver = stream_version(strm);
        
        switch (ver)
        {
            case 0:
                strm >> m_rr >> m_shift;
                break;
            default:
                XQLIB_THROW("unhandled stream version: " << ver);
        }
    }
    
private:
    object_t<IRollRule>             m_rr;
    int                             m_shift;
};

class RollRuleShiftedWeekday: public Implements<RollRuleShiftedWeekday, IRollRule, IScheduleGenerator, IRollRuleShiftedWeekdayW, IPretty>
{
public:
    // IRollRule
    
    virtual date_t Floor(date_t d) const
    {
        date_t    tmp0 = m_rr->Floor(d);
        date_t    tmp1 = m_rr->Add(m_rr->Floor(d),-1,true);
        date_t    tmp2 = m_rr->Ceil(d);
        int       step = m_after ? 1 : -1;
        for (tmp0+=step; weekday(tmp0)!=m_weekday; tmp0+=step);
        for (tmp1+=step; weekday(tmp1)!=m_weekday; tmp1+=step);
        for (tmp2+=step; weekday(tmp2)!=m_weekday; tmp2+=step);
        if (m_after && tmp0<=d) {
            return tmp0;
        } else if (m_after && tmp0>d) {
            return tmp1;
        } else if (!m_after && tmp2<=d) {
            return tmp2;
        } else {
            return tmp0;
        }
    }
    
    virtual date_t Ceil(date_t d) const
    {
        date_t    tmp0 = m_rr->Ceil(d);
        date_t    tmp1 = m_rr->Add(m_rr->Ceil(d),1,true);
        date_t    tmp2 = m_rr->Floor(d);
        int       step = m_after ? 1 : -1;
        for (tmp0+=step; weekday(tmp0)!=m_weekday; tmp0+=step);
        for (tmp1+=step; weekday(tmp1)!=m_weekday; tmp1+=step);
        for (tmp2+=step; weekday(tmp2)!=m_weekday; tmp2+=step);
        if (!m_after && tmp0<d) {
            return tmp1;
        } else if (!m_after && tmp0>=d) {
            return tmp0;
        } else if (m_after && tmp2>=d) {
            return tmp2;
        } else {
            return tmp0;
        }
    }
    
    virtual date_t Add(date_t d, int n, bool unchecked) const
    {
        if (!unchecked && weekday(d) != m_weekday)
            THROW(invalid_argument, d.to_string() << " not in roll sequence, weekday = " << weekday(d).to_string() << ", expected " << m_weekday.to_string());
        else
            DEBUG_ASSERT(weekday(d) == m_weekday);    // check anyway in debug mode - this is really important!
        
        date_t tmp = m_after ? m_rr->Floor(d) : m_rr->Ceil(d);
        tmp = m_rr->Add(tmp, n, unchecked);
        int       step = m_after ? 1 : -1;
        for (tmp+=step; weekday(tmp)!=m_weekday; tmp+=step);
        return tmp;
    }
    
    virtual int Sub(date_t d1, date_t d2, bool unchecked) const
    {
        if (!unchecked && weekday(d1) != m_weekday)
            THROW(invalid_argument, d1.to_string() << " not in roll sequence, floor = " << Floor(d1).to_string());
        else
            DEBUG_ASSERT(weekday(d1) == m_weekday);  // check anyway in debug mode - this is really important!
        
        if (!unchecked && weekday(d2) != m_weekday)
            THROW(invalid_argument, d2.to_string() << " not in roll sequence, floor = " << Floor(d2).to_string());
        else
            DEBUG_ASSERT(weekday(d2) == m_weekday);  // check anyway in debug mode - this is really important!
        
        date_t tmp1 = m_after ? m_rr->Floor(d1) : m_rr->Ceil(d1);
        date_t tmp2 = m_after ? m_rr->Floor(d2) : m_rr->Ceil(d2);
        return m_rr->Sub(tmp1, tmp2, unchecked);
    }
    
    virtual double Freq(date_t d, bool unchecked) const
    {
        if (!unchecked && weekday(d) != m_weekday)
            THROW(invalid_argument, d.to_string() << " not in roll sequence, floor = " << Floor(d).to_string());
        else
            DEBUG_ASSERT(weekday(d) == m_weekday);    // check anyway in debug mode - this is really important!
        
        date_t tmp = m_after ? m_rr->Floor(d) : m_rr->Ceil(d);
        return m_rr->Freq(tmp, unchecked);
    }
    
    // IScheduleGenerator
    
    virtual void MakeSchedule(
                              date_t                          start,
                              date_t                          finish,
                              const maybe_t<size_t>&          fs,
                              const maybe_t<size_t>&          bs,
                              std::vector<date_t>&            dates
                              ) const
    {
        GenSchedule(ThisObject<IRollRule>(this), start, finish, fs, bs, dates);
    }
    
    // IRollRuleShiftedWeekdayW
    
    virtual void Define(
                        const object_t<IRollRule>&  rr,
                        bool after,
                        weekday_t weekday
                        )
    {
        m_rr = rr;
        m_after = after;
        m_weekday = weekday;
    }
    
    // IPretty
    
    virtual void PrettyPrint(const object_t<IPrettyFormatterW>& fmt) const
    {
        const size_t n = 16;
        date_t d = date_t::today();
        
        fmt->OpenGrid(n, 2);
        
        for (size_t i = 0; i < n; ++i)
        {
            d = i ? Add(d, 1, false) : Floor(d);
            
            fmt->OpenCell();
            pretty<int>::print(fmt, i);
            fmt->CloseCell();
            fmt->OpenCell();
            pretty<date_t>::print(fmt, d);
            fmt->CloseCell();
        }
        
        fmt->CloseGrid();
    }
    
protected:
    void Save(const object_t<IOStreamW>& strm) const
    {
        const int32_t ver = 0;
        
        switch (ver)
        {
            case 0:
                version_stream(strm, ver) << m_rr << m_after << m_weekday;
                break;
            default:
                XQLIB_THROW("unhandled stream version: " << ver);
        }
    }
    
    void Load(const object_t<IIStreamW>& strm)
    {
        const int32_t ver = stream_version(strm);
        
        switch (ver)
        {
            case 0:
                strm >> m_rr >> m_after >> m_weekday;
                break;
            default:
                XQLIB_THROW("unhandled stream version: " << ver);
        }
    }
    
private:
    object_t<IRollRule>             m_rr;
    bool                            m_after;
    weekday_t                       m_weekday;
};

class RollRuleStub: public Implements<RollRuleStub, IRollRule, IRollRuleStubW>
{
public:
    // IRollRule
    
    virtual date_t Floor(date_t d) const
    {
        date_t f = m_rr->Floor(d);
        
        if(f > m_dates[0] && f < m_dates[1])
            return m_rr->Floor(m_dates[0]);
        else if(f > m_dates[m_dates.size()-2] && f < m_dates[m_dates.size()-1])
            return m_rr->Floor(m_dates[m_dates.size()-2]);
        else
            return f;
    }
    
    virtual date_t Ceil(date_t d) const
    {
        date_t c = m_rr->Ceil(d);
        
        if(c > m_dates[0] && c < m_dates[1])
            return m_rr->Ceil(m_dates[1]);
        else if(c > m_dates[m_dates.size()-2] && c < m_dates[m_dates.size()-1])
            return m_rr->Ceil(m_dates[m_dates.size()-1]);
        else
            return c;
    }
    
    virtual date_t Add(date_t d, int n, bool unchecked) const
    {
        return m_rr->Add(d, n, unchecked);
    }
    
    virtual int Sub(date_t d1, date_t d2, bool unchecked) const
    {
        return m_rr->Sub(d1, d2, unchecked);
    }
    
    virtual double Freq(date_t d, bool unchecked) const
    {
        return m_rr->Freq(d, unchecked);
    }
    
    // IRollRuleStubW
    
    virtual void Define(
                        const object_t<IRollRule>&      rr,
                        date_t							start,		// first date in schedule
                        date_t							finish,		// last date in schedule
                        const maybe_t< int >&			fs,			// front stub control
                        const maybe_t< int >&			bs			// back stub control
    )
    {
        XQLIB_ASSERT(start < finish);
        
        m_rr = rr;
        m_dates = schedule(rr, start, finish, fs, bs);
    }
    
protected:
    void Save(const object_t<IOStreamW>& strm) const
    {
        const int32_t ver = 0;
        
        switch (ver)
        {
            case 0:
                version_stream(strm, ver) << m_rr << m_dates;
                break;
            default:
                XQLIB_THROW("unhandled stream version: " << ver);
        }
    }
    
    void Load(const object_t<IIStreamW>& strm)
    {
        const int32_t ver = stream_version(strm);
        
        switch (ver)
        {
            case 0:
                strm >> m_rr >> m_dates;
                break;
            default:
                XQLIB_THROW("unhandled stream version: " << ver);
        }
    }
    
private:
    object_t<IRollRule>             m_rr;
    array_t<date_t> 				m_dates;
};

class RollRuleCustomized: public Implements<RollRuleCustomized, IRollRule, IRollRuleCustomizedW>
{
public:
    
    int index(date_t date) const
    {
        std::vector<date_t> dates = m_dates.to_vector();
        std::vector<date_t>::const_iterator it = std::upper_bound(dates.begin(), dates.end(), date, comparedate);
        
        size_t i = it - dates.begin();
        return int(i-1);
    }
    
    // IRollRule
    virtual date_t Floor(date_t d) const
    {
        //XQLIB_ASSERT(d>=m_dates[0] && d<=m_dates[m_dates.size()-1]);
        if(d<m_dates[0])
            return Cortex::Temporal::Date::add(m_dates[0], m_tenor, -1, none, none, none);//a dummy floor
        
        if(d>m_dates[m_dates.size()-1])
            return m_dates[m_dates.size()-1];
        
        int i = index(d);
        
        return m_dates[i];
    }
    
    virtual date_t Ceil(date_t d) const
    {
        //XQLIB_ASSERT(d>=m_dates[0] && d<=m_dates[m_dates.size()-1]);
        if(d<m_dates[0])
            return m_dates[0];
        
        if(d>m_dates[m_dates.size()-1])
            return Cortex::Temporal::Date::add(m_dates[m_dates.size()-1], m_tenor, 1, none, none, none);//a dummy ceil
        
        date_t d2 = Floor(d);
        int i = index(d);
        
        return d2 < d ? m_dates[i+1] : d2;
    }
    
    virtual date_t Add(date_t d, int n, bool unchecked) const
    {
        XQLIB_ASSERT(d>=m_dates[0] && d<=m_dates[m_dates.size()-1]);
        
        if(!unchecked && Floor(d) != d)
            THROW(invalid_argument, d.to_string() << " not in roll sequence, floor = " << Floor(d).to_string());
        else
            DEBUG_ASSERT(Floor(d) == d);	// check anyway in debug mode - this is really important!
        
        int i = index(d);
        if(i+n > int(m_dates.size())-1)
            THROW(invalid_argument, "resulted date exceeds schedule range: cannot add " << n << " tenors to date " << d.to_string());
        
        return m_dates[i+n];
    }
    
    virtual int Sub(date_t d1, date_t d2, bool unchecked) const
    {
        //XQLIB_ASSERT(d1>=m_dates[0] && d1<=m_dates[m_dates.size()-1]);
        //XQLIB_ASSERT(d2>=m_dates[0] && d2<=m_dates[m_dates.size()-1]);
        
        if (!unchecked && Floor(d1) != d1)
            THROW(invalid_argument, d1.to_string() << " not in roll sequence, floor = " << Floor(d1).to_string());
        else
            DEBUG_ASSERT(Floor(d1) == d1);	// check anyway in debug mode - this is really important!
        
        if (!unchecked && Floor(d2) != d2)
            THROW(invalid_argument, d2.to_string() << " not in roll sequence, floor = " << Floor(d2).to_string());
        else
            DEBUG_ASSERT(Floor(d2) == d2);	// check anyway in debug mode - this is really important!
        
        //int i1 = index(d1);
        //int i2 = index(d2);
        
        int i1, i2;
        if(d1 > m_dates[m_dates.size() - 1])
            i1 = m_dates.size();
        else if(d1 < m_dates[0])
            i1 = -1;
        else
            i1 = index(d1);
        
        if(d2 > m_dates[m_dates.size() - 1])
            i2 = m_dates.size();
        else if(d2 < m_dates[0])
            i2 = -1;
        else
            i2 = index(d2);
        
        return i1 - i2;
    }
    
    virtual double Freq(date_t d, bool unchecked) const
    {
        
         //if (!unchecked && Floor(d) != d)
         //THROW(invalid_argument, d.to_string() << " not in roll sequence, floor = " << Floor(d).to_string());
         //else
         //DEBUG_ASSERT(Floor(d) == d);	// check anyway in debug mode - this is really important!
 
        
        XQLIB_ASSERT(Floor(d) == d);
        
        tenor_t tenorused;
        if (d >= m_dates.back())
            tenorused = m_tenor;
        else
        {
            int i = index(d);
            if((m_tenor.unit() == TimeUnit::Year && std::abs(add(d, m_tenor, none, none, none, none) - m_dates[i+1]) < 2)
               || (m_tenor.unit() == TimeUnit::Month && std::abs(add(d, m_tenor, none, none, none, none) - m_dates[i+1]) < 4))
            {
                tenorused = m_tenor;
            }
            else
            {
                tenorused = tenor_t(TimeUnit::Day, m_dates[i+1] - d);
            }
        }
        
        switch (tenorused.unit())
        {
            case TimeUnit::Day:
                if(tenorused.count() == 7)
                    return 52.0;
                else if(tenorused.count() == 28)
                    return 13.0;
                else if(tenorused.count() == 91)
                    return 4.0;
                else if(tenorused.count() == 182)
                    return 2.0;
                else if(tenorused.count() == 364)
                    return 1.0;
                else
                    return 365. / tenorused.count();
            case TimeUnit::BusDay:
                return 252. / tenorused.count();
            case TimeUnit::Week:
                return 52. / tenorused.count();
            case TimeUnit::Month:
                return 12. / tenorused.count();
            case TimeUnit::Year:
                return 1. / tenorused.count();
            default: THROW(invalid_argument, "frequency not supported for tenor: " << tenorused.to_string());
        }
    }
    
    // IRollRuleCustomizedW
    
    virtual void Define(
                        const string_t&				    tenor,      // for Freq calculation
                        const array_t<date_t>&  		dates		// all dates in the sequence
    )
    {
        XQLIB_ASSERT(dates.size()>0);
        m_tenor = tenor_t(tenor);
        m_dates = dates;
    }
protected:
    void Save(const object_t<IOStreamW>& strm) const
    {
        const int32_t ver = 0;
        
        switch (ver)
        {
            case 0:
                version_stream(strm, ver) << m_tenor << m_dates;
                break;
            default:
                XQLIB_THROW("unhandled stream version: " << ver);
        }
    }
    
    void Load(const object_t<IIStreamW>& strm)
    {
        const int32_t ver = stream_version(strm);
        
        switch (ver)
        {
            case 0:
                strm >> m_tenor >> m_dates;
                break;
            default:
                XQLIB_THROW("unhandled stream version: " << ver);
        }
    }
    
private:
    tenor_t     				    m_tenor;
    array_t<date_t> 				m_dates;
};

class RollRuleAdjusted: public Implements<RollRuleAdjusted, IRollRule, IScheduleGenerator, IRollRuleAdjustedW, IPretty>
{
public:
    
    int index(date_t date) const
    {
        std::vector<date_t>::const_iterator it = std::upper_bound(m_dates.begin(), m_dates.end(), date, comparedate);
        
        size_t i = it - m_dates.begin();
        return int(i-1);
    }
    
    // IRollRule
    virtual date_t Floor(date_t d) const
    {
        XQLIB_ASSERT(d>=m_dates[0] && d<=m_dates[m_dates.size()-1]);
        int i = index(d);
        
        return m_dates[i];
    }
    
    virtual date_t Ceil(date_t d) const
    {
        XQLIB_ASSERT(d>=m_dates[0] && d<=m_dates[m_dates.size()-1]);
        date_t d2 = Floor(d);
        int i = index(d);
        
        return d2 < d ? m_dates[i+1] : d2;
    }
    
    virtual date_t Add(date_t d, int n, bool unchecked) const
    {
        XQLIB_ASSERT(d>=m_dates[0] && d<=m_dates[m_dates.size()-1]);
        
        if(!unchecked && Floor(d) != d)
            THROW(invalid_argument, d.to_string() << " not in roll sequence, floor = " << Floor(d).to_string());
        else
            DEBUG_ASSERT(Floor(d) == d);	// check anyway in debug mode - this is really important!
        
        int i = index(d);
        if(i+n > int(m_dates.size())-1)
            THROW(invalid_argument, "resulted date exceeds schedule range: cannot add " << n << " tenors to date " << d.to_string());
        
        return m_dates[i+n];
    }
    
    virtual int Sub(date_t d1, date_t d2, bool unchecked) const
    {
        XQLIB_ASSERT(d1>=m_dates[0] && d1<=m_dates[m_dates.size()-1]);
        XQLIB_ASSERT(d2>=m_dates[0] && d2<=m_dates[m_dates.size()-1]);
        
        if (!unchecked && Floor(d1) != d1)
            THROW(invalid_argument, d1.to_string() << " not in roll sequence, floor = " << Floor(d1).to_string());
        else
            DEBUG_ASSERT(Floor(d1) == d1);	// check anyway in debug mode - this is really important!
        
        if (!unchecked && Floor(d2) != d2)
            THROW(invalid_argument, d2.to_string() << " not in roll sequence, floor = " << Floor(d2).to_string());
        else
            DEBUG_ASSERT(Floor(d2) == d2);	// check anyway in debug mode - this is really important!
        
        int i1 = index(d1);
        int i2 = index(d2);
        return i1 - i2;
    }
    
    virtual double Freq(date_t d, bool unchecked) const
    {
        if (!unchecked && Floor(d) != d)
            THROW(invalid_argument, d.to_string() << " not in roll sequence, floor = " << Floor(d).to_string());
        else
            DEBUG_ASSERT(Floor(d) == d);	// check anyway in debug mode - this is really important!
        
        switch (m_tenor.unit())
        {
            case TimeUnit::Day:
                if(m_tenor.count() == 7)
                    return 52.0;
                else if(m_tenor.count() == 28)
                    return 13.0;
                else if(m_tenor.count() == 91)
                    return 4.0;
                else if(m_tenor.count() == 182)
                    return 2.0;
                else if(m_tenor.count() == 364)
                    return 1.0;
                else
                    return 365. / m_tenor.count();
            case TimeUnit::BusDay:
                return 252. / m_tenor.count();
            case TimeUnit::Week:
                return 52. / m_tenor.count();
            case TimeUnit::Month:
                return 12. / m_tenor.count();
            case TimeUnit::Year:
                return 1. / m_tenor.count();
            default: THROW(invalid_argument, "frequency not supported for tenor: " << m_tenor.to_string());
        }
    }
    
    // IScheduleGenerator
    
    virtual void MakeSchedule(
                              date_t							start,
                              date_t							finish,
                              const maybe_t<size_t>&			fs,
                              const maybe_t<size_t>&			bs,
                              std::vector<date_t>&			dates
                              ) const
    {
        GenSchedule(ThisObject<IRollRule>(this), start, finish, fs, bs, dates);
    }
    
    // IRollRuleAdjustedW
    
    virtual void Define(
                        const tenor_t&							tenor,    // tenor, e.g. @3M@
                        date_t									anchor,   // some date in the sequence. the subsequential dates will be adjusted to businessday if falling on holiday, and the sequence will roll on this new date.
                        const object_t<ICalendar> &             calendar, //
                        const maybe_t<string_t> &               maturity  // pre-store dates in the sequence up to anchor +/- maturity.
    )
    {
        if (add(anchor, tenor, 1, none, none, calendar) <= anchor)
            THROW(invalid_argument, "tenor fails to generate increasing dates: " << tenor.to_string());
        
        if(!calendar->IsBusinessDay(anchor))
            THROW(invalid_argument, "anchor should be a business day: " << anchor.to_string());
        
        date_t rawstart = add(anchor, maybe(maturity, "40y"), -1, none, none, none);
        date_t rawend = add(anchor, maybe(maturity, "40y"), 1, none, none, none);
        
        m_tenor = tenor;
        m_anchor = anchor;
        m_calendar = calendar;
        
        std::vector<date_t> dates;
        dates.push_back(m_anchor);
        while(dates[dates.size()-1] <= rawend)
        {
            date_t thisdate = dates[dates.size()-1];
            date_t nextdate = add(thisdate, m_tenor, 1, none, adjustment_t(Adjustment::ModifiedFollowing), m_calendar);
            dates.push_back(nextdate);
        }
        
        while(dates[0] >= rawstart)
        {
            date_t thisdate = dates[0];
            date_t prevdate = add(thisdate, m_tenor, -1, none, adjustment_t(Adjustment::Preceding), m_calendar);
            
            std::vector<date_t>::iterator it;
            it = dates.begin();
            dates.insert(it, 1, prevdate);
        }
        
        m_dates = dates;
    }
    
    // IPretty
    
    virtual void PrettyPrint(const object_t<IPrettyFormatterW>& fmt) const
    {
        date_t d = m_anchor;
        int i = index(d);
        const size_t n = m_dates.size() - i < 16 ? m_dates.size() - i : 16;
        
        fmt->OpenGrid(n, 2);
        
        for (size_t i = 0; i < n; ++i)
        {
            d = i ? Add(d, 1, false) : Floor(d);
            
            fmt->OpenCell();
            pretty<int>::print(fmt, i);
            fmt->CloseCell();
            fmt->OpenCell();
            pretty<date_t>::print(fmt, d);
            fmt->CloseCell();
        }
        
        fmt->CloseGrid();
    }
    
protected:
    void Save(const object_t<IOStreamW>& strm) const
    {
        const int32_t ver = 0;
        
        switch (ver)
        {
            case 0:
                version_stream(strm, ver) << m_tenor << m_anchor << m_dates << m_calendar;
                break;
            default:
                XQLIB_THROW("unhandled stream version: " << ver);
        }
    }
    
    void Load(const object_t<IIStreamW>& strm)
    {
        const int32_t ver = stream_version(strm);
        
        switch (ver)
        {
            case 0:
                strm >> m_tenor >> m_anchor >> m_dates >> m_calendar;
                break;
            default:
                XQLIB_THROW("unhandled stream version: " << ver);
        }
    }
    
private:
    tenor_t							m_tenor;
    date_t							m_anchor;
    std::vector<date_t>			    m_dates;
    object_t<ICalendar>         	m_calendar;
};


*/