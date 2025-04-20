#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;

//helper function prototype
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& shiftsCount,
    size_t day,
    size_t workerIndex);

// Add your implementation of schedule() and other helper functions here
bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    
    //initialize schedule with empty vectors for each day
    sched.resize(avail.size());
    for(size_t i = 0; i < avail.size(); i++)
    {
        sched[i].resize(dailyNeed, INVALID_ID);
    }
    
    //initialize shift counts for each worker
    vector<size_t> shiftsCount(avail[0].size(), 0);
    
    //start recursive backtracking
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsCount, 0, 0);
}

//recursive helper function to find a valid schedule
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& shiftsCount,
    size_t day,
    size_t workerIndex)
{
    //base case: if we've scheduled all days
    if(day == avail.size())
    {
        return true;
    }
    
    //if we've filled all workers for this day, move to next day
    if(workerIndex == dailyNeed)
    {
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsCount, day + 1, 0);
    }
    
    //try each worker for this position
    for(size_t worker = 0; worker < avail[day].size(); worker++)
    {
        //check if worker is available and hasn't exceeded max shifts
        if(avail[day][worker] && shiftsCount[worker] < maxShifts)
        {
            //check if worker is already scheduled for this day
            bool alreadyScheduled = false;
            for(size_t i = 0; i < workerIndex; i++)
            {
                if(sched[day][i] == worker)
                {
                    alreadyScheduled = true;
                    break;
                }
            }
            
            if(!alreadyScheduled)
            {
                //try this worker
                sched[day][workerIndex] = worker;
                shiftsCount[worker]++;
                
                //recurse to next position
                if(scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsCount, day, workerIndex + 1))
                {
                    return true;
                }
                
                //backtrack
                sched[day][workerIndex] = INVALID_ID;
                shiftsCount[worker]--;
            }
        }
    }
    
    return false;
}

