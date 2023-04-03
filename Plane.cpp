#include "Plane.h"
#include<iostream>
using std::cout, std::endl;

Plane::Plane(int flt, int time, Plane_status status, int fuel)
/*
Post:  The Plane data members flt_num, clock_start,
       and state and fuel_level are set to the values of the parameters flt,
       time and status, respectively.
*/

{
   flt_num = flt;
   clock_start = time;
   state = status;
   fuel_level = fuel;

   cout << "Plane number " << flt << " ready to ";
   if (status == arriving)
      cout << "land." << endl;
   else
      cout << "take off." << endl;
}

Plane::Plane(int flt, int time, Plane_status status)
:Plane(flt, time, status, -1)
{

}

Plane::Plane()
/*
Post:  The Plane data members flt_num, clock_start,
       state are set to illegal default values.
*/
{
   flt_num = -1;
   clock_start = -1;
   state = null;
   fuel_level = -1;
}


void Plane::refuse() const
/*
Post: Processes a Plane wanting to use Runway, when
      the Queue is full.
*/

{
   cout << "Plane number " << flt_num;
   if (state == arriving)
      cout << " directed to another airport" << endl;
   else
      cout << " told to try to takeoff again later" << endl;
}


void Plane::land(int time) const
/*
Post: Processes a Plane that is landing at the specified time.
*/

{
   int wait = time - clock_start;
   cout << time << ": Plane number " << flt_num << " landed after "
        << wait << " time unit" << ((wait == 1) ? "" : "s")
        << " in the takeoff queue." << endl;
}

void Plane::emergency_land(int time) const
{
   cout << time << ": Plane number " << flt_num << " EMERGENCY landed!" << endl;
}


void Plane::fly(int time) const
/*
Post: Process a Plane that is taking off at the specified time.
*/

{
   int wait = time - clock_start;
   cout << time << ": Plane number " << flt_num << " took off after "
        << wait << " time unit" << ((wait == 1) ? "" : "s")
        << " in the takeoff queue." << endl;
}


int Plane::started() const
/*
Post: Return the time that the Plane entered the airport system.
*/
{
   return clock_start;
}

int Plane::get_fuel_level() const
{
   return fuel_level;
}

void Plane::set_fuel_level(int fuel)
{
   fuel_level = fuel;
}

void Plane::crash(int time) const
{
   int wait = time - clock_start;
   cout << time << ": Plane number " << flt_num << " CRASHED after "
        << wait << " time unit" << ((wait == 1) ? "" : "s")
        << endl;
}