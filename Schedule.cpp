//
//  Schedule.cpp
//  XQLib
//
//  Created by Yassine Bounfour on 09/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#include "Schedule.hpp"
#include <XQLib/DateTime/YearFraction.hpp>
#include <XQLib/DateTime/RollConvention.hpp>
#include <XQLib/DateTime/ShifterType.hpp>
//#include <XQLib/Finance/IR/Convention.hpp> // FIXME
#include <XQLib/Finance/Compounding.hpp>


//Remove this once we have a QMR schedule generator
//#include <XQLib/Interface/Cortex/CR/CDSScheduleGenerator.hpp>
//#include <XQLib/Interface/Cortex/IR/ScheduleGenerator.hpp>

namespace XQLib
{
    //Use private iniator for code management in the following.
    void Schedule::init( Date const& effective_date
                        , Date const& terminationDate
                        , std::string const& tenor
                        , RollConvention accrual_roll
                        , RollConvention termination_roll
                        , RollConvention paymentRoll
                        , std::shared_ptr<Calendar> const& cal
                        , std::string const& paymentDelay
                        , std::string const& resetDelay
                        , DayCount const& basis)
    {
        XQLIB_ASSERT( (resetDelay.find('-') == std::string::npos) , "The reset delay should not contain a minus sign: " + resetDelay );
        
        effective_date_ = effective_date;
        basis_ = basis;
        DateAdjust termination_adjust(termination_roll);
        
        DateAdjust accrual_adjust(accrual_roll);
        DateAdjust payment_adjust(paymentRoll);
        
        Date adjusted_termination_date = termination_adjust.adjust(terminationDate, *cal);
        
        XQLIB_ASSERT( (add_tenor(adjusted_termination_date, tenor, -1) >= effective_date) , "Tenor is too long!");
        //Keep track of unadjusted end dates:
        
        Date unadj_result, unadj_prev;
        
        unadj_result = terminationDate;
        unadj_prev = terminationDate;
        
        std::vector<Date> unadjusted_end_dates;
        
        
        
        while(unadj_prev > effective_date_)
        {
            
            unadjusted_end_dates.insert(unadjusted_end_dates.begin(), unadj_result);
            unadj_result = add_tenor(*unadjusted_end_dates.begin(), tenor, -1);
            
            
            if((unadj_result.day() != terminationDate.day() ) && (unadj_result != unadj_result.end_of_month()) && tenor.find('d') == std::string::npos && tenor.find('w') == std::string::npos )
            {
                
                if(static_cast<unsigned short>(terminationDate.day()) > static_cast<unsigned short>((unadj_result.end_of_month()).day()))
                {
                    unadj_result = unadj_result.end_of_month();
                    
                }
                else
                {
                    unadj_result = Date(static_cast<unsigned short>(unadj_result.year()), static_cast<unsigned short>(unadj_result.month()),static_cast<unsigned short>(terminationDate.day()));
                    
                }
                
            }
            
            unadj_prev = unadj_result;
            
            
        }
        
        //Calculate the end_dates_: These are adjusted according to the "Following" adjustment from
        //the unadjusted end dates.
        
        for(std::size_t j = 0; j < unadjusted_end_dates.size(); ++j)
        {
            end_dates_.push_back(accrual_adjust.adjust(unadjusted_end_dates[j], *cal));
        }
        
        //Next populate the start_dates_. The first member is always the effective_date_:
        start_dates_.push_back(accrual_adjust.adjust(effective_date_, *cal));
        
        for(std::vector<Date>::const_iterator it = end_dates_.begin(); it != --end_dates_.end(); ++it)
        {
            start_dates_.push_back(*it);
        }
        
        //populate unadjusted_start_dates_:
        unadjusted_start_dates_.push_back(effective_date_);
        
        for(std::vector<Date>::const_iterator it = unadjusted_end_dates.begin(); it != unadjusted_end_dates.end(); ++it)
        {
            unadjusted_start_dates_.push_back(*it);
        }
        
        //AccrualFactors_: start and end date is adjusted according to the accrual_roll:
        
        for(std::size_t j = 0; j < start_dates_.size(); ++j)
        {
            accrual_factors_.push_back(yf(accrual_adjust.adjust(start_dates_[j], *cal), accrual_adjust.adjust(end_dates_[j], *cal), basis_));
        }
        
        //payment_dates_ = payment_adjust(end_dates_) + paymentDelay and then the result is adjusted using the paymentRoll:
        
        //Adjust all but last:
        for(std::size_t j = 0; j < end_dates_.size()-1; ++j)
        {
            
            payment_dates_.push_back(payment_adjust.adjust(add_tenor(payment_adjust.adjust(unadjusted_end_dates[j], *cal), paymentDelay), *cal));
            
        }
        //Adjust last using termination_adjust:
        payment_dates_.push_back(termination_adjust.adjust(add_tenor(termination_adjust.adjust(unadjusted_end_dates.back(), *cal), paymentDelay), *cal));
        
        
        //reset_dates_ = start_dates_ - resetDelay. The result is NOT adjusted
        for(std::size_t j = 0; j < start_dates_.size(); ++j)
        {
            reset_dates_.push_back(add_tenor(start_dates_[j], resetDelay, -1));
        }
        
        //mappings_: Since we aren't compounding: mappings_[i] = i.
        //See Cortex/Temporal/Date/Date.cpp line 2541 onwards.
        
        mappings_.push_back(0);
        std::size_t count = 0;
        
        for(std::vector<Date>::const_iterator it = start_dates_.begin(); it != start_dates_.end(); ++it)
        {
            count++;
            mappings_.push_back(count);
        }
        
    }
    
    /*
    Schedule::Schedule( Date const& effective_date
                       , Date const& terminationDate
                       , std::string const& tenor
                       , RollConvention accrualadjustment
                       , RollConvention terminationadjustment
                       , RollConvention paymentadjustment
                       , DayCount const& basis
                       , std::string const& resetDelay
                       , const boost::optional<std::shared_ptr<Calendar> > & resetcalendar
                       , const boost::optional<std::string> & paymentDelay
                       , const boost::optional<std::shared_ptr<Calendar> > & paymentcalendar
                       , const boost::optional<Date> & rollDate
                       , const boost::optional<Rolltype> & rollType
                       , const boost::optional<std::vector<Date> > & customStartDates
                       , const boost::optional<std::vector<Date> > & customEndDates
                       , const boost::optional<std::vector<double> > & customAccruals
                       , const boost::optional<std::vector<Date> > & customPaymentDates
                       , const boost::optional<std::vector<Date> > & customResetDates
                       , const LegCompoundingType & compoundingtype
                       , bool customOnly
                       , bool inArrears
                       , bool longFirst
                       , bool longLast )
    {
        XQLIB_THROW("Not implemented yet.");
    }
     */
    
    
    
    void CDSSchedule(
                     Date const& effective_date
                     , Date const& terminationDate
                     , std::string const& tenor
                     , RollConvention accrual_roll
                     , RollConvention termination_roll
                     , RollConvention paymentRoll
                     , std::string const& paymentDelay
                     , DayCount const& basis
                     , std::string const& calendar
                     , bool lastEndDateOverride
                     , Schedule& schedule)
    {
        std::vector<Date> unadjusted_start_dates, start_dates, end_dates, payment_dates;
        std::vector<double> accrual_factors;
        
        
        
        ///Doesn't use the calendar tag.
        ///This will be fixed when static holiday data has been implemented using the Calendar.
        ///Default Calendar (Sat and Sun are holidays) is assumed.
        
        std::string zero_reset_delay("0d");
        
        //Default calendar:
        std::shared_ptr<Calendar> cal = std::make_shared<Calendar>(Calendar());
        
        Schedule cds_schedule( effective_date
                              , terminationDate
                              , tenor
                              , accrual_roll
                              , termination_roll
                              , paymentRoll
                              , cal
                              , paymentDelay
                              , zero_reset_delay
                              , basis);
        //We now fixed the dates according to the conventions for CDS:
        
        //To do: Tidy up the following to help code management!
        
        //******************************** StartDates: ************************
        Date second_start_date = cds_schedule.start_dates()[1];
        Date changed_start_date = add_tenor(second_start_date, tenor, -1);
        
        //Only tenors without d and w require the following adjustment:
        if(tenor.find('d') == std::string::npos && tenor.find('w') == std::string::npos)
        {
            changed_start_date = DateAdjust(accrual_roll).adjust(changed_start_date, *cal);
        }
        else
        {
            second_start_date = cds_schedule.unadjusted_start_dates()[1];
            changed_start_date = add_tenor(second_start_date , tenor, -1);
            changed_start_date = DateAdjust(accrual_roll).adjust(changed_start_date, *cal);
        }
        
        //31st of month and 1st of month convention: Depends on tenor:
        if(terminationDate.day() == 31 && tenor.find('d') == std::string::npos && tenor.find('w') == std::string::npos)
        {
            changed_start_date = ((changed_start_date - roll_to_previous_eom(changed_start_date)).days() < (changed_start_date.end_of_month() - changed_start_date).days())
            ? roll_to_previous_eom(changed_start_date) : changed_start_date.end_of_month();
        }
        else if (terminationDate.day() == 1 && tenor.find('d') == std::string::npos && tenor.find('w') == std::string::npos)
        {
            
            Date proposed_change = Date(static_cast<unsigned short>(changed_start_date.year()), static_cast<unsigned short>(changed_start_date.month()), 1);
            
            changed_start_date = ((changed_start_date - proposed_change).days() <  (roll_to_next_som(changed_start_date) - changed_start_date).days())
            ? proposed_change : roll_to_next_som(changed_start_date);
        }
        
        //Further correction for end of feb:
        if((terminationDate.day() == 28 || terminationDate.day() == 29) && terminationDate.month() == 2 && (changed_start_date != changed_start_date.end_of_month()) && tenor.find('d') == std::string::npos && tenor.find('w') == std::string::npos)
        {
            boost::gregorian::month_iterator one_month(changed_start_date);
            --one_month;
            Date roll_back, roll_forward;
            
            //Account for leap years for the roll_back:
            if(boost::gregorian::gregorian_calendar::is_leap_year(one_month->year()) && one_month->month()==2)
            {
                roll_back = Date(static_cast<unsigned short>(one_month->year()), 2, 29);
            }
            else if ( !(boost::gregorian::gregorian_calendar::is_leap_year(one_month->year())) && one_month->month()==2)
            {
                roll_back = Date(static_cast<unsigned short>(one_month->year()), 2, 28);
            }
            else
            {
                roll_back = Date(static_cast<unsigned short>(one_month->year()), static_cast<unsigned short>(one_month->month()), static_cast<unsigned short>(terminationDate.day()));
            }
            
            //Account for leap years for the roll_forward:
            if(boost::gregorian::gregorian_calendar::is_leap_year(changed_start_date.year()) && changed_start_date.month()==2)
            {
                roll_forward = Date(static_cast<unsigned short>(changed_start_date.year()), 2, 29);
            }
            else if ( !(boost::gregorian::gregorian_calendar::is_leap_year(changed_start_date.year())) && changed_start_date.month()==2)
            {
                roll_forward = Date(static_cast<unsigned short>(changed_start_date.year()), 2, 28);
            }
            else
            {
                roll_forward = Date(static_cast<unsigned short>(changed_start_date.year()), static_cast<unsigned short>(changed_start_date.month()), static_cast<unsigned short>(terminationDate.day()) );
            }
            
            changed_start_date = ((changed_start_date - roll_back).days() < (roll_forward - changed_start_date).days())
            ? roll_back : roll_forward;
            
        }
        
        //Correction for non-start of month:
        if((changed_start_date.day() != terminationDate.day() ) && (changed_start_date != changed_start_date.end_of_month()) && tenor.find('d') == std::string::npos && tenor.find('w') == std::string::npos)
        {
            boost::gregorian::month_iterator one_month(changed_start_date);
            --one_month;
            Date roll_back, roll_forward;
            
            //Account for leap years for the roll_back:
            if(boost::gregorian::gregorian_calendar::is_leap_year(one_month->year()) && one_month->month()==2)
            {
                roll_back = Date(static_cast<unsigned short>(one_month->year()), 2, 29);
            }
            else if ( !(boost::gregorian::gregorian_calendar::is_leap_year(one_month->year())) && one_month->month()==2)
            {
                roll_back = Date(static_cast<unsigned short>(one_month->year()), 2, 28);
            }
            else
            {
                roll_back = Date(static_cast<unsigned short>(one_month->year()), static_cast<unsigned short>(one_month->month()), static_cast<unsigned short>(terminationDate.day()));
            }
            
            //Account for leap years for the roll_forward:
            if(boost::gregorian::gregorian_calendar::is_leap_year(changed_start_date.year()) && changed_start_date.month()==2)
            {
                roll_forward = Date(static_cast<unsigned short>(changed_start_date.year()), 2, 29);
            }
            else if ( !(boost::gregorian::gregorian_calendar::is_leap_year(changed_start_date.year())) && changed_start_date.month()==2)
            {
                roll_forward = Date(static_cast<unsigned short>(changed_start_date.year()), 2, 28);
            }
            else
            {
                roll_forward = Date(static_cast<unsigned short>(changed_start_date.year()), static_cast<unsigned short>(changed_start_date.month()), static_cast<unsigned short>(terminationDate.day()) );
            }
            
            changed_start_date = ((changed_start_date - roll_back).days() < (roll_forward - changed_start_date).days())
            ? roll_back : roll_forward;
        }
        
        //Use accrual ajustment at end, only if not d or w for tenor:
        if(tenor.find('d') == std::string::npos && tenor.find('w') == std::string::npos)
        {
            changed_start_date = DateAdjust(accrual_roll).adjust(changed_start_date, *cal);
        }
        
        //Insert the result into the class.
        cds_schedule.setStartDate(changed_start_date);
        
        // ************************ set UnadjustedStartDates: ***************************
        
        //Review this line:
        cds_schedule.setUnadjustedStartDate(add_tenor(cds_schedule.unadjusted_start_dates()[1], tenor, -1));
        
        Date unadj_result = cds_schedule.unadjusted_start_dates()[0];
        
        if((unadj_result.day() != terminationDate.day() ) && (unadj_result != unadj_result.end_of_month()) && tenor.find('d') == std::string::npos && tenor.find('w') == std::string::npos)
        {
            if(static_cast<unsigned short>(terminationDate.day()) > static_cast<unsigned short>((unadj_result.end_of_month()).day()))
            {
                unadj_result = unadj_result.end_of_month();
            }
            else
            {
                unadj_result = Date(static_cast<unsigned short>(unadj_result.year()), static_cast<unsigned short>(unadj_result.month()),static_cast<unsigned short>(terminationDate.day()));
            }
            
        }
        //Add the adjusted value to the class:
        cds_schedule.setUnadjustedStartDate(unadj_result);
        
        // ***************************************** EndDates adjustment: **********************
        Date penultimate_date, changed_date;
        
        if(tenor.find('d') == std::string::npos && tenor.find('w') == std::string::npos)
        {
            penultimate_date = cds_schedule.end_dates()[cds_schedule.end_dates().size()-2];
            changed_date = add_tenor(penultimate_date, tenor, 1);
            
        }
        else //use unadjusted start dates:
        {
            penultimate_date = cds_schedule.unadjusted_start_dates()[cds_schedule.unadjusted_start_dates().size()-2];
            changed_date = add_tenor(penultimate_date, tenor, 1);
            
        }
        
        bool snap_month_end = (add_tenor(changed_date.end_of_month(), tenor, -1) <= penultimate_date) ? true : false;
        //Need to correct this date as above:
        
        if((changed_date.day() != terminationDate.day() ) && (changed_date != changed_date.end_of_month()) && tenor.find('d') == std::string::npos  && tenor.find('w') == std::string::npos)
        {
            //if snapping to end of month DOESN'T result in a difference more than the tenor:
            if( (static_cast<unsigned short>(terminationDate.day()) > static_cast<unsigned short>((changed_date.end_of_month()).day())) && (snap_month_end))
            {
                changed_date = changed_date.end_of_month();
            }
            //if snaping to end of month DOES result in a difference more than the tenor then roll
            //back to end of previous month:
            else if ((static_cast<unsigned short>(terminationDate.day()) > static_cast<unsigned short>((changed_date.end_of_month()).day())) && !(snap_month_end))
            {
                
                boost::gregorian::month_iterator one_month(changed_date);
                --one_month;
                changed_date = one_month->end_of_month();
            }
            else
            {
                changed_date = Date(static_cast<unsigned short>(changed_date.year()), static_cast<unsigned short>(changed_date.month()),static_cast<unsigned short>(terminationDate.day()));
                
                if(add_tenor(changed_date - DateDuration(5), tenor, -1) > penultimate_date) //With m or y tenors: correction is at most 5 days due to rolling
                {
                    changed_date = Date(static_cast<unsigned short>(changed_date.year()), static_cast<unsigned short>(changed_date.month()-1),static_cast<unsigned short>(terminationDate.day()));
                }
            }
        }
        if((changed_date.day() != terminationDate.day() ) && (changed_date == changed_date.end_of_month()) && tenor.find('d') == std::string::npos  && tenor.find('w') == std::string::npos)
        {
            changed_date = Date(static_cast<unsigned short>(changed_date.year()), static_cast<unsigned short>(changed_date.month()), static_cast<unsigned short>(terminationDate.day()));
        }
        
        //Insert into class:
        cds_schedule.setLastEndDate(changed_date) ;
        
        //Adjust if lastEndDateOverride is true:
        if(lastEndDateOverride)
        {
            //Now correct due to lastEndDateOverride:
            cds_schedule.adjustLastEndDate(1);
        }
        
        //Use the getter methods to populate the above empty containers:
        unadjusted_start_dates = cds_schedule.unadjusted_start_dates();
        start_dates = cds_schedule.start_dates();
        end_dates = cds_schedule.end_dates();
        payment_dates = cds_schedule.payment_dates();
        accrual_factors = cds_schedule.accrual_factors();
        
        schedule = Schedule(unadjusted_start_dates, start_dates,end_dates,payment_dates,accrual_factors,basis,effective_date);
    }
    
    
    
    Schedule::Schedule( std::vector<Date> const& unadjusted_start_dates
                       , std::vector<Date> const& start_dates
                       , std::vector<Date> const& end_dates
                       , std::vector<Date> const& payment_dates
                       , DayCount const& basis
                       , Date const& effective_date
                       , const std::vector<std::size_t>& mappings)
    : unadjusted_start_dates_(unadjusted_start_dates)
    , start_dates_(start_dates)
    , end_dates_(end_dates)
    , payment_dates_(payment_dates)
    , mappings_(mappings)
    {
        effective_date_ = effective_date;
        basis_ = basis;
        std::size_t nb_periods = start_dates.size();
        XQLIB_ASSERT(end_dates.size() == nb_periods, "Schedule::Schedule: start_dates.size = " << start_dates.size() << ", end_dates.size = " << end_dates.size());
        accrual_factors_.resize(nb_periods);
        for (std::size_t i=0;i<nb_periods;++i)
            accrual_factors_[i] = yf(start_dates[i],end_dates[i],basis_);
    }
    
    Schedule::Schedule( Date const& effective_date
                       , Date const& terminationDate
                       , std::string const& tenor
                       , RollConvention accrual_roll
                       , RollConvention termination_roll
                       , RollConvention paymentRoll
                       , std::shared_ptr<Calendar> const& cal
                       , std::string const& paymentDelay
                       , std::string const& resetDelay
                       , DayCount const& basis)
    
    {
        init( effective_date
             , terminationDate
             , tenor
             , accrual_roll
             , termination_roll
             , paymentRoll
             , cal
             , paymentDelay
             , resetDelay
             , basis);
        
    }
    
    //Uses the Calendar default operator: Only Sat and Sun are taken as holidays
    
    Schedule::Schedule( Date const& effective_date
                       , Date const& terminationDate
                       , std::string const& tenor
                       , RollConvention accrual_roll
                       , RollConvention termination_roll
                       , RollConvention paymentRoll
                       , std::string const& paymentDelay
                       , std::string const& resetDelay
                       , DayCount const& basis)
    {
        
        //Create a default Calendar and std::shared_ptr to it:
        
        std::shared_ptr<Calendar> cal = std::make_shared<Calendar>(Calendar());
        
        init( effective_date
             , terminationDate
             , tenor
             , accrual_roll
             , termination_roll
             , paymentRoll
             , cal
             , paymentDelay
             , resetDelay
             , basis);
    }
    
    
    
    
    void Schedule::setStartDate(Date const& d, std::size_t i)
    {
        XQLIB_ASSERT(start_dates_.size() > i, "Schedule::setStartDate: tried to set element " << i << " of start_dates with size " << start_dates_.size());
        XQLIB_ASSERT(end_dates_.size() > i,   "Schedule::setStartDate: tried to set element " << i << " of start_dates but end_dates has size " << end_dates_.size());
        start_dates_[i] = d;
        accrual_factors_[i] = yf(d, end_dates_[i], basis_);
    }
    
    void Schedule::setUnadjustedStartDate(Date const& d, std::size_t i)
    {
        XQLIB_ASSERT(unadjusted_start_dates_.size() > i, "Schedule::setUnadjustedStartDate: tried to set element " << i << " of unadjusted_start_dates_ with size " << unadjusted_start_dates_.size());
        XQLIB_ASSERT(end_dates_.size() > i,   "Schedule::setUnadjustedStartDate: tried to set element " << i << " of unadjusted_start_dates_ but end_dates has size " << end_dates_.size());
        unadjusted_start_dates_[i] = d;
    }
    
    void Schedule::adjustLastEndDate(int days)
    {
        end_dates_.back() = end_dates_.back() + DateDuration(days);
        accrual_factors_.back() = yf(start_dates_.back(), end_dates_.back(), basis_);
    }
    
    void Schedule::setLastEndDate(Date const& d)
    {
        end_dates_.back() = d;
        accrual_factors_.back() = yf(start_dates_.back(), d, basis_);
        
    }
    
    bool Schedule::isValidCDSSchedule(std::string& msg)
    {
        //check that this is a valid CDS schedule
        unsigned int nb_periods = start_dates_.size();
        if (end_dates_[0] != payment_dates_[0]){
            msg = "Schedule::Schedule invalid CDS schedule given: end_dates_[0] != payment_dates_[0]";
            return false;
        }
        if (std::fabs(accrual_factors_[0] - yf(start_dates_[0], end_dates_[0], basis_)) > 1.0e-13) {
            msg = "Schedule::Schedule invalid CDS schedule given: accrual_factors_[0] != end_dates_[0] - start_dates_[0]";
            return false;
        }
        
        unsigned int i = 1;
        for (;i+1<nb_periods;++i) {
            if (start_dates_[i] != payment_dates_[i-1]) {
                msg = "Schedule::Schedule invalid CDS schedule given: start_dates_[i] != payment_dates_[i-1]";
                return false;
            }
            if (end_dates_[i] != payment_dates_[i]) {
                msg = "Schedule::Schedule invalid CDS schedule given: end_dates_[i] != payment_dates_[i]";
                return false;
            }
            if (std::fabs(accrual_factors_[i] - yf(start_dates_[i], end_dates_[i], basis_)) > 1.0e-13) {
                msg = "Schedule::Schedule invalid CDS schedule given: accrual_factors_[i] != yf(start_dates_[i], end_dates_[i], basis)";
                return false;
            }
        }
        if (nb_periods > 1) {
            if (start_dates_[i] != payment_dates_[i-1]) {
                msg = "Schedule::Schedule invalid CDS schedule given: start_dates_[i] != payment_dates_[i-1]";
                return false;
            }
            if (std::fabs(accrual_factors_[i] - yf(start_dates_[i], end_dates_[i], basis_)) > 1.0e-13) {
                msg = "Schedule::Schedule invalid CDS schedule given: accrual_factors_[i] != yf(start_dates_[i], end_dates_[i], basis)";
                return false;
            }
        }
        msg = "";
        return true;
    }
    
    
} // namespace XQLib

