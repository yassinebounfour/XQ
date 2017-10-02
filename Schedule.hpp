/*!  \file Schedule.hpp Provides some basis schedule functionality based on vectors of dates
 */
//
//  Schedule.hpp
//  XQLib
//
//  Created by Yassine Bounfour on 09/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#ifndef Schedule_hpp
#define Schedule_hpp

#include <vector>
#include <XQLib/Core/Memory.hpp>
#include <boost/make_shared.hpp>
#include <XQLib/Core/Maybe.hpp>
#include <XQLib/DateTime/Date.hpp>
#include <XQLib/DateTime/DayCount.hpp>
#include <XQLib/DateTime/Calendar.hpp>
#include <XQLib/DateTime/RollConvention.hpp>
#include <XQLib/DateTime/RollRule.hpp>
#include <XQLib/DateTime/ShifterType.hpp>
#include <XQLib/Finance/Compounding.hpp>


namespace XQLib
{
    
    /*! \class Schedule
     \brief Schedule class.
     */
    class Schedule
    {
    public:
        /// constructor
        Schedule(){}
        
        
        /// constructor - Obtain accrual factors from start_dates, end_dates and the basis
        /// mappings is an optional argument
        Schedule( std::vector<Date> const& unadjusted_start_dates
                 , std::vector<Date> const& start_dates
                 , std::vector<Date> const& end_dates
                 , std::vector<Date> const& payment_dates
                 , DayCount const& basis
                 , Date const& effective_date = Date()
                 , std::vector<std::size_t> const& mappings = std::vector<std::size_t>());
        
        
        /// constructor - Create a schedule from start date, end date
        /// roll conventions, reset and payment delays, and the basis.
        /// Note that a default calendar is assumed here.
        Schedule( Date const& effective_date
                 , Date const& terminationDate
                 , std::string const& tenor
                 , RollConvention accrual_roll
                 , RollConvention termination_roll
                 , RollConvention paymentRoll
                 , std::string const& paymentDelay
                 , std::string const& resetDelay
                 , DayCount const& basis);
        
        /// constructor - Create a schedule from start date, end date
        /// roll conventions, reset and payment delays, and the basis.
        /// A Calendar is needed here.
        
        Schedule( Date const& effective_date
                 , Date const& terminationDate
                 , std::string const& tenor
                 , RollConvention accrual_roll
                 , RollConvention termination_roll
                 , RollConvention paymentRoll
                 , std::shared_ptr<Calendar> const& cal
                 , std::string const& paymentDelay
                 , std::string const& resetDelay
                 , DayCount const& basis);
        
        /// Full schedule constructor with calendars and customisation options
        
        Schedule( Date const& effective_date
                 , Date const& terminationDate
                 , std::string const& tenor
                 , RollConvention accrualadjustment
                 , RollConvention terminationadjustment
                 , RollConvention paymentadjustment
                 , DayCount const& basis
                 , std::string const& resetDelay
                 , const boost::optional<std::shared_ptr<Calendar> > & resetcalendar  = boost::none
                 , const boost::optional<std::string> & paymentDelay = boost::none  // default to "0d"?
                 , const boost::optional<std::shared_ptr<Calendar> > & paymentcalendar = boost::none
                 , const boost::optional<Date> & rollDate = boost::none // default to terminationDate?
                 , const boost::optional<Rolltype> & rolltype = boost::none // default to RollType::Default?
                 , const boost::optional<std::vector<Date> > & customStartDates = boost::none
                 , const boost::optional<std::vector<Date> > & customEndDates = boost::none
                 , const boost::optional<std::vector<double> > & customAccruals = boost::none
                 , const boost::optional<std::vector<Date> > & custompayment_dates = boost::none
                 , const boost::optional<std::vector<Date> > & customResetDates = boost::none
                 , LegCompoundingType compoundingtype = LegCompoundingType::LCT_NONE
                 , bool customOnly = false
                 , bool inArrears = false
                 , bool longFirst = false
                 , bool longLast = false);
        
        
        ///Constructor independent of basis. Used in some IR products.
        Schedule(std::vector<Date> const& unadjusted_start_dates,
                 std::vector<Date> const& start_dates,
                 std::vector<Date> const& end_dates,
                 std::vector<Date> const& payment_dates,
                 std::vector<Date> const& reset_dates,
                 std::vector<double> const&  accruals,
                 std::vector<std::size_t> const&  mappings)
        : unadjusted_start_dates_(unadjusted_start_dates),
        start_dates_(start_dates),
        end_dates_(end_dates),
        payment_dates_(payment_dates),
        reset_dates_(reset_dates),
        accrual_factors_(accruals),
        mappings_(mappings)
        {}
        
        ///Constructor independent of basis and reset_dates. Used in some IR Products
        Schedule(std::vector<Date> const& unadjusted_start_dates,
                 std::vector<Date> const& start_dates,
                 std::vector<Date> const& end_dates,
                 std::vector<Date> const& payment_dates,
                 std::vector<double> const&  accruals,
                 std::vector<std::size_t> const&  mappings)
        : unadjusted_start_dates_(unadjusted_start_dates),
        start_dates_(start_dates),
        end_dates_(end_dates),
        payment_dates_(payment_dates),
        accrual_factors_(accruals),
        mappings_(mappings)
        {}
        
        
        /// constructor - Define all schedule dates and accrual factors explicitly
        /// mappings is an optional argument
        Schedule(std::vector<Date> const& unadjusted_start_dates,
                 std::vector<Date> const& start_dates,
                 std::vector<Date> const& end_dates,
                 std::vector<Date> const& payment_dates,
                 std::vector<double> const&  accrual_factors,
                 DayCount const& basis,
                 Date const& effective_date = Date(),
                 std::vector<std::size_t> const& mappings = std::vector<std::size_t>())
        : unadjusted_start_dates_(unadjusted_start_dates)
        , start_dates_(start_dates)
        , end_dates_(end_dates)
        , payment_dates_(payment_dates)
        , accrual_factors_(accrual_factors)
        , basis_(basis)
        , effective_date_(effective_date)
        , mappings_(mappings)
        , initial_exchange_date_(start_dates.front())
        {}
        
        /// constructor - Define schedule with reset dates for floating leg schedules.
        /// mappings is an optional argument
        Schedule( std::vector<Date> const& unadjusted_start_dates
                 , std::vector<Date> const& start_dates
                 , std::vector<Date> const& end_dates
                 , std::vector<Date> const& payment_dates
                 , std::vector<double> const& accrual_factors
                 , std::vector<Date> const& reset_dates
                 , DayCount const& basis
                 , Date const& effective_date = Date()
                 , std::vector<std::size_t> const& mappings = std::vector<std::size_t>())
        : unadjusted_start_dates_(unadjusted_start_dates)
        , start_dates_(start_dates)
        , end_dates_(end_dates)
        , payment_dates_(payment_dates)
        , reset_dates_(reset_dates)
        , accrual_factors_(accrual_factors)
        , basis_(basis)
        , effective_date_(effective_date)
        , mappings_(mappings)
        , initial_exchange_date_(start_dates.front())
        {}
        
        
        /// If stub condition is not full: reduce length of period i
        /// from (end_date - start_date) to (end_date - d)
        /// Also sets the accrual factor appropriately.
        void setStartDate(Date const& d, std::size_t i = 0);
        
        void setUnadjustedStartDate(Date const& d, std::size_t i = 0);
        
        ///Adjust the last element of EndDates by adding days onto the last value.
        ///Also sets the accrual factor appropriately.
        ///Needed when considering a CDS Schedule, for example.
        void adjustLastEndDate(int days);
        
        void setLastEndDate(Date const& d);
        
        /// check schedule is a valid CDS schedule
        bool isValidCDSSchedule(std::string& msg);
        
        /// accessor
        //@{
        std::vector<Date> const&           unadjusted_start_dates()    const { return unadjusted_start_dates_; }
        std::vector<Date> const&           start_dates()               const { return start_dates_; }
        std::vector<Date> const&           end_dates()                 const { return end_dates_; }
        std::vector<Date> const&           payment_dates()             const { return payment_dates_; }
        std::vector<Date> const&           reset_dates()               const { return reset_dates_; }
        std::vector<double> const&         accrual_factors()           const { return accrual_factors_; }
        std::vector<std::size_t> const&    mappings()                  const { return mappings_; }
        DayCount const&                    basis()                     const { return basis_; }
        Date const&                        effective_date()            const { return effective_date_; }
        Date const&                        initial_exchange_date()     const { return initial_exchange_date_; }
        //@}
    private:
        std::vector<Date> unadjusted_start_dates_, start_dates_, end_dates_, payment_dates_, reset_dates_;
        std::vector<double> accrual_factors_;
        DayCount basis_;
        Date effective_date_;
        std::vector<std::size_t> mappings_;
        Date initial_exchange_date_;
        
        void init(  Date const& effective_date
                  , Date const& terminationDate
                  , std::string const& tenor
                  , RollConvention accrual_roll
                  , RollConvention termination_roll
                  , RollConvention paymentRoll
                  , std::shared_ptr<Calendar> const& cal
                  , std::string const& paymentDelay
                  , std::string const& resetDelay
                  , DayCount const& basis);
    };
    
    
    /*! \fn CDSSchedule
     \brief Generate a CDS schedule. Accrual dates are made equal to pay dates
     
     */
    void CDSSchedule(Date const& effective_date,
                     Date const& terminationDate,
                     std::string const& tenor,
                     RollConvention accrual_roll,
                     RollConvention termination_roll,
                     RollConvention paymentRoll,
                     std::string const& paymentDelay,
                     DayCount const& basis,
                     std::string const& calendar,
                     bool lastEndDateOverride,
                     Schedule& schedule);
    
    
    
} // namespace XQLib


#endif /* Schedule_hpp */
