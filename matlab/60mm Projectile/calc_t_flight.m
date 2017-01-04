  function t_flight=calc_t_flight(Charge,mils)

  % calculate time of flight
  %
  % Usage: t_flight=calc_t_flight(g,v0,theta,y0)
  %
  % where: g = gravitational acceleration (positive)
  %        v0 = initial velocity (magnitude)
  %        theta = launch angle (degrees)
  %        y0 = launch distance from ground

  % Solve -0.5*g*t^2+v0*sin(2*pi*(theta/180))*t+y0=0


    if(Charge == 0)
        t_flight= 9.05486956159104E-17*mils^6 - 6.17683814831614E-13*mils^5 + 1.73157959330906E-09*mils^4 - 2.55427862310546E-06*mils^3 + 0.00208671147208696*mils^2 - 0.884873604992918*mils + 159.005671279707
    else
        t_flight = -1.306811187764690E-16*mils^6 + 9.215333498585810E-13*mils^5 - 2.690529993834070E-09*mils^4 + 4.156580627133360E-06*mils^3 - 3.589682417863350E-03*mils^2 + 1.661033446980590E+00*mils - 3.077666724205040E+02
    end