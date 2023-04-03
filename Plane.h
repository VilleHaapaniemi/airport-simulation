#pragma once

enum Plane_status {null, arriving, departing};

class Plane {
public:
   Plane();
   Plane(int flt, int time, Plane_status status);
   Plane(int flt, int time, Plane_status status, int fuel);
   void refuse() const;
   void land(int time) const;
   void emergency_land(int time) const;
   void fly(int time) const;
   void crash(int time) const;
   int started() const;
   int get_fuel_level() const;
   void set_fuel_level(int);

private:
   int flt_num;
   int clock_start;
   Plane_status state;
   int fuel_level;
};