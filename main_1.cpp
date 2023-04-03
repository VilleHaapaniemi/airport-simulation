#include <iostream>
#include "Utility.h"
#include "Plane.h"
#include "Queue.h"
#include "Random.h"
#include "Runway.h"
#include "mains.h"

void main_1()     //  Airport simulation program
/*
Pre:  The user must supply the number of time intervals the simulation is to
      run, the expected number of planes arriving, the expected number
      of planes departing per time interval, and the
      maximum allowed size for runway queues.
Post: The program performs a random simulation of the airport, showing
      the status of the runway at each time interval, and prints out a
      summary of airport operation at the conclusion.
Uses: Classes Runway, Plane, Random and functions run_idle, initialize.
*/

{
   int end_time;            //  time to run simulation
   int queue_limit;         //  size of Runway queues
   int flight_number = 0;
   double arrival_rate, departure_rate;
   initialize(end_time, queue_limit, arrival_rate, departure_rate);
   Random variable;
   Runway small_airport(queue_limit);
   for (int current_time = 0; current_time < end_time; current_time++) { //  loop over time intervals
      int number_arrivals = variable.poisson(arrival_rate);  //  current arrival requests
      for (int i = 0; i < number_arrivals; i++) {
         Plane current_plane(flight_number++, current_time, arriving);
         if (small_airport.can_land(current_plane) != success)
            current_plane.refuse();
      }

      int number_departures= variable.poisson(departure_rate); //  current departure requests
      for (int j = 0; j < number_departures; j++) {
         Plane current_plane(flight_number++, current_time, departing);
         if (small_airport.can_depart(current_plane) != success)
            current_plane.refuse();
      }

      Plane moving_plane;
      switch (small_airport.activity(current_time, moving_plane)) {
        //  Let at most one Plane onto the Runway at current_time.
      case land:
         moving_plane.land(current_time);
         break;
      case take_off:
         moving_plane.fly(current_time);
         break;
      case idle:
         run_idle(current_time);
      }
   }
   small_airport.shut_down(end_time);
}