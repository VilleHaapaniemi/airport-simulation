#include "Utility.h"
#include "Plane.h"
#include "Queue.h"
#include "Runway.h"
#include <iostream>

using std::cout, std::endl;

Runway::Runway(int limit)
/*
Post:  The Runway data members are initialized to record no
       prior Runway use and to record the limit on queue sizes.
*/

{
   queue_limit = limit;
   num_land_requests = num_takeoff_requests = 0;
   num_landings = num_takeoffs = 0;
   num_land_refused = num_takeoff_refused = 0;
   num_land_accepted = num_takeoff_accepted = 0;
   land_wait = takeoff_wait = idle_time = 0;
}

Error_code Runway::can_land(const Plane &current)
/*
Post:  If possible, the Plane current is added to the
       landing Queue; otherwise, an Error_code of overflow is
       returned. The Runway statistics are updated.
Uses:  class Extended_queue.
*/

{
   Error_code result;
   if (landing.size() < queue_limit)
      result = landing.append(current);
   else
      result = fail;
   num_land_requests++;

   if (result != success)
      num_land_refused++;
   else
      num_land_accepted++;

   return result;
}

Error_code Runway::can_land_check_fuel(const Plane &current)
/*
Post:  If possible, the Plane current is added to the
       landing Queue; otherwise, an Error_code of overflow is
       returned. The Runway statistics are updated.
Uses:  class Extended_queue.
*/

{
   Error_code result;
   if (landing.size() < queue_limit)
      if (current.get_fuel_level() < landing.size())
      {
         result = emergency_landing.append(current);
      }
      else
      {
         result = landing.append(current);
      }
   else
      result = fail;
   num_land_requests++;

   if (result != success)
      num_land_refused++;
   else
      num_land_accepted++;

   return result;
}

Error_code Runway::can_depart(const Plane &current)
/*
Post:  If possible, the Plane current is added to the
       takeoff Queue; otherwise, an Error_code of overflow is
       returned. The Runway statistics are updated.
Uses:  class Extended_queue.
*/

{
   Error_code result;
   if (takeoff.size() < queue_limit)
      result = takeoff.append(current);
   else
      result = fail;
   num_takeoff_requests++;
   if (result != success)
      num_takeoff_refused++;
   else
      num_takeoff_accepted++;

   return result;
}

Runway_activity Runway::activity(int time, Plane &moving)
/*
Post:  If the landing Queue has entries, its front
       Plane is copied to the parameter moving
       and a result  land is returned. Otherwise,
       if the takeoff Queue has entries, its front
       Plane is copied to the parameter moving
       and a result  takeoff is returned. Otherwise,
       idle is returned. Runway statistics are updated.
Uses:  class Extended_queue.
*/

{
   Runway_activity in_progress;
   if (!emergency_landing.empty())
   {
      emergency_landing.retrieve(moving);
      land_wait += time - moving.started();
      num_landings++;
      num_emergency_landings++;
      in_progress = emergency_land;
      emergency_landing.serve();
   }
   else if (!landing.empty())
   {
      landing.retrieve(moving);
      land_wait += time - moving.started();
      num_landings++;
      in_progress = land;
      landing.serve();
   }

   else if (!takeoff.empty())
   {
      takeoff.retrieve(moving);
      takeoff_wait += time - moving.started();
      num_takeoffs++;
      in_progress = take_off;
      takeoff.serve();
   }

   else
   {
      idle_time++;
      in_progress = idle;
   }
   return in_progress;
}

Runway_activity Runway::activity(int time, Plane &arriving, Plane &departing)
/*
Post:
Uses:  class Extended_queue.
*/

{
   Runway_activity in_progress;
   bool isIdle = true;

   if (!landing.empty())
   {
      isIdle = false;
      landing.retrieve(arriving);
      land_wait += time - arriving.started();
      num_landings++;
      in_progress = land;
      landing.serve();
   }

   if (!takeoff.empty())
   {
      isIdle = false;
      takeoff.retrieve(departing);
      takeoff_wait += time - departing.started();
      num_takeoffs++;
      if (in_progress == land)
      {
         in_progress = land_and_take_off;
      }
      else
      {
         in_progress = take_off;
      }
      takeoff.serve();
   }

   if (isIdle)
   {
      idle_time++;
      in_progress = idle;
   }
   return in_progress;
}

Runway_activity Runway::activity(int time, Plane &moving_plane1, Plane &moving_plane2, Plane &moving_plane3)
/*
Post:
Uses:  class Extended_queue.
*/
{
   Runway_activity in_progress;
   bool isIdle = true;

   if (!landing.empty())
   {
      isIdle = false;
      landing.retrieve(moving_plane1);
      land_wait += time - moving_plane1.started();
      num_landings++;
      in_progress = land;
      landing.serve();
   }

   if (!takeoff.empty())
   {
      isIdle = false;
      takeoff.retrieve(moving_plane2);
      takeoff_wait += time - moving_plane2.started();
      num_takeoffs++;
      if (in_progress == land)
      {
         in_progress = land_and_take_off;
      }
      else
      {
         in_progress = take_off;
      }
      takeoff.serve();
   }

   if (!landing.empty())
   {
      landing.retrieve(moving_plane3);
      land_wait += time - moving_plane3.started();
      num_landings++;
      if (in_progress == land_and_take_off)
      {
         in_progress = land_2_planes_and_takeoff;
      }
      else
      {
         in_progress = land_2_planes;
      }
      landing.serve();
   }
   else if (!takeoff.empty())
   {
      takeoff.retrieve(moving_plane3);
      takeoff_wait += time - moving_plane3.started();
      num_takeoffs++;
      if (in_progress == land_and_take_off)
      {
         in_progress = take_off_2_planes_and_land;
      }
      else
      {
         in_progress = take_off_2_planes;
      }
      takeoff.serve();
   }

   if (isIdle)
   {
      idle_time++;
      in_progress = idle;
   }
   return in_progress;
}

Runway_activity Runway::activity_landing_secured(int time, Plane &moving_plane1, Plane &moving_plane2)
/*
Post:
Uses:  class Extended_queue.
*/
{
   Runway_activity in_progress;
   bool isIdle = true;

   if (landing.size() == queue_limit)
   {
      clearing_arrivals = true;
      cout << "Landing Queue full!\n";
   }

   if (clearing_arrivals)
   {
      if (!landing.empty())
      {
         landing.retrieve(moving_plane1);
         land_wait += time - moving_plane1.started();
         num_landings++;
         in_progress = land;
         landing.serve();

         if (!landing.empty())
         {
            landing.retrieve(moving_plane2);
            land_wait += time - moving_plane2.started();
            num_landings++;
            in_progress = land_2_planes;
            landing.serve();
         }
      }

      if (landing.empty())
      {
         clearing_arrivals = false;
         cout << "Landing Queue cleared!\n";
      }

      return in_progress;
   }

   if (!landing.empty())
   {
      isIdle = false;
      landing.retrieve(moving_plane1);
      land_wait += time - moving_plane1.started();
      num_landings++;
      in_progress = land;
      landing.serve();

      if (takeoff.empty() && !landing.empty())
      {
         landing.retrieve(moving_plane2);
         land_wait += time - moving_plane2.started();
         num_landings++;
         in_progress = land_2_planes;
         landing.serve();
      }
   }

   if (!takeoff.empty())
   {
      isIdle = false;
      takeoff.retrieve(moving_plane2);
      takeoff_wait += time - moving_plane2.started();
      num_takeoffs++;

      if (in_progress == land)
      {
         in_progress = land_and_take_off;
      }
      else
      {
         in_progress = take_off;
      }
      takeoff.serve();

      if (landing.empty() && !takeoff.empty())
      {
         takeoff.retrieve(moving_plane1);
         takeoff_wait += time - moving_plane1.started();
         num_takeoffs++;
         in_progress = take_off_2_planes;
         takeoff.serve();
      }
   }

   if (isIdle)
   {
      idle_time++;
      in_progress = idle;
   }
   return in_progress;
}

void Runway::shut_down(int time) const
/*
Post: Runway usage statistics are summarized and printed.
*/

{
   cout << "Simulation has concluded after " << time << " time units." << endl
        << "Total number of planes processed "
        << (num_land_requests + num_takeoff_requests) << endl
        << "Total number of planes asking to land "
        << num_land_requests << endl
        << "Total number of planes asking to take off "
        << num_takeoff_requests << endl
        << "Total number of planes accepted for landing "
        << num_land_accepted << endl
        << "Total number of planes accepted for takeoff "
        << num_takeoff_accepted << endl
        << "Total number of planes refused for landing "
        << num_land_refused << endl
        << "Total number of planes refused for takeoff "
        << num_takeoff_refused << endl
        << "Total number of planes that landed "
        << num_landings << endl
        << "Total number of planes that took off "
        << num_takeoffs << endl
        << "Total number of planes left in landing queue "
        << landing.size() << endl
        << "Total number of planes left in takeoff queue "
        << takeoff.size() << endl;
   cout << "Percentage of time runway idle "
        << 100.0 * ((float)idle_time) / ((float)time) << "%" << endl;
   cout << "Average wait in landing queue "
        << ((float)land_wait) / ((float)num_landings) << " time units";
   cout << endl
        << "Average wait in takeoff queue "
        << ((float)takeoff_wait) / ((float)num_takeoffs)
        << " time units" << endl;
   cout << "Average observed rate of planes wanting to land "
        << ((float)num_land_requests) / ((float)time)
        << " per time unit" << endl;
   cout << "Average observed rate of planes wanting to take off "
        << ((float)num_takeoff_requests) / ((float)time)
        << " per time unit" << endl;
   cout << "Planes crashed " << num_crashed << endl;
   cout << "Emergency landings " << num_emergency_landings << endl;
   cout << "*****************************************************************\n\n";
}

void Runway::consumpt_fuel_waiting_landing(int current_time)
{
   Plane waiting_plane;
   for (int i = 0 ; i < landing.size() ; i++)
   {
      landing.retrieve(waiting_plane);
      landing.serve();
      if (waiting_plane.get_fuel_level() > 0)
      {
         waiting_plane.set_fuel_level(waiting_plane.get_fuel_level() - 1);
         landing.append(waiting_plane);
      } else {
         num_crashed ++;
         waiting_plane.crash(current_time);
      }
   }
}