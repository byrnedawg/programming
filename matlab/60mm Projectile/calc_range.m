function range=calc_range(Charge,mils)

  % Calculate projectile range
  %
  % Usage: range=calc_range(v0,theta,t_flight)
  %
  % where: v0 = initial velocity (magnitude)
  %        theta = launch angle (degrees)
  %        t_flight = time of flight

   if(Charge == 0)
          range = 1.198813927608880E-15*mils^6 - 8.028486302795600E-12*mils^5 + 2.238826572914050E-08*mils^4 - 3.300090428478920E-05*mils^3 + 2.625547193694750E-02*mils^2 - 1.042030849009390E+01*mils + 1.982486074178250E+03
          diff(range)
   else
          range = -2.210430524886830E-15*mils^6 + 1.562485897106020E-11*mils^5 - 4.448824468500770E-08*mils^4 + 6.607418743678220E-05*mils^3 - 5.661465598869360E-02*mils^2 + 2.707433752398800E+01*mils - 4.232207094595680E+03
          diff(range)
   end   
  